/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 7 2016
 */

#include <BuyingNavigator.hpp>

bool BuyingNavigator::Fixture::validatePayment(const Coin::Signature & payment, int numberOfPayments) const {

    joystream::paymentchannel::Payee payee(numberOfPayments,
                                           peerToSellMode.message().terms().minLock(),
                                           peerToSellMode.message().terms().minPrice(),
                                           contractPrepared.value(),
                                           peerToSellMode.message().terms().settlementFee(),
                                           buyModeStarted.terms().refundFee(),
                                           contractPrepared.anchor(),

                                           // Private key is not relevant, as we wil only be validating payment signatures, not generating
                                           Coin::KeyPair(joiningContract.message().contractPk(), Coin::PrivateKey()),
                                           joiningContract.message().finalPkHash(),
                                           contractPrepared.contractKeyPair().pk(),
                                           contractPrepared.finalPkHash(),
                                           Coin::Signature());

    return payee.registerPayment(payment);
}

BuyingNavigator::BuyingNavigator(const Fixture & fixture)
    : _fixture(fixture) {
}

void BuyingNavigator::toBuyMode(protocol_statemachine::CBStateMachine * machine) {
    machine->process_event(_fixture.buyModeStarted);
}

void BuyingNavigator::toSellerHasJoined(protocol_statemachine::CBStateMachine * machine) {
    machine->process_event(_fixture.peerToSellMode);
    machine->process_event(_fixture.inviteSeller);
    machine->process_event(_fixture.joiningContract);
}

void BuyingNavigator::toProcessingPiece(protocol_statemachine::CBStateMachine * machine) {
    toSellerHasJoined(machine);
    machine->process_event(_fixture.contractPrepared);
    machine->process_event(_fixture.requestPiece);
    machine->process_event(_fixture.fullPiece);
}
