#ifndef JOYSTREAM_NODE_OUTPOINT_HPP
#define JOYSTREAM_NODE_OUTPOINT_HPP

#include <nan.h>

namespace Coin {
    class typesafeOutPoint;
}

namespace joystream {
namespace node {
namespace outpoint {

    /* @brief Creates JavaScript Object representing a Coin::typesafeOutPoint
     * @param {const Coin::typesafeOutPoint&}
     * @return {v8::Local<v8::Value>} v8::Object
     * {
     *   txid: transactionid value,  // from transactionid::toValue
     *   index: Number
     * }
     */
    v8::Local<v8::Value> toValue(const Coin::typesafeOutPoint &);

    /* @brief Converts a JavaScript Object (of the form
     * generated by @outpoint::toValue) to a native Coin::typesafeOutPoint
     * @param {v8::Local<v8::Value>}
     * @return {Coin::typesafeOutPoint}
     * @throws std::runtime_error if parameter not valid outpoint
     */
    Coin::typesafeOutPoint fromValue(const v8::Local<v8::Value>&);

}}}
#endif
