# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 28 2015

include(../mSIGNA.pri)
include(../openssl.pri)
include(../boost.pri)

TARGET = wallet
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core sql

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

# not sure if this is needed, CONFIG + c++11 should be enough, but something was failing at one point
QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    include/wallet/Wallet.hpp \
    include/wallet/WalletKey.hpp \
    include/wallet/WalletKeyControllingReceiveAddress.hpp \
    include/wallet/Payee.hpp \
    include/wallet/Slot.hpp \
    include/wallet/SPVValidator.hpp \
    include/wallet/SPVWAllet.hpp \
    include/wallet/Payer.hpp \
    include/wallet/WalletEntity.hpp \
    include/wallet/WalletAddress.hpp \
    include/wallet/BlockHeader.hpp \
    include/wallet/Transaction.hpp \
    include/wallet/OutPoint.hpp \
    include/wallet/Input.hpp \
    include/wallet/TransactionHasInput.hpp \
    include/wallet/Output.hpp \
    include/wallet/TransactionHasOutput.hpp \
    include/wallet/InBoundPayment.hpp \
    include/wallet/OutBoundPayment.hpp \
    include/wallet/OuputFundsPayer.hpp \
    include/wallet/Metadata.hpp \
    include/wallet/Seed.hpp \
    include/wallet/WalletUtilities.hpp

SOURCES += \
    src/Wallet.cpp \
    src/WalletKey.cpp \
    src/WalletKeyControllingReceiveAddress.cpp \
    src/Payee.cpp \
    src/Slot.cpp \
    src/SPVValidator.cpp \
    src/SPVWAllet.cpp \
    src/Payer.cpp \
    src/WalletEntity.cpp \
    src/WalletAddress.cpp \
    src/BlockHeader.cpp \
    src/CoinWrappers_templated_functions.cpp \
    src/Transaction.cpp \
    src/OutPoint.cpp \
    src/Input.cpp \
    src/TransactionHasInput.cpp \
    src/Output.cpp \
    src/TransactionHasOutput.cpp \
    src/InBoundPayment.cpp \
    src/OutBoundPayment.cpp \
    src/OuputFundsPayer.cpp \
    src/Metadata.cpp \
    src/Seed.cpp \
    src/WalletUtilities.cpp

# common ###############################################################
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
