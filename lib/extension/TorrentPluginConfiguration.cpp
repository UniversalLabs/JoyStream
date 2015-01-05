#include "TorrentPluginConfiguration.hpp"
#include "PeerPluginConfiguration.hpp"

TorrentPluginConfiguration::TorrentPluginConfiguration()
    : _startedPluginMode(PluginMode::Sell)
    , _enableBanningSets(true) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(PluginMode pluginMode, bool enableBanningSets)
    : _startedPluginMode(pluginMode)
    , _enableBanningSets(enableBanningSets) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    // NOT IMPLEMENTED
}

/*
void TorrentPluginConfiguration::insertPeerPluginConfiguration(const PeerPluginConfiguration * peerPluginConfiguration) {
    _peerPluginConfigurations.insert(std::make(peerPluginConfiguration->peerPluginConfiguration));
}

const std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *>::const_iterator TorrentPluginConfiguration::getBeginPeerPluginConfigurationsIterator() const {
    return _peerPluginConfigurations.begin();
}

const std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *>::const_iterator TorrentPluginConfiguration::getEndPeerPluginConfigurationsIterator() const {
    return _peerPluginConfigurations.end();
}
*/
void TorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {
    // NOT IMPLEMENTED
}

PluginMode TorrentPluginConfiguration::getPluginMode() const {
    return _startedPluginMode;
}

bool TorrentPluginConfiguration::getEnableBanningSets() const {
    return _enableBanningSets;
}
