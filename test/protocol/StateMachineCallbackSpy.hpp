/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef STATEMACHINECALLBACKSPY
#define STATEMACHINECALLBACKSPY

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/wire/MessageType.hpp>
#include <protocol/ContractInvitation.hpp>
#include <protocol/PieceData.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace protocol {

    class BuyerTerms;
    class SellerTerms;

    namespace wire {
        class ExtendedMessagePayload;
    }
}
}

using namespace joystream::protocol::statemachine;

class StateMachineCallbackSpy {

public:

    StateMachineCallbackSpy();

    // Utility routines for creating state machines in given mode, and caller owns object
    CBStateMachine * createFreshMachineInObserveMode();
    CBStateMachine * createFreshMachineInBuyMode(const joystream::protocol::BuyerTerms & terms);
    CBStateMachine * createFreshMachineInSellMode(const joystream::protocol::SellerTerms & terms);
    CBStateMachine * createFreshMachine();

    // Reset all callback indicators
    void reset();

    // Getters
    bool hasBeenInvitedToOutdatedContract() const;

    bool hasBeenInvitedToJoinContract() const;
    joystream::protocol::ContractInvitation invitation() const;

    bool messageSent() const;
    const joystream::protocol::wire::ExtendedMessagePayload *message() const;

    bool contractHasBeenPrepared() const;
    Coin::typesafeOutPoint anchor() const;

    bool pieceHasBeenRequested() const;
    int piece() const;

    bool paymentInterrupted() const;

    bool receivedInvalidPayment() const;
    Coin::Signature invalidPaymentSignature() const;

    bool receivedValidPayment() const;
    Coin::Signature validPaymentSignature() const;

    bool sellerHasJoined() const;

    bool sellerHasInterruptedContract() const;

    bool hasReceivedFullPiece() const;

    joystream::protocol::PieceData pieceData() const;

private:

    // ***
    // Variables for encoding result of most recent callback
    // ***

    // InvitedToOutdatedContract
    bool _hasBeenInvitedToOutdatedContract;

    // InvitedToJoinContract
    bool _hasBeenInvitedToJoinContract;
    joystream::protocol::ContractInvitation _invitation;

    // Send
    CBStateMachine::Send _send;
    bool _messageSent;
    const joystream::protocol::wire::ExtendedMessagePayload * _message;

    // ContractIsReady
    bool _contractHasBeenPrepared;
    Coin::typesafeOutPoint _anchor;

    // PieceRequested
    bool _pieceHasBeenRequested;
    int _piece;

    // PeerInterruptedPayment
    bool _paymentInterrupted;

    // InvalidPayment
    bool _receivedInvalidPayment;
    Coin::Signature _invalidPaymentSignature;

    // ValidPayment
    bool _receivedValidPayment;
    Coin::Signature _validPaymentSignature;

    // SellerJoined
    bool _sellerHasJoined;

    // SellerInterruptedContract
    bool _sellerHasInterruptedContract;

    // ReceivedFullPiece
    bool _hasReceivedFullPiece;
    joystream::protocol::PieceData _pieceData;
};

#endif // STATEMACHINECALLBACKSPY

