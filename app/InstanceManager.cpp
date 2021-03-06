/* Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <InstanceManager.hpp>

InstanceManager::InstanceManager(const QString & name, QObject *parent) :
    QObject(parent), _isMain(false), _name(name), _semaphore(name, 1, QSystemSemaphore::Open)
{
    QObject::connect(&_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));

    // Blocking to synchronize across multiple instances
    if(!_semaphore.acquire()) {
        // This should only happen as a result of a system error
        throw std::runtime_error("instance manager unable to acquire semaphore");
    }

    _client.connectToServer(_name);

    // Try to connect to server
    if(_client.waitForConnected(3000)){

        // Server is already running so there is an instance already running
        _client.disconnectFromServer();
        _isMain = false;

    } else {

        _isMain = true;

        // Remove server from previous session if application crashed
        QLocalServer::removeServer(_name);

        // Create local server
        if(!_server.listen(_name)) {
            throw std::runtime_error("instance manager unable to start local server");
        }
    }

    _semaphore.release();
}

void InstanceManager::incomingConnection() {
    // Server received a connection from another instance trying to start

    QLocalSocket *remoteSocket = _server.nextPendingConnection();

    // Disconnect the client
    if(remoteSocket) remoteSocket->close();

    // Signal the application to main window to appear
    emit activate();
}
