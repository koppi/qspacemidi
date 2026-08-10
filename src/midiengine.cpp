/*
 * midiengine.cpp - libremidi based MIDI output for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "midiengine.h"

#include <QMetaObject>
#include <QtGlobal>

#include <libremidi/libremidi.hpp>

#include <memory>
#include <vector>

namespace qspacemidi {

namespace {

/*!
 * \brief Creates a midi_out that announces itself as "QSpaceMIDI".
 *
 * Without this the back-ends fall back to their own default client name
 * ("libremidi client" on ALSA), which is not what a user connecting to the
 * port in Renoise expects to see.
 */
std::unique_ptr<libremidi::midi_out> makeOutput()
{
    auto apiConfig = libremidi::midi_out_configuration_for(libremidi::midi1::default_api());

    // libremidi::set_client_name() hard-codes std::visit and therefore does
    // not compile against the Boost.Variant2 build of libremidi, so visit the
    // configuration through the alias libremidi itself uses.
    libremidi_variant_alias::visit(
        [](auto &backend) {
            if constexpr (requires { backend.client_name; }) {
                backend.client_name = MidiEngine::virtualPortName().toStdString();
            }
        },
        apiConfig);

    return std::make_unique<libremidi::midi_out>(libremidi::output_configuration{}, apiConfig);
}

/*! Display name of \a port, falling back to the device/port pair. */
QString portDisplayName(const libremidi::output_port &port)
{
    if (!port.display_name.empty()) {
        return QString::fromStdString(port.display_name);
    }
    if (!port.device_name.empty() && !port.port_name.empty()) {
        return QString::fromStdString(port.device_name) + QStringLiteral(": ")
               + QString::fromStdString(port.port_name);
    }
    if (!port.port_name.empty()) {
        return QString::fromStdString(port.port_name);
    }
    return QString::fromStdString(port.device_name);
}

} // namespace

struct MidiEngine::Private
{
    libremidi::observer observer{};
    std::unique_ptr<libremidi::midi_out> out;
    std::vector<libremidi::output_port> ports;
    QStringList portNames;
    QString connection;
};

MidiEngine::MidiEngine(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<Private>())
{
    refreshPorts();
}

MidiEngine::~MidiEngine()
{
    closePort();
}

bool MidiEngine::supportsVirtualPorts()
{
#if defined(Q_OS_WIN)
    // The Windows MIDI stack has no notion of virtual ports; a loopback
    // driver such as loopMIDI provides them instead.
    return false;
#else
    return true;
#endif
}

QString MidiEngine::virtualPortName()
{
    return QStringLiteral("QSpaceMIDI");
}

QStringList MidiEngine::refreshPorts()
{
    d->ports = d->observer.get_output_ports();
    d->portNames.clear();
    d->portNames.reserve(static_cast<int>(d->ports.size()));
    for (const auto &port : d->ports) {
        d->portNames.append(portDisplayName(port));
    }
    return d->portNames;
}

QStringList MidiEngine::ports() const
{
    return d->portNames;
}

bool MidiEngine::openPort(const QString &name)
{
    closePort();

    // Re-enumerate so that ports plugged in after start-up are found.
    refreshPorts();

    const int index = d->portNames.indexOf(name);
    if (index < 0) {
        emit error(tr("MIDI output port \"%1\" was not found.").arg(name));
        return false;
    }

    auto out = makeOutput();
    if (auto err = out->open_port(d->ports[static_cast<size_t>(index)],
                                 virtualPortName().toStdString());
        err != stdx::error{}) {
        emit error(tr("Could not open MIDI output port \"%1\".").arg(name));
        return false;
    }

    d->out = std::move(out);
    d->connection = name;
    emit connectionChanged();
    return true;
}

bool MidiEngine::openVirtualPort()
{
    closePort();

    if (!supportsVirtualPorts()) {
        emit error(tr("This platform does not support virtual MIDI ports. "
                      "Install a loopback driver such as loopMIDI and select "
                      "its port instead."));
        return false;
    }

    auto out = makeOutput();
    if (auto err = out->open_virtual_port(virtualPortName().toStdString());
        err != stdx::error{}) {
        emit error(tr("Could not create the virtual MIDI port \"%1\".")
                       .arg(virtualPortName()));
        return false;
    }

    d->out = std::move(out);
    d->connection = tr("%1 (virtual port)").arg(virtualPortName());
    emit connectionChanged();
    return true;
}

void MidiEngine::closePort()
{
    if (!d->out) {
        return;
    }
    d->out->close_port();
    d->out.reset();
    d->connection.clear();
    emit connectionChanged();
}

bool MidiEngine::isOpen() const
{
    return d->out && d->out->is_port_open();
}

QString MidiEngine::connectionName() const
{
    return d->connection;
}

void MidiEngine::sendControlChange(int channel, int cc, int value)
{
    if (!d->out) {
        return;
    }
    if (channel < 1 || channel > 16 || cc < 0 || cc > 127 || value < 0 || value > 127) {
        return;
    }

    const auto status = static_cast<unsigned char>(0xB0 | ((channel - 1) & 0x0F));
    d->out->send_message(status, static_cast<unsigned char>(cc),
                         static_cast<unsigned char>(value));
}

} // namespace qspacemidi
