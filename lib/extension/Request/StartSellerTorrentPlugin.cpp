#include "StartSellerTorrentPlugin.hpp"

StartSellerTorrentPlugin::StartSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPluginConfiguration & configuration)
    : _infoHash(info_hash)
    , _configuration(configuration) {
}

PluginRequestType StartSellerTorrentPlugin::getTorrentPluginRequestType() const {
    return PluginRequestType::StartSellerTorrentPlugin;
}

libtorrent::sha1_hash StartSellerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartSellerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash &infoHash) {
    _infoHash = infoHash;
}
SellerTorrentPluginConfiguration StartSellerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartSellerTorrentPlugin::setConfiguration(const SellerTorrentPluginConfiguration &configuration) {
    _configuration = configuration;
}
