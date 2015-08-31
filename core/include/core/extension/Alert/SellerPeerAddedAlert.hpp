/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_PEER_PLUGIN_ADDED_ALERT_HPP
#define SELLER_PEER_PLUGIN_ADDED_ALERT_HPP

#include <core/extension/Alert/TorrentPluginAlert.hpp>
#include <core/extension/Alert/AlertTypes.hpp>
#include <core/extension/SellerPeerPlugin.hpp>

class SellerPeerAddedAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = SELLER_PEER_PLUGIN_ADDED_ALERT_ID;

    // Constructor from members
    SellerPeerAddedAlert(const libtorrent::sha1_hash & infoHash,
                        const libtorrent::tcp::endpoint & endPoint,
                        const SellerPeerPlugin::Status & status);

    // Copy Constructor
    SellerPeerAddedAlert(const SellerPeerAddedAlert & alert);

    // Virtual routines from libtrorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<alert> clone_impl() const;

    // Getters and setters
    libtorrent::tcp::endpoint endPoint() const;
    void setEndPoint(const libtorrent::tcp::endpoint &endPoint);

    SellerPeerPlugin::Status status() const;
    void setStatus(const SellerPeerPlugin::Status &status);

private:

    // Endpoint for peer started
    libtorrent::tcp::endpoint _endPoint;

    // Status for started peer
    SellerPeerPlugin::Status _status;
};

#endif // SELLER_PEER_PLUGIN_ADDED_ALERT_HPP
