/*
 * settings.cpp - QSettings persistence for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "settings.h"

#include <QByteArray>
#include <QSettings>

#include <algorithm>

namespace qspacemidi {

namespace {

/*! Settings group of axis \a index, e.g. "axis/RZ". */
QString axisGroup(int index)
{
    static const char *const keys[NumAxes] = {"X", "Y", "Z", "RX", "RY", "RZ"};
    return QStringLiteral("axis/") + QString::fromLatin1(keys[index]);
}

QString buttonGroup(int index)
{
    return QStringLiteral("button/%1").arg(index + 1);
}

} // namespace

Config loadConfig()
{
    Config config;
    QSettings settings;

    for (int i = 0; i < NumAxes; ++i) {
        AxisMapping &axis = config.axes[i];
        settings.beginGroup(axisGroup(i));
        axis.cc = std::clamp(settings.value(QStringLiteral("cc"), axis.cc).toInt(), 0, 127);
        axis.mode = ccModeFromKey(settings.value(QStringLiteral("mode")).toString(), axis.mode);
        axis.sensitivity = std::clamp(
            settings.value(QStringLiteral("sensitivity"), axis.sensitivity).toInt(), 0, 100);
        axis.enabled = settings.value(QStringLiteral("enabled"), axis.enabled).toBool();
        axis.invert = settings.value(QStringLiteral("invert"), axis.invert).toBool();
        settings.endGroup();
    }

    for (int i = 0; i < NumButtons; ++i) {
        ButtonMapping &button = config.buttons[i];
        settings.beginGroup(buttonGroup(i));
        button.cc = std::clamp(settings.value(QStringLiteral("cc"), button.cc).toInt(), 0, 127);
        button.mode
            = buttonModeFromKey(settings.value(QStringLiteral("mode")).toString(), button.mode);
        button.enabled = settings.value(QStringLiteral("enabled"), button.enabled).toBool();
        settings.endGroup();
    }

    settings.beginGroup(QStringLiteral("midi"));
    config.channel
        = std::clamp(settings.value(QStringLiteral("channel"), config.channel).toInt(), 1, 16);
    config.portName = settings.value(QStringLiteral("port"), config.portName).toString();
    config.useVirtualPort
        = settings.value(QStringLiteral("virtualPort"), config.useVirtualPort).toBool();
    settings.endGroup();

    settings.beginGroup(QStringLiteral("tray"));
    config.trayIconStyle
        = settings.value(QStringLiteral("iconStyle"), config.trayIconStyle).toString();
    if (config.trayIconStyle != QLatin1String("colour")
        && config.trayIconStyle != QLatin1String("color")
        && config.trayIconStyle != QLatin1String("monochrome")) {
        config.trayIconStyle = QStringLiteral("auto");
    }
    settings.endGroup();

    settings.beginGroup(QStringLiteral("input"));
    config.deadZone
        = std::clamp(settings.value(QStringLiteral("deadZone"), config.deadZone).toInt(), 0, 50);
    config.axisLock = settings.value(QStringLiteral("axisLock"), config.axisLock).toBool();
    config.updateRate = std::clamp(
        settings.value(QStringLiteral("updateRate"), config.updateRate).toInt(), 10, 200);
    const int idleTimeout
        = settings.value(QStringLiteral("idleTimeout"), config.idleTimeout).toInt();
    config.idleTimeout = idleTimeout <= 0 ? 0 : std::clamp(idleTimeout, 40, 2000);
    settings.endGroup();

    return config;
}

void saveConfig(const Config &config)
{
    QSettings settings;

    for (int i = 0; i < NumAxes; ++i) {
        const AxisMapping &axis = config.axes[i];
        settings.beginGroup(axisGroup(i));
        settings.setValue(QStringLiteral("cc"), axis.cc);
        settings.setValue(QStringLiteral("mode"), ccModeKey(axis.mode));
        settings.setValue(QStringLiteral("sensitivity"), axis.sensitivity);
        settings.setValue(QStringLiteral("enabled"), axis.enabled);
        settings.setValue(QStringLiteral("invert"), axis.invert);
        settings.endGroup();
    }

    for (int i = 0; i < NumButtons; ++i) {
        const ButtonMapping &button = config.buttons[i];
        settings.beginGroup(buttonGroup(i));
        settings.setValue(QStringLiteral("cc"), button.cc);
        settings.setValue(QStringLiteral("mode"), buttonModeKey(button.mode));
        settings.setValue(QStringLiteral("enabled"), button.enabled);
        settings.endGroup();
    }

    settings.beginGroup(QStringLiteral("midi"));
    settings.setValue(QStringLiteral("channel"), config.channel);
    settings.setValue(QStringLiteral("port"), config.portName);
    settings.setValue(QStringLiteral("virtualPort"), config.useVirtualPort);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("tray"));
    settings.setValue(QStringLiteral("iconStyle"), config.trayIconStyle);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("input"));
    settings.setValue(QStringLiteral("deadZone"), config.deadZone);
    settings.setValue(QStringLiteral("axisLock"), config.axisLock);
    settings.setValue(QStringLiteral("updateRate"), config.updateRate);
    settings.setValue(QStringLiteral("idleTimeout"), config.idleTimeout);
    settings.endGroup();
}

QByteArray loadWindowGeometry()
{
    QSettings settings;
    return settings.value(QStringLiteral("window/geometry")).toByteArray();
}

void saveWindowGeometry(const QByteArray &geometry)
{
    QSettings settings;
    settings.setValue(QStringLiteral("window/geometry"), geometry);
}

} // namespace qspacemidi
