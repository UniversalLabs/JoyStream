/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/JoiningContract.hpp>
#include <protocol_wire/MessageType.hpp>

#include <common/PublicKey.hpp>

namespace joystream {
namespace protocol_wire {

    JoiningContract::JoiningContract() {
    }

    JoiningContract::JoiningContract(const Coin::PublicKey & contractPk, const Coin::PubKeyHash &finalPkHash)
        : _contractPk(contractPk)
        , _finalPkHash(finalPkHash) {
    }

    bool JoiningContract::operator==(const JoiningContract & rhs) const {
        return rhs.contractPk() == rhs.contractPk() &&
               rhs.finalPkHash() == rhs.finalPkHash();
    }

    JoiningContract::JoiningContract(std::istream & stream) {
        stream >> _contractPk >> _finalPkHash;
    }

    MessageType JoiningContract::messageType() const {
        return MessageType::joining_contract;
    }

    uint32_t JoiningContract::length() const {
        return Coin::PublicKey::compressedLength() + Coin::PubKeyHash::rawLength();
    }

    void JoiningContract::write(std::ostream & stream) const {
        stream << _contractPk << _finalPkHash;
    }

    Coin::PublicKey JoiningContract::contractPk() const {
        return _contractPk;
    }

    Coin::PubKeyHash JoiningContract::finalPkHash() const {
        return _finalPkHash;
    }

}
}
