#ifndef NATIVE_EXTENSION_GRAB_H
#define NATIVE_EXTENSION_GRAB_H

#include <nan.h>
#include <iostream>
#include <QString>
#include <QByteArray>
#include <joystream_libtorrent_session/Session.hpp>


// Example with node ObjectWrap
// Based on https://nodejs.org/api/addons.html#addons_wrapping_c_objects but using NAN
class SessionWrap : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    explicit SessionWrap();
    ~SessionWrap();

    static NAN_METHOD(New);
    static NAN_METHOD(AddTorrent);
    static NAN_METHOD(RemoveTorrent);
    static NAN_METHOD(ListenPort);
    static NAN_METHOD(PostTorrentUpdates);
    static NAN_METHOD(Pause);
    static NAN_METHOD(IsPaused);
    static NAN_METHOD(Resume);
    static Nan::Persistent<v8::Function> constructor;
    QString string_;
    QByteArray ba_;
    Session session_;
};

#endif
