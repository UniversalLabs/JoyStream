/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_EXCEPTION_HPP
#define JOYSTREAM_CORE_EXCEPTION_HPP

#include <core/Node.hpp>

#include <exception>
#include <QJsonValue>

namespace joystream {
namespace core {
namespace exception {

    class MissingJSONKey : public std::runtime_error {

    public:

        MissingJSONKey(const std::string & key)
            : std::runtime_error(toMessage(key))
            , key(key){
        }

        std::string key;

    private:

        static std::string toMessage(const std::string & key) {
            return std::string("Missing key ") + key;
        }
    };

    class InvalidJSONValueType : public std::runtime_error {

    public:

        InvalidJSONValueType(const std::string & key, QJsonValue::Type expected, QJsonValue::Type found)
            : std::runtime_error(toMessage(key, expected, found))
            , key(key)
            , expected(expected)
            , found(found) {
        }

        std::string key;

        QJsonValue::Type expected;

        QJsonValue::Type found;

    private:

        static std::string toString(QJsonValue::Type type) {

            switch(type) {

                case QJsonValue::Type::Array: return std::string("Array");
                case QJsonValue::Type::Bool: return std::string("Bool");
                case QJsonValue::Type::Double: return std::string("Double");
                case QJsonValue::Type::Null: return std::string("Null");
                case QJsonValue::Type::Object: return std::string("Object");
                case QJsonValue::Type::String: return std::string("String");
                case QJsonValue::Type::Undefined: return std::string("Undefined");

                default:
                    assert(false);
            }
        }

        static std::string toMessage(const std::string & key, QJsonValue::Type expected, QJsonValue::Type found) {

            return std::string("Found value type ")
                   + toString(found)
                   + ", but expected type "
                   + toString(expected)
                   + " for key"
                   + key;
        }

    };

    class CanOnlyStartStoppedNode : public std::runtime_error {

    public:

        CanOnlyStartStoppedNode(Node::State badState)
            : std::runtime_error("Node can only be started when stopped.")
            , badState(badState) {
        }

        // The state in the node
        Node::State badState;
    };

    class CannotStopStoppedNode : public std::runtime_error {

    public:

        CannotStopStoppedNode()
            : std::runtime_error("Node cannot be stopped when already stopped") {
        }
    };

    class NoSuchTorrentExists : public std::runtime_error {

    public:

        NoSuchTorrentExists(const libtorrent::sha1_hash & infoHash)
            : std::runtime_error(toMessage(infoHash))
            , infoHash(infoHash) {
        }

        libtorrent::sha1_hash infoHash;

    private:

        static std::string toMessage(const libtorrent::sha1_hash & infoHash) {
            return std::string("No torrent added with info_hash: ") + infoHash.to_string();
        }
    };

}
}
}

#endif // JOYSTREAM_CORE_EXCEPTION_HPP
