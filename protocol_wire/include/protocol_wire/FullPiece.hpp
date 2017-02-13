/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_FULLPIECE_HPP
#define JOYSTREAM_PROTOCOL_WIRE_FULLPIECE_HPP

#include <protocol_wire/Message.hpp>
#include <protocol_wire/PieceData.hpp>

namespace joystream {
namespace protocol_wire {

    class FullPiece : public Message {

    public:

        FullPiece();
        FullPiece(const PieceData &);

        virtual ~FullPiece() {}

        bool operator==(const FullPiece &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;

        // Getters
        PieceData pieceData() const;

        void setPieceData(const PieceData&);
    private:

        // Piece data;
        PieceData _pieceData;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_FULLPIECE_HPP
