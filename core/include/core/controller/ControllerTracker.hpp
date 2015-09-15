/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CONTROLLER_TRACKER_HPP
#define CONTROLLER_TRACKER_HPP

#include <QObject>
#include <QEventLoop>

#include <core/controller/Controller.hpp>

class ControllerTracker : public QObject
{
    Q_OBJECT

public:

    // Constructor
    ControllerTracker();

    // Add client to run
    void addClient(Controller * controller);

    // Blocks untill controllerCount==0
    void blockUntilAllControllersDone();

private:

    // Number of controllers
    int _controllerCount;

    // Underlying event loop which is executed ones per controllerCount registered
    QEventLoop _loop;

public slots:

    // Accepts signal from controllers
    void controllerClosed();

};

#endif // CONTROLLER_TRACKER_HPP
