/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 21 2016
 */

#include <core/TorrentIdentifier.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace joystream {
namespace core {

TorrentIdentifier::TorrentIdentifier(const libtorrent::sha1_hash & infoHash)
    : _type(Type::Hash)
    , _infoHash(infoHash) {
}

TorrentIdentifier::TorrentIdentifier(const MagnetLink & magnetLink)
    : _type(Type::MagnetLink)
    , _magnetLink(magnetLink)
    , _infoHash(magnetLink.infoHash()) {
}

TorrentIdentifier::TorrentIdentifier(const boost::shared_ptr<libtorrent::torrent_info> & torrentFile)
    : _type(Type::TorrentFile)
    , _torrentFile(torrentFile)
    , _infoHash(torrentFile->info_hash()) {
}

TorrentIdentifier::TorrentIdentifier(const TorrentIdentifier* ti)
    : _type(ti->type())
    , _torrentFile(ti->torrentFile())
    , _infoHash(ti->infoHash()) {
}

TorrentIdentifier* TorrentIdentifier::fromTorrentFilePath(const char *path)
{
    libtorrent::error_code ec;
    boost::shared_ptr<libtorrent::torrent_info> ti;

    if(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)) {
        ti = boost::make_shared<libtorrent::torrent_info>(std::string(path), boost::ref(ec), 0);

        if (ec) {
            return nullptr;
        }

        return new joystream::core::TorrentIdentifier(ti);
    }

    return nullptr;
}

TorrentIdentifier* TorrentIdentifier::fromTorrentFileContents(const std::vector<unsigned char> &torrentFileData) {
    libtorrent::error_code ec;
    boost::shared_ptr<libtorrent::torrent_info> ti;

    ti = boost::make_shared<libtorrent::torrent_info>((const char*)torrentFileData.data(), torrentFileData.size(), boost::ref(ec), 0);

    if (ec) {
        return nullptr;
    }

    return new joystream::core::TorrentIdentifier(ti);

}

TorrentIdentifier* TorrentIdentifier::fromHashString(const char *hexHashString)
{

    if(strlen(hexHashString) == 40) {
        char buf[21];

        if(!libtorrent::from_hex(hexHashString, 40, buf))
            return nullptr;

        return new joystream::core::TorrentIdentifier(libtorrent::sha1_hash(buf));
    }

    return nullptr;
}

TorrentIdentifier* TorrentIdentifier::fromMagnetLinkString(const char *uri)
{
    try {
        auto magnetLink = joystream::core::MagnetLink::fromURI(uri);
        return new joystream::core::TorrentIdentifier(magnetLink);
    } catch (std::exception &e) {

    }

    return nullptr;
}

TorrentIdentifier::Type TorrentIdentifier::type() const noexcept {
    return _type;
}

libtorrent::sha1_hash TorrentIdentifier::infoHash() const noexcept {
    return _infoHash;
}

MagnetLink TorrentIdentifier::magnetLink() const noexcept {
    return _magnetLink;
}

boost::shared_ptr<libtorrent::torrent_info> TorrentIdentifier::torrentFile() const noexcept {
    return _torrentFile;
}

}
}
