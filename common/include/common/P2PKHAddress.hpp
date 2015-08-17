/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#ifndef COIN_P2PKH_ADDRESS_HPP
#define COIN_P2PKH_ADDRESS_HPP

#include <common/PubKeyHash.hpp>

namespace Coin {

enum class Network;
class PublicKey;

class P2PKHAddress {

public:

    // Default constructor
    P2PKHAddress();

    // Constructor from public keys
    P2PKHAddress(Network network, const PubKeyHash & pubKeyHash);

    // Constructor from members
    P2PKHAddress(Network network, const Coin::PublicKey & pk);

    // Factory from Base58CheckEncoding
    static P2PKHAddress fromBase58CheckEncoding(const QString & encoded);

    // Base58CheckEncode
    QString toBase58CheckEncoding() const;

    // Generate output script
    //uchar_vector toScriptPubKey() const;

    // Getters and setters
    Network network() const;
    void setNetwork(Network network);

    PubKeyHash pubKeyHash() const;
    void setPubKeyHash(const PubKeyHash & pubKeyHash);

private:

    // Network to which this address corresponds
    Network _network;

    // Pub key hash
    PubKeyHash _pubKeyHash;

};

}

#endif // COIN_P2PKH_ADDRESS_HPP