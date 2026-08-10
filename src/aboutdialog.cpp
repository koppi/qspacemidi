/*
 * aboutdialog.cpp - about box for qspacemidi
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "aboutdialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#ifndef QSPACEMIDI_VERSION
#define QSPACEMIDI_VERSION "0.0.0"
#endif

namespace qspacemidi {

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About QSpaceMIDI"));
    setModal(true);

    auto *icon = new QLabel(this);
    icon->setPixmap(QIcon(QStringLiteral(":/icons/qspacemidi.png"))
                        .pixmap(72, 72));
    icon->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    icon->setFixedWidth(88);

    auto *title = new QLabel(this);
    title->setTextFormat(Qt::RichText);
    title->setText(QStringLiteral("<span style='font-size:16pt; font-weight:600;'>"
                                  "QSpaceMIDI</span><br>")
                   + tr("Version %1").arg(QStringLiteral(QSPACEMIDI_VERSION)));

    auto *body = new QLabel(this);
    body->setTextFormat(Qt::RichText);
    body->setWordWrap(true);
    body->setOpenExternalLinks(true);
    body->setText(
        tr("<p>Maps the six axes and the two buttons of a 3Dconnexion "
           "SpaceNavigator to MIDI control change messages, for use with "
           "Renoise and any other MIDI host.</p>"
           "<p>No 3DxWare driver is needed: the device is read directly "
           "through evdev on Linux, IOKit HID on macOS and raw HID on "
           "Windows.</p>"
           "<p>Copyright &copy; 2013-2026 Jakob Flierl<br>"
           "Licensed under the "
           "<a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'>"
           "GNU LGPL, version 2.1 or later</a>.</p>"
           "<p>Built with Qt %1 and "
           "<a href='https://github.com/atsushieno/libremidi'>libremidi</a>.<br>"
           "<a href='https://github.com/koppi/qspacemidi'>"
           "github.com/koppi/qspacemidi</a></p>")
            .arg(QStringLiteral(QT_VERSION_STR)));

    auto *textLayout = new QVBoxLayout;
    textLayout->setSpacing(8);
    textLayout->addWidget(title);
    textLayout->addWidget(body);
    textLayout->addStretch(1);

    auto *contentLayout = new QHBoxLayout;
    contentLayout->setSpacing(12);
    contentLayout->addWidget(icon);
    contentLayout->addLayout(textLayout, 1);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 12);
    layout->addLayout(contentLayout, 1);
    layout->addWidget(buttons);

    setMinimumWidth(460);
}

} // namespace qspacemidi
