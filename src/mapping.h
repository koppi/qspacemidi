/*
 * mapping.h - axis/button to MIDI CC mapping model for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 */

#ifndef QSPACEMIDI_MAPPING_H
#define QSPACEMIDI_MAPPING_H

#include <QString>

#include <array>

namespace qspacemidi {

/*! Number of degrees of freedom reported by a 3D mouse. */
inline constexpr int NumAxes = 6;

/*! Number of hardware buttons handled by this application. */
inline constexpr int NumButtons = 2;

/*! Axis order, matching SpaceNavigator::Axes. */
enum Axis { AxisX = 0, AxisY, AxisZ, AxisRX, AxisRY, AxisRZ };

/*!
 * \brief How the deflection of one axis is turned into CC values.
 *
 * Absolute sends the deflection itself (centre = 64), which is what most
 * hosts - Renoise included - expect when learning a parameter.  The three
 * relative encodings send an increment per update, using the three
 * encodings commonly found in DAWs.
 */
enum class CcMode {
    Absolute = 0,           //!< 0..127, centre 64
    RelativeBinaryOffset,   //!< 64 + delta (64 = no change)
    RelativeTwosComplement, //!< 1..63 up, 127..65 down
    RelativeSignedBit,      //!< 65..127 up, 1..63 down
};

/*! Human readable name of \a mode, as shown in the mode combo boxes. */
QString ccModeName(CcMode mode);

/*! Settings key spelling of \a mode. */
QString ccModeKey(CcMode mode);

/*! Inverse of ccModeKey(); returns \a fallback for unknown strings. */
CcMode ccModeFromKey(const QString &key, CcMode fallback = CcMode::Absolute);

/*! How a hardware button press is turned into CC values. */
enum class ButtonMode {
    Momentary = 0, //!< 127 while held, 0 on release
    Toggle,        //!< alternates between 127 and 0 on every press
};

QString buttonModeName(ButtonMode mode);
QString buttonModeKey(ButtonMode mode);
ButtonMode buttonModeFromKey(const QString &key, ButtonMode fallback = ButtonMode::Momentary);

/*! Per-axis configuration. */
struct AxisMapping {
    int cc = 1;                       //!< CC number, 0..127
    CcMode mode = CcMode::Absolute;   //!< value encoding
    int sensitivity = 50;             //!< 0..100, 50 == 1.0x
    bool enabled = true;              //!< false mutes the axis
    bool invert = false;              //!< flip the direction
};

/*! Per-button configuration. */
struct ButtonMapping {
    int cc = 80;
    ButtonMode mode = ButtonMode::Momentary;
    bool enabled = true;
};

/*!
 * \brief Complete application configuration.
 *
 * The defaults are the ones documented in the README: CC 1/7/74 for the
 * translation axes, CC 10/71/91 for the rotation axes and CC 80/81 for the
 * two buttons.
 */
struct Config {
    std::array<AxisMapping, NumAxes> axes{};
    std::array<ButtonMapping, NumButtons> buttons{};

    int channel = 1;         //!< MIDI channel, 1..16
    int deadZone = 8;        //!< percent of full deflection, 0..50
    bool axisLock = false;   //!< only transmit the dominant axis
    int updateRate = 60;     //!< CC updates per second, 10..200

    /*!
     * \brief Silence after which the cap counts as released, in ms.
     *
     * A 3D mouse reports continuously while the cap is deflected and goes
     * quiet once it springs back - but the report that carries the centre
     * position is easily missed when the cap is released quickly, and
     * relative devices never send one at all.  When the device has said
     * nothing for this long, the axes are treated as centred.
     *
     * 0 disables the watchdog, for the rare device that only reports on
     * change and would otherwise be re-centred while held still.
     */
    int idleTimeout = 150;

    QString portName;        //!< output port to connect to, empty = virtual
    bool useVirtualPort = true;

    Config();
};

/*! Default axis name, e.g. "X" or "RZ". */
QString axisName(int axis);

/*! Long axis description, e.g. "Left / Right". */
QString axisDescription(int axis);

/*!
 * \brief Removes the dead zone from \a deflection and rescales the rest.
 *
 * \param deflection raw deflection in [-1, 1]
 * \param deadZone   dead zone in percent, 0..50
 * \return deflection in [-1, 1] that reaches 0 at the dead zone edge and
 *         still reaches +-1 at full deflection.
 */
double applyDeadZone(double deflection, int deadZone);

/*!
 * \brief Multiplier for a sensitivity knob position.
 *
 * 0 -> 0.25x, 50 -> 1.0x, 100 -> 4.0x (exponential, so the knob feels
 * linear).
 */
double sensitivityFactor(int sensitivity);

/*! Maps \a deflection in [-1, 1] to an absolute CC value, 0..127 (centre 64). */
int absoluteCcValue(double deflection);

/*!
 * \brief Encodes an increment for one of the relative CC modes.
 *
 * \param mode  a relative mode; Absolute returns \a delta clamped to 0..127
 * \param delta increment, clamped to the range the encoding can express
 */
int relativeCcValue(CcMode mode, int delta);

} // namespace qspacemidi

#endif // QSPACEMIDI_MAPPING_H
