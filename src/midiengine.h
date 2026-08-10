/*
 * midiengine.h - libremidi based MIDI output for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_MIDIENGINE_H
#define QSPACEMIDI_MIDIENGINE_H

#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

namespace libremidi {
class midi_out;
class observer;
}

namespace qspacemidi {

/*!
 * \brief Thin Qt wrapper around a libremidi MIDI 1.0 output port.
 *
 * The engine either connects to an existing output port or - on the
 * platforms that support it (ALSA on Linux, CoreMIDI on macOS) - publishes
 * a virtual port other applications can connect to.  On Windows there are
 * no virtual ports, so a loopback driver such as loopMIDI is needed and
 * only the port list is offered.
 */
class MidiEngine : public QObject
{
    Q_OBJECT

public:
    explicit MidiEngine(QObject *parent = nullptr);
    ~MidiEngine() override;

    /*! True when the platform can publish virtual MIDI ports. */
    static bool supportsVirtualPorts();

    /*! Name of the virtual port published by openVirtualPort(). */
    static QString virtualPortName();

    /*!
     * \brief Re-enumerates the available MIDI output ports.
     *
     * The returned names are the display names reported by the back-end and
     * are what openPort() expects.
     */
    QStringList refreshPorts();

    /*! Output port names from the last refreshPorts() call. */
    QStringList ports() const;

    /*!
     * \brief Connects to the output port called \a name.
     *
     * \return true when the port was opened.
     */
    bool openPort(const QString &name);

    /*! Publishes a virtual output port; only useful where supported. */
    bool openVirtualPort();

    /*! Closes the current connection, if any. */
    void closePort();

    /*! True while a port is open. */
    bool isOpen() const;

    /*! Description of the open connection, for the status line. */
    QString connectionName() const;

    /*!
     * \brief Sends a control change message.
     *
     * \param channel MIDI channel, 1..16
     * \param cc      controller number, 0..127
     * \param value   controller value, 0..127
     */
    void sendControlChange(int channel, int cc, int value);

signals:
    /*! Emitted when the connection state changes. */
    void connectionChanged();

    /*! Emitted on back-end errors, with a human readable \a message. */
    void error(const QString &message);

private:
    struct Private;
    std::unique_ptr<Private> d;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_MIDIENGINE_H
