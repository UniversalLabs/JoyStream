#include <joystreamd_lib/RPCBuyTorrent.hpp>

RPCBuyTorrent::RPCBuyTorrent(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::appkit::AppKit* appKit)
    : RPCRequestNormal(service, cq), appKit_(appKit)
{
    service_->RequestBuyTorrent(&ctx_, &request_, &responder_, cq_, cq_, this);
}

RPCBuyTorrent::buyingModeActivated()
{
    joystream::daemon::rpc::Void response;

    /*if(addResult->getError() != joystream::appkit::WorkerResult::Error::NoError) {
        this->finish(response, false);
        return;
    }*/
    std::cout << "Buying Torrent" << std::endl;
    this->finish(response, true);
}

void RPCBuyTorrent::process()
{
    new RPCBuyTorrent(service_, cq_, appKit_);


    joystream::protocol_session::BuyingPolicy buyingPolicy(request_.secondsbeforecreatingcontract(),
                                                           request_.secondsbeforepiecetimeout(),
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::protocol_wire::BuyerTerms buyerTerms(request_.price(), request_.locktime(), request_.nsellers(), request_.contractfeerate());

    auto buyResult = appKit_->buyTorrent(libtorrent::sha1_hash(request_.infohash().c_str()), buyingPolicy, buyerTerms);
    std::cout << "We are buying the torrent" << std::endl;
    QObject::connect(buyResult.get(), &joystream::appkit::WorkerResult::finished, this, &RPCBuyTorrent::buyingModeActivated);
}
