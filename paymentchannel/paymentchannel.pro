# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015

TARGET = paymentchannel
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core sql

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/paymentchannel/Commitment.hpp \
    include/paymentchannel/Contract.hpp \
    include/paymentchannel/Payee.hpp \
    include/paymentchannel/Payor.hpp \
    include/paymentchannel/Refund.hpp \
    include/paymentchannel/Settlement.hpp \
    include/paymentchannel/Termination.hpp

SOURCES += \
    src/Commitment.cpp \
    src/Contract.cpp \
    src/Payee.cpp \
    src/Payor.cpp \
    src/Refund.cpp \
    src/Settlement.cpp \
    src/Termination.cpp

# common  ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/libcommon.a

include(../mSIGNA.pri)
include(../openssl.pri)
include(../boost.pri)