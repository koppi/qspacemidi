/*
 * autostart.h - start qspacemidi with the desktop session
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_AUTOSTART_H
#define QSPACEMIDI_AUTOSTART_H

#include <QString>

namespace qspacemidi::autostart {

/*!
 * \brief Whether autostart can be configured on this platform.
 *
 * True on Linux (XDG autostart), macOS (launch agent) and Windows (the
 * per-user Run key).
 */
bool isSupported();

/*! Whether the autostart entry currently exists. */
bool isEnabled();

/*!
 * \brief Creates or removes the autostart entry.
 *
 * The entry starts the application minimised, so that the mapping is live
 * right after login without a window popping up.
 *
 * \return true when the entry afterwards has the requested state.
 */
bool setEnabled(bool enabled);

/*!
 * \brief Where the autostart entry lives, for the tool tip.
 *
 * A file path on Linux and macOS, the registry key on Windows.
 */
QString location();

} // namespace qspacemidi::autostart

#endif // QSPACEMIDI_AUTOSTART_H
