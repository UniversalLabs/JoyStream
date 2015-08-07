/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "Payment.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Payment::Payment() {
}

Payment::Payment(const Signature & sig)
    : _sig(sig) {
}

Payment::Payment(QDataStream & stream, quint8 lengthOfSignature) {

    // Check that signature has valid length
    if(lengthOfSignature > Signature::maxLength)
        throw std::runtime_error("Maximum signature length exceeded.");

    // Read signature
    _sig.readFromStream(stream, lengthOfSignature);
}

MessageType Payment::messageType() const {
    return MessageType::payment;
}

quint32 Payment::length() const {
    return _sig.length();
}

void Payment::write(QDataStream & stream) const {
    _sig.writeToStream(stream);
}

Signature Payment::sig() const {
    return _sig;
}

void Payment::setSig(const Signature & sig) {
    _sig = sig;
}
