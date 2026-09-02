/*
 * main.cpp - qspacemidi entry point
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "mainwindow.h"
#include "singleinstance.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QPixmap>
#include <QStyleFactory>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTranslator>

#ifndef QSPACEMIDI_VERSION
#define QSPACEMIDI_VERSION "0.0.0"
#endif

namespace {

/*! Loads the Aqua-ish style sheet shipped in the resources. */
void applyStyleSheet(QApplication &app)
{
    QFile file(QStringLiteral(":/style/qspacemidi.qss"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        app.setStyleSheet(QString::fromUtf8(file.readAll()));
    }
}

/*!
 * \brief Loads the UI translation matching \a locale, plus Qt's own.
 *
 * Qt's translations give the standard dialog buttons their local wording;
 * they are optional, since a qtbase_xx.qm is not always installed.  The
 * translators are parented to the application, which owns them from here on.
 */
void installTranslators(QApplication &app, const QLocale &locale)
{
    auto *qtTranslator = new QTranslator(&app);
    if (qtTranslator->load(locale, QStringLiteral("qtbase"), QStringLiteral("_"),
                           QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        QCoreApplication::installTranslator(qtTranslator);
    }

    auto *appTranslator = new QTranslator(&app);
    if (appTranslator->load(locale, QStringLiteral("qspacemidi"), QStringLiteral("_"),
                            QStringLiteral(":/i18n"))) {
        QCoreApplication::installTranslator(appTranslator);
    }
}

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(QStringLiteral("koppi"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("koppi.github.io"));
    QCoreApplication::setApplicationName(QStringLiteral("QSpaceMIDI"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QSPACEMIDI_VERSION));

    // Fusion plus the bundled style sheet gives the same look on every
    // platform, which is what the MIDI Navigator layout was drawn for.
    if (QStyleFactory::keys().contains(QStringLiteral("Fusion"), Qt::CaseInsensitive)) {
        QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    }
    applyStyleSheet(app);

    // QSPACEMIDI_LANG overrides the system locale, which is handy for
    // checking a translation without changing the desktop language.
    const QString forced = qEnvironmentVariable("QSPACEMIDI_LANG");
    installTranslators(app, forced.isEmpty() ? QLocale::system() : QLocale(forced));

    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/qspacemidi.png")));

    QCommandLineParser parser;
    parser.setApplicationDescription(
        QCoreApplication::translate("main",
                                    "Maps a 3Dconnexion SpaceNavigator to MIDI CC messages."));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption minimizedOption(
        {QStringLiteral("m"), QStringLiteral("minimized")},
        QCoreApplication::translate("main", "Start hidden in the system tray."));
    parser.addOption(minimizedOption);

    QCommandLineOption screenshotOption(
        QStringLiteral("screenshot"),
        QCoreApplication::translate("main",
                                    "Render the window to <file> and exit (used to "
                                    "generate the documentation screenshot)."),
        QStringLiteral("file"));
    parser.addOption(screenshotOption);

    parser.process(app);

    // Only one instance per user: a second launch asks the first to show
    // its window and then bows out.  --screenshot is exempt, it renders a
    // throwaway window and exits without touching the running instance.
    qspacemidi::SingleInstance instance;
    if (!parser.isSet(screenshotOption)
        && !instance.tryToRun(!parser.isSet(minimizedOption))) {
        return 0;
    }

    qspacemidi::MainWindow window;

    QObject::connect(&instance, &qspacemidi::SingleInstance::activationRequested,
                     &window, [&window] { window.showAndRaise(); });

    if (parser.isSet(screenshotOption)) {
        const QString path = parser.value(screenshotOption);
        window.applyDemoState();
        window.show();
        // One event loop turn so the layout settles before grabbing.
        QTimer::singleShot(0, &window, [&window, path] {
            window.applyDemoState();
            const QPixmap pixmap = window.grab();
            QCoreApplication::exit(pixmap.save(path) ? 0 : 1);
        });
        return app.exec();
    }

    if (parser.isSet(minimizedOption) && QSystemTrayIcon::isSystemTrayAvailable()) {
        // The mapping runs without the window being visible.
        QApplication::setQuitOnLastWindowClosed(false);
    } else {
        window.showAndRaise();
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        // Closing the window keeps the application alive in the tray.
        QApplication::setQuitOnLastWindowClosed(false);
    }

    return app.exec();
}
