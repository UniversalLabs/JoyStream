/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <Test.hpp>

#include "bitcoin_regtest_framework.hpp"

#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/UnspentOutputSet.hpp>
#include <common/Utilities.hpp>

#include <paymentchannel/paymentchannel.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

using namespace joystream::test::bitcoin;
using namespace joystream::paymentchannel;

void Test::initTestCase() {

    QVERIFY(regtest::init() == 0);
}

void Test::init() {

}

void Test::cleanup() {

}

void Test::cleanupTestCase() {
    // Stop bitcoind
    regtest::shutdown();
}

void Test::RefundLocking() {

    // Contract funding
    const float amount = 50000;
    std::stringstream amount_ss;
    amount_ss << amount / 100000000;
    Coin::PrivateKey fundsSk = Coin::PrivateKey::generate();
    Coin::P2PKHAddress fundsAddress(Coin::Network::regtest, fundsSk.toPublicKey().toPubKeyHash());

    std::string fundsTxId;
    QCOMPARE(regtest::send_to_address(fundsAddress.toBase58CheckEncoding().toStdString(), amount_ss.str(), fundsTxId), 0);

    // the sendtoaddress tx output index of our funds is not deterministic, it will either be 0 or 1
    // so we figure out which is the change address and assume the other one is our output
    std::string utxoJson = regtest::listunspent();
    QJsonParseError parseErr;
    QJsonDocument utxos = QJsonDocument::fromJson(QByteArray(utxoJson.c_str()), &parseErr);

    QCOMPARE(parseErr.error, QJsonParseError::ParseError::NoError);
    QCOMPARE(utxos.isArray(), true);

    auto firstUtxo = utxos.array().at(0).toObject();
    int vout = firstUtxo.value("vout").toInt();

    Coin::typesafeOutPoint fundsOutpoint(Coin::TransactionId::fromRPCByteOrder(fundsTxId), vout ^= 1);

    Coin::UnspentOutputSet funding;
    auto funds = new Coin::UnspentP2PKHOutput(Coin::KeyPair(fundsSk), fundsOutpoint, amount);
    funding.insert(funding.end(), std::shared_ptr<Coin::UnspentOutput>(funds));

    // Generate Keys for the payment channel commitment
    auto buyerSk = Coin::PrivateKey::generate();
    auto sellerSk = Coin::PrivateKey::generate();

    // For easy regtesting we will use locktime in block units
    const uint lockTimeBlocks = 20;
    Coin::RelativeLockTime locktime(Coin::RelativeLockTime::Units::Blocks, lockTimeBlocks);

    // Construct a Contract with one commitment
    Contract contract(funding);
    Commitment commitment(amount - 1000, buyerSk.toPublicKey(), sellerSk.toPublicKey(), locktime);
    contract.addCommitment(commitment);

    auto contractTx = contract.transaction();

    //std::cout << "bitcore::scriptPubKey::" << contractTx.outputs.at(0).scriptPubKey.getHex() << std::endl;

    std::string rawContractTransaction = contractTx.getSerialized().getHex();

    // Broadcast the contract
    QCOMPARE(regtest::send_raw_transaction(rawContractTransaction), 0);

    // Construct a buyer full Refund
    auto contractTxId = Coin::TransactionId::fromTx(contractTx);
    Refund refund(Coin::typesafeOutPoint(contractTxId, 0), commitment, Coin::KeyPair(buyerSk));

    // Get an unspent output to use to finance our refund transaction
    auto refundUnspentOutput = refund.getUnspentOutput();
    Coin::UnspentOutputSet refundUnspentOutputSet;
    refundUnspentOutputSet.insert(refundUnspentOutputSet.end(), std::shared_ptr<Coin::UnspentOutput>(refundUnspentOutput));

    Coin::Transaction refundTx;

    // To indicate that we accept new consensus rules (BIP68 - Relative Lock Time, we must set the transaction
    // version to be greater than 1
    refundTx.version = 2;

    // Send the refund to a bitcoind wallet controlled address
    auto refundDestination = Coin::P2PKHAddress::fromBase58CheckEncoding(QString::fromStdString(regtest::getnewaddress()));
    auto refundOutput = Coin::TxOut(amount - 2000, Coin::P2PKHScriptPubKey(refundDestination.pubKeyHash()).serialize());
    refundTx.addOutput(refundOutput);

    // finance the transaction
    refundUnspentOutputSet.finance(refundTx, Coin::SigHashType::standard());

    // Check that our tx was properly financed
    QCOMPARE(refundTx.inputs.size(), (size_t)1);

    // ... and the sequence number has been set correctly
    QCOMPARE(refundTx.inputs.at(0).sequence, locktime.toSequenceNumber());

    std::string rawRefundTransaction = refundTx.getSerialized().getHex();

    //std::cout << "bitcore::scriptSig::" << refundTx.inputs.at(0).scriptSig.getHex() << std::endl;
    //std::cout << "Transaction:" << refundTx.toJson() << std::endl;
    //std::cout << "bitcore::tx:" << rawRefundTransaction << std::endl;

    // Trying to spend the refund Tx before the locktime expires.. should fail
    QCOMPARE(regtest::send_raw_transaction(rawRefundTransaction) == 0, false);

    // Generate enough blocks for the locktime to expire
    QCOMPARE(regtest::generate_blocks(lockTimeBlocks+1), 0);

    // Trying to spend the refund after the locktime expires should work
    QCOMPARE(regtest::send_raw_transaction(rawRefundTransaction) == 0, true);

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
