/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_BUY_HPP
#define JOYSTREAM_PROTOCOL_WIRE_BUY_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>
#include <protocol/BuyerTerms.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class Buy : public ExtendedMessagePayload {

    public:

        // Default constructor
        Buy();

        // Constructor based on members
        Buy(const joystream::protocol::BuyerTerms & terms);

        // Constructor based on raw payload
        // NB: Substitute with static factory in future, so that you cannot create stale
        // payload objects if there is an error in the reading from stream
        Buy(QDataStream & stream);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters and setters
        joystream::protocol::BuyerTerms terms() const;
        void setTerms(const joystream::protocol::BuyerTerms & terms);

    private:

        // Buyer terms
        joystream::protocol::BuyerTerms _terms;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_BUY_HPP
