/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/viewmodel/Payor.hpp>

namespace joystream {
namespace core {
namespace viewmodel {

    Payor::Payor(QObject * parent, const Payor::Status & status)
        : QObject(parent)
        , _state(status.state())
        , _utxo(status.utxo())
        , _contractTxId(status.contractTxId())
        , _numberOfSignatures(status.numberOfSignatures()) {

        // Create view models for all channels
        std::vector<Payor::Channel::Status> channelStatuses = status.channels();

        for(int i = 0;i < channelStatuses.size();i++)
            _channels.append(new ChannelViewModel(this, i, channelStatuses[i]));
    }

    /**
    PayorViewModel::~PayorViewModel() {

        for(QVector<ChannelViewModel *>::const_iterator
            i = _channelViewModels.constBegin(),
            end = _channelViewModels.constEnd();i != end;i++)
            delete (*i);
    }
    */

    void Payor::update(const Payor::Status & status) {

        if(_state != status.state()) {
            _state = status.state();
            emit stateChanged(status.state());
        }

        if(_utxo != status.utxo()) {
            _utxo = status.utxo();
            emit utxoChanged(status.utxo());
        }

        if(_contractTxId != status.contractTxId()) {
            _contractTxId = status.contractTxId();
            emit contractTxIdChanged(_contractTxId);
        }

        if(_numberOfSignatures != status.numberOfSignatures()) {
            _numberOfSignatures = status.numberOfSignatures();
            emit numberOfSignaturesChanged(status.numberOfSignatures());
        }

        // Update each channel view model with new status
        std::vector<Payor::Channel::Status> channelStatuses = status.channels();

        Q_ASSERT(_channels.size() == channelStatuses.size());

        for(int i = 0;i < _channels.size();i++)
            _channels[i]->update(channelStatuses[i]);
    }

    Payor::State Payor::state() const {
        return _state;
    }

    Coin::UnspentP2PKHOutput Payor::utxo() const {
        return _utxo;
    }

    Coin::TransactionId Payor::contractTxId() const {
        return _contractTxId;
    }

    quint32 Payor::numberOfSignatures() const {
        return _numberOfSignatures;
    }

    QVector<ChannelViewModel *> Payor::channelViewModels() const {
        return _channels;
    }

}
}
}
