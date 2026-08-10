/*
 * aboutdialog.h - about box for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_ABOUTDIALOG_H
#define QSPACEMIDI_ABOUTDIALOG_H

#include <QDialog>

namespace qspacemidi {

/*!
 * \brief "About QSpaceMIDI" - version, credits and licence.
 *
 * Shown from the tray menu and from the About button in the main window.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
};

} // namespace qspacemidi

#endif // QSPACEMIDI_ABOUTDIALOG_H
