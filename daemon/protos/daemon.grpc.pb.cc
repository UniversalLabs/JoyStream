// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: daemon.proto

#include "daemon.pb.h"
#include "daemon.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace joystream {
namespace daemon {
namespace rpc {

static const char* Daemon_method_names[] = {
  "/joystream.daemon.rpc.Daemon/Pause",
  "/joystream.daemon.rpc.Daemon/GetEvents",
  "/joystream.daemon.rpc.Daemon/AddTorrent",
  "/joystream.daemon.rpc.Daemon/RemoveTorrent",
  "/joystream.daemon.rpc.Daemon/ListTorrents",
  "/joystream.daemon.rpc.Daemon/GetTorrentState",
};

std::unique_ptr< Daemon::Stub> Daemon::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< Daemon::Stub> stub(new Daemon::Stub(channel));
  return stub;
}

Daemon::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Pause_(Daemon_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetEvents_(Daemon_method_names[1], ::grpc::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_AddTorrent_(Daemon_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RemoveTorrent_(Daemon_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ListTorrents_(Daemon_method_names[4], ::grpc::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_GetTorrentState_(Daemon_method_names[5], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Daemon::Stub::Pause(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::joystream::daemon::rpc::Void* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_Pause_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::Void>* Daemon::Stub::AsyncPauseRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::Void>(channel_.get(), cq, rpcmethod_Pause_, context, request);
}

::grpc::ClientReader< ::joystream::daemon::rpc::Event>* Daemon::Stub::GetEventsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) {
  return new ::grpc::ClientReader< ::joystream::daemon::rpc::Event>(channel_.get(), rpcmethod_GetEvents_, context, request);
}

::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Event>* Daemon::Stub::AsyncGetEventsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Event>(channel_.get(), cq, rpcmethod_GetEvents_, context, request, tag);
}

::grpc::Status Daemon::Stub::AddTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_AddTorrent_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>* Daemon::Stub::AsyncAddTorrentRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>(channel_.get(), cq, rpcmethod_AddTorrent_, context, request);
}

::grpc::Status Daemon::Stub::RemoveTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_RemoveTorrent_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>* Daemon::Stub::AsyncRemoveTorrentRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>(channel_.get(), cq, rpcmethod_RemoveTorrent_, context, request);
}

::grpc::ClientReader< ::joystream::daemon::rpc::Torrent>* Daemon::Stub::ListTorrentsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) {
  return new ::grpc::ClientReader< ::joystream::daemon::rpc::Torrent>(channel_.get(), rpcmethod_ListTorrents_, context, request);
}

::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Torrent>* Daemon::Stub::AsyncListTorrentsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Torrent>(channel_.get(), cq, rpcmethod_ListTorrents_, context, request, tag);
}

::grpc::Status Daemon::Stub::GetTorrentState(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetTorrentState_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>* Daemon::Stub::AsyncGetTorrentStateRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>(channel_.get(), cq, rpcmethod_GetTorrentState_, context, request);
}

Daemon::Service::Service() {
  (void)Daemon_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      Daemon_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Daemon::Service, ::joystream::daemon::rpc::Void, ::joystream::daemon::rpc::Void>(
          std::mem_fn(&Daemon::Service::Pause), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Daemon_method_names[1],
      ::grpc::RpcMethod::SERVER_STREAMING,
      new ::grpc::ServerStreamingHandler< Daemon::Service, ::joystream::daemon::rpc::Void, ::joystream::daemon::rpc::Event>(
          std::mem_fn(&Daemon::Service::GetEvents), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Daemon_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Daemon::Service, ::joystream::daemon::rpc::Torrent, ::joystream::daemon::rpc::TorrentState>(
          std::mem_fn(&Daemon::Service::AddTorrent), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Daemon_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Daemon::Service, ::joystream::daemon::rpc::Torrent, ::joystream::daemon::rpc::TorrentState>(
          std::mem_fn(&Daemon::Service::RemoveTorrent), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Daemon_method_names[4],
      ::grpc::RpcMethod::SERVER_STREAMING,
      new ::grpc::ServerStreamingHandler< Daemon::Service, ::joystream::daemon::rpc::Void, ::joystream::daemon::rpc::Torrent>(
          std::mem_fn(&Daemon::Service::ListTorrents), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Daemon_method_names[5],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< Daemon::Service, ::joystream::daemon::rpc::Torrent, ::joystream::daemon::rpc::TorrentState>(
          std::mem_fn(&Daemon::Service::GetTorrentState), this)));
}

Daemon::Service::~Service() {
}

::grpc::Status Daemon::Service::Pause(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::joystream::daemon::rpc::Void* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Daemon::Service::GetEvents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Event>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Daemon::Service::AddTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Daemon::Service::RemoveTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Daemon::Service::ListTorrents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Torrent>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Daemon::Service::GetTorrentState(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace joystream
}  // namespace daemon
}  // namespace rpc

