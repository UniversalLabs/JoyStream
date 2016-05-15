/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLING_HPP
#define JOYSTREAM_PROTOCOL_SELLING_HPP

#include <protocol_session/Session.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace Coin {
    class typesafeOutPoint;
    class Signature;
}

namespace joystream {
namespace protocol_session {
namespace detail {

template <class ConnectionIdType>
class Buying;

template <class ConnectionIdType>
class Observing;

template <class ConnectionIdType>
class Selling {

public:

    Selling(Session<ConnectionIdType> *,
            const RemovedConnectionCallbackHandler<ConnectionIdType> &,
            const GenerateKeyPairsCallbackHandler &,
            const GenerateP2PKHAddressesCallbackHandler &,
            const LoadPieceForBuyer<ConnectionIdType> &,
            const ClaimLastPayment<ConnectionIdType> &,
            const AnchorAnnounced<ConnectionIdType> &,
            const SellingPolicy &,
            const protocol_wire::SellerTerms &);

    //// Connection level client events

    // Adds connection, and return the current number of connections
    uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

    // Connection with given id has been removed
    void removeConnection(const ConnectionIdType &);

    // Data for given piece has been loaded
    void pieceLoaded(const ConnectionIdType & id, const protocol_wire::PieceData &, int);

    //// Connection level state machine events

    void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);
    void invitedToOutdatedContract(const ConnectionIdType &);
    void invitedToJoinContract(const ConnectionIdType &);
    void contractPrepared(const ConnectionIdType &, quint64, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::PubKeyHash &);
    void pieceRequested(const ConnectionIdType &, int);
    void invalidPieceRequested(const ConnectionIdType &);
    void paymentInterrupted(const ConnectionIdType &);
    void receivedValidPayment(const ConnectionIdType &, const Coin::Signature &);
    void receivedInvalidPayment(const ConnectionIdType &, const Coin::Signature &);

    //// Change mode

    void leavingState();

    //// Change state

    // Starts a stopped session by becoming fully operational
    void start();

    // Immediately closes all existing connections
    void stop();

    // Accepts new connections, but only advertises mode.
    void pause();

    //// Miscellenous

    // Time out processing hook
    // NB: Later give some indication of how to set timescale for this call
    void tick();

    // Update terms
    void updateTerms(const protocol_wire::SellerTerms &);

    //// Getters and setters

    protocol_wire::SellerTerms terms() const;

private:

    // Client removes connection with givne id with given cause
    void removeConnection(const ConnectionIdType &, DisconnectCause);

    // Join if terms are good enough, buyer on given connection
    // NB: Assumes in state protocol_statemachine::Invited
    void tryToJoin(detail::Connection<ConnectionIdType> *);

    // Loads. .....
    // NB: Assumes in state protocol_statemachine::LoadingPiece
    void tryToLoadPiece(detail::Connection<ConnectionIdType> *);

    //// Members

    // Reference to core of session
    Session<ConnectionIdType> * _session;

    // Callback handlers
    RemovedConnectionCallbackHandler<ConnectionIdType> _removedConnection;
    GenerateKeyPairsCallbackHandler _generateKeyPairs;
    GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddresses;
    LoadPieceForBuyer<ConnectionIdType> _loadPieceForBuyer;
    ClaimLastPayment<ConnectionIdType> _claimLastPayment;
    AnchorAnnounced<ConnectionIdType> _anchorAnnounced;

    // Controls behaviour of session
    SellingPolicy _policy;

    // Terms for selling
    protocol_wire::SellerTerms _terms;
};

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Selling.cpp>

#endif // JOYSTREAM_PROTOCOL_SELLING_HPP
