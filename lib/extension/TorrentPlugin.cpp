
#include "TorrentPlugin.hpp"
#include "PeerPlugin.hpp" // needed since we construct object

#include <libtorrent/error_code.hpp>

#include <QLoggingCategory>

TorrentPlugin::TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn)
    : plugin_(plugin)
    , torrent_(torrent)
    , category_(category)
    , tokensReceived_(0)
    , tokensSent_(0)
    , pluginOn_(pluginOn) {
}

TorrentPlugin::~TorrentPlugin() {
    // No need to explicltly delete PeerPlugin, since libtorrent has shared_ptr
}

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    // Role of peer
    PeerPlugin::PEER_ROLE role = (torrent_->bytes_left() > 0) ? PeerPlugin::buyer : PeerPlugin::seller;

    // Create peer level plugin
    PeerPlugin * peerPlugin = new PeerPlugin(this, peerConnection, category_, role);

    // Add to collection
    peerPlugins.push_back(peerPlugin);

    // Notify
    libtorrent::error_code ec;
    libtorrent::address const & addr = peerConnection->remote().address();
    const char * peerAddress = NULL;

    if (addr.is_v6())
        peerAddress = addr.to_string(ec).c_str();
    else
        peerAddress = addr.to_string(ec).c_str();

    if(ec) {
        qCCritical(category_) << ec.message().c_str();
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    short port = peerConnection->remote().port();

    qCDebug(category_) << "Peer #" << peerPlugins.size() << "[" << peerAddress << ":" << port << "] added to " << this->torrent_->name().c_str();
    //(libtorrent::to_hex(torrent_->info_hash().to_string())).c_str() << ".";

    // Return pointer as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void TorrentPlugin::on_piece_pass(int index) {

}

void TorrentPlugin::on_piece_failed(int index) {

}

void TorrentPlugin::tick() {

    //qCDebug(category_) << "Peer.tick()";

    // Send status signal
    sendTorrentPluginStatusSignal();

}

bool TorrentPlugin::on_resume() {
    return false;
}

bool TorrentPlugin::on_pause() {
    return false;
}

void TorrentPlugin::on_files_checked() {

}

void TorrentPlugin::on_state(int s) {

}

void TorrentPlugin::on_add_peer(libtorrent::tcp::endpoint const & tcpEndPoint, int src, int flags) {
    qCDebug(category_) << "Peer added to list " << tcpEndPoint.address().to_string().c_str() << ": " << tcpEndPoint.port();
}

// Returns plugin
Plugin * TorrentPlugin::getPlugin() {
    return plugin_;
}

libtorrent::torrent * TorrentPlugin::getTorrent() {
    return torrent_;
}

void TorrentPlugin::sendTorrentPluginStatusSignal() {

    int numberOfPeers = peerPlugins.size();

    int numberOfPeersWithExtension = 0;
    for(std::vector<PeerPlugin *>::iterator i = peerPlugins.begin(),
        end(peerPlugins.end()); i != end; i++) {

        if((*i)->getPeerBEP43SupportedStatus() == PeerPlugin::supported)
            numberOfPeersWithExtension++;
    }

    // Emit status
    libtorrent::sha1_hash hash = torrent_->info_hash();
    TorrentPluginStatus status(hash, numberOfPeers, numberOfPeersWithExtension, pluginOn_, tokensReceived_, tokensSent_);

    emit updateTorrentPluginStatus(status);
}