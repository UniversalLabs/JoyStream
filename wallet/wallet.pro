# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 28 2015

TARGET = wallet
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum, std::array

QT      += core sql network # network added due to blockcypher integration

INCLUDEPATH += $$PWD/include # be able to include w.r.t root of this project

HEADERS += \
    include/wallet/Payee.hpp \
    include/wallet/Slot.hpp \
    include/wallet/Payer.hpp \
    include/wallet/BlockHeader.hpp \
    include/wallet/Transaction.hpp \
    include/wallet/OutPoint.hpp \
    include/wallet/Input.hpp \
    include/wallet/TransactionHasInput.hpp \
    include/wallet/TransactionHasOutput.hpp \
    include/wallet/InBoundPayment.hpp \
    include/wallet/OutBoundPayment.hpp \
    include/wallet/OuputFundsPayer.hpp \
    include/wallet/Metadata.hpp \
    include/wallet/Key.hpp \
    include/wallet/Manager.hpp \
    include/wallet/Address.hpp \
    include/wallet/Utilities.hpp \
    include/wallet/UtxoCreated.hpp \
    include/wallet/UtxoDestroyed.hpp \
    include/wallet/Output.hpp \
    include/wallet/NullAble.hpp \
    include/wallet/TransactionMinedInBlock.hpp

SOURCES += \
    src/Payee.cpp \
    src/Slot.cpp \
    src/Payer.cpp \
    src/BlockHeader.cpp \
    src/Transaction.cpp \
    src/OutPoint.cpp \
    src/Input.cpp \
    src/TransactionHasInput.cpp \
    src/TransactionHasOutput.cpp \
    src/InBoundPayment.cpp \
    src/OutBoundPayment.cpp \
    src/OuputFundsPayer.cpp \
    src/Metadata.cpp \
    src/Key.cpp \
    src/Manager.cpp \
    src/Address.cpp \
    src/Utilities.cpp \
    src/UtxoCreated.cpp \
    src/UtxoDestroyed.cpp \
    src/Output.cpp \
    src/TransactionMinedInBlock.cpp

# blockcypher
INCLUDEPATH += $$PWD/../blockcypher/include
DEPENDPATH += $$PWD/../blockcypher/include

# common
INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common/include

include(../config.pri)
