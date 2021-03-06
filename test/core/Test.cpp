/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015
 */

#include <Test.hpp>
#include <core/core.hpp>
#include <paymentchannel/paymentchannel.hpp>
#include <boost/asio/impl/src.hpp>

using namespace joystream;
using namespace joystream::core;

void Test::initTestCase() {
    Node::registerMetaTypes();
}

void Test::integration_test() {

    Node::BroadcastTransaction callback([](const Coin::Transaction & ) -> void {});

    //Node * node = Node::create(callback);

}

void Test::listen_succeeded_alert() {

}

void Test::metadata_received_alert() {

}

void Test::metadata_failed_alert(){

}

void Test::add_torrent_alert(){

}

void Test::torrent_finished_alert(){

}

void Test::state_update_alert()
{

}

void Test::torrent_removed_alert() {

}

void Test::torrent_resumed_alert() {

}

void Test::save_resume_data_alert() {

}

void Test::save_resume_data_failed_alert() {

}

void Test::torrent_paused_alert() {

}

void Test::torrent_checked_alert() {

}

void Test::peer_connect_alert() {

}

void Test::peer_disconnected_alert() {

}

void Test::read_piece_alert() {

}

void Test::piece_finished_alert() {

}

void Test::payor() {

    paymentchannel::Payor payor_paymentchannel;

    Payor * payor = Payor::create(payor_paymentchannel);

    /// Assert properties

    QCOMPARE(payor->anchor(), payor_paymentchannel.anchor());

    /// Check signal emission after state update

    // NumberOfPaymentsMade
    {
        payor_paymentchannel.setNumberOfPaymentsMade(32);

        QSignalSpy spy(payor, SIGNAL(numberOfPaymentsMadeChanged(quint64)));

        payor->update(payor_paymentchannel);

        QCOMPARE(spy.count(), 1);

        QCOMPARE((quint64)(spy.takeFirst().at(0).toUInt()), payor_paymentchannel.numberOfPaymentsMade());
    }

    // anchor
    {
        QSignalSpy spy(payor, SIGNAL(anchorChanged(Coin::typesafeOutPoint)));

        payor_paymentchannel.setAnchor(Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                                              55));

        payor->update(payor_paymentchannel);

        QCOMPARE(spy.count(), 1);

        QCOMPARE(qvariant_cast<Coin::typesafeOutPoint>(spy.takeFirst().at(0)), payor_paymentchannel.anchor());

    }

}

void Test::payee() {

    paymentchannel::Payee payee_paymentchannel;

    Payee * payee = Payee::create(payee_paymentchannel);

    /// Assert properties

    QCOMPARE(payee->anchor(), payee_paymentchannel.contractOutPoint());

    /// Check signal emission after state update

    // anchor
    {
        QSignalSpy spy(payee, SIGNAL(anchorChanged(Coin::typesafeOutPoint)));

        payee_paymentchannel.setContractOutPoint(Coin::typesafeOutPoint(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                                                        55));

        payee->update(payee_paymentchannel);

        QCOMPARE(spy.count(), 1);

        QCOMPARE(qvariant_cast<Coin::typesafeOutPoint>(spy.takeFirst().at(0)), payee_paymentchannel.contractOutPoint());

    }

}

void Test::cbstatemachine() {

}

void Test::connection() {

}

void Test::seller() {

}

void Test::buying() {

}

void Test::selling() {

}

void Test::session() {

}

void Test::torrent_plugin() {

}

void Test::peer() {

}

void Test::torrent() {

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
