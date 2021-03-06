/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#ifndef BLOCKCYPHER_WEBSOCKET_CLIENT_HPP
#define BLOCKCYPHER_WEBSOCKET_CLIENT_HPP

#include <blockcypher/Event.hpp>

#include <QtWebSockets/QtWebSockets>
#include <QUrl>
#include <QTimer>
#include <QList>

namespace Coin {
    enum class Network;
}

/**
 * http://dev.blockcypher.com/#restful-resources
 * =============================================
 * Bitcoin      Main        wss://socket.blockcypher.com/v1/btc/main
 * Bitcoin      Testnet3    wss://socket.blockcypher.com/v1/btc/test3
 * Dogecoin     Main        wss://socket.blockcypher.com/v1/doge/main
 * Litecoin     Main        wss://socket.blockcypher.com/v1/ltc/main
 * BlockCypher	Test        wss://socket.blockcypher.com/v1/bcy/test
 */

#define BLOCKCYPHER_MAINNET_SOCKET_ENDPOINT "wss://socket.blockcypher.com/v1/btc/main"
#define BLOCKCYPHER_TESTNET3_SOCKET_ENDPOINT "wss://socket.blockcypher.com/v1/btc/test3"
#define PING_MS_INTERVAL 12000 // 12s

namespace blockcypher {

    class TX;

    class WebSocketClient : public QObject {

        Q_OBJECT

    public:

        WebSocketClient(Coin::Network network, QString token = "");

        // Endpoint for given network
        static const char * endPoint(Coin::Network network);

        // Add event filter
        void addEvent(const Event & e);

        // Connect to server
        void connect();

        // Disconnect from server
        void disconnect();

        // Whether WebSocket is connected with server
        bool isConnected() const;

        // Network client is configured for
        Coin::Network network() const { return _network; }

    signals:

        // Connected to server
        void connected();

        // Disconnected from server
        void disconnected();

        // Error on WebSocket
        void error(QAbstractSocket::SocketError e);

        // JSON parsing error
        void parseError(const QString & err);

        // API error
        void apiError(const QString & err);

        // TX payload arrived
        void txArrived(const TX & tx);

        // void blockArrived(const Block & block);

    private:

        // Hooks for WebSocket signals,
        // are private for correct visibility,
        // but websocket signals are processed
        // by lambdas scoped by this.
        void webSocketConnected();
        void webSocketDisconnected();
        void webSocketErrorOccured(QAbstractSocket::SocketError error);
        void webSocketTextMessageArrived(QString msg);


        // Attempts to sends ping to server to keep connection alive.
        // Is automatically initiated by _pingTimer,
        // should ignore request if connection is not established.
        void sendPing();

        // Sends event to server.
        // Should only be called when we are connected.
        void sendEvent(const Event &event);

        // WebSocket used for communicating
        QWebSocket _webSocket;

        // Endpoint to which we should connect,
        // is set based on the Network set in constructor
        QUrl webSocketEndpoint;

        // Network for which we are communicating with
        Coin::Network _network;

        // Drives ping message sending.
        // It fires all the time, its the sending
        // routine's responsibility to only send if
        // connection is live
        QTimer _pingTimer;

        // All events installed.
        // Events are added through addEvent,
        // even before connection has been started,
        // and list is always sent on (re)connect.
        QList<Event> _addedEvents;

        //optinal token to send with event requests
        QString _apiToken;
    };

}

#endif // BLOCKCYPHER_WEBSOCKET_CLIENT_HPP
