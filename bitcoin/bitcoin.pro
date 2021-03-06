# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential

QT -= gui

TARGET = bitcoin
TEMPLATE = lib
CONFIG += staticlib
CONFIG  += c++11

INCLUDEPATH += $$PWD/include

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

INCLUDEPATH += $$PWD/../blockcypher/include
DEPENDPATH += $$PWD/../blockcypher/include

#store
HEADERS += include/bitcoin/Store.hpp \
           src/detail/store/Schema.hpp \
           include/bitcoin/SPVWallet.hpp \
           include/bitcoin/Common.hpp

SOURCES += src/detail/store/Schema-odb.cxx \
           src/detail/store/Schema.cpp \
           src/Store.cpp \
           src/SPVWallet.cpp

include(../config.pri)
