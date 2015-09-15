/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015
 */

#include <Test.hpp>

#include <QNetworkAccessManager>

#include <core/controller/ControllerTracker.hpp>

#include <core/logger/LoggerManager.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp> // for configurations
#include <core/extension/SellerTorrentPlugin.hpp> // for configurations
#include <core/extension/PluginMode.hpp>
#include <common/Bitcoin.hpp> // defines STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE
#include <common/UnspentP2PKHOutput.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <common/Seed.hpp>
#include <gui/MainWindow.hpp>
#include <wallet/Manager.hpp>

#include <libtorrent/error_code.hpp>

#include <QNetworkAccessManager>

void Test::init() {

    _wallet_seed_counter = 0;
    _dns_seed_counter = 0;

}

void Test::download_and_streaming() {

    /**
     * SCENARIO: Downloading & streaming
     * =================================


    // Create a controller tracker
    ControllerTracker controllerTracker;

    // Load torrent
    libtorrent::torrent_info torrentInfo = load_torrent(TORRENT_FILE);

    // Buyers
    Controller * loneBuyer = create_controller(controllerConfiguration,
                                                manager,
                                                true,
                                                true,
                                                torrentInfo,
                                                QString("lone_buyer"));

    controllerTracker.addClient(loneBuyer);

    // Sellers
    add_sellers_with_plugin(controllerConfiguration,
                            manager,
                            controllerTracker,
                            false,
                            true,
                            torrentInfo,
                            QVector<SellerTorrentPlugin::Configuration>()

                            << SellerTorrentPlugin::Configuration(false,
                                                                  10, // Minimum piece price (satoshi)
                                                                  2*3600, // Minimum lock time on refund (seconds)
                                                                  BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                                                  1, // Max #seller
                                                                  17*60) // Maximum contract confirmation delay (seconds)
                            );

    */
}

void Test::paid_uploading() {

    // Network io access manager
    QNetworkAccessManager manager;

    // Create a controller tracker
    ControllerTracker controllerTracker;

    // Load default configurations for the controller
    Controller::Configuration controllerConfiguration;

    // Load torrent
    libtorrent::torrent_info torrentInfo = load_torrent(LITTLE_SIMZ_FILE);

    // Sellers
    Controller * loneSeller = create_controller(controllerConfiguration,
                                                &manager,
                                                true,
                                                true,
                                                torrentInfo,
                                                QString("lone_seller"));

    controllerTracker.addClient(loneSeller);

    // Buyers
    add_buyers_with_plugin(controllerConfiguration,
                           &manager,
                           controllerTracker,
                           true,
                           true,
                           torrentInfo,
                           QVector<BuyerTorrentPlugin::Configuration>()
                           << BuyerTorrentPlugin::Configuration(false,
                                                                20, // Maximum piece price (satoshi)
                                                                4*3600, // Maximum lock time on refund (seconds)
                                                                10000, // BitCoinRepresentation NOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                                1) // #sellers
//                           << BuyerTorrentPlugin::Configuration(false,
//                                                                88, // Maximum piece price (satoshi)
//                                                                5*3600, // Maximum lock time on refund (seconds)
//                                                                10000, // BitCoinRepresentationNOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
//                                                                1) // #sellers

                           );

    // Block
    controllerTracker.blockUntilAllControllersDone();
}

libtorrent::torrent_info Test::load_torrent(const char * path) {

    libtorrent::error_code ec;
    libtorrent::torrent_info torrentInfo(path, ec);

    if(ec) {
        qDebug() << "Invalid torrent file 1: " << ec.message().c_str();
        throw std::runtime_error("Could not load torrent file");
    }

    return torrentInfo;
}

Controller::Torrent::Configuration Test::create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name) {

    return Controller::Torrent::Configuration(torrentInfo.info_hash()
                                              ,torrentInfo.name()
                                              ,(QString(TORRENT_LOCATION) + name).toStdString()
                                              ,std::vector<char>()
                                              ,libtorrent::add_torrent_params::flag_update_subscribe
                                              //+libtorrent::add_torrent_params::flag_auto_managed
                                              ,&torrentInfo);
}

Controller * Test::create_controller(Controller::Configuration controllerConfiguration,
                               QNetworkAccessManager * manager,
                               bool show_gui,
                               bool use_stdout_logg,
                               libtorrent::torrent_info & torrentInfo,
                               const QString & name) {

    // Create logging category
    QLoggingCategory * category = global_log_manager.createLogger(name, use_stdout_logg, false);

    // Load wallet
    QString walletFile = QString(WALLET_LOCATION) + name + QString("_wallet.sqlite3");

    // Create wallet if it does not exist
    if(!QFile(walletFile).exists()) {

        qDebug() << "Creating a fresh wallet " << walletFile;

        // Get predefined seed
        Coin::Seed seed = Coin::Seed::testSeeds[_wallet_seed_counter];
        Q_ASSERT(_wallet_seed_counter <= Coin::Seed::testSeeds.size());

        // Create wallet
        Wallet::Manager::createNewWallet(walletFile, BITCOIN_NETWORK, seed);
    }

    // WE MUST INCREMENT THIS REGARDLESS OF WHETHER A NEW WALLET IS ACTUALLY CREATED
    // BECAUSE A GIVEN CALL SHOULD ALWAYS GIVE THE SAME SEED!
    _wallet_seed_counter++;

    // Load wallet
    Wallet::Manager * wallet = new Wallet::Manager(walletFile);

    // Initiliaze
    wallet->BLOCKCYPHER_init(manager);

    // Create controller: Dangling, but we don't care
    Controller * controller = new Controller(controllerConfiguration, wallet, manager, *category);

    if(show_gui) {
        MainWindow * view = new MainWindow(controller, wallet);
        view->show();
    }

    return controller;
}


void Test::add_sellers_with_plugin(Controller::Configuration controllerConfiguration,
                                   QNetworkAccessManager * manager,
                                   ControllerTracker & controllerTracker,
                                   bool show_gui,
                                   bool use_stdout_logg,
                                   libtorrent::torrent_info & torrentInfo,
                                   const QVector<SellerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create name
        QString name = QString("seller_") + QString::number(i+1);

        /**
        // Show view
        if(showView)
            _view.show();

        // Update wallet balance in ui
        _view.updateWalletBalance(_wallet->lastComputedZeroConfBalance());
        */

        // Create controller and torrent configuration
        Controller * controller = create_controller(controllerConfiguration, manager, show_gui, use_stdout_logg, torrentInfo, name);

        // Create torrent configuration
        Controller::Torrent::Configuration torrentConfiguration = create_torrent_configuration(torrentInfo, name);

        // Grab configuration
        const SellerTorrentPlugin::Configuration & pluginConfiguration = configurations[i];

        // Add to controller
        controller->addTorrent(torrentConfiguration, pluginConfiguration);

        // Track controller
        controllerTracker.addClient(controller);
    }
}

void Test::add_buyers_with_plugin(Controller::Configuration controllerConfiguration,
                            QNetworkAccessManager * manager,
                            ControllerTracker & controllerTracker,
                            bool show_gui,
                            bool use_stdout_logg,
                            libtorrent::torrent_info & torrentInfo,
                            const QVector<BuyerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create controller name
        QString name = QString("buyer_") + QString::number(i+1);

        // Create controller and torrent configuration
        Controller * controller = create_controller(controllerConfiguration, manager, show_gui, use_stdout_logg, torrentInfo, name);

        // Create torrent configuration
        Controller::Torrent::Configuration torrentConfiguration = create_torrent_configuration(torrentInfo, name);

        // Grab configuration
        BuyerTorrentPlugin::Configuration pluginConfiguration = configurations[i];

        // Amount needed to fund contract (satoshies)
        quint64 minFunds = Payor::minimalFunds(torrentInfo.num_pieces(),
                                               pluginConfiguration.maxPrice(),
                                               pluginConfiguration.numberOfSellers(),
                                               pluginConfiguration.maxFeePerKb());

        // Get funding output - this has to be grabbed from wallet/chain later
        //Coin::UnspentP2PKHOutput utxo; // = controller->wallet().getUtxo(minFunds, 1);
        Coin::UnspentP2PKHOutput utxo = controller->wallet()->BLOCKCYPHER_lock_one_utxo(minFunds);

        // Check that an utxo was indeed found
        if(utxo.value() == 0) {

            qDebug() << "No utxo found with value no less than:" << minFunds;

            //throw std::exception("No utxo found with sufficient funds.");

            controller->addTorrent(torrentConfiguration);

        } else {

            // Add to controller
            controller->addTorrent(torrentConfiguration, pluginConfiguration, utxo);

        }

        // Track controller
        controllerTracker.addClient(controller);
    }
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
