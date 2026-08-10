/*
 * autostart.cpp - start qspacemidi with the desktop session
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "autostart.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

#if defined(Q_OS_WIN)
#include <QSettings>
#endif

namespace qspacemidi::autostart {

namespace {

/*! Absolute path of the running executable, quoted where needed. */
QString executablePath()
{
#if defined(Q_OS_MACOS)
    // Point at the bundle rather than at the binary inside it, so that
    // launchd starts the application the way Finder would.
    QDir dir(QCoreApplication::applicationDirPath()); // Contents/MacOS
    if (dir.cdUp() && dir.cdUp() && dir.absolutePath().endsWith(QStringLiteral(".app"))) {
        return dir.absolutePath();
    }
#endif
    return QCoreApplication::applicationFilePath();
}

#if defined(Q_OS_WIN)

const char *const RunKey = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const char *const ValueName = "QSpaceMIDI";

#elif defined(Q_OS_MACOS)

QString launchAgentPath()
{
    const QString home = QDir::homePath();
    return home + QStringLiteral("/Library/LaunchAgents/org.koppi.qspacemidi.plist");
}

#else

/*! ~/.config/autostart/qspacemidi.desktop, per the XDG autostart spec. */
QString autostartFilePath()
{
    const QString config
        = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return config + QStringLiteral("/autostart/qspacemidi.desktop");
}

#endif

} // namespace

bool isSupported()
{
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS) || defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    return true;
#else
    return false;
#endif
}

QString location()
{
#if defined(Q_OS_WIN)
    return QString::fromLatin1(RunKey) + QStringLiteral("\\") + QString::fromLatin1(ValueName);
#elif defined(Q_OS_MACOS)
    return launchAgentPath();
#else
    return autostartFilePath();
#endif
}

bool isEnabled()
{
#if defined(Q_OS_WIN)
    QSettings run(QString::fromLatin1(RunKey), QSettings::NativeFormat);
    return run.contains(QString::fromLatin1(ValueName));
#else
    return QFileInfo::exists(location());
#endif
}

bool setEnabled(bool enabled)
{
#if defined(Q_OS_WIN)
    QSettings run(QString::fromLatin1(RunKey), QSettings::NativeFormat);
    if (enabled) {
        const QString command = QStringLiteral("\"%1\" --minimized")
                                    .arg(QDir::toNativeSeparators(executablePath()));
        run.setValue(QString::fromLatin1(ValueName), command);
    } else {
        run.remove(QString::fromLatin1(ValueName));
    }
    run.sync();
    return run.status() == QSettings::NoError && isEnabled() == enabled;
#else
    const QString path = location();

    if (!enabled) {
        if (!QFile::exists(path)) {
            return true;
        }
        return QFile::remove(path);
    }

    if (!QDir().mkpath(QFileInfo(path).absolutePath())) {
        return false;
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
#if defined(Q_OS_MACOS)
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
           "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
        << "<plist version=\"1.0\">\n"
        << "<dict>\n"
        << "\t<key>Label</key>\n"
        << "\t<string>org.koppi.qspacemidi</string>\n"
        << "\t<key>ProgramArguments</key>\n"
        << "\t<array>\n"
        << "\t\t<string>/usr/bin/open</string>\n"
        << "\t\t<string>-a</string>\n"
        << "\t\t<string>" << executablePath() << "</string>\n"
        << "\t\t<string>--args</string>\n"
        << "\t\t<string>--minimized</string>\n"
        << "\t</array>\n"
        << "\t<key>RunAtLoad</key>\n"
        << "\t<true/>\n"
        << "</dict>\n"
        << "</plist>\n";
#else
    out << "[Desktop Entry]\n"
        << "Type=Application\n"
        << "Version=1.0\n"
        << "Name=QSpaceMIDI\n"
        << "Comment=Map a 3Dconnexion SpaceNavigator to MIDI CC messages\n"
        << "Exec=\"" << executablePath() << "\" --minimized\n"
        << "Icon=qspacemidi\n"
        << "Terminal=false\n"
        << "X-GNOME-Autostart-enabled=true\n";
#endif
    out.flush();
    const bool ok = file.error() == QFile::NoError;
    file.close();
    return ok;
#endif
}

} // namespace qspacemidi::autostart
