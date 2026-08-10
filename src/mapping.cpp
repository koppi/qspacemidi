/*
 * mapping.cpp - axis/button to MIDI CC mapping model for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "mapping.h"

#include <QCoreApplication>

#include <algorithm>
#include <cmath>

namespace qspacemidi {

namespace {

/*!
 * Defaults asked for by the README table:
 *
 *   X  left/right          CC 1
 *   Y  forward/backward    CC 7
 *   Z  up/down             CC 74
 *   RX tilt left/right     CC 10
 *   RY tilt fwd/backward   CC 71
 *   RZ rotate              CC 91
 */
constexpr int defaultAxisCc[NumAxes] = {1, 7, 74, 10, 71, 91};
constexpr int defaultButtonCc[NumButtons] = {80, 81};

} // namespace

Config::Config()
{
    for (int i = 0; i < NumAxes; ++i) {
        axes[i].cc = defaultAxisCc[i];
    }
    for (int i = 0; i < NumButtons; ++i) {
        buttons[i].cc = defaultButtonCc[i];
    }
}

QString ccModeName(CcMode mode)
{
    switch (mode) {
    case CcMode::Absolute:
        return QCoreApplication::translate("mapping", "Absolute");
    case CcMode::RelativeBinaryOffset:
        return QCoreApplication::translate("mapping", "Relative (Bin Offset)");
    case CcMode::RelativeTwosComplement:
        return QCoreApplication::translate("mapping", "Relative (2's Comp)");
    case CcMode::RelativeSignedBit:
        return QCoreApplication::translate("mapping", "Relative (Signed Bit)");
    }
    return QString();
}

QString ccModeKey(CcMode mode)
{
    switch (mode) {
    case CcMode::Absolute:               return QStringLiteral("absolute");
    case CcMode::RelativeBinaryOffset:   return QStringLiteral("relative-binary-offset");
    case CcMode::RelativeTwosComplement: return QStringLiteral("relative-twos-complement");
    case CcMode::RelativeSignedBit:      return QStringLiteral("relative-signed-bit");
    }
    return QStringLiteral("absolute");
}

CcMode ccModeFromKey(const QString &key, CcMode fallback)
{
    for (auto mode : {CcMode::Absolute, CcMode::RelativeBinaryOffset,
                      CcMode::RelativeTwosComplement, CcMode::RelativeSignedBit}) {
        if (ccModeKey(mode) == key) {
            return mode;
        }
    }
    return fallback;
}

QString buttonModeName(ButtonMode mode)
{
    switch (mode) {
    case ButtonMode::Momentary:
        return QCoreApplication::translate("mapping", "Momentary");
    case ButtonMode::Toggle:
        return QCoreApplication::translate("mapping", "Toggle");
    }
    return QString();
}

QString buttonModeKey(ButtonMode mode)
{
    return mode == ButtonMode::Toggle ? QStringLiteral("toggle")
                                      : QStringLiteral("momentary");
}

ButtonMode buttonModeFromKey(const QString &key, ButtonMode fallback)
{
    if (key == QStringLiteral("toggle")) {
        return ButtonMode::Toggle;
    }
    if (key == QStringLiteral("momentary")) {
        return ButtonMode::Momentary;
    }
    return fallback;
}

QString axisName(int axis)
{
    static const char *const names[NumAxes] = {"X", "Y", "Z", "X", "Y", "Z"};
    return (axis >= 0 && axis < NumAxes) ? QString::fromLatin1(names[axis]) : QString();
}

QString axisDescription(int axis)
{
    switch (axis) {
    case AxisX:
        return QCoreApplication::translate("mapping", "Translate X - left / right");
    case AxisY:
        return QCoreApplication::translate("mapping", "Translate Y - forward / backward");
    case AxisZ:
        return QCoreApplication::translate("mapping", "Translate Z - up / down");
    case AxisRX:
        return QCoreApplication::translate("mapping", "Rotate X - tilt left / right");
    case AxisRY:
        return QCoreApplication::translate("mapping", "Rotate Y - tilt forward / backward");
    case AxisRZ:
        return QCoreApplication::translate("mapping", "Rotate Z - spin");
    default:
        return QString();
    }
}

double applyDeadZone(double deflection, int deadZone)
{
    const double dz = std::clamp(deadZone, 0, 50) / 100.0;
    const double magnitude = std::abs(deflection);
    if (magnitude <= dz) {
        return 0.0;
    }
    if (dz >= 1.0) {
        return 0.0;
    }
    const double scaled = (magnitude - dz) / (1.0 - dz);
    return std::copysign(std::min(scaled, 1.0), deflection);
}

double sensitivityFactor(int sensitivity)
{
    const double s = std::clamp(sensitivity, 0, 100);
    // 0 -> 4^-1 = 0.25, 50 -> 4^0 = 1, 100 -> 4^1 = 4
    return std::pow(4.0, (s - 50.0) / 50.0);
}

int absoluteCcValue(double deflection)
{
    const double clamped = std::clamp(deflection, -1.0, 1.0);
    const int value = 64 + static_cast<int>(std::lround(clamped * 63.0));
    return std::clamp(value, 0, 127);
}

int relativeCcValue(CcMode mode, int delta)
{
    switch (mode) {
    case CcMode::RelativeBinaryOffset:
        // 64 is "no change", 65.. is up, ..63 is down.
        return std::clamp(64 + std::clamp(delta, -63, 63), 0, 127);

    case CcMode::RelativeTwosComplement:
        // 1..63 counts up, 127..65 counts down (two's complement in 7 bit).
        if (delta >= 0) {
            return std::clamp(delta, 0, 63);
        }
        return 128 + std::clamp(delta, -63, -1);

    case CcMode::RelativeSignedBit:
        // Bit 6 is the sign: 65..127 counts up, 1..63 counts down.
        if (delta >= 0) {
            return 64 + std::clamp(delta, 0, 63);
        }
        return std::clamp(-delta, 0, 63);

    case CcMode::Absolute:
        break;
    }
    return std::clamp(delta, 0, 127);
}

} // namespace qspacemidi
