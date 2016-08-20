/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2016
 */

#ifndef JOYSTREAM_CORE_PAYEE_HPP
#define JOYSTREAM_CORE_PAYEE_HPP

#include <QObject>

namespace joystream {
namespace paymentchannel {
    class Payee;
}
namespace core {

class Payee : public QObject {

    Q_OBJECT

private:

    Payee(const paymentchannel::Payee &);

public:

    // The number of payments which have been successfully made
    quint64 numberOfPaymentsMade() const noexcept;

    // Payment channel lock time
    quint32 lockTime() const noexcept;

    // Price (#satoshies) increment per payment
    quint64 price() const noexcept;

    // Amount (#satoshies) assigned to contract output
    quint64 funds() const noexcept;

    // Amount (#satoshies) used in fee for settlement
    quint64 settlementFee() const noexcept;

signals:

    // Changed the number of payments which have been successfully made
    void numberOfPaymentsMadeChanged(quint64);

    // Cahnged payment channel lock time
    void lockTimeChanged(quint32);

    // Changed price (#satoshies) increment per payment
    void priceChanged(quint64);

    // Changed amount (#satoshies) assigned to contract output
    void fundsChanged(quint64);

    // Cahnged amount (#satoshies) used in fee for settlement
    void settlementFeeChanged(quint64);

private:

    friend class CBStateMachine;

    void update(const paymentchannel::Payee &);

    // The number of payments which have been successfully made
    quint64 _numberOfPaymentsMade;

    // Payment channel lock time
    quint32 _lockTime;

    // Price (#satoshies) increment per payment
    quint64 _price;

    // Amount (#satoshies) assigned to contract output
    quint64 _funds;

    // Amount (#satoshies) used in fee for settlement
    quint64 _settlementFee;
};

}
}
#endif // JOYSTREAM_CORE_PAYEE_HPP