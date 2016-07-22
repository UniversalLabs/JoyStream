/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#ifndef JOYSTREAM_CORE_SELLER_HPP
#define JOYSTREAM_CORE_SELLER_HPP

#include <protocol_session/protocol_session.hpp>

#include <libtorrent/socket.hpp>

#include <QObject>

namespace joystream {
namespace core {

class Seller : public QObject {

    Q_OBJECT

private:

    Seller(const protocol_session::status::Seller<libtorrent::tcp::endpoint> &);

public:

    protocol_session::SellerState state() const noexcept;

    libtorrent::tcp::endpoint connectionId() const noexcept;

signals:

    void stateChanged(protocol_session::SellerState);

private:

    friend class Buying;

    void update(const protocol_session::status::Seller<libtorrent::tcp::endpoint> &);

    protocol_session::SellerState _state;

    libtorrent::tcp::endpoint _connectionId;
};

}
}

#endif // JOYSTREAM_CORE_SELLER_HPP