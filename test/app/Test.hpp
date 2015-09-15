/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>
#include <QNetworkAccessManager>

#include <core/controller/Controller.hpp> // cant forward declare inner class

#include <libtorrent/torrent_info.hpp> // can this be forward declared perhaps?

class ControllerBarrier;

class Test : public QObject
{
    Q_OBJECT

private slots:

     // Will be called before each testfunction is executed.
    void init();

    /**
     * Test routines
     */

    void download_and_streaming();

    void paid_uploading();

public:

    Controller * create_controller(const Controller::Configuration & configuration,
                                   bool show_gui,
                                   bool use_stdout_logg,
                                   libtorrent::torrent_info & torrentInfo,
                                   const QString & name);

    void add_buyers_with_plugin(const Controller::Configuration & configuration,
                                ControllerBarrier & barrier,
                                bool show_gui,
                                bool use_stdout_logg,
                                libtorrent::torrent_info & torrentInfo,
                                const QVector<BuyerTorrentPlugin::Configuration> & configurations);

    void add_sellers_with_plugin(const Controller::Configuration & configuration,
                                 ControllerBarrier & barrier,
                                 bool show_gui,
                                 bool use_stdout_logg,
                                 libtorrent::torrent_info & torrentInfo,
                                 const QVector<SellerTorrentPlugin::Configuration> & configurations);

private:

    // Counters used to pick correct dns and wallet seed for given session
    // **Both are reset between each test**
    int _wallet_seed_counter = 0,
        _dns_seed_counter = 0;

    // Manages HTTP access
    QNetworkAccessManager _manager;

};

libtorrent::torrent_info load_torrent(const char * path);

Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name);

#endif // TEST_HPP
