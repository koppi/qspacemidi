/*
 * settings.h - QSettings persistence for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_SETTINGS_H
#define QSPACEMIDI_SETTINGS_H

#include "mapping.h"

class QByteArray;

namespace qspacemidi {

/*!
 * \brief Loads the configuration previously written by saveConfig().
 *
 * Missing keys fall back to the defaults in Config, so a fresh install and
 * a partially written configuration both give a usable setup.
 */
Config loadConfig();

/*! Writes \a config to the platform's QSettings store. */
void saveConfig(const Config &config);

/*! Restores the main window geometry, or an empty array when unset. */
QByteArray loadWindowGeometry();

/*! Stores the main window geometry. */
void saveWindowGeometry(const QByteArray &geometry);

} // namespace qspacemidi

#endif // QSPACEMIDI_SETTINGS_H
