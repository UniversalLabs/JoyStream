# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015

TARGET = blockcypher-test
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11
CONFIG  += testcase

QT      += core network
QT      += testlib

HEADERS += \
    Test.hpp

SOURCES += \
    Test.cpp

# blockcypher ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../blockcypher/release/ -lblockcypher
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../blockcypher/debug/ -lblockcypher
else:unix: LIBS += -L$$OUT_PWD/../../blockcypher/ -lblockcypher

INCLUDEPATH += $$PWD/../../blockcypher/include
DEPENDPATH += $$PWD/../../blockcypher

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/release/libblockcypher.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/debug/libblockcypher.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/release/blockcypher.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/debug/blockcypher.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../blockcypher/libblockcypher.a

# common ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/ -lcommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/libcommon.a


include(../../config.pri)
