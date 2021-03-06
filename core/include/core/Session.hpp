/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_SESSION_HPP
#define JOYSTREAM_CORE_SESSION_HPP

#include <protocol_session/protocol_session.hpp>

#include <libtorrent/socket.hpp>

#include <QObject>

namespace joystream {
namespace core {

class Selling;
class Buying;
class Connection;

/**
 * @brief Handle for session
 * @note Detect expiry of this object by listening to the Torrent::torrentPluginRemoved signal
 */
class Session : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Session(const protocol_session::SessionMode & mode,
            const protocol_session::SessionState & state,
            Selling * selling,
            Buying * buying);

    static Session * create(const protocol_session::status::Session<libtorrent::tcp::endpoint> &);

    ~Session();

    protocol_session::SessionMode mode() const noexcept;

    protocol_session::SessionState state() const noexcept;

    /**
     * @brief Checks existence of selling handle
     * @return Whether selling handle is set
     */
    bool sellingSet() const noexcept;

    /**
     * @brief Returns selling handle if present
     * @throws exception::HandleNotSet if handle is not present, i.e. !sellingSet()
     * @return Selling handle
     */
    Selling * selling() const;

    /**
     * @brief buyingSet
     * @return Whether buying handle is set
     */
    bool buyingSet() const noexcept;

    /**
     * @brief Returns buying handle if present
     * @throws exception::HandleNotSet if handle is not present, i.e. !buyingSet()
     * @return Buying handle
     */
    Buying * buying() const;

signals:

    void modeChanged(protocol_session::SessionMode);

    void stateChanged(protocol_session::SessionState);

private:

    friend class TorrentPlugin;

    void update(const protocol_session::status::Session<libtorrent::tcp::endpoint> &);

    // Session mode
    protocol_session::SessionMode _mode;

    // Current state of session
    protocol_session::SessionState _state;

    /// Substates

    // Selling mode
    std::unique_ptr<Selling> _selling;

    // Buying mode
    std::unique_ptr<Buying> _buying;

    // If mode has not changed, then status is updated, if it has, then old substate is discarded
    // (if it was buying or selling) and mode change signal is emitted
    void updateSubstate(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status);
};

}
}

#endif // JOYSTREAM_CORE_SESSION_HPP
