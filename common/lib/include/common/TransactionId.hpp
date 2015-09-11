/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef COIN_TRANSACTION_ID_HPP
#define COIN_TRANSACTION_ID_HPP

#include <common/UCharArray.hpp>

#define TXID_BYTE_LENGTH 32

namespace Coin {

// Define Bitcoin types as spesific length fixed uchar arrays
//typedef UCharArray<TXID_BYTE_LENGTH> TransactionId;

// Transaction id, is stored as big endian encoding,
// i.e. same encoding as on wire.
class TransactionId : public UCharArray<TXID_BYTE_LENGTH> {

public:

    TransactionId();
    TransactionId(const uchar_vector & vector);
    TransactionId(const QByteArray & array);

    // Constructor from hex encoded string
    // of little endian byte sequence.
    // OBS: each byte is reversed whend eco
    static TransactionId fromLittleEndianHex(const std::string & str);

    // Encodes as hex string,
    std::string toLittleEndianHex() const;
};

}

//Q_DECLARE_METATYPE(Coin::TransactionId)

#endif // COIN_TRANSACTION_ID_HPP
