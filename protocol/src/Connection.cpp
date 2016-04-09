/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>
#include <wire/MessageType.hpp>
#include <wire/Observe.hpp>
#include <wire/Buy.hpp>
#include <wire/Sell.hpp>
#include <wire/JoinContract.hpp>
#include <wire/JoiningContract.hpp>
#include <wire/Ready.hpp>
#include <wire/RequestFullPiece.hpp>
#include <wire/FullPiece.hpp>
#include <wire/Payment.hpp>
#include <protocol/statemachine/event/Recv.hpp>

namespace joystream {
namespace protocol {

    template <class ConnectionIdType>
    Connection<ConnectionIdType>::Connection(const ConnectionIdType & connectionId,
                                             const statemachine::CBStateMachine::InvitedToOutdatedContract & invitedToOutdatedContract,
                                             const statemachine::CBStateMachine::InvitedToJoinContract & invitedToJoinContract,
                                             const statemachine::CBStateMachine::Send & send,
                                             const statemachine::CBStateMachine::ContractIsReady & contractIsReady,
                                             const statemachine::CBStateMachine::PieceRequested & pieceRequested,
                                             const statemachine::CBStateMachine::InvalidPieceRequested & invalidPieceRequested,
                                             const statemachine::CBStateMachine::PeerInterruptedPayment & peerInterruptedPayment,
                                             const statemachine::CBStateMachine::ValidPayment & validPayment,
                                             const statemachine::CBStateMachine::InvalidPayment & invalidPayment,
                                             const statemachine::CBStateMachine::SellerJoined & sellerJoined,
                                             const statemachine::CBStateMachine::SellerInterruptedContract & sellerInterruptedContract,
                                             const statemachine::CBStateMachine::ReceivedFullPiece & receivedFullPiece)
        : _connectionId(connectionId)
        , _machine(invitedToOutdatedContract,
                   invitedToJoinContract,
                   send,
                   contractIsReady,
                   pieceRequested,
                   invalidPieceRequested,
                   peerInterruptedPayment,
                   validPayment,
                   invalidPayment,
                   sellerJoined,
                   sellerInterruptedContract,
                   receivedFullPiece,
                   0) {
    }

    template <class ConnectionIdType>
    void Connection<ConnectionIdType>::process(const wire::ExtendedMessagePayload * message) {

        // Get message type
        joystream::wire::MessageType messageType = message->messageType();

        // Call relevant message handler
        switch(messageType) {

            case joystream::wire::MessageType::observe:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Observe>(dynamic_cast<const joystream::wire::Observe *>(message)));
                break;
            case joystream::wire::MessageType::buy:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Buy>(dynamic_cast<const joystream::wire::Buy *>(message)));
                break;
            case joystream::wire::MessageType::sell:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Sell>(dynamic_cast<const joystream::wire::Sell *>(message)));
                break;
            case joystream::wire::MessageType::join_contract:
                _machine.process_event(statemachine::event::Recv<joystream::wire::JoinContract>(dynamic_cast<const joystream::wire::JoinContract *>(message)));
                break;
            case joystream::wire::MessageType::joining_contract:
                _machine.process_event(statemachine::event::Recv<joystream::wire::JoiningContract>(dynamic_cast<const joystream::wire::JoiningContract *>(message)));
                break;
            case joystream::wire::MessageType::ready:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Ready>(dynamic_cast<const joystream::wire::Ready *>(message)));
                break;
            case joystream::wire::MessageType::request_full_piece:
                _machine.process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(dynamic_cast<const joystream::wire::RequestFullPiece *>(message)));
                break;
            case joystream::wire::MessageType::full_piece:
                _machine.process_event(statemachine::event::Recv<joystream::wire::FullPiece>(dynamic_cast<const joystream::wire::FullPiece *>(message)));
                break;
            case joystream::wire::MessageType::payment:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Payment>(dynamic_cast<const joystream::wire::Payment *>(message)));
                break;

            default:
                assert(false);
        }
    }
}
}
