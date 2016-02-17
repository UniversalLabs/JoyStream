#ifndef BITCOIN_BLOCKCYPHERWALLET_HPP
#define BITCOIN_BLOCKCYPHERWALLET_HPP

#include <QObject>
#include <blockcypher/UTXOManager.hpp>
#include <bitcoin/Store.hpp>

namespace joystream {
namespace bitcoin {

class BlockCypherWallet : public QObject
{
    Q_OBJECT

public:
    BlockCypherWallet(QString storePath,
                      Coin::Network network,
                      BlockCypher::Client * restClient,
                      BlockCypher::WebSocketClient * wsClient);

    // Create a new wallet with auto generated seed
    bool Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    bool Create(Coin::Seed seed);

    // Open the wallet. Will throw exception on failure.
    void Open();

    // Re-synchronize the wallet with network. This should be called
    // when websocket connectivity is lost.
    bool Sync(uint tries = 1);

signals:

public slots:

private:
    Store _store;
    std::string _storePath;

    Coin::Network _network;

    BlockCypher::Client *_restClient;
    BlockCypher::WebSocketClient *_wsClient;

    BlockCypher::UTXOManager *_utxoManager;

    // true if utxo manager is successfully initialized
    // will be false when Wallet is created, will be set to true once we
    // successfully sync for the first time
    bool _utxoManagerIsInitialized;

    // The wallet is considered in sync when we have both successfully initialized the
    // utxo manager at some point and the websocket client is connected
    bool InSync() { return _utxoManager != nullptr && _utxoManagerIsInitialized && _wsClient->isConnected(); }
};

}
}
#endif // BITCOIN_BLOCKCYPHERWALLET_HPP
