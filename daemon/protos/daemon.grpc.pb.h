// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: daemon.proto
#ifndef GRPC_daemon_2eproto__INCLUDED
#define GRPC_daemon_2eproto__INCLUDED

#include "daemon.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace joystream {
namespace daemon {
namespace rpc {

class Daemon GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Pause(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::joystream::daemon::rpc::Void* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::Void>> AsyncPause(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::Void>>(AsyncPauseRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::joystream::daemon::rpc::Event>> GetEvents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::joystream::daemon::rpc::Event>>(GetEventsRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::joystream::daemon::rpc::Event>> AsyncGetEvents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::joystream::daemon::rpc::Event>>(AsyncGetEventsRaw(context, request, cq, tag));
    }
    virtual ::grpc::Status AddTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>> AsyncAddTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>>(AsyncAddTorrentRaw(context, request, cq));
    }
    virtual ::grpc::Status RemoveTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>> AsyncRemoveTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>>(AsyncRemoveTorrentRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::joystream::daemon::rpc::Torrent>> ListTorrents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::joystream::daemon::rpc::Torrent>>(ListTorrentsRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::joystream::daemon::rpc::Torrent>> AsyncListTorrents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::joystream::daemon::rpc::Torrent>>(AsyncListTorrentsRaw(context, request, cq, tag));
    }
    virtual ::grpc::Status GetTorrentState(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>> AsyncGetTorrentState(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>>(AsyncGetTorrentStateRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::Void>* AsyncPauseRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::joystream::daemon::rpc::Event>* GetEventsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::joystream::daemon::rpc::Event>* AsyncGetEventsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>* AsyncAddTorrentRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>* AsyncRemoveTorrentRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::joystream::daemon::rpc::Torrent>* ListTorrentsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::joystream::daemon::rpc::Torrent>* AsyncListTorrentsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::joystream::daemon::rpc::TorrentState>* AsyncGetTorrentStateRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status Pause(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::joystream::daemon::rpc::Void* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::Void>> AsyncPause(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::Void>>(AsyncPauseRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::joystream::daemon::rpc::Event>> GetEvents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::joystream::daemon::rpc::Event>>(GetEventsRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Event>> AsyncGetEvents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Event>>(AsyncGetEventsRaw(context, request, cq, tag));
    }
    ::grpc::Status AddTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>> AsyncAddTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>>(AsyncAddTorrentRaw(context, request, cq));
    }
    ::grpc::Status RemoveTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>> AsyncRemoveTorrent(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>>(AsyncRemoveTorrentRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::joystream::daemon::rpc::Torrent>> ListTorrents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::joystream::daemon::rpc::Torrent>>(ListTorrentsRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Torrent>> AsyncListTorrents(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Torrent>>(AsyncListTorrentsRaw(context, request, cq, tag));
    }
    ::grpc::Status GetTorrentState(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::joystream::daemon::rpc::TorrentState* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>> AsyncGetTorrentState(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>>(AsyncGetTorrentStateRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::Void>* AsyncPauseRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientReader< ::joystream::daemon::rpc::Event>* GetEventsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) GRPC_OVERRIDE;
    ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Event>* AsyncGetEventsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>* AsyncAddTorrentRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>* AsyncRemoveTorrentRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientReader< ::joystream::daemon::rpc::Torrent>* ListTorrentsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request) GRPC_OVERRIDE;
    ::grpc::ClientAsyncReader< ::joystream::daemon::rpc::Torrent>* AsyncListTorrentsRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Void& request, ::grpc::CompletionQueue* cq, void* tag) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::joystream::daemon::rpc::TorrentState>* AsyncGetTorrentStateRaw(::grpc::ClientContext* context, const ::joystream::daemon::rpc::Torrent& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_Pause_;
    const ::grpc::RpcMethod rpcmethod_GetEvents_;
    const ::grpc::RpcMethod rpcmethod_AddTorrent_;
    const ::grpc::RpcMethod rpcmethod_RemoveTorrent_;
    const ::grpc::RpcMethod rpcmethod_ListTorrents_;
    const ::grpc::RpcMethod rpcmethod_GetTorrentState_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Pause(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::joystream::daemon::rpc::Void* response);
    virtual ::grpc::Status GetEvents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Event>* writer);
    virtual ::grpc::Status AddTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response);
    virtual ::grpc::Status RemoveTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response);
    virtual ::grpc::Status ListTorrents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Torrent>* writer);
    virtual ::grpc::Status GetTorrentState(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Pause : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_Pause() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Pause() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Pause(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::joystream::daemon::rpc::Void* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPause(::grpc::ServerContext* context, ::joystream::daemon::rpc::Void* request, ::grpc::ServerAsyncResponseWriter< ::joystream::daemon::rpc::Void>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetEvents : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetEvents() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_GetEvents() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetEvents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Event>* writer) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetEvents(::grpc::ServerContext* context, ::joystream::daemon::rpc::Void* request, ::grpc::ServerAsyncWriter< ::joystream::daemon::rpc::Event>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_AddTorrent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_AddTorrent() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_AddTorrent() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AddTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestAddTorrent(::grpc::ServerContext* context, ::joystream::daemon::rpc::Torrent* request, ::grpc::ServerAsyncResponseWriter< ::joystream::daemon::rpc::TorrentState>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_RemoveTorrent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_RemoveTorrent() {
      ::grpc::Service::MarkMethodAsync(3);
    }
    ~WithAsyncMethod_RemoveTorrent() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RemoveTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRemoveTorrent(::grpc::ServerContext* context, ::joystream::daemon::rpc::Torrent* request, ::grpc::ServerAsyncResponseWriter< ::joystream::daemon::rpc::TorrentState>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(3, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_ListTorrents : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_ListTorrents() {
      ::grpc::Service::MarkMethodAsync(4);
    }
    ~WithAsyncMethod_ListTorrents() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ListTorrents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Torrent>* writer) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestListTorrents(::grpc::ServerContext* context, ::joystream::daemon::rpc::Void* request, ::grpc::ServerAsyncWriter< ::joystream::daemon::rpc::Torrent>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(4, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetTorrentState : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetTorrentState() {
      ::grpc::Service::MarkMethodAsync(5);
    }
    ~WithAsyncMethod_GetTorrentState() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTorrentState(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetTorrentState(::grpc::ServerContext* context, ::joystream::daemon::rpc::Torrent* request, ::grpc::ServerAsyncResponseWriter< ::joystream::daemon::rpc::TorrentState>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(5, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Pause<WithAsyncMethod_GetEvents<WithAsyncMethod_AddTorrent<WithAsyncMethod_RemoveTorrent<WithAsyncMethod_ListTorrents<WithAsyncMethod_GetTorrentState<Service > > > > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_Pause : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_Pause() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Pause() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Pause(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::joystream::daemon::rpc::Void* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetEvents : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetEvents() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_GetEvents() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetEvents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Event>* writer) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_AddTorrent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_AddTorrent() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_AddTorrent() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status AddTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_RemoveTorrent : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_RemoveTorrent() {
      ::grpc::Service::MarkMethodGeneric(3);
    }
    ~WithGenericMethod_RemoveTorrent() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RemoveTorrent(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_ListTorrents : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_ListTorrents() {
      ::grpc::Service::MarkMethodGeneric(4);
    }
    ~WithGenericMethod_ListTorrents() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ListTorrents(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Void* request, ::grpc::ServerWriter< ::joystream::daemon::rpc::Torrent>* writer) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetTorrentState : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetTorrentState() {
      ::grpc::Service::MarkMethodGeneric(5);
    }
    ~WithGenericMethod_GetTorrentState() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTorrentState(::grpc::ServerContext* context, const ::joystream::daemon::rpc::Torrent* request, ::joystream::daemon::rpc::TorrentState* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
};

}  // namespace rpc
}  // namespace daemon
}  // namespace joystream


#endif  // GRPC_daemon_2eproto__INCLUDED
