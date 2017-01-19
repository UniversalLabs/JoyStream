#include <common/PublicKey.hpp>
#include <addon/util/buffers.hpp>
#include <addon/common/PublicKey.hpp>

namespace joystream {
namespace node {
namespace publickey {

v8::Local<v8::Value> toValue(const Coin::PublicKey &pk) {
    return UCharVectorBasedToV8Value<Coin::PublicKey>(pk);
}

Coin::PublicKey fromValue(const v8::Local<v8::Value>& value) {
    return fromV8ValueToUCharVectorBased<Coin::PublicKey>(value);
}

}}}
