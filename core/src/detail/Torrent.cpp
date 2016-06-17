/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#include <core/detail/Torrent.hpp>

namespace joystream {
namespace core {
namespace detail {

Torrent::Torrent(const libtorrent::sha1_hash & infoHash,
                             const std::string & name,
                             const std::string & savePath,
                             const std::vector<char> & resumeData,
                             quint64 flags,
                             //libtorrent::torrent_info * torrentInfo,
                             const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                             Status event)
    : _infoHash(infoHash)
    , _name(name)
    , _savePath(savePath)
    , _resumeData(resumeData)
    , _flags(flags)
    //, _handle(handle)
    //, _torrentInfo(torrentInfo)
    , _status(event)
    , _pluginInstalled(PluginInstalled::None)
    , _model(infoHash,
             name,
             savePath,
             torrentFile) {
}

void Torrent::addPlugin(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Seller;
    _model.addPlugin(status);
}

void Torrent::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;
    _model.addPlugin(status);
}

libtorrent::sha1_hash Torrent::infoHash() const {
    return _infoHash;
}

std::string Torrent::name() const {
    return _name;
}

void Torrent::setName(const std::string & name) {
    _name = name;
}

std::string Torrent::savePath() const {
    return _savePath;
}

void Torrent::setSavePath(const std::string & savePath) {
    _savePath = savePath;
}

std::vector<char> Torrent::resumeData() const {
    return _resumeData;
}

void Torrent::setResumeData(const std::vector<char> & resumeData) {
    _resumeData = resumeData;
}

quint64 Torrent::flags() const {
    return _flags;
}

void Torrent::setFlags(quint64 flags) {
    _flags = flags;
}

/**
libtorrent::torrent_info * Torrent::torrentInfo() {
    return _torrentInfo;
}
*/

libtorrent::torrent_handle Torrent::handle() const {
    return _handle;
}

void Torrent::setHandle(const libtorrent::torrent_handle & handle) {
    _handle = handle;
}

Torrent::Status Torrent::status() const {
    return _status;
}

void Torrent::setStatus(Status event) {
    _status = event;
}

PluginInstalled Torrent::pluginInstalled() const {
    return _pluginInstalled;
}

TorrentViewModel * Torrent::model() {
    return &_model;
}

void Torrent::addStream(Stream * stream) {
    _streams.insert(stream);
}

void Torrent::removeStream(Stream * stream) {
    _streams.remove(stream);
}

void Torrent::pieceRead(const boost::shared_array<char> & buffer,
                                    int pieceIndex,
                                    int size) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceRead(buffer, pieceIndex, size);
}

void Torrent::pieceFinished(int piece) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceDownloaded(piece);
}


}
}
}