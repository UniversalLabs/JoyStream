# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

include (../boost.pri)
include (../libtorrent.pri)

TARGET = BitSwapr
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
QT      += core network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Needed for including QApplication

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project
INCLUDEPATH += $$PWD/.. # be able to include w.r.t root of project, in particular content from lib sub project

SOURCES += main.cpp

# lib ###############################################################
win32:CONFIG(release, debug|release):       LIBS += -L$$OUT_PWD/../lib/release/ -lQtBitSwapr
else:win32:CONFIG(debug, debug|release):    LIBS += -L$$OUT_PWD/../lib/debug/ -lQtBitSwapr
else:unix:                                  LIBS += -L$$OUT_PWD/../lib/ -lQtBitSwapr

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/release/ -lQtBitSwapr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/debug/ -lQtBitSwapr
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lQtBitSwapr

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/libQtBitSwapr.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/libQtBitSwapr.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/QtBitSwapr.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/QtBitSwapr.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libQtBitSwapr.a

# common ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/libcommon.a
