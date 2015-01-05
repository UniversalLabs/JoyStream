#include "SetConfigurationTorrentPluginRequest.hpp"

SetConfigurationTorrentPluginRequest::SetConfigurationTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, TorrentPluginConfiguration * torrentPluginConfiguration)
    : TorrentPluginRequest(info_hash)
    , _torrentPluginConfiguration(torrentPluginConfiguration) {
}

TorrentPluginRequestType SetConfigurationTorrentPluginRequest::getTorrentPluginRequestType() const {
    return TorrentPluginRequestType::SetConfiguration;
}

const TorrentPluginConfiguration * SetConfigurationTorrentPluginRequest::getTorrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}