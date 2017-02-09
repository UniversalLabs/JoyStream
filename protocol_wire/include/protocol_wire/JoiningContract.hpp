/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_JOINING_CONTRACT_HPP
#define JOYSTREAM_WIRE_JOINING_CONTRACT_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <common/PublicKey.hpp>


namespace joystream {
namespace protocol_wire {

    class JoiningContract : public ExtendedMessagePayload {

    public:

        JoiningContract();
        JoiningContract(const Coin::PublicKey &, const Coin::PubKeyHash &finalPkHash);
        JoiningContract(std::istream & stream);

        virtual ~JoiningContract() {}

        bool operator==(const JoiningContract &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual uint32_t length() const;
        virtual void write(std::ostream & stream) const;

        // Getters
        Coin::PublicKey contractPk() const;

        Coin::PubKeyHash finalPkHash() const;

        void setContractPk(const Coin::PublicKey&);

        void setFinalPkHash(const Coin::PubKeyHash&);

    private:

        // Contract output seller multisig key
        Coin::PublicKey _contractPk;

        // Payment seller output
        Coin::PubKeyHash _finalPkHash;
    };

}
}

#endif // JOYSTREAM_WIRE_JOINING_CONTRACT_HPP
