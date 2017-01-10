#include <addon/common/PubKeyHash.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

Nan::Persistent<v8::Function> PubKeyHash::constructor;

NAN_MODULE_INIT(PubKeyHash::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PubKeyHash").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "toBuffer", ToBuffer);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PubKeyHash").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> PubKeyHash::NewInstance(const Coin::PubKeyHash &hash) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    PubKeyHash* pubKeyHash = ObjectWrap::Unwrap<PubKeyHash>(instance);
    pubKeyHash->_pubKeyHash = hash;
    return scope.Escape(instance);
}

Coin::PubKeyHash PubKeyHash::pubKeyHash() const {
    return _pubKeyHash;
}

NAN_METHOD(PubKeyHash::New) {
  if (info.IsConstructCall()) {
    PubKeyHash *obj = new PubKeyHash();

    if(info.Length() > 0){
        // If argument is provided, it must be a buffer
        if(!info[0]->IsUint8Array()) {
            Nan::ThrowTypeError("Argument must be a buffer");
            return;
        }

        try {
            obj->_pubKeyHash = Coin::PubKeyHash(util::NodeBufferToUCharVector(info[0]));
        } catch (const std::exception &ex) {
            Nan::ThrowTypeError(ex.what());
            return;
        }
    }

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New(constructor);
    if(info.Length() > 0) {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked());
      return;
    }
    info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked());
  }
}

NAN_METHOD(PubKeyHash::ToBuffer) {
    PubKeyHash* pubKeyHash = ObjectWrap::Unwrap<PubKeyHash>(info.This());
    auto data = pubKeyHash->_pubKeyHash.toUCharVector();
    info.GetReturnValue().Set(util::UCharVectorToNodeBuffer(data));
}

}}}
