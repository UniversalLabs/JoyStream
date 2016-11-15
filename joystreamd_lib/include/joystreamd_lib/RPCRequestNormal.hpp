#ifndef RPCREQUESTNORMAL_HPP
#define RPCREQUESTNORMAL_HPP

#include <joystreamd_lib/RPCRequest.hpp>


/**
 * @brief Template class for all normal rpc call
 *
 **/
template <class RESP, class REQ, class SERVICE>
class RPCRequestNormal : public RPCRequest {

    public:
        RPCRequestNormal(SERVICE* service, grpc::ServerCompletionQueue* cq)
            : RPCRequest(cq), service_(service), responder_(&ctx_) {}


        /**
         * @brief Will answer to the client
         * and update the status_ to FINISHED.
         * Only for normal RPC (no Stream).
         * */
        void finish(const RESP &response, bool success )
        {
            if (success) {
                responder_.Finish(response, grpc::Status::OK, this);
            } else {
                responder_.Finish(response, grpc::Status::CANCELLED, this);
            }
            status_ = FINISHED;
        }

    protected:
        grpc::ServerAsyncResponseWriter<RESP> responder_;
        SERVICE* service_;
        REQ request_;

    private:
        using RPCRequest::status_;

};
#endif // RPCREQUESTNORMAL_HPP
