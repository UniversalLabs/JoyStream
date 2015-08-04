/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Payee.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

quint8 Payee::encodeState(State state) {

    switch(state) {
        case State::waiting_for_payor_information: return 0;
        case State::has_all_information_required: return 1;

        defualt:
            Q_ASSERT(false);
    }
}

Payee::State Payee::decodeState(quint8 state) {

    switch(state) {
        case 0: return State::waiting_for_payor_information;
        case 1: return State::has_all_information_required;

        default:
            Q_ASSERT(false);
    }
}

Payee::Payee(quint64 id,
             State state,
             quint64 numberOfPaymentsMade,
             const Coin::Signature & lastValidPayerPaymentSignature,
             quint64 price,
             quint64 funds,
             quint32 maximumNumberOfSellers,
             quint64 payeeContractWalletKeyId,
             quint64 payeePaymentWalletKeyId,
             const Coin::TransactionId & contractTransactionId,
             quint32 contractOutput,
             const Coin::PublicKey & payerContractPublicKey,
             const Coin::PublicKey & payerFinalPublicKey,
             const Coin::TransactionId & paymentTransactionId)
    : _id(id)
    , _state(state)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _lastValidPayerPaymentSignature(lastValidPayerPaymentSignature)
    , _price(price)
    , _funds(funds)
    , _maximumNumberOfSellers(maximumNumberOfSellers)
    , _payeeContractWalletKeyId(payeeContractWalletKeyId)
    , _payeePaymentWalletKeyId(payeePaymentWalletKeyId)
    , _contractTransactionId(contractTransactionId)
    , _contractOutput(contractOutput)
    , _payerContractPublicKey(payerContractPublicKey)
    , _payerFinalPublicKey(payerFinalPublicKey)
    , _paymentTransactionId(paymentTransactionId) {
}

QSqlQuery Payee::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Payee (\
        id                              INTEGER,\
        state                           INTEGER     NOT NULL,\
        numberOfPaymentsMade            INTEGER     NOT NULL,\
        lastValidPayerPaymentSignature  BLOB,\
        price                           INTEGER     NOT NULL,\
        funds                           INTEGER,\
        maximumNumberOfSellers          INTEGER     NOT NULL,\
        payeeContractWalletKeyId        INTEGER     NOT NULL,\
        contractTransactionId           BLOB,\
        contractOutput                  INTEGER,\
        payerContractPublicKey          BLOB,\
        payerFinalPublicKey             BLOB,\
        paymentTransactionId            BLOB,\
        PRIMARY KEY(id),\
        FOREIGN KEY payeeContractWalletKeyId REFERENCES WalletKey(index),\
        FOREIGN KEY payeePaymentWalletKeyId REFERENCES WalletKey(index),\
        FOREIGN KEY paymentTransactionId REFERENCES Transaction(id)\
    )");
}

QSqlQuery Payee::unboundedInsertQuery() {

    return QSqlQuery("\
    INSERT INTO Payer \
    (id, state, numberOfPaymentsMade, lastValidPayerPaymentSignature, price, funds, maximumNumberOfSellers, payeeContractWalletKeyId, contractTransactionId, contractOutput, payerContractPublicKey, payerFinalPublicKey, paymentTransactionId)\
    VALUES\
    (:id, :state, :numberOfPaymentsMade, :lastValidPayerPaymentSignature, :price, :funds, :maximumNumberOfSellers, :payeeContractWalletKeyId, :contractTransactionId, :contractOutput, :payerContractPublicKey, :payerFinalPublicKey, :paymentTransactionId)\
    ");
}

QSqlQuery Payee::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // Bind values to query fields
    query.bindValue(":id", _id);
    query.bindValue(":state", encodeState(_state));
    query.bindValue(":numberOfPaymentsMade", _numberOfPaymentsMade);
    query.bindValue(":lastValidPayerPaymentSignature", Coin::uchar_vector_to_QByteArray(_lastValidPayerPaymentSignature.raw()));
    query.bindValue(":price", _price);
    query.bindValue(":funds", _funds);
    query.bindValue(":maximumNumberOfSellers", _maximumNumberOfSellers);
    query.bindValue(":payeeContractWalletKeyId", _payeeContractWalletKeyId);
    query.bindValue(":contractTransactionId", _contractTransactionId.toByteArray());
    query.bindValue(":contractOutput", _contractOutput);
    query.bindValue(":payerContractPublicKey", Coin::uchar_vector_to_QByteArray(_payerContractPublicKey.raw()));
    query.bindValue(":payerFinalPublicKey", Coin::uchar_vector_to_QByteArray(_payerFinalPublicKey.raw()));
    query.bindValue(":paymentTransactionId", _paymentTransactionId.toByteArray());

    return query;
}
