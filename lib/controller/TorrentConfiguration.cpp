#include "TorrentConfiguration.hpp"
#include "Exceptions/InvalidBitSwaprStateEntryException.hpp"

TorrentConfiguration::TorrentConfiguration(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags, const libtorrent::torrent_info & torrent_info, const TorrentPluginConfiguration & torrentPluginConfiguration)
    : _info_hash(info_hash)
    , _name(name)
    , _save_path(save_path)
    , _resume_data(resume_data)
    , _flags(flags)
    , _torrent_info(new libtorrent::torrent_info(torrent_info))
    , _torrentPluginConfiguration(torrentPluginConfiguration) {
}

TorrentConfiguration::TorrentConfiguration()
    : _torrent_info(NULL) {
}

TorrentConfiguration::~TorrentConfiguration() {

    // Delete if was set to object
    if(_torrent_info != NULL)
        delete _torrent_info;
}

TorrentConfiguration & TorrentConfiguration::operator=(const TorrentConfiguration & rhs) {

    // Copy members
    _info_hash = rhs.getInfoHash();
    _name = rhs.getName();
    _save_path = rhs.getSavePath();
    _resume_data = rhs.getConstResumeData();
    _flags = rhs.getFlags();

    // Delete if was set to object
    if(_torrent_info != NULL)
        delete _torrent_info;

    _torrent_info = new libtorrent::torrent_info(*rhs.getTorrentInfo());

    _torrentPluginConfiguration = rhs.getTorrentPluginConfiguration();

    // Return self reference
    return *this;
}

TorrentConfiguration::TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {

    // Check that info_hash is present
    if(dictionaryEntry.count("info_hash") == 1) {

        // Get entry
        libtorrent::entry infoHashEntry = dictionaryEntry.find("info_hash")->second;

        // Check that entry is of type entry::string_t
        if(infoHashEntry.type() == libtorrent::entry::string_t)
            _info_hash = libtorrent::sha1_hash(infoHashEntry.string()); // Why do we need to call constructor, why no conversion?
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key is not of type entry::string_type.");

    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key should have .count == 1.");

    // Check that name is present
    if(dictionaryEntry.count("name") == 1) {

        // Get entry
        libtorrent::entry nameEntry = dictionaryEntry.find("name")->second;

        // Check that entry is of type entry::string_t
        if(nameEntry.type() == libtorrent::entry::string_t)
            _name = nameEntry.string();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key should have .count == 1.");

    // Check that save_path is present
    if(dictionaryEntry.count("save_path") == 1) {

        // Get entry
        libtorrent::entry savePathEntry = dictionaryEntry.find("save_path")->second;

        // Check that entry is of type entry::string_t
        if(savePathEntry.type() == libtorrent::entry::string_t)
            _save_path = savePathEntry.string();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");


    // Check that resume_data is present
    if(dictionaryEntry.count("resume_data") == 1) {

        // Get entry
        libtorrent::entry resumeDataEntry = dictionaryEntry.find("resume_data")->second;

        // Check that entry is of type entry::string_type
        if(resumeDataEntry.type() == libtorrent::entry::string_t) {

            // Use as entry::string_type
            libtorrent::entry::string_type resumeDataStringEntry = resumeDataEntry.string();

            // Populate resume_data_ vector
            for(std::string::const_iterator i = resumeDataStringEntry.begin(),
                    end(resumeDataStringEntry.end());i != end;i++)
                    _resume_data.push_back(*i);

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");

    // Check that flags is present
    if(dictionaryEntry.count("flags") == 1) {

        // Get entry
        libtorrent::entry flagsEntry = dictionaryEntry.find("flags")->second;

        // Check that entry is of type entry::int_t
        if(flagsEntry.type() == libtorrent::entry::int_t)
            _flags = flagsEntry.integer();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key is not of type entry::int_t.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key should have .count == 1.");

    // torrentPluginConfiguration
    if(dictionaryEntry.count("torrentPluginConfiguration") == 1) {

        // Get entry
        libtorrent::entry torrentPluginConfigurationEntry = dictionaryEntry.find("torrentPluginConfiguration")->second;

        // Check that entry is of type entry::dictionary_t
        if(torrentPluginConfigurationEntry.type() == libtorrent::entry::dictionary_t)
            _torrentPluginConfiguration = TorrentPluginConfiguration(torrentPluginConfigurationEntry.dict());
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "torrentPluginConfiguration key is not of type entry::dictionary_t.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "torrentPluginConfiguration key should have .count == 1.");
}

void TorrentConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // info_hash
    dictionaryEntry["info_hash"] = libtorrent::entry::string_type(_info_hash.to_string());

    // name
    dictionaryEntry["name"] = libtorrent::entry::string_type(_name);

    // save_path
    dictionaryEntry["save_path"] = libtorrent::entry::string_type(_save_path);

    // resume_data
    std::string resume_data_string;
    for(std::vector<char>::const_iterator i = _resume_data.begin(),
        end(_resume_data.end());i != end;i++)
            resume_data_string.append(&(*i));

    dictionaryEntry["resume_data"] = libtorrent::entry::string_type(resume_data_string);

    // flags
    dictionaryEntry["flags"] = libtorrent::entry::integer_type(_flags);

    // torrentPluginConfiguration
    libtorrent::entry::dictionary_type torrentPluginConfigurationDictionaryEntry;
    _torrentPluginConfiguration.toDictionaryEntry(torrentPluginConfigurationDictionaryEntry);
    dictionaryEntry["torrentPluginConfiguration"] = torrentPluginConfigurationDictionaryEntry;
}

const libtorrent::sha1_hash & TorrentConfiguration::getInfoHash() const {
    return _info_hash;
}

const std::string & TorrentConfiguration::getName() const {
     return _name;
}

const std::string & TorrentConfiguration::getSavePath() const {
    return _save_path;
}

std::vector<char> & TorrentConfiguration::getResumeData() {
    return _resume_data;
}

const std::vector<char> & TorrentConfiguration::getConstResumeData() const {
    return _resume_data;
}

quint64 TorrentConfiguration::getFlags() const {
    return _flags;
}

const TorrentPluginConfiguration & TorrentConfiguration::getTorrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}

libtorrent::torrent_info *TorrentConfiguration::getTorrentInfo() const {
    return _torrent_info;
}

void TorrentConfiguration::setTorrent_info(const libtorrent::torrent_info & torrent_info) {

    // Delete if we set to object
    if(_torrent_info != NULL)
        delete _torrent_info;

    // Allocate new object
    _torrent_info = new libtorrent::torrent_info(torrent_info);
}

