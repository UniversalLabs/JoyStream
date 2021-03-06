/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_KEY_HPP
#define WALLET_KEY_HPP

#include <common/PrivateKey.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;
class QSqlRecord;

namespace Wallet {
namespace Key {

    /**
    enum class Purpose {
        Receive,
        ContractChange,
        PayerInContractOutput,
        ContractFinal,
        PayeeInContractOutput,
        ContractPayment
    };
    */

    // Primary key
    typedef quint64 PK;

    struct Record {

        Record();
        Record(PK index,
               const Coin::PrivateKey & sk,
               const QDateTime & generated,
               bool issued);
        Record(const QSqlRecord & record);

        // The (hd) index number is wallet
        PK _index;

        // Private key
        Coin::PrivateKey _privateKey;

        // Purpose for which key was generated
        //Purpose _purpose;

        // When key was generated
        QDateTime _generated;

        // Has key been issued,
        // that is been returned to wallet user in any way.
        // An issued may still not actually have been used, but
        // it should still not be reissued (
        bool _issued;

    };

    // Query which creates table corresponding to entity
    bool createTable(QSqlDatabase db);

    // Prepared insert query
    bool insert(QSqlDatabase db, const Record & r);

    // Finds biggest index in table, throws exception if table is empty
    quint64 maxIndex(QSqlDatabase db);

    // Counts the number of keys in the wallet
    quint64 numberOfKeysInWallet(QSqlDatabase db);

    // Conversion routines
    //static quint8 encodePurpose(Purpose purpose);
    //static Purpose decodePurpose(quint8 encoded);

    // Checks whether record exists with given primary key, if so, it is written to r
    bool exists(QSqlDatabase db, const PK & pk, Record & r);
    bool exists(QSqlDatabase db, const PK & pk);
}
}

#endif // WALLET_KEY_HPP
