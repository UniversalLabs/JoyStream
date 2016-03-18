/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <Test.hpp>

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/Buying.hpp>
#include <protocol/statemachine/Observing.hpp>
#include <protocol/wire/Observe.hpp>
#include <protocol/wire/MessageType.hpp>

#include <iostream>

void Test::initTestCase() {

    // InvitedToOutdatedContract
    _invitedToOutdatedContract = [this](void) {
        _hasBeenInvitedToOutdatedContract = true;
    };

    // InvitedToJoinContract
    _invitedToJoinContract = [this](const ContractInvitation & invitation) {
        _hasBeenInvitedToJoinContract = true;
        _invitation = invitation;
    };

    // Send
    _send = [this](const wire::ExtendedMessagePayload * m) {

        std::cout << "Sending message: " <<  wire::MessageTypeToString(m->messageType()) << std::endl;

        _messageSent = true;
        _sendMessage = m;
    };

    // ContractIsReady
    _contractIsReady = [this](const Coin::typesafeOutPoint & o) {
        _contractHasBeenPrepared = true;
        _anchor = o;
    };

    // PieceRequested
    _pieceRequested = [this](int i) {
        _pieceHasBeenRequested = true;
        _piece = i;
    };
}

void Test::init() {
    resetCallbackState();
}

void Test::clientToSellMode() {

    // Get machine into mode
    SellerTerms terms(1,2,3,4,5);
    statemachine::CBStateMachine * machine = createFreshMachineInSellMode(terms);

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Selling).name());

    // Check that sell message was sent
    QVERIFY(_messageSent);
    QCOMPARE(_sendMessage->messageType(), wire::MessageType::sell);

    // Check that same terms have been sent
    const wire::Sell * m = static_cast<const wire::Sell *>(_sendMessage);
    QCOMPARE(m->terms(), terms);
}

void Test::clientToBuyMode() {

    // Get machine into mode
    BuyerTerms terms(1,2,3,4,5);
    statemachine::CBStateMachine * machine = createFreshMachineInBuyMode(terms);

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Buying).name());

    // Check that sell message was sent
    QVERIFY(_messageSent);
    QCOMPARE(_sendMessage->messageType(), wire::MessageType::buy);

    // Check that same terms have been sent
    const wire::Buy * m = static_cast<const wire::Buy *>(_sendMessage);
    QCOMPARE(m->terms(), terms);
}

void Test::clientToObserveMode() {

    // Get machine into mode
    statemachine::CBStateMachine * machine = createFreshMachineInObserveMode();

    // Check that we are in correct state
    QCOMPARE(machine->getInnerStateName(), typeid(statemachine::Observing).name());

    // Check that sell message was sent
    QVERIFY(_messageSent);
    QCOMPARE(_sendMessage->messageType(), wire::MessageType::observe);
}

void Test::peerToSellMode() {

    // Get machine into mode
    statemachine::CBStateMachine * machine = createFreshMachineInObserveMode();

    // Recieve mode message from peer
    SellerTerms terms(1,2,3,4,5);
    uint32_t index = 17;
    wire::Sell m(terms, index);
    machine->process_event(statemachine::event::Recv<wire::Sell>(&m));

    // Check that new peer state recorded is valid
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), PeerModeAnnounced::ModeAnnounced::sell);
    QCOMPARE(announced.sellModeTerms(), terms);
    QCOMPARE(announced.index(), index);
}

void Test::peerToBuyMode() {

    // Get machine into mode
    statemachine::CBStateMachine * machine = createFreshMachineInObserveMode();

    // Recieve mode message from peer
    BuyerTerms terms(1,2,3,4,5);
    wire::Buy m(terms);
    machine->process_event(statemachine::event::Recv<wire::Buy>(&m));

    // test deep history transition at various times?
    PeerModeAnnounced announced = machine->peerAnnouncedMode();
    QCOMPARE(announced.modeAnnounced(), PeerModeAnnounced::ModeAnnounced::buy);
    QCOMPARE(announced.buyModeTerms(), terms);
}

void Test::peerToObserveMode() {

    // Get machine into mode
    statemachine::CBStateMachine * machine = createFreshMachineInObserveMode();

    // Recieve mode message from peer
    wire::Observe m;
    machine->process_event(statemachine::event::Recv<wire::Observe>(&m));

    // test deep history transition at various times?
    QCOMPARE(machine->peerAnnouncedMode().modeAnnounced(), PeerModeAnnounced::ModeAnnounced::observe);
}

statemachine::CBStateMachine * Test::createFreshMachineInObserveMode() {

    // Create and initiate state machine
    statemachine::CBStateMachine * machine = createFreshMachine();

    // Switch to observe mode
    machine->process_event(statemachine::event::ObserveModeStarted());

    return machine;
}

statemachine::CBStateMachine * Test::createFreshMachineInBuyMode(const BuyerTerms & terms) {

    // Create and initiate state machine
    statemachine::CBStateMachine * machine = createFreshMachine();

    // Switch to buy mode
    machine->process_event(statemachine::event::BuyModeStarted(terms));

    return machine;
}

statemachine::CBStateMachine * Test::createFreshMachineInSellMode(const SellerTerms & terms) {

    // Create and initiate state machine
    statemachine::CBStateMachine * machine = createFreshMachine();

    // Switch to sell mode
    machine->process_event(statemachine::event::SellModeStarted(terms));

    return machine;
}

statemachine::CBStateMachine * Test::createFreshMachine() {

    // Create
    statemachine::CBStateMachine * machine = new statemachine::CBStateMachine(_invitedToOutdatedContract, _invitedToJoinContract, _send, _contractIsReady, _pieceRequested);

    // Initiate machine
    machine->initiate();

    return machine;
}

void Test::resetCallbackState() {

    // InvitedToOutdatedContract
    _hasBeenInvitedToOutdatedContract = false;

    // InvitedToJoinContract
    _hasBeenInvitedToJoinContract = false;
    _invitation = ContractInvitation();

    // Send
    _messageSent = false;
    _sendMessage = NULL;

    // ContractIsReady
    _contractHasBeenPrepared = false;
    _anchor = Coin::typesafeOutPoint();

    // PieceRequested
    _pieceHasBeenRequested = false;
    _piece = 0;
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
