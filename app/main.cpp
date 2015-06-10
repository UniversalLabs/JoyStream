#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>
#include <QNetworkAccessManager>

#include <lib/Config.hpp>
#include <lib/ControllerTracker.hpp>
#include <lib/controller/Controller.hpp>
#include <lib/extension/BuyerTorrentPlugin.hpp> // for configurations
#include <lib/extension/SellerTorrentPlugin.hpp> // for configurations
#include <lib/logger/LoggerManager.hpp>
#include <lib/extension/PluginMode.hpp>
#include <lib/extension/BitCoin/BitCoin.hpp> // defines STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE

#include <common/BitCoinRepresentation.hpp>

#include <libtorrent/torrent_info.hpp>
#include <libtorrent/error_code.hpp>

#ifndef Q_MOC_RUN
#include <boost/intrusive_ptr.hpp>
#endif Q_MOC_RUN

// Forward declarations
bool updateManager();

void add_buyers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                            bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                            const QVector<BuyerTorrentPlugin::Configuration> & configurations);

void add_sellers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                             bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                             const QVector<SellerTorrentPlugin::Configuration> & configurations);

// BitSwapr entry point
void main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(QString::number(BITSWAPR_VERSION_MAJOR) + "." + QString::number(BITSWAPR_VERSION_MINOR));

    // Setup command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(APPLICATION_DESCRIPTION);

    // Add options
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption showNoUpdateOption("n", "Do not run update manager.");
    parser.addOption(showNoUpdateOption);
    QCommandLineOption showConsoleModeOption("c", "Run in console mode.");
    parser.addOption(showConsoleModeOption);
    QCommandLineOption showFreshOption("f", "Create and use a fresh parameter file.");
    parser.addOption(showFreshOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Call update manager, if allowed
    if(!parser.isSet(showNoUpdateOption)) {

        bool doRestart = updateManager();

        if(doRestart) {
            // Call update runner
        }
    }

    // Check console flag
    bool showView = false;
    if(!parser.isSet(showConsoleModeOption))
        showView = true;

    // Load default state
    Controller::Configuration controllerConfiguration;

    // If fresh flag is not passed,
    // then open existing parameter file
    if(!parser.isSet(showFreshOption)) {

        // Get name of file name
        QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
        std::string fileString = file.toStdString();

        // Check that file exists, and that it actually is a file
        if(!QFile::exists(file)) {

            qDebug() << "WARNING: parameter file"
                     << fileString.c_str()
                     << "does not exist. Try fresh run option if problem persists";

            exit(EXIT_FAILURE);

        } else // Load state from file
            controllerConfiguration = Controller::Configuration(fileString.c_str());
    }

    // Network access manager instance used by all code trying to use network
    QNetworkAccessManager manager;

    /**
     * Load torrent ================================================
     */

    libtorrent::error_code ec;
    libtorrent::torrent_info torrentInfo("C:/TORRENTS/RRB.torrent", ec);

    if(ec) {
        qDebug() << "Invalid torrent file 1: " << ec.message().c_str();
        return;
    }

    // Create a controller tracker
    ControllerTracker controllerTracker;

    /**
     * Downloading & streaming
     */

    int seller_count = 1;

    // Buyers
    add_buyers_with_plugin(controllerConfiguration, manager, controllerTracker, true , true, torrentInfo,
                           QVector<BuyerTorrentPlugin::Configuration>()

                           << BuyerTorrentPlugin::Configuration(false,
                                                                300, // Maximum piece price (satoshi)
                                                                5*3600, // Maximum lock time on refund (seconds)
                                                                BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                                seller_count) // #sellers
                           );

    // Sellers
    add_sellers_with_plugin(controllerConfiguration, manager, controllerTracker, true , true, torrentInfo,
                            QVector<SellerTorrentPlugin::Configuration>()

                            << SellerTorrentPlugin::Configuration(false,
                                                                  25, // Minimum piece price (satoshi)
                                                                  2*3600, // Minimum lock time on refund (seconds)
                                                                  BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                                                  seller_count, // Max #seller
                                                                  17*60) // Maximum contract confirmation delay (seconds)
                            );
    /**
     * Paid uploading


    // Buyers

    // Sellers

    */

    /**
     * Run
     */

    // Start event loop: this is the only Qt event loop in the entire application
    app.exec();

    qDebug() << "Application event loop exited, application closing.";
}

/**
 * Buyers =======================================================
 */
void add_buyers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                            bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                            const QVector<BuyerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create logging category
        QLoggingCategory * category = global_log_manager.createLogger(QString("buyer_") + QString::number(i+1), use_stdout_logg, false);

        // Create wallet name
        controllerConfiguration.setWalletFile(QString("c:/WALLETS/buyer_wallet") + QString::number(i+1) + QString(".dat"));

        // Create controller: Dangling, but we don't care
        Controller * buyerClient = new Controller(controllerConfiguration,
                                                  show_gui,
                                                  manager,
                                                  "Faucet http://faucet.xeno-genesis.com/",
                                                  *category);

        // Create buyer torrent configuration
        Controller::Torrent::Configuration buyerTorrentConfiguration(torrentInfo.info_hash()
                                                      ,torrentInfo.name()
                                                      ,(std::string("C:/SAVE_OUTPUT/buyer_") + std::to_string(i+1))
                                                      ,std::vector<char>()
                                                      ,libtorrent::add_torrent_params::flag_update_subscribe
                                                      //+libtorrent::add_torrent_params::flag_auto_managed
                                                      ,&torrentInfo);
        // Grab configuration
        const BuyerTorrentPlugin::Configuration & pluginConfiguration = configurations[i];

        // Amount needed to fund contract (satoshies)
        quint64 minFunds = Payor::minimalFunds(torrentInfo.num_pieces(), pluginConfiguration.maxPrice(), pluginConfiguration.numberOfSellers(), pluginConfiguration.maxFeePerKb());

        // Synch wallet
        buyerClient->wallet().synchronize();

        // Get funding output - this has to be grabbed from wallet/chain later
        UnspentP2PKHOutput utxo = buyerClient->wallet().getUtxo(minFunds, 1);

        // Check that an utxo was indeed found
        if(utxo.value() == 0) {

            qDebug() << "No utxo found with value no less than:" << minFunds;

            throw std::exception("No utxo found with sufficient funds.");

            //buyerClient->addTorrent(buyerTorrentConfiguration);

        } else {

            // Add to client
            buyerClient->addTorrent(buyerTorrentConfiguration, pluginConfiguration, utxo);

        }

        // Track controller
        controllerTracker.addClient(buyerClient);
    }
}

/**
 * Sellers =======================================================
 */
void add_sellers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                             bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                             const QVector<SellerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create logging category
        QLoggingCategory * category = global_log_manager.createLogger(QString("seller_") + QString::number(i+1), use_stdout_logg, false);

        // Create wallet name
        controllerConfiguration.setWalletFile(QString("c:/WALLETS/seller_wallet") + QString::number(i+1) + QString(".dat"));

        // Create controller: Dangling, but we don't care
        Controller * sellerClient = new Controller(controllerConfiguration,
                                                   show_gui,
                                                   manager,
                                                   "Faucet http://faucet.xeno-genesis.com/",
                                                   *category);

        // Create torrent configuration
        Controller::Torrent::Configuration sellerTorrentConfiguration(torrentInfo.info_hash()
                                                      ,torrentInfo.name()
                                                      ,(std::string("C:/SAVE_OUTPUT/seller_") + std::to_string(i+1))
                                                      ,std::vector<char>()
                                                      ,libtorrent::add_torrent_params::flag_update_subscribe
                                                      ,&torrentInfo);

        // Grab configuration
        const SellerTorrentPlugin::Configuration & pluginConfiguration = configurations[i];

        // Add to client
        sellerClient->addTorrent(sellerTorrentConfiguration, pluginConfiguration);

        // Track controller
        controllerTracker.addClient(sellerClient);
    }
}

bool updateManager() {

    // If we have just downloaded a new binary, then run it and then quit

    // Query blockchain or most recent location

    // Connect to location and check what version is there

    // If its newer, download it to particular location

    // then return to restart:

    return true;
}
