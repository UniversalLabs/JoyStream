#ifndef JOYSTREAM_EXTENSION_INTERFACE_TORRENT_INTERFACE_HPP
#define JOYSTREAM_EXTENSION_INTERFACE_TORRENT_INTERFACE_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>

namespace libtorrent {
    class sha1_hash;
    class torrent_handle;
}

namespace joystream {
namespace extension {
namespace interface {

class TorrentInterface {
public:
    virtual libtorrent::sha1_hash infoHash() const = 0;
    virtual libtorrent::torrent_handle native_handle() const = 0;

    virtual ~TorrentInterface() {}
};

}
}
}

#endif // JOYSTREAM_EXTENSION_INTERFACE_TORRENT_INTERFACE_HPP