#ifndef TORRENT_CONFIGURATION_HPP
#define TORRENT_CONFIGURATION_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/entry.hpp> // because you cant forward declare typedefs (libtorrent::entry::dictionary_type)

#include <QtGlobal> // quint64

#include <vector>

class TorrentPluginConfiguration;

class TorrentConfiguration {

public:

    // Constructor from members
    TorrentConfiguration(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags, TorrentPluginConfiguration * torrentPluginConfiguration = NULL);

    // Constructor from dictionary
    TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Default constructor, needed for adding type by value to map
    TorrentConfiguration();

    // Assignment operator, needed for adding type by value to map
    TorrentConfiguration & operator=(const TorrentConfiguration & rhs);

    // Destructor
    ~TorrentConfiguration();

    /**
     * Write state into dictionary
     * ===============================================================
     *
     * Torrent parameters as they persist across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * "info_hash" -> entry::string_type object representing info hash of torrent.
     *
     * "name" -> entry::string_type object representing name of torrent.
     *
     * "save_path" -> entry::string_type object representing save_path parameter.
     *
     * "resume_data" -> entry::string_type object representing resume data.
     *
     * "flags" -> entry::integer_type object representing flag parameter.
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters
    const libtorrent::sha1_hash & getInfoHash() const;
    const std::string & getName() const;
    const std::string & getSavePath() const;
    std::vector<char> & getResumeData();
    const std::vector<char> & getConstResumeData() const;
    quint64 getFlags() const;
    TorrentPluginConfiguration * getTorrentPluginConfiguration() const;

protected:

    // Info hash of torrent
    libtorrent::sha1_hash _info_hash;

    // Name of torrent
    std::string _name;

    // Save path
    std::string _save_path;

    // Resume data
    std::vector<char> _resume_data;

    // Flags
    quint64 _flags;

    // Configuration
    TorrentPluginConfiguration * _torrentPluginConfiguration;
};

#endif // TORRENT_CONFIGURATION_HPP