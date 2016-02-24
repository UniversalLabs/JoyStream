/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <blockcypher/UTXOManager.hpp>
#include <blockcypher/Address.hpp>

#include <common/P2PKHScriptPubKey.hpp>

namespace BlockCypher {

    UTXOManager::UTXOManager(WebSocketClient * wsclient, Coin::Network network)
        : _wsClient(wsclient),
          _network(network),
          _balance(0),
          _balance_zero_conf(0)
    {
        // connect signals from websocket client to our private slots
        QObject::connect(_wsClient, &WebSocketClient::txArrived, [this](const TX & tx){
            TxResult r = processTx(tx);
            updateUtxoSets(r);
            updateBalances(true);
        });
    }

    UTXOManager* UTXOManager::createManager(WebSocketClient * wsClient, Coin::Network network)
    {
        if(!wsClient) throw std::runtime_error("UTXOManager requires a websocket client pointer");

        UTXOManager* manager = new UTXOManager(wsClient, network);

        if(!manager) throw std::runtime_error("unable to create a UTXO Manager");

        return manager;
    }

    std::vector<QString> UTXOManager::batchAddresses(const std::set<QString> &addresses) {
        // Create a vector of batches of addresses (QString of semicolon separated list)
        // The maximum number of elements that can be batched in a single call is 100.
        std::vector<QString> batches;
        uint counter = 0;
        uint batchNo;
        for(const QString &addr : addresses) {
            batchNo =  counter / 100;
            if(batches.empty() || batches.size() < batchNo) {
                batches.push_back(addr);
            } else {
                batches[batchNo] += ";" + addr;
            }
            counter++;
        }

        return batches;
    }

    std::vector<Address> UTXOManager::fetchAddresses(Client * restClient, const std::vector<QString> & batches) {
        std::vector<Address> addresses;

        for(const QString & batch : batches) {
            std::vector<Address> response = restClient->addressEndPoint(batch);
            addresses.insert(addresses.end(), response.begin(), response.end());
        }

        return addresses;
    }

    bool UTXOManager::refreshUtxoState(Client* restClient, const std::list<Coin::P2PKHAddress> &p2pkhaddresses) {

        std::set<QString> addresses;

        for(auto & addr : p2pkhaddresses)
            addresses.insert(addr.toBase58CheckEncoding());

        std::vector<Address> fetchedAddresses;

        try {
            fetchedAddresses = fetchAddresses(restClient, batchAddresses(addresses));
        } catch(std::exception &e) {
            return false;
        }

        std::vector<TxResult> results = processTxRefs(fetchedAddresses);

        _unconfirmedUtxoSet.clear();
        _confirmedUtxoSet.clear();

        for(auto & r : results) updateUtxoSets(r);

        updateBalances(true);

        for(auto addr : addresses) {
            addAddress(addr);
        }

        return true;
    }

    std::set<UTXO> UTXOManager::getUtxoSet(uint64_t minValue, uint32_t minConfirmations, uint32_t currentBlockHeight) {
        std::set<UTXO> selectedUtxos;
        uint32_t totalValue = 0;
        uint32_t confirmations;

        // Always try to pick confirmed utxos
        for(const UTXO &utxo : _confirmedUtxoSet) {
            // is utxo loacked ?
            if(_lockedUtxoSet.find(utxo) != _lockedUtxoSet.end()) continue;

            confirmations = currentBlockHeight - utxo.height() + 1;

            // Does it meet minimal confirmations requirement
            if(minConfirmations > 0 && confirmations < minConfirmations) continue;

            selectedUtxos.insert(utxo);
            totalValue += utxo.value();

            if(totalValue >= minValue) {
                break;
            }
        }

        // select unconfirmed utxo if minimal confirmations requirement == 0
        // and we still haven't reached minimum required value
        if(minConfirmations == 0 && totalValue < minValue) {
            for(const UTXO &utxo : _unconfirmedUtxoSet) {
                if(_lockedUtxoSet.find(utxo) != _lockedUtxoSet.end()) continue;

                selectedUtxos.insert(utxo);
                totalValue += utxo.value();

                if(totalValue >= minValue) {
                    break;
                }
            }
        }

        if(totalValue < minValue) {
            // not enough utxo
            return std::set<UTXO>();
        }

        // Lock and return the selected utxos
        lockUtxoSet(selectedUtxos);
        return selectedUtxos;
    }

    void UTXOManager::releaseUtxoSet(std::set<UTXO> utxos) {
        for(auto & utxo : utxos) _lockedUtxoSet.erase(utxo);
    }

    void UTXOManager::lockUtxoSet(std::set<UTXO> utxos) {
        for(auto & utxo : utxos) _lockedUtxoSet.insert(utxo);
    }

    std::vector<TxResult> UTXOManager::processTxRefs(const std::vector<Address> &addresses) {
        std::vector<TxResult> results;

        for(const Address & address : addresses) {

            std::vector<TXRef> txrefs;
            txrefs.insert(txrefs.end(), address._txrefs.begin(), address._txrefs.end());
            txrefs.insert(txrefs.end(), address._unconfirmed_txrefs.begin(), address._unconfirmed_txrefs.end());

            for(const TXRef &t : txrefs) {
                if(t._tx_output_n >= 0 && !t._double_spend) {
                    Coin::typesafeOutPoint outpoint(t._tx_hash, t._tx_output_n);

                    try{
                        Coin::P2PKHScriptPubKey script = Coin::P2PKHScriptPubKey::deserialize(t._script);
                        Coin::P2PKHAddress addr(_network, script.pubKeyHash());

                        UTXO utxo(addr.toBase58CheckEncoding(), outpoint, t._value, t._block_height);

                        TxResult result;
                        result.confirmations(t._confirmations);

                        if(t._spent) {
                            result.destroys(utxo);
                        }else {
                            result.creates(utxo);
                        }

                        results.push_back(result);

                    } catch(std::exception &e) {
                        continue;
                    }
                }
            }
        }
        return results;
    }

    bool UTXOManager::addAddress(const Coin::P2PKHAddress & address) {
        return addAddress(address.toBase58CheckEncoding());
    }

    bool UTXOManager::addAddress(const QString & addr) {
        //return false if address already exists
        if(hasAddress(addr)) return false;

        //insert address and add tx_confirmation event to websocket filters
        _addresses.insert(addr);
        _wsClient->addEvent(Event::makeTxConfirmation(2, addr));
        return true;
    }

    TxResult UTXOManager::processTx(const TX & tx) {
        TxResult result;

        Coin::TransactionId txid = Coin::TransactionId::fromRPCByteOrder(tx.hash().toStdString());

        if(tx.double_spend()) return result;

        result.confirmations(tx.confirmations());

        for(const TXInput & input : tx.inputs()) {
            if(input.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            // We expect only one address in output.addresses() array
            QString addr = input.addresses()[0];

            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(Coin::TransactionId::fromRPCByteOrder(input.prev_hash().toStdString()), input.index());
            UTXO utxo(addr, outpoint, input.value(), tx.block_height());

            result.destroys(utxo);
        }

        int32_t index = -1;
        for(const TXOutput & output : tx.outputs()) {
            index++;

            if(output.script_type() != ScriptType::pay_to_pubkey_hash) continue;
            //we expect only one address in output.addresses() array
            QString addr = output.addresses()[0];
            if(!hasAddress(addr)) continue;

            Coin::typesafeOutPoint outpoint(txid, index);
            UTXO utxo(addr, outpoint, output.value(), tx.block_height());

            if(output.spent_by()) {
                result.destroys(utxo);
            } else {
                result.creates(utxo);
            }
        }

        return result;
    }

    bool UTXOManager::hasAddress(const QString &address) {
        return _addresses.find(address) != _addresses.end();
    }

    void UTXOManager::updateUtxoSets(const TxResult & r) {

        for(const UTXO &utxo : r.creates()) {
            if(utxo.confirmed()) {
                // erasing and inserting to update the block height of the utxo
                // incase of reorg
                _confirmedUtxoSet.erase(utxo);
                _confirmedUtxoSet.insert(utxo);

                _unconfirmedUtxoSet.erase(utxo);
            } else {
                _unconfirmedUtxoSet.insert(utxo);
            }
        }

        for(const UTXO &utxo: r.destroys()) {
            _confirmedUtxoSet.erase(utxo);
            _unconfirmedUtxoSet.erase(utxo);
        }
    }

    void UTXOManager::updateBalances(bool notify) {

        uint64_t confirmedBalance = 0;

        // Calculate new confirmed balance
        for(const UTXO & utxo: _confirmedUtxoSet) {
            confirmedBalance += utxo.value();
        }

        // Calculate new unconfirmed balance
        uint64_t unconfirmedBalance = confirmedBalance;
        for(const UTXO & utxo: _unconfirmedUtxoSet) {
            unconfirmedBalance += utxo.value();
        }

        //if changed emit signals
        if(_balance != confirmedBalance || _balance_zero_conf != unconfirmedBalance) {
            _balance = confirmedBalance;
            _balance_zero_conf = unconfirmedBalance;
            if(notify) emit balanceChanged(confirmedBalance, unconfirmedBalance);
        }

    }
}
