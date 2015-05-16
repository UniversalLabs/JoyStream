#include "SellerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"
#include "SellerPeerPluginViewModel.hpp"

SellerTorrentPluginViewModel::SellerTorrentPluginViewModel(QObject * parent, const SellerTorrentPlugin::Status & status)
    : QObject(parent)
    , _minPrice(status.minPrice())
    , _minLock(status.minLock())
    , _minFeePerByte(status.minFeePerByte())
    , _maxNumberOfSellers(status.maxNumberOfSellers())
    , _maxContractConfirmationDelay(status.maxContractConfirmationDelay()) {

    // Add peers
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd();
        i != end;i++)
        addPeer(i.key(), i.value());
}

/**
SellerTorrentPluginViewModel::~SellerTorrentPluginViewModel() {

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *>::const_iterator
        i = _sellerPeerPluginViewModels.constBegin(),
        end = _sellerPeerPluginViewModels.constEnd();
        i != end;i++)
        delete i.value();
}

*/

#include <libtorrent/socket_io.hpp> // libtorrent::print_endpoint
#include <QDebug>

void SellerTorrentPluginViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status) {

    // Convert to std::string
    std::string endPointStdString = libtorrent::print_endpoint(endPoint);

    // Convert to QString
    QString endPointQString = endPointStdString.c_str();

    qDebug() << "addPeer:" << endPointQString;


    Q_ASSERT(!_sellerPeerPluginViewModels.contains(endPoint));

    // Create new view model
    SellerPeerPluginViewModel * model = new SellerPeerPluginViewModel(this, endPoint, status);

    // Add to map
    _sellerPeerPluginViewModels[endPoint] = model;

    // Notify that peer was added
    emit peerAdded(model);
}

void SellerTorrentPluginViewModel::update(const SellerTorrentPlugin::Status & status) {

    if(_minPrice != status.minPrice()) {
        _minPrice = status.minPrice();
        emit minPriceChanged(status.minPrice());
    }

    if(_minLock != status.minLock()) {
        _minLock = status.minLock();
        emit minLockChanged(status.minLock());
    }

    if(_minFeePerByte != status.minFeePerByte()) {
        _minFeePerByte = status.minFeePerByte();
        emit minFeePerByteChanged(status.minFeePerByte());
    }

    if(_maxNumberOfSellers != status.maxNumberOfSellers()) {
        _maxNumberOfSellers = status.maxNumberOfSellers();
        emit maxNumberOfSellersChanged(status.maxNumberOfSellers());
    }

    if(_maxContractConfirmationDelay != status.maxContractConfirmationDelay()) {
        _maxContractConfirmationDelay = status.maxContractConfirmationDelay();
        emit maxContractConfirmationDelayChanged(status.maxContractConfirmationDelay());
    }

    // Update peers
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd();
        i != end;i++) {

        // Get endpoint
        libtorrent::tcp::endpoint endPoint = i.key();

        Q_ASSERT(_sellerPeerPluginViewModels.contains(endPoint));

        // Update
        _sellerPeerPluginViewModels[endPoint]->update(i.value());
    }
}

quint64 SellerTorrentPluginViewModel::minPrice() const {
    return _minPrice;
}

quint32 SellerTorrentPluginViewModel::minLock() const {
    return _minLock;
}

quint64 SellerTorrentPluginViewModel::minFeePerByte() const {
    return _minFeePerByte;
}

quint32 SellerTorrentPluginViewModel::maxNumberOfSellers() const {
    return _maxNumberOfSellers;
}

quint32 SellerTorrentPluginViewModel::maxContractConfirmationDelay() const {
    return _maxContractConfirmationDelay;
}

QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> SellerTorrentPluginViewModel::sellerPeerPluginViewModels() const {
    return _sellerPeerPluginViewModels;
}
