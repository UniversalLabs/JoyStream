/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_PAYMENT_HPP
#define JOYSTREAM_WIRE_PAYMENT_HPP

#include <wire/ExtendedMessagePayload.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace wire {

    class Payment : public ExtendedMessagePayload {

    public:

        // Defualt constructor
        Payment();

        // Constructor from members
        Payment(const Coin::Signature & sig);

        // Constructor based on raw payload
        // NB: Substitute with static factory in future, so that you cannot create stale
        // payload objects if there is an error in the reading from stream
        Payment(QDataStream & stream, quint8 lengthOfSignature);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters and setters
        Coin::Signature sig() const;
        void setSig(const Coin::Signature & sig);

    private:

        // Buyer payment signature
        Coin::Signature _sig;
    };

}
}

#endif // JOYSTREAM_WIRE_PAYMENT_HPP
