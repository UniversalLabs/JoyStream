#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp>

#include <bitcoin/SPVWallet.hpp>

namespace joystream {
namespace bitcoin {

const CoinQ::CoinParams regtestParams(
        0xDAB5BFFA, 70001, "18444", 0x6f, 0xc4, "regtest", "regtest", 100000000, "testBTC", 21000000, 0,
        &sha256_2, &sha256_2,
        Coin::CoinBlockHeader(1, 1296688602, 0x207fffff, 2, uchar_vector(32, 0), uchar_vector("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")));

const CoinQ::CoinParams getCoinParamsForNetwork(Coin::Network network) {

    if(network == Coin::Network::mainnet) {
        return CoinQ::getBitcoinParams();

    } else if(network == Coin::Network::testnet3) {
        return CoinQ::getTestnet3Params();

    }else if(network == Coin::Network::regtest){
        return regtestParams;
    }

    throw std::runtime_error("network not supported");
}

SPVWallet::SPVWallet(std::string storePath, std::string blockTreeFile, Coin::Network network) :
  _storePath(storePath),
  _network(network),
  _networkSync(getCoinParamsForNetwork(network)),
  _walletStatus(UNINITIALIZED),
  _blockTreeFile(blockTreeFile),
  _blockTreeLoaded(false),
  _blockTreeError(false),
  _unconfirmedBalance(0),
  _confirmedBalance(0)
{
    // == Important == Make sure to catch all exceptions in callbacks
    // failing to do so might cause undefined behaviour in netsync

    _networkSync.subscribeStatus([this](const std::string& message)
    {
        emit NetSyncStatusMessage(message);
    });

    _networkSync.subscribeStarted([this]()
    {
        emit NetSyncStarted();
    });

    _networkSync.subscribeOpen([this]()
    {
        updateStatus(CONNECTED);
    });

    _networkSync.subscribeProtocolError([this](const std::string& error, int code)
    {
        emit ProtocolError(error);
    });

    _networkSync.subscribeConnectionError([this](const std::string& error, int code)
    {
        emit ConnectionError(error);
    });

    _networkSync.subscribeClose([this]()
    {
        updateStatus(DISCONNECTED);
    });

    _networkSync.subscribeTimeout([this]()
    {
        emit ConnectionTimeout();
    });

    _networkSync.subscribeStopped([this]()
    {
        updateStatus(OFFLINE);
    });

    _networkSync.subscribeBlockTreeError([this](const std::string& error, int code) {
        onBlockTreeError(error, code);
    });

    _networkSync.subscribeSynchingHeaders([this](){ onSynchingHeaders(); });
    _networkSync.subscribeHeadersSynched([this](){ onHeadersSynched(); });
    _networkSync.subscribeSynchingBlocks([this](){ onSynchingBlocks(); });
    _networkSync.subscribeBlocksSynched([this](){ onBlocksSynched(); });

    _networkSync.subscribeNewTx([this](const Coin::Transaction& cointx)
    {
        onNewTx(cointx);
    });

    _networkSync.subscribeMerkleTx([this](const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount)
    {
        onMerkleTx(chainmerkleblock, cointx, txindex, txcount);
    });

    _networkSync.subscribeTxConfirmed([this](const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount)
    {
        onTxConfirmed(chainmerkleblock, txhash, txindex, txcount);
    });

    _networkSync.subscribeMerkleBlock([this](const ChainMerkleBlock& chainmerkleblock)
    {
        onMerkleBlock(chainmerkleblock);
    });

    _networkSync.subscribeBlockTreeChanged([this]()
    {
        onBlockTreeChanged();
    });

}

void SPVWallet::Create() {

    if(_walletStatus != UNINITIALIZED) {
        throw std::runtime_error("wallet already opened");
    }

    if(!_store.create(_storePath, _network)){
        throw std::runtime_error("unable to create store");
    }

    LoadBlockTree();

    updateStatus(OFFLINE);
}

void SPVWallet::Create(Coin::Seed seed, uint32_t timestamp) {

    if(_walletStatus != UNINITIALIZED) {
        throw std::runtime_error("wallet already opened");
    }

    if(!_store.create(_storePath, _network, seed, timestamp == 0 ? std::time(nullptr) : timestamp)){
        throw std::runtime_error("unable to create store");
    }

    LoadBlockTree();

    updateStatus(OFFLINE);
}

void SPVWallet::Open() {

    // Only open the store once
    if(_walletStatus == UNINITIALIZED) {
        if(!_store.open(_storePath)) {
            throw std::runtime_error("failed to open wallet");
        }

        if(_network != _store.network()){
            throw std::runtime_error("store network type mistmatch");
        }

        LoadBlockTree();

        updateStatus(OFFLINE);

        recalculateBalance();
    } else {
        throw std::runtime_error("wallet already opened");
    }
}

void SPVWallet::LoadBlockTree() {
    // Creates blocktree file or loads it if found

    // Only load the blocktree once
    if(_blockTreeLoaded) return;

    _blockTreeError = false;

    _networkSync.loadHeaders(_blockTreeFile, false);

    if(!_blockTreeError) {
        _blockTreeLoaded = true;
        updateStatus(OFFLINE);
    } else {
        throw std::runtime_error("failed to load blocktree");
    }
}

void SPVWallet::Sync(std::string host, int port) {

    // Wallet must be opened or created before synching
    if(_walletStatus == UNINITIALIZED) {
        throw std::runtime_error("wallet not initialized");
    }

    // Only start synching from offline state
    if(_walletStatus != OFFLINE) return;

    updateStatus(CONNECTING);

    _networkSync.start(host, port);
}

void SPVWallet::StopSync() {
    _networkSync.stop();
}

Coin::PrivateKey SPVWallet::GetKey(bool createReceiveAddress) {
    if(_walletStatus == UNINITIALIZED) {
        throw std::runtime_error("wallet not initialized");
    }

    Coin::PrivateKey sk = _store.getKey(createReceiveAddress);

    if(createReceiveAddress) {
        updateBloomFilter();
    }

    return sk;
}

std::vector<Coin::PrivateKey> SPVWallet::GetKeys(uint32_t numKeys, bool createReceiveAddress) {
    if(_walletStatus == UNINITIALIZED) {
        throw std::runtime_error("wallet not initialized");
    }

    std::vector<Coin::PrivateKey> keys = _store.getKeys(numKeys, createReceiveAddress);
    if(createReceiveAddress) {
        updateBloomFilter();
    }
    return keys;
}

std::vector<Coin::KeyPair>
SPVWallet::GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    if(_walletStatus == UNINITIALIZED) {
        throw std::runtime_error("wallet not initialized");
    }

    std::vector<Coin::KeyPair> keyPairs = _store.getKeyPairs(num_pairs, createReceiveAddress);
    if (createReceiveAddress) {
        updateBloomFilter();
    }
    return keyPairs;
}

void SPVWallet::ReleaseKey(const Coin::PrivateKey &sk) {
    if(_walletStatus == UNINITIALIZED) {
        throw std::runtime_error("wallet not initialized");
    }

    _store.releaseKey(sk);
}

Coin::P2PKHAddress
SPVWallet::GetReceiveAddress()
{
    if(_walletStatus == UNINITIALIZED) {
        throw std::runtime_error("wallet not initialized");
    }

    Coin::P2PKHAddress addr = _store.getReceiveAddress();
    updateBloomFilter();
    return addr;
}


void SPVWallet::BroadcastTx(Coin::Transaction & cointx) {
    if(_walletStatus < CONNECTED) {
        throw std::runtime_error("cannot broadcast tx, wallet offline");
    }

    //_store.addTransaction(tx);
    _networkSync.sendTx(cointx);

    // add to the store when received from the network
    _networkSync.getTx(cointx.hash());
}

int32_t SPVWallet::bestHeight() const {
    if(_walletStatus == UNINITIALIZED) {
        return 0;
    }

    return _store.getBestHeaderHeight();
}

Coin::UnspentP2PKHOutput SPVWallet::GetOneUnspentOutput(uint64_t minValue) {
    for(Coin::UnspentP2PKHOutput & utxo : _store.getUnspentTransactionsOutputs()) {
        if(utxo.value() >= minValue)
            return utxo;
    }

    return Coin::UnspentP2PKHOutput();
}

Coin::Transaction SPVWallet::SendToAddress(uint64_t value, const Coin::P2PKHAddress &addr, uint64_t fee) {

    // Get UnspentUTXO
    Coin::UnspentP2PKHOutput utxo(GetOneUnspentOutput(value + fee));

    // Create Destination output
    Coin::P2PKHScriptPubKey destinationScript(addr.pubKeyHash());
    
    // Create Change output
    Coin::P2PKHAddress changeAddr = GetReceiveAddress();
    Coin::P2PKHScriptPubKey changeScript(changeAddr.pubKeyHash());

    // Create an unsigned Transaction
    Coin::Transaction cointx;
    cointx.addOutput(Coin::TxOut(value, destinationScript.serialize()));

    uint64_t change = utxo.value() - (value + fee);
    if(change > 0) {
        cointx.addOutput(Coin::TxOut(change, changeScript.serialize()));
    }

    // Set Input
    cointx.addInput(Coin::TxIn(utxo.outPoint().getClassicOutPoint(), uchar_vector(), 0xFFFFFFFF));

    // Sign the input
    Coin::setScriptSigToSpendP2PKH(cointx, 0, utxo.keyPair().sk());

    BroadcastTx(cointx);

    return cointx;
}

uint64_t SPVWallet::Balance() const {
    return _confirmedBalance;
}

uint64_t SPVWallet::UnconfirmedBalance() const {
    return _unconfirmedBalance;
}

void SPVWallet::updateStatus(wallet_status_t status) {
    if(_walletStatus != status) {
        _walletStatus = status;

        emit StatusChanged(status);

        if(status == OFFLINE) emit Offline();
        if(status == CONNECTING) emit Connecting();
        if(status == CONNECTED) emit Connected();
        if(status == DISCONNECTED) emit Disconnected();
        if(status == SYNCHING_HEADERS) emit SynchingHeaders();
        if(status == SYNCHING_BLOCKS)  emit SynchingBlocks();
        if(status == SYNCHED) emit BlocksSynched();
    }
}

void SPVWallet::onBlockTreeError(const std::string& error, int code) {
    // Ignore file not found error - not critical
    if(error == "Blocktree file not found.") return;

    _blockTreeError = true;
    emit BlockTreeError(error);
}

void SPVWallet::onBlockTreeChanged() {

}

void SPVWallet::onSynchingHeaders() {
    updateStatus(SYNCHING_HEADERS);
}

void SPVWallet::onHeadersSynched() {

    uint32_t startTime = _store.created();

    std::vector<bytes_t> locatorHashes;

    std::vector<std::string> hashes = _store.getLatestBlockHeaderHashes();

    if(hashes.size() > 1) {
        std::vector<std::string>::iterator i = hashes.begin();
        i++; // skip first so we can resync last block
        for(;i != hashes.end(); i++){
            locatorHashes.push_back(uchar_vector(*i));
        }
    }

    updateBloomFilter();

    _networkSync.syncBlocks(locatorHashes, startTime);
}

void SPVWallet::onSynchingBlocks() {
    updateStatus(SYNCHING_BLOCKS);
}

void SPVWallet::onBlocksSynched() {

    recalculateBalance();

    updateStatus(SYNCHED);

    _networkSync.getMempool();
}

void SPVWallet::onNewTx(const Coin::Transaction& cointx) {
    try {
        if(!transactionShouldBeStored(cointx)) return;

        _store.addTransaction(cointx);
        recalculateBalance();
    } catch(const std::exception & e) {
        emit StoreError(e.what());
    }
}

void SPVWallet::onTxConfirmed(const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount){
    try {
        _store.confirmTransaction(uchar_vector(txhash).getHex(), chainmerkleblock, txindex == 0, [this](std::string txhash){
            _networkSync.addToMempool(uchar_vector(txhash));
        });
    } catch(const std::exception & e) {
        emit StoreError(e.what());
    }
}

void SPVWallet::onMerkleTx(const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount){
    try {
        if(transactionShouldBeStored(cointx)) {
            _store.addTransaction(cointx, chainmerkleblock, txindex == 0, [this](std::string txhash){
                _networkSync.addToMempool(uchar_vector(txhash));
            });
        } else {
            if( txindex == 0 ) {
                _store.addBlockHeader(chainmerkleblock, [this](std::string txhash){
                    _networkSync.addToMempool(uchar_vector(txhash));
                });
            }
        }
    } catch(const std::exception & e) {
        emit StoreError(e.what());
    }
}

void SPVWallet::onMerkleBlock(const ChainMerkleBlock& chainmerkleblock) {
    try {
        _store.addBlockHeader(chainmerkleblock, [this](std::string txhash){
            _networkSync.addToMempool(uchar_vector(txhash));
        });
    } catch(const std::exception & e) {
        emit StoreError(e.what());
    }
}

void SPVWallet::updateBloomFilter() {
    if(_walletStatus == UNINITIALIZED) return;

    // TODO: Only update the bloom filter if elements have changed from last call
    _networkSync.setBloomFilter(makeBloomFilter(0.0001, 0, 0));
}

Coin::BloomFilter SPVWallet::makeBloomFilter(double falsePositiveRate, uint32_t nTweak, uint32_t nFlags) {
    // TODO: Keep track of elements being added - to acheive two purposes
    // 1. Can be used to filter incoming false positive transactions
    // 2. Only update the bloom filter if we are adding new elements

    // Side Note: There is a limit on the maximum size of the bloom filter
    // We can keep out keys of addresses we know have been spent and are certain
    // would never be used again by a human (keys associated with joystream p2p protocol)

    std::vector<uchar_vector> elements;

    std::list<Coin::PrivateKey> privateKeys = _store.listPrivateKeys();

    for(auto &sk : privateKeys) {
        //public key - to capture inputs that spend outputs we control
        elements.push_back(sk.toPublicKey().toUCharVector()); //compressed public key
        _bloomFilterCompressedPubKeys.insert(_bloomFilterCompressedPubKeys.begin(), elements.back());

        //pubkeyhash - to capture outputs we control
        elements.push_back(sk.toPublicKey().toPubKeyHash().toUCharVector());
        _bloomFilterPubKeyHashes.insert(_bloomFilterPubKeyHashes.begin(), elements.back());
    }

    if(elements.size() == 0) return Coin::BloomFilter();

    Coin::BloomFilter filter(elements.size(), falsePositiveRate, nTweak, nFlags);

    for(auto elm : elements) {
        filter.insert(elm);
    }

    return filter;
}

bool SPVWallet::transactionShouldBeStored(const Coin::Transaction & cointx) const {
    for(const Coin::TxIn & txin : cointx.inputs) {
        if(spendsWalletOutput(txin)) return true;
    }

    for(const Coin::TxOut & txout : cointx.outputs) {
        if(createsWalletOutput(txout)) return true;
    }

    return false;
}

bool SPVWallet::spendsWalletOutput(const Coin::TxIn & txin) const {
    try{
        uchar_vector pubkey = Coin::P2PKHScriptSig::deserialize(txin.scriptSig).pk().toUCharVector();

        if(_bloomFilterCompressedPubKeys.find(pubkey) != _bloomFilterCompressedPubKeys.end()) {
            return true;
        }

    } catch(std::runtime_error &e) {
        // not a p2pkh script sig
    }

    return false;
}

bool SPVWallet::createsWalletOutput(const Coin::TxOut & txout) const {
    try{
        // get the pubkeyhash
        Coin::P2PKHScriptPubKey script = Coin::P2PKHScriptPubKey::deserialize(txout.scriptPubKey);

        // if it matches bloom filter element we have a matching transaction
        if(_bloomFilterPubKeyHashes.find(script.pubKeyHash().toUCharVector()) != _bloomFilterPubKeyHashes.end()) {
            return true;
        }

    } catch(std::runtime_error &e) {
        // not a p2pkh pubkey script
    }

    return false;
}

void SPVWallet::recalculateBalance() {
    if(_walletStatus == UNINITIALIZED) {
        return;
    }

    uint64_t confirmed = 0;

    if(_store.getBestHeaderHeight() != 0) {
        confirmed = _store.getWalletBalance(1, _store.getBestHeaderHeight());
    }

    uint64_t unconfirmed = _store.getWalletBalance();

    if(_confirmedBalance != confirmed || _unconfirmedBalance != unconfirmed) {
        _confirmedBalance = confirmed;
        _unconfirmedBalance = unconfirmed;
        emit BalanceChanged(confirmed, unconfirmed);
    }
}

void SPVWallet::test_syncBlocksStaringAtHeight(int32_t height) {
    _networkSync.syncBlocks(height);
}

}
}
