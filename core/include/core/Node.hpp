/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_NODE_HPP
#define JOYSTREAM_CORE_NODE_HPP

#include <core/detail/NodeImpl.hpp>
#include <extension/extension.hpp>
#include <libtorrent/session.hpp>

#include <QObject>

#include <memory>
#include <functional>

namespace joystream {
namespace extension {
    class Plugin;
}
namespace core {

class Torrent;
class TorrentIdentifier;

class Node : public QObject {

    Q_OBJECT

public:

    typedef std::function<void()> NodeUnPaused;
    typedef std::function<void()> NodePaused;
    typedef extension::request::AddTorrent::AddTorrentHandler AddedTorrent;
    typedef extension::request::SubroutineHandler RemovedTorrent;

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    /**
     * @brief Creates node based on session and plugin which are already
     * started, and the latter installed on the former.
     * @param sessionn started libtorrent session with plugin installed
     * @param plugin extension plugin
     */
    Node(libtorrent::session * session,
         const boost::shared_ptr<extension::Plugin> & plugin);

    /**
     * @brief Creates a node instance
     * @param broadcastTransaction called to broadcast transaction.
     * @throws exception::FailedToStartNodeException if starting failed.
     * @return a starte node instance
     */
    static Node * create();

    /**
     * @brief Terminates all connections on all torrents, and stops all plugins, but
     *        BitTorrent and DHT daemons still run.
     *
     * @param nodeStopped callback about being actually stopped
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
     * @signal removedTorrent if torrent was successfully removed
     */
    void removeTorrent(const libtorrent::sha1_hash & info_hash, const RemovedTorrent & handler);

    /**
     * @brief Requests updates on status of all known torrents on the libtorrent session,
     * result appears as signal on core::
     */
    void postTorrentStatusUpdates() const noexcept;

    /**
     * @brief Requests updates on status of all known torrent plugins, result
     * appears as signal core::TorrentPlugin::statusUpdated on corresponding objects.
     */
    void postTorrentPluginStatusUpdates() const noexcept;

    /**
     * @brief Port on which node is currently listening for BitTorrent
     * connections
     * @return Port used by Node.
     */
    unsigned short port() const noexcept;

    /**
     * @brief Returns if assisted peer discovery is enabled. Assisted peer discovery adds an additional
     * DHT announce and peer search for all torrents with <INFOHASH>_JS to find JoyStream nodes.
     * connections
     * @return Assisted peer discovery enabled.
     */
    bool assistedPeerDiscovery() const noexcept;

    /**
     * @brief Enable/disable assisted peer discovery.
     */
    void setAssistedPeerDiscovery(bool assistedPeerDiscovery) noexcept;

    /**
     * @brief Returns how often (in seconds) we announce all torrents to the
     * assisted peer discovery network. Default: every 2 hours.
     * @return Assisted peer discovery announce interval.
     */
    int announceTimerIntervalSeconds() const noexcept;
    /**
     * @brief Sets how often (in seconds) we announce all torrents to the
     * assisted peer discovery network.
     */
    void setAnnounceTimerIntervalSeconds(int seconds) noexcept;

    /**
     * @brief Returns how often (in seconds) we search for JoyStream peers for
     * all torrents. Default: every 5 minutes.
     * @return Assisted peer discovery refresh interval.
     */
    int getPeersTimerIntervalSeconds() const noexcept;
    /**
     * @brief Sets how often (in seconds) we search for JoyStream peers for
     * all torrents.
     */
    void setGetPeersTimerIntervalSeconds(int seconds) noexcept;

    /**
     * @brief Returns map with all torrent handles, indexed by their info hashes
     * @return Map of all torrent handles, indexed by their info hashes
     */
    std::map<libtorrent::sha1_hash, Torrent *> torrents() const noexcept;

    /**
     * @brief Lookup for a torrent in Node from sha1_hash
     * @param The sha1_hash of the torrent we are looking for
     * @return Torrent object or nullptr if not found
     */
    Torrent* torrent(const libtorrent::sha1_hash & info_hash);

signals:

    // Signals are emitted for any change in state of the node.
    // While the callbacks associated with the spesific calls above
    // also report on potential failures with the corresponding calls only to the caller,
    // these signals only notify about successful state changes, and
    // anyone can subscribe. In other words, the former is for an actor,
    // while the latter is for an observer. E.g. a HTTP daemon wrapping joystream::core library
    // would use callbacks to service RPC calls, and signals to populate websocket streams.

    // BitTorrent daemon started on given endpoint
    void startedListening(const libtorrent::tcp::endpoint & endPoint);

    // Torrent added
    void addedTorrent(Torrent *);

    // Torrent with given info hash was removed
    void removedTorrent(const libtorrent::sha1_hash & info_hash);

    // Status update with all torrent plugins
    void torrentPluginStatusUpdate(const std::map<libtorrent::sha1_hash, extension::status::TorrentPlugin> &);

    // A status arrived from libtorrent
    void alertArrived(const libtorrent::alert *);

    /**
     * @brief Assisted peer discovery is enabled/disabled
     */
    void assistedPeerDiscoveryChanged(bool assistedPeerDiscovery);

private:

    // PIMPL
    detail::NodeImpl _pimpl;

    // Callback handler for PIMPL
    void pimplStartedListeningHandler(const libtorrent::tcp::endpoint & endPoint);
    void pimplTorrentAdded(core::Torrent * torrent);
    void pimplTorrentRemoved(const libtorrent::sha1_hash & info_hash);
    void pimplTorrentPluginStatusUpdate(const std::map<libtorrent::sha1_hash, extension::status::TorrentPlugin> &);
    void pimplAlertArrived(const libtorrent::alert *);

    // Entry point for callback from libtorrent, warning about 0->1 alert in queue.
    // NB: Do not under any circumstance have a call to libtorrent in this routine, since the network
    // thread in libtorrent will be making this call, and a new call will result in a dead lock.
    void libtorrent_alert_notification_entry_point();

    // Process all pending alerts in the libtorrent queue
    Q_INVOKABLE void processAlertQueue();

    /// Static utility routines

    // Generate session settings pack used with libtorrent
    static libtorrent::settings_pack session_settings(bool enableDHT) noexcept;

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
