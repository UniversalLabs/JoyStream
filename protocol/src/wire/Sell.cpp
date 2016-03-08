/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/wire/Sell.hpp>
#include <protocol/wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol {
namespace wire {

    Sell::Sell() {
    }

    Sell::Sell(const joystream::protocol::SellerTerms & terms, uint32_t index)
        : _terms(terms)
        , _index(index) {
    }

    Sell::Sell(QDataStream & stream) {

        quint64 price, lock, maxSellers, minContractFeePerKb, settlementFee;

        stream >> price >> lock >> maxSellers >> minContractFeePerKb >> settlementFee;

        _terms.setMinPrice(price);
        _terms.setLock(lock);
        _terms.setMaxSellers(maxSellers);
        _terms.setMinContractFeePerKb(minContractFeePerKb);
        _terms.setSettlementFee(settlementFee);
    }

    MessageType Sell::messageType() const {
        return MessageType::sell;
    }

    quint32 Sell::length() const {
        return sizeof(_terms.minPrice()) + sizeof(_terms.lock()) + sizeof(_terms.maxSellers()) + sizeof(_terms.minContractFeePerKb()) + sizeof(_terms.settlementFee());
    }

    void Sell::write(QDataStream & stream) const {
        stream << _terms.minPrice() << _terms.lock() << _terms.maxSellers() << _terms.minContractFeePerKb() << _terms.settlementFee();
    }

    joystream::protocol::SellerTerms Sell::terms() const  {
        return _terms;
    }

    void Sell::setTerms(const joystream::protocol::SellerTerms & terms) {
        _terms = terms;
    }

    uint32_t Sell::index() const {
        return _index;
    }

    void Sell::setIndex(uint32_t index) {
        _index = index;
    }
}
}
}
