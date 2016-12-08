#ifndef TORRENTSELLER_HPP
#define TORRENTSELLER_HPP

#include <QObject>

#include <bitcoin/SPVWallet.hpp>
#include <core/Node.hpp>

#include <app_kit/Worker.hpp>
#include <app_kit/WorkerResult.hpp>

namespace joystream {
namespace appkit {


class TorrentSeller : public Worker
{
public:
    static std::shared_ptr<WorkerResult> sell(QObject* parent, core::Node*, bitcoin::SPVWallet*,
                                              libtorrent::sha1_hash,
                                              const protocol_session::SellingPolicy&,
                                              const protocol_wire::SellerTerms&,
                                              protocol_session::GenerateP2SHKeyPairCallbackHandler,
                                              protocol_session::GenerateReceiveAddressesCallbackHandler);

protected slots:
    void onTorrentStateChanged(libtorrent::torrent_status::state_t state, float progress);
    void onTorrentRemoved(const libtorrent::sha1_hash&);
    void start();
    void abort();

private:
    TorrentSeller(QObject* parent, core::Node*, bitcoin::SPVWallet*, std::shared_ptr<WorkerResult> response,
                 libtorrent::sha1_hash,
                 const protocol_session::SellingPolicy&,
                 const protocol_wire::SellerTerms&,
                 protocol_session::GenerateP2SHKeyPairCallbackHandler,
                 protocol_session::GenerateReceiveAddressesCallbackHandler);

    bitcoin::SPVWallet* const _wallet;
    core::Node* const _node;
    const protocol_session::SellingPolicy _policy;
    const protocol_wire::SellerTerms _terms;
    std::shared_ptr<WorkerResult> _response;

    const protocol_session::GenerateP2SHKeyPairCallbackHandler _paychanKeysGenerator;
    const protocol_session::GenerateReceiveAddressesCallbackHandler _receiveAddressesGenerator;
    const protocol_session::GenerateChangeAddressesCallbackHandler _changeAddressesGenerator;

    void finished();
    void finished(WorkerResult::Error);
    void finished(std::exception_ptr);

    core::Torrent* getTorrentPointerOrFail();
    void startSelling();
    void startPlugin();

};

}
}

#endif // TORRENTSELLER_HPP
