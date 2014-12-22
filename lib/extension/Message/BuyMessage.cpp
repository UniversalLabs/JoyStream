#include "BuyMessage.hpp"

MessageType BuyMessage::getMessageType() const {
    return MessageType::buy;
}

quint32 BuyMessage::rawPayloadLength() const {
    return 0;
}

void BuyMessage::toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & extendedMessageStream) const {

    // Write extended message id
    extendedMessageStream << mapping.id(MessageType::buy);
}
