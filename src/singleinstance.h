/*
 * singleinstance.h - keep only one qspacemidi running per user
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#ifndef QSPACEMIDI_SINGLEINSTANCE_H
#define QSPACEMIDI_SINGLEINSTANCE_H

#include <QObject>
#include <QString>

class QLocalServer;

namespace qspacemidi {

/*!
 * \brief A per-user lock that hands a second launch back to the first.
 *
 * The primary instance listens on a QLocalServer whose name is derived from
 * the application and the current user, so two desktop sessions on the same
 * machine each get their own instance.  A later launch connects to that
 * server, which makes the primary instance emit activationRequested() so it
 * can raise its window, and then exits.
 */
class SingleInstance : public QObject
{
    Q_OBJECT

public:
    explicit SingleInstance(QObject *parent = nullptr);

    /*!
     * \brief Becomes the primary instance, or defers to the running one.
     *
     * \param raiseExisting  when another instance is found, whether to ask
     *                       it to show its window.  A launch that only wants
     *                       the mapping running - autostart's --minimized -
     *                       passes false and leaves the window as it was.
     *
     * \return true when no other instance was running and we are now
     *         listening for later launches; the caller should carry on.
     *         false when another instance answered; the caller should exit.
     */
    bool tryToRun(bool raiseExisting = true);

signals:
    /*! A later launch asked the primary instance to show its window. */
    void activationRequested();

private:
    QString m_serverName;
    QLocalServer *m_server = nullptr;
};

} // namespace qspacemidi

#endif // QSPACEMIDI_SINGLEINSTANCE_H
