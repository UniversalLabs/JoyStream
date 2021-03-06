# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, April 12 2016

TARGET = protocol_session-test
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11
CONFIG  -= core

HEADERS += \
    SessionSpy.hpp \
    SessionTest.hpp

SOURCES += \
    SessionSpy.cpp \
    SessionTest.cpp \
    main.cpp

# GTest configs
QMAKE_CXXFLAGS +=-pthread
LIBS += -pthread
LIBS += -lgtest

# paymentchannels #################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../paymentchannel/release/ -lpaymentchannel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../paymentchannel/debug/ -lpaymentchannel
else:unix: LIBS += -L$$OUT_PWD/../../paymentchannel/ -lpaymentchannel

INCLUDEPATH += $$PWD/../../paymentchannel/include
DEPENDPATH += $$PWD/../../paymentchannel/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/release/libpaymentchannel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/debug/libpaymentchannel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/release/paymentchannel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/debug/paymentchannel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../paymentchannel/libpaymentchannel.a

# protocol_session ################################################################################

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../protocol_session/release/ -lprotocol_session
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../protocol_session/debug/ -lprotocol_session
else:unix: LIBS += -L$$OUT_PWD/../../protocol_session/ -lprotocol_session

INCLUDEPATH += $$PWD/../../protocol_session/include
DEPENDPATH += $$PWD/../../protocol_session/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_session/release/libprotocol_session.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_session/debug/libprotocol_session.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_session/release/protocol_session.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_session/debug/protocol_session.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../protocol_session/libprotocol_session.a

# protocol_statemachine ###########################################################################

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../protocol_statemachine/release/ -lprotocol_statemachine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../protocol_statemachine/debug/ -lprotocol_statemachine
else:unix: LIBS += -L$$OUT_PWD/../../protocol_statemachine/ -lprotocol_statemachine

INCLUDEPATH += $$PWD/../../protocol_statemachine/include
DEPENDPATH += $$PWD/../../protocol_statemachine/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_statemachine/release/libprotocol_statemachine.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_statemachine/debug/libprotocol_statemachine.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_statemachine/release/protocol_statemachine.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_statemachine/debug/protocol_statemachine.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../protocol_statemachine/libprotocol_statemachine.a

# protocol_wire ###################################################################################

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../protocol_wire/release/ -lprotocol_wire
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../protocol_wire/debug/ -lprotocol_wire
else:unix: LIBS += -L$$OUT_PWD/../../protocol_wire/ -lprotocol_wire

INCLUDEPATH += $$PWD/../../protocol_wire/include
DEPENDPATH += $$PWD/../../protocol_wire/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/release/libprotocol_wire.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/debug/libprotocol_wire.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/release/protocol_wire.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/debug/protocol_wire.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../protocol_wire/libprotocol_wire.a

# common ###################################################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/ -lcommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/libcommon.a

include(../../config.pri)
