include(gmock.pri)

TARGET = extension-test
TEMPLATE = app

CONFIG  += c++11

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../../extension/include

HEADERS += \
    include/MockPeer.hpp \
    include/MockTorrent.hpp \
    include/MockSession.hpp \
    include/MockAlertManager.hpp \
    include/PollableInterface.hpp \
    include/Buyer.hpp \
    include/Seller.hpp \
    include/BuyerImpl.hpp

SOURCES += src/main.cpp \
    src/Buyer.cpp \
    src/Seller.cpp

# extension ################################################################################

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../extension/release/ -lextension
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../extension/debug/ -lextension
else:unix: LIBS += -L$$OUT_PWD/../../extension/ -lextension

INCLUDEPATH += $$PWD/../../extension/include
DEPENDPATH += $$PWD/../../extension/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../extension/release/libextension.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../extension/debug/libextension.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../extension/release/extension.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../extension/debug/extension.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../extension/libextension.a

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
