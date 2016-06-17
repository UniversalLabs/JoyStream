# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include(config.pri)

TARGET = core
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core
QT      += network # sockets, qnam
QT      += websockets

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/core/viewmodel/BuyerPeerPlugin.hpp \
    include/core/viewmodel/BuyerTorrentPlugin.hpp \
    include/core/viewmodel/Channel.hpp \
    include/core/viewmodel/Payee.hpp \
    include/core/viewmodel/Payor.hpp \
    include/core/viewmodel/PeerPlugin.hpp \
    include/core/viewmodel/SellerPeerPlugin.hpp \
    include/core/viewmodel/SellerTorrentPlugin.hpp \
    include/core/viewmodel/TorrentPlugin.hpp \
    include/core/viewmodel/Torrent.hpp \
    include/core/Stream.hpp \
    include/core/core.hpp \
    include/core/Configuration.hpp \
    include/core/detail/Torrent.hpp \
    include/core/Exception.hpp \
    include/core/Torrent.hpp \
    include/core/Node.hpp

SOURCES += \
    src/viewmodel/BuyerPeerPlugin.cpp \
    src/viewmodel/BuyerTorrentPlugin.cpp \
    src/viewmodel/Channel.cpp \
    src/viewmodel/Payee.cpp \
    src/viewmodel/Payor.cpp \
    src/viewmodel/PeerPlugin.cpp \
    src/viewmodel/SellerPeerPlugin.cpp \
    src/viewmodel/SellerTorrentPlugin.cpp \
    src/viewmodel/TorrentPlugin.cpp \
    src/viewmodel/Torrent.cpp \
    src/Stream.cpp \
    src/detail/Torrent.cpp \
    src/Torrent.cpp \
    src/Configuration.cpp \
    src/Node.cpp

# extension
INCLUDEPATH += $$PWD/../extension/include
DEPENDPATH += $$PWD/../extension/include

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

# bitcoin
INCLUDEPATH += $PWD/.../bitcoin/include
DEPENDPATH += $PWD/.../bitcoin/include

include(../config.pri)
