# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

# compiled dependencies
#INCLUDEPATH += /usr/local/include
QMAKE_CXXFLAGS += -isystem /usr/local/include
LIBS += -L/usr/local/lib

# libtorrent
LIBTORRENT_LOCATION= $$PWD/deps/osx/dist

# mSIGNA
MSIGNA_SYSROOT = $$PWD/deps/osx/src/mSIGNA-joystream/sysroot

# Boost
# boost was build with --layout=tagged option so libraries are suffixed
BOOST_LIB_SUFFIX = -mt
