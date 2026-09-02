/*
 * singleinstance.cpp - keep only one qspacemidi running per user
 *
 * Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include "singleinstance.h"

#include <QAbstractSocket>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>
#include <QLockFile>

namespace qspacemidi {

namespace {

/*! How long to wait for the running instance to answer, in milliseconds. */
constexpr int HandshakeTimeout = 300;

/*!
 * \brief A name unique to this application and this user.
 *
 * The home directory keeps two users on one machine - or the same user in
 * two desktop sessions with different homes - from sharing an instance,
 * while staying within the length a Unix domain socket path allows.
 */
QString instanceKey()
{
    const QByteArray seed = (QCoreApplication::organizationName() + QLatin1Char('/')
                             + QCoreApplication::applicationName() + QLatin1Char('@')
                             + QDir::homePath())
                                .toUtf8();
    const QByteArray digest =
        QCryptographicHash::hash(seed, QCryptographicHash::Sha1).toHex().left(16);
    return QStringLiteral("qspacemidi-") + QString::fromLatin1(digest);
}

} // namespace

SingleInstance::SingleInstance(QObject *parent)
    : QObject(parent)
    , m_serverName(instanceKey())
{
}

bool SingleInstance::tryToRun(bool raiseExisting)
{
    // Two cold starts in the same instant - autostart plus a click at login -
    // could both find no server and both start listening.  A lock file around
    // the check makes that sequence atomic; if the lock cannot be taken we
    // carry on regardless, a rare double start beating a refusal to run.
    QLockFile lock(QDir::temp().filePath(m_serverName + QStringLiteral(".lock")));
    lock.tryLock(HandshakeTimeout * 3);

    // Hand off to a running instance: connect to its server and tell it
    // whether to bring its window forward.  Returns true when a live server
    // answered.
    const auto handOff = [this, raiseExisting] {
        QLocalSocket socket;
        socket.connectToServer(m_serverName);
        if (!socket.waitForConnected(HandshakeTimeout)) {
            return false;
        }
        socket.write(raiseExisting ? "activate\n" : "ping\n");
        socket.flush();
        socket.waitForBytesWritten(HandshakeTimeout);
        socket.disconnectFromServer();
        if (socket.state() != QLocalSocket::UnconnectedState) {
            socket.waitForDisconnected(HandshakeTimeout);
        }
        return true;
    };

    if (handOff()) {
        return false;
    }

    // Nobody answered.  A socket file left behind by a crashed instance
    // would stop us from listening, so clear it first; this is a no-op when
    // the name is free, and on Windows named pipes it does nothing at all.
    QLocalServer::removeServer(m_serverName);

    m_server = new QLocalServer(this);
    m_server->setSocketOptions(QLocalServer::UserAccessOption);
    connect(m_server, &QLocalServer::newConnection, this, [this] {
        while (QLocalSocket *client = m_server->nextPendingConnection()) {
            connect(client, &QLocalSocket::disconnected, client, &QObject::deleteLater);
            connect(client, &QLocalSocket::readyRead, this, [this, client] {
                // The command is one newline-terminated line; wait for all
                // of it in the unlikely event it arrives in pieces.
                QByteArray command = client->property("command").toByteArray();
                command += client->readAll();
                if (!command.contains('\n')) {
                    client->setProperty("command", command);
                    return;
                }
                // "activate" wants the window up front; "ping" only checks
                // that we are here, so the mapping keeps running untouched.
                if (command.startsWith("activate")) {
                    emit activationRequested();
                }
                client->disconnectFromServer();
            });
        }
    });

    if (m_server->listen(m_serverName)) {
        return true;
    }

    // The name was taken between the hand-off attempt and here - another
    // instance won the race.  If it is reachable, defer to it after all.
    if (m_server->serverError() == QAbstractSocket::AddressInUseError && handOff()) {
        return false;
    }

    qWarning("QSpaceMIDI: could not start the single-instance server (%s); "
             "a second launch will start another instance.",
             qUtf8Printable(m_server->errorString()));
    return true;
}

} // namespace qspacemidi
