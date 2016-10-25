#include <QCoreApplication>
#include <core/Node.hpp>
#include <core/TorrentIdentifier.hpp>

#include <boost/asio/impl/src.hpp>

#include <unistd.h>
#include <syscall.h>
#include <thread>
#include <chrono>
#include <grpc++/alarm.h>
#include "async-call-handler.h"

#include "protos/daemon.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::ServerAsyncResponseWriter;
using grpc::Status;
using joystream::daemon::rpc::Daemon;
using joystream::daemon::rpc::Void;
using joystream::daemon::rpc::Torrent;
using joystream::daemon::rpc::TestResponce;
using joystream::daemon::rpc::TestRequest;

///////////////////////////////////////////////////////////////////

static std::string ssprintf(const char * format, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));

static std::string ssprintf(const char * format, ...)
{
  va_list arglist;
  char buf[1024] = "";

  va_start(arglist, format);
  vsnprintf(buf, sizeof(buf) - 1, format, arglist);
  va_end(arglist);

  return buf;
}

static inline pid_t gettid(void)
{
  return (pid_t) syscall (SYS_gettid);
}

#define PDBG(...) \
    fprintf(stderr, "[%d] %s(): %6d ", gettid(), __func__, __LINE__), \
    fprintf(stderr, __VA_ARGS__), \
    fputc('\n', stderr), \
    fflush(stderr)


// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}

///////////////////////////////////////////////////////////////////


class AsyncDaemonService
  : public AsyncCallHandler<Daemon::AsyncService> {

public:

  AsyncDaemonService(joystream::core::Node* node, QCoreApplication* app)
    : AsyncCallHandler(node, app)
  {
    AddMethod(&NormalRpcDaemon::OnCall, &Daemon::AsyncService::Requesttest1);
    AddMethod(&PauseRpcDaemon::OnCall, &Daemon::AsyncService::RequestPause);
  }

  ///////

  class PauseRpcDaemon {
    Void response;
  public:
    bool OnCall(bool fok, ServerContext* context, ServerCompletionQueue * cq, const Void * request,
        ServerAsyncResponseWriter<Void>* response_writer, void * tag)
    {
      /*(void)(fok);
      (void) (context);
      (void) (cq);*/
      // PDBG("GOT %s", request->clientmessage().c_str());
      QCoreApplication* app = QApplication::instance();
      node_->pause([app](){
          std::cout << "Node was paused" << std::endl;
          app->exit();
      });

      response_writer->Finish(response, Status::OK, tag);

      return true;
    }

  private :
    joystream::core::Node* node_;
  };

  class NormalRpcDaemon {
    TestResponce responce;
  public:
    bool OnCall(bool fok, ServerContext* context, ServerCompletionQueue * cq, const TestRequest * request,
        ServerAsyncResponseWriter<TestResponce>* response_writer, void * tag)
    {
      /*(void)(fok);
      (void) (context);
      (void) (cq);*/
      PDBG("GOT %s", request->clientmessage().c_str());
      responce.set_servermessage("This is a server message");
      response_writer->Finish(responce, Status::OK, tag);
      return true;
    }
  };
};

  ///////

class ServerImpl final
{
  public:
    ServerImpl(joystream::core::Node* node, QCoreApplication* app)
      : node_(node),
        app_(app),
        the_thread()
      {
       Init();
      }

    ~ServerImpl() {
      if(the_thread.joinable()) the_thread.join();
      server_->Shutdown();
      std::cout << "Server destroyed" << std::endl;
      // Always shutdown the completion queue after the server.
      // cq_->Shutdown();
    }

    void Init(){
        the_thread = std::thread(&ServerImpl::Run,this);
    }

    // There is no shutdown handling in this code.
    void Run() {
      std::string server_address("0.0.0.0:3002");

      ServerBuilder builder;
      AsyncDaemonService daemonService(node_, app_);

      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

      builder.RegisterService(&daemonService);
     // cq_ = builder.AddCompletionQueue();
     // daemonService.setCompletionQueue(cq_);
     daemonService.setCompletionQueue(builder.AddCompletionQueue());

      server_ = builder.BuildAndStart();
      std::cout << "Server listening on " << server_address << std::endl;

      // run() is blocking
      daemonService.run();

      delete this;
    }

  private:
    joystream::core::Node *node_;
    QCoreApplication *app_;
    std::unique_ptr<Server> server_;
    std::thread the_thread;
    std::unique_ptr<ServerCompletionQueue> cq_;
};



int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  // Create the objects we need here to pass to the daemon
  auto node = joystream::core::Node::create([](const Coin::Transaction &tx){
    // broadcast tx
  });

  ServerImpl server(node, &a);

  return a.exec();
}
