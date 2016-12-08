#include <joystreamd_lib/RPCListTorrents.hpp>

RPCListTorrents::RPCListTorrents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq,  joystream::core::Node* node)
    : RPCRequestStreamServer(service, cq), node_(node)
{
    service_->RequestListTorrents(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void RPCListTorrents::process()
{
    joystream::daemon::rpc::Torrent response;

    new RPCListTorrents(service_, cq_, node_);

    for (const auto t : node_->torrents()) {
        response.set_infohash(libtorrent::to_hex(t.first.to_string()));
        response.set_name(t.second->name());
        this->write(response);
    }

    this->finish(true);

}