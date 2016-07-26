/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_NODE_HPP
#define JOYSTREAM_CORE_NODE_HPP

#include <core/detail/detail.hpp>
#include <core/Callbacks.hpp>
//#include <core/controller/Stream.hpp>
#include <extension/extension.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <QObject>

#include <functional>

namespace libtorrent {
    class peer_connection;
}

namespace joystream {
namespace bitcoin {
    class SPVWallet;
}
namespace extension {
    class Plugin;
}
namespace core {

class Torrent;
class TorrentIdentifier;

class Node : public QObject {

    Q_OBJECT

    /**
     * IMPORTANT
     *
     * 1. No public routines are thread safe. Not only are simultaneous calls from
     * distinct threads not safe, but even synchronized calls are not safe, as
     * one call may invalidate the view model object another thread is relying on.
     * Only use in single thread context.
     *
     * 2. Users should never trust std::shared_ptr references from core across multiple calls,
     * since the underlying objects may expire. Only keep std::weak_ptr references, and lock to get
     * std::shared_ptr reference. This will always be safe, without explicit synchronization,
     * so long as user code core calls are made on same thread.
     *
     */

public:

    /**
     * @brief Start node.
     *
     * @param broadcastTransaction called to broadcast transaction.
     * @throws exception::FailedToStartNodeException if starting failed.
     * @signal startedListeningOnPort if starting succeeded.
     */
    Node(const BroadcastTransaction & broadcastTransaction);

    /**
     * @brief Terminates all connections on all torrents, and stops all plugins, but
     *        BitTorrent and DHT daemons still run.
     *
     * @param nodeStopped callback about being actually stopped
     * @throws exception::StateIncompatibleOperation: if node is not already State::started.
     * @throws all exceptions thrown by joystream::protocol_session::Stop
     * @signal nodeStopped if node is successfully
     */
    void pause(const NodePaused & nodeStopped);

    /**
     * @brief Tries to add torrent.
     *
     * @param uploadLimit Maximum (bytes/s) upstream bandwidth utilization
     * @param downloadLimit Maximum (bytes/s) downstream bandwidth utilization
     * @param name Display name
     * @param resumeData Fast resume data
     * @param savePath Location where data is stored (loaded from) when downloading (uploading)
     * @param paused Whether initial state of torrent is paused or not
     * @param torrentReference
     * @param addedTorrent callback called after operation has been completed
     * @throws exception::TorrentAlreadyExists if torrent already has been added
     * @throws exception::StateIncompatibleOperation if node not in mode @State::started
     * @signal addedTorrent if torrent is successfully added
     */
    void addTorrent(const boost::optional<uint> & uploadLimit,
                    const boost::optional<uint> & downloadLimit,
                    const std::string & name,
                    const std::vector<char> & resumeData,
                    const std::string & savePath,
                    bool paused,
                    const TorrentIdentifier & torrentReference,
                    const AddedTorrent & addedTorrent);

    /**
     * @brief Tries to remove torrent.
     *
     * @param info_hash info_hash of torrent
     * @param RemoveTorrent callback when operation is completed
     * @throws exception::StateIncompatibleOperation if node not in mode @State::started
     * @throws exception::NoSuchTorrentExists if no such torrent exists
     * @signal removedTorrent if torrent was successfully removed
     */
    void removeTorrent(const libtorrent::sha1_hash & info_hash, const RemovedTorrent & handler);

    /**
     * Stream management

    // If there is a torrent for the given info hash, then the given stream is added to the
    // torrents streams set, and lastly a handle for the torrent is removed. Otherwise,
    // same rules as getTorrentHandle apply.
    libtorrent::torrent_handle registerStream(Stream * stream);

    // Removes stream registration
    void unRegisterStream(Stream * stream);
    void unRegisterStream(Stream * stream, Stream::Error error);

    // If torrent corresponding to the given info hash has a buyer torrent plugin installed,
    // then the plugin is requested to alter download location
    void changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex);
    */

    // Triggers an update of the status of all torrents,
    // and any changes in state will be emitted as signals.
    void updateStatus();

    // Get all info hashes
    std::set<libtorrent::sha1_hash> torrents() const;

    // Get torrents
    std::weak_ptr<Torrent> torrent(const libtorrent::sha1_hash & infoHash) const;    

signals:

    // Signals are emitted for any change in state of the node.
    // While the callbacks associated with the spesific calls above
    // also report on potential failures with the corresponding calls only to the caller,
    // these signals only notify about successful state changes, and
    // anyone can subscribe. In other words, the former is for an actor,
    // while the latter is for an observer. E.g. a HTTP daemon wrapping joystream::core library
    // would use callbacks to service RPC calls, and signals to populate websocket streams.

    // BitTorrent daemon now listens on given local endpoint.
    // Is triggered on initial start of node, and on subsequent interface changes locally.
    void startedListeningOnPort(const libtorrent::tcp::endpoint &);

    // Node paused
    void paused();

    // Node unpaused
    void unPaused();

    // Torrent added
    void addedTorrent(const std::shared_ptr<Torrent> &);

    // Torrent with given info hash was removed
    void removedTorrent(const libtorrent::sha1_hash & info_hash);

private slots:

    /**
     * Handlers for TCP streamng server

    // Checks server for pending connections
    void handleConnection();

    // Checks server for pending error
    void handleAcceptError(QAbstractSocket::SocketError socketError);

    // Streaming server signals
    void registerStream(const Stream * handler);
    void handleFailedStreamCreation(QAbstractSocket::SocketError socketError);

    // Stream signal
    void registerRequestedPathOnStream(const Stream * handler, const QByteArray & requestedPath) const;

    // Will ask libtorrent to read given piece
    void readPiece(int piece);
    */

private:

    // Underlying libtorrent session
    libtorrent::session _session;

    // Plugin
    boost::shared_ptr<extension::Plugin> _plugin;

    // Torrents added to session
    // A shared pointer is used, in order to give weak pointers as
    // public handle. This is safe, sinc user and this object is maintained
    // by same thread.
    //
    // NB: This container should not be used as reliable indication of
    // state inside this class, it should simply be updated based on inbound
    // libtorrent alerts. To check on torrents, this object should use the libtorrent
    // session itself.
    std::map<libtorrent::sha1_hash, std::shared_ptr<Torrent>> _torrents;

    // Transaction broadcasting
    BroadcastTransaction _broadcastTransaction;

    // TCP streaming server
    //QTcpServer _streamingServer;

    /// Libtorrent entry points for libtorrent::alert processing
    // Entry point for callback from libtorrent, warning about 0->1 alert in queue.
    // NB: Do not under any circumstance have a call to libtorrent in this routine, since the network
    // thread in libtorrent will be making this call, and a new call will result in a dead lock.
    void libtorrent_alert_notification_entry_point();

    /// Alert processing routines

    // Process all pending alerts in the libtorrent queue
    Q_INVOKABLE void processAlertQueue();

    // Processes a spesific alert
    void processAlert(const libtorrent::alert * a);

    // Processing (standard) libtorrent alerts of given type
    void process(const libtorrent::listen_succeeded_alert *);
    void process(const libtorrent::metadata_received_alert *);
    void process(const libtorrent::metadata_failed_alert *);
    void process(const libtorrent::add_torrent_alert *);
    void process(const libtorrent::torrent_finished_alert *);
    void process(const libtorrent::state_update_alert *);
    void process(const libtorrent::torrent_removed_alert *);
    void process(const libtorrent::torrent_resumed_alert *);
    void process(const libtorrent::save_resume_data_alert *);
    void process(const libtorrent::save_resume_data_failed_alert * p);
    void process(const libtorrent::torrent_paused_alert *);
    void process(const libtorrent::torrent_checked_alert *);
    void process(const libtorrent::peer_connect_alert *);
    void process(const libtorrent::peer_disconnected_alert *);
    void processReadPieceAlert(const libtorrent::read_piece_alert *);
    void processPieceFinishedAlert(const libtorrent::piece_finished_alert *);

    // Processing (plugin) alerts
    void process(const extension::alert::RequestResult * p);
    void process(const extension::alert::PluginStatus * p);

    // Generate session settings pack used with libtorrent
    static libtorrent::settings_pack session_settings() noexcept;

    // Generates dht settings used with libtorrent
    static libtorrent::dht_settings dht_settings() noexcept;

    // Generate parameters for addign torrent
    static libtorrent::add_torrent_params toAddTorrentParams(const boost::optional<uint> & uploadLimit,
                                                             const boost::optional<uint> & downloadLimit,
                                                             const std::string & name,
                                                             const std::vector<char> & resumeData,
                                                             const std::string & savePath,
                                                             bool pause,
                                                             const TorrentIdentifier & torrentIdentifier) noexcept;
};

}
}

#endif // JOYSTREAM_CORE_NODE_HPP
