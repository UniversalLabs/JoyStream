#include <app_kit/TorrentState.hpp>
#include <libtorrent/create_torrent.hpp>

namespace joystream {
namespace appkit {

TorrentState::TorrentState()
{

}

TorrentState::TorrentState(const core::Torrent *t)
    : _savePath(QString::fromStdString(t->savePath())),
      _name(QString::fromStdString(t->name())),
      _torrentPaused(t->isPaused()),
      _uploadLimit(t->uploadLimit()),
      _downloadLimit(t->downloadLimit()),
      _resumeData(t->resumeData())
{

    auto ti = t->metaData().lock();

    if(ti) {
        _metaData = bencodeMetaData(*ti);
    }

    if(t->torrentPluginSet())
        _torrentPluginState = TorrentPluginState(t->torrentPlugin());
}

TorrentState::TorrentState(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting object json value");

    QJsonObject state = value.toObject();

    _savePath = state["savePath"].toString();
    _name = state["name"].toString();
    _torrentPaused = state["paused"].toBool();
    _uploadLimit = state["uploadLimit"].toInt();
    _downloadLimit = state["downloadLimit"].toInt();
    _resumeData = base64StringToCharVector(state["resumeData"].toString());
    _metaData = base64StringToCharVector(state["metaData"].toString());
    _torrentPluginState = TorrentPluginState(state["pluginState"]);
}

std::string TorrentState::savePath() const {
    return _savePath.toStdString();
}

std::string TorrentState::name() const {
    return _name.toStdString();
}

bool TorrentState::paused() const {
    return _torrentPaused;
}

int TorrentState::uploadLimit() const {
    return _uploadLimit;
}

int TorrentState::downloadLimit() const {
    return _downloadLimit;
}

std::vector<char> TorrentState::resumeData() const {
    return _resumeData;
}

boost::shared_ptr<libtorrent::torrent_info> TorrentState::metaData() const {
    libtorrent::bdecode_node decodedMetaData;
    libtorrent::error_code ec;

    libtorrent::bdecode(_metaData.data(), _metaData.data() + _metaData.size(), decodedMetaData, ec);

    if(ec) {
        return boost::shared_ptr<libtorrent::torrent_info>(nullptr);
    }

    return boost::shared_ptr<libtorrent::torrent_info>(new libtorrent::torrent_info(decodedMetaData));
}

TorrentPluginState TorrentState::pluginState() const {
    return _torrentPluginState;
}

QJsonValue TorrentState::toJson() const {
    QJsonObject state;
    state["savePath"] = QJsonValue(_savePath);
    state["name"] = QJsonValue(_name);
    state["paused"] = QJsonValue(_torrentPaused);
    state["uploadLimit"] = QJsonValue(_uploadLimit);
    state["downloadLimit"] = QJsonValue(_downloadLimit);
    state["resumeData"] = QJsonValue(charVectorToBase64String(_resumeData));
    state["metaData"] = QJsonValue(charVectorToBase64String(_metaData));
    state["pluginState"] = _torrentPluginState.toJson();
    return state;
}

std::vector<char> TorrentState::bencodeMetaData(const libtorrent::torrent_info & ti) {
    libtorrent::create_torrent ct(ti);

    auto metadata = ct.generate();

    std::vector<char> encoded;

    if(metadata.type() != libtorrent::entry::undefined_t) {
        libtorrent::bencode(std::back_inserter(encoded), metadata);
    }

    return encoded;
}

QString TorrentState::charVectorToBase64String(const std::vector<char> &vec) {
    QByteArray data(vec.data(), vec.size());
    return data.toBase64();
}

std::vector<char> TorrentState::base64StringToCharVector(const QString &base64EncodedTorrentInfo) {
    QByteArray data = QByteArray::fromBase64(base64EncodedTorrentInfo.toLocal8Bit());
    return std::vector<char>(data.begin(), data.end());
}

}
}
