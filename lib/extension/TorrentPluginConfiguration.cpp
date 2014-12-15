#include "TorrentPluginConfiguration.hpp"

TorrentPluginConfiguration::TorrentPluginConfiguration(PluginMode pluginMode, bool enableBanningSets, bool pluginOn)
    : _pluginMode(pluginMode)
    , _enableBanningSets(enableBanningSets)
    , _pluginOn(pluginOn) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    // NOT IMPLEMENTED
}

void TorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {
    // NOT IMPLEMENTED
}

bool TorrentPluginConfiguration::getEnableBanningSets() {
    return _enableBanningSets;
}

PluginMode TorrentPluginConfiguration::getPluginMode() {
    return _pluginMode;
}
