# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 23 2015

include(config.pri)

TARGET = extension
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum
CONFIG  -= core

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/extension/TorrentPlugin.hpp \
    include/extension/Plugin.hpp \
    include/extension/PeerPlugin.hpp \
    include/extension/BEPSupportStatus.hpp \
    include/extension/ExtendedMessageIdMapping.hpp \
    include/extension/extension.hpp \
    include/extension/Request.hpp \
    include/extension/Status.hpp \
    include/extension/Alert.hpp \
    include/extension/Exception.hpp \
    include/extension/detail.hpp \
    include/extension/AlertManager.hpp \
    include/extension/Session.hpp \
    include/extension/Torrent.hpp \
    include/extension/Peer.hpp \
    include/extension/interface/AlertManagerInterface.hpp \
    include/extension/interface/PeerInterface.hpp \
    include/extension/interface/SessionInterface.hpp \
    include/extension/interface/TorrentInterface.hpp \
    include/extension/Common.hpp \
    include/extension/MessageType.hpp \
    include/extension/ExtendedMessage.hpp

SOURCES += \
    src/TorrentPlugin.cpp \
    src/Plugin.cpp \
    src/PeerPlugin.cpp \
    src/ExtendedMessageIdMapping.cpp \
    src/detail.cpp \
    src/Peer.cpp \
    src/Torrent.cpp \
    src/Session.cpp \
    src/AlertManager.cpp \
    src/MessageType.cpp \
    src/ExtendedMessage.cpp

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

# protocol_statemachine
INCLUDEPATH += $$PWD/../protocol_statemachine/include
DEPENDPATH += $$PWD/../protocol_statemachine/include

# protocol_wire
INCLUDEPATH += $$PWD/../protocol_wire/include
DEPENDPATH += $$PWD/../protocol_wire/include

# paymentchannel
INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel/include

# protocol_session
INCLUDEPATH += $$PWD/../protocol_session/include
DEPENDPATH += $$PWD/../protocol_session/include

include(../config.pri)
