#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <ctime>
#include <QTimer>

#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/UnspentOutputSet.hpp>

#include <CoinQ/CoinQ_coinparams.h>

#include <signal.h>

#include <boost/asio/impl/src.hpp>

bool shuttingDown = false;

void handleSignal(int sig)
{
    shuttingDown = true;
}

using joystream::bitcoin::SPVWallet;

void sendAllCoinsToBlockCypherFaucet(joystream::bitcoin::SPVWallet & wallet) {
    // mwmabpJVisvti3WEP5vhFRtn3yqHRD9KNP
}

void sendAllCoinsToTPFaucet(joystream::bitcoin::SPVWallet & wallet) {
    /* https://tpfaucet.appspot.com/ */
    auto faucet = Coin::P2PKHAddress::fromBase58CheckEncoding("n2eMqTT929pb1RDNuqEnxdaLau1rxy3efi");

    Coin::Transaction Tx;

    auto balance = wallet.balance();

    Tx.addOutput(Coin::TxOut(balance, Coin::P2PKHScriptPubKey(faucet.pubKeyHash()).serialize()));

    auto funds = wallet.lockOutputs(balance);

    funds.finance(Tx, Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, false));

    wallet.broadcastTx(Tx);
}

int main(int argc, char *argv[])
{
    if(getenv("DEBUG") != NULL) {
        INIT_LOGGER("netsync.log");
    }

    QCoreApplication a(argc, argv);

    joystream::bitcoin::SPVWallet wallet("test-store.db", "test-blocktree.dat", Coin::Network::testnet3);

    std::cout << "Wallet Seed Words:\n" << wallet.getSeedWords() << std::endl;

    auto recvAddresses = wallet.listReceiveAddresses();

    if(recvAddresses.size() == 0) {
        wallet.generateReceiveAddress();
    }

    std::cout << "Wallet contains the following receive addresses:\n";

    for(auto addr : recvAddresses) {
        std::cout << addr.toBase58CheckEncoding().toStdString() << std::endl;
    }

    std::cout << std::endl << "Balance: " << wallet.unconfirmedBalance() << std::endl;

    // Timer used for checking if a signal was caught..
    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, [&wallet, &timer](){
        if(!shuttingDown) return;
        timer->stop();
        std::cout << "Stopping..." << std::endl;
        wallet.stopSync();
    });

    timer->start(2000);
    signal(SIGINT, &handleSignal);
    signal(SIGTERM, &handleSignal);

    QObject::connect(&wallet, &SPVWallet::storeUpdateFailed, [](std::string err){
        std::cerr << "StoreError: " << err << std::endl;
        shuttingDown = true;
    });

    QObject::connect(&wallet, &SPVWallet::blockTreeUpdateFailed, [&wallet](std::string err){
        // This is something we can't really recover from, and we have to disconnect from the peer
        std::cerr << "BlockTree Error: " << err << std::endl;
        shuttingDown = true;
    });

    QObject::connect(&wallet, &SPVWallet::statusMessageUpdated, [](std::string message){
        std::cout << message << std::endl;
    });

    QObject::connect(&wallet, &SPVWallet::balanceChanged, [](uint64_t confirmed, uint64_t unconfirmed){
        std::cout << "confirmed balance: " << confirmed << std::endl;
        std::cout << "0-conf    balance: " << unconfirmed << std::endl;
    });

    QObject::connect(&wallet, &SPVWallet::disconnected, [](){
        std::cout << "Disconnected" << std::endl;
        shuttingDown = true;
    });

    QObject::connect(&wallet, &SPVWallet::statusChanged, [&wallet, &a, &timer](SPVWallet::wallet_status_t status){
       if(status == SPVWallet::wallet_status_t::SYNCHED)  {
           std::cout << std::endl << "Wallet Synched" << std::endl;

           //std::cout << "Sending all coins to Faucet\n";
           //sendAllCoinsToTPFaucet(wallet);
       }

       if(status == SPVWallet::wallet_status_t::SYNCHING_HEADERS) {
           std::cout << "synching headers" << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::SYNCHING_BLOCKS) {
           std::cout << "synching blocks" << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::CONNECTING) {
           std::cout << "Connecting.." << std::endl;
       }

       if(status == SPVWallet::wallet_status_t::OFFLINE) {
           // Exit application when netsync stops
           /* when computer wakes from sleep for some reason status
           doesn't goto OFFLINE after going to DISCONNECTED */

            if(shuttingDown) {
                std::cout << "Application Exiting..\n";
                a.exit();
            }
       }
    });

    // First argument should be a node running testnet3 on default port 18333
    // https://tpfaucet.appspot.com/
    // 52.4.156.236
    // testnet-seed.bitcoin.petertodd.org

    if(argc > 1) {
        wallet.sync(argv[1], 18333);
    } else {
        // if no peer provided use a peer by dns lookup
        wallet.sync("testnet-seed.bitcoin.petertodd.org", 18333);
    }

    // return a.exec(); // << results in seg fault and other undefined behaviour?!

    // No problems with this approach!
    int ret = a.exec();
    std::cout << "Exited application event loop with code: " << ret << std::endl;
    return ret;
}
