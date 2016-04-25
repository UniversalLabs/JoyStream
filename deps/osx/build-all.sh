#!/bin/bash

# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

#set -x

LIB_BOOST_VER="boost_1_59_0"
LIB_BOOST_TARBALL="${LIB_BOOST_VER}.tar.gz"
LIB_BOOST_VERSION="1.59.0"

LIBTORRENT_VERSION="libtorrent-1_0_7"
LIBTORRENT_TARBALL="${LIBTORRENT_VERSION}.tar.gz"

OPENSSL_VERSION="openssl-1.0.2g"
OPENSSL_TARBALL="${OPENSSL_VERSION}.tar.gz"

LIBPNG_VERSION="libpng-1.6.21"
LIBPNG_TARBALL="${LIBPNG_VERSION}.tar.gz"

mkdir -p src/
mkdir -p dist/

pushd src
if [ ! -e "${OPENSSL_TARBALL}" ]
then
  # if we are downloading new version remove old build
  rm -fr openssl/

  # download openssl
  echo "Downloding ${OPENSSL_TARBALL}"
  if wget -O ${OPENSSL_TARBALL} "https://www.openssl.org/source/${OPENSSL_TARBALL}"
    then
        echo "Download Successful"
    else
        rm ${OPENSSL_TARBALL};
        exit 1
  fi
fi

if [ ! -r "openssl" ]
then
    if tar -xzvf "${OPENSSL_TARBALL}"
    then
        mv ${OPENSSL_VERSION}/ openssl
        pushd openssl/
        ./Configure shared darwin64-x86_64-cc --openssldir=/usr/local/
        if [ $? -ne 0 ]; then
            echo "Failed to configure openssl"
            popd
            rm -fr openssl/
            exit 1
        fi
        sed -ie "s/^CFLAG= -/CFLAG= -mmacosx-version-min=10.7 -/" Makefile
        if [ $? -ne 0 ]; then
            echo "Failed to patch openssl"
            popd
            rm -fr openssl/
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Building OpenSSL failed";
            popd
            rm -fr openssl/
            exit 1
        fi
        make install
        if [ $? -ne 0 ]; then
            echo "Installing OpenSSL failed";
            popd
            rm -fr openssl/
            exit 1
        fi
        popd
    else
        echo "Failed Extracting ${OPENSSL_TARBALL}";
        rm ${OPENSSL_TARBALL}
        rm -fr ${OPENSSL_VERSION}/
        exit 1
    fi
fi
popd

pushd src
if [ ! -e "${LIB_BOOST_TARBALL}" ]
then
  # remove old build
  rm -fr boost/

  # download boost
  echo "Downloding ${LIB_BOOST_TARBALL}"
  if wget -O ${LIB_BOOST_TARBALL} "http://sourceforge.net/projects/boost/files/boost/${LIB_BOOST_VERSION}/${LIB_BOOST_TARBALL}/download"
  then
      echo "Download Successful"
  else
      "Downloading Boost (${LIB_BOOST_TARBALL}) Failed"
      rm ${LIB_BOOST_TARBALL}
      exit 1
  fi
fi

if [ ! -e "boost" ]
then
  if tar -xzvf "${LIB_BOOST_TARBALL}"
  then
    mv ${LIB_BOOST_VER}/ boost
    cd boost/
    ./bootstrap.sh
    if [ $? -ne 0 ]; then
        echo "Failed to bootstrap Boost"
        cd ../
        rm -fr ${LIB_BOOST_VER}/
        exit 1
    fi
    patch tools/build/src/tools/darwin.jam ../../boost-patch.diff
    if [ $? -ne 0 ]; then
        echo "Failed to patch Boost"
        cd ../
        rm -fr ${LIB_BOOST_VER}/
        exit 1
    fi
  else
    rm ${LIB_BOOST_TARBALL}
    rm -fr ${LIB_BOOST_VER}/
    exit 1
  fi
fi
popd

./build-boost.sh
if [ $? -ne 0 ]; then
    echo "Failed to Build Boost"
    exit 1
fi

pushd src
if [ ! -e "${LIBTORRENT_TARBALL}" ]
then
    rm -fr libtorrent/

    # download libtorrent
    echo "Downloding ${LIBTORRENT_TARBALL}"
    if wget -O ${LIBTORRENT_TARBALL} "https://github.com/arvidn/libtorrent/archive/${LIBTORRENT_TARBALL}"
    then
        echo "Downloadin Successful"
    else
        echo "Downloading libtorrent failed"
        rm ${LIBTORRENT_TARBALL}
        exit 1
    fi
fi

if [ ! -e "libtorrent" ]
then
  if tar -xzvf ${LIBTORRENT_TARBALL}
  then
      mv libtorrent-${LIBTORRENT_VERSION}/ libtorrent
      cd libtorrent/
      patch src/bt_peer_connection.cpp ../../libtorrent-patch.diff
  else
      echo "Failed Extracting Libtorrent"
      rm ${LIBTORRENT_TARBALL}
      rm -fr libtorrent-${LIBTORRENT_VERSION}/
      exit 1
  fi
fi
popd

if [ "$BUILD_TYPE" == "DEBUG" ]
then
    ./build-libtorrent-debug.sh
else
    ./build-libtorrent.sh
fi

if [ $? -ne 0 ]; then
    echo "Failed to Build Libtorrent"
    exit 1
fi

pushd src
# ODB Compiler
if [ ! -e "odb-2.4.0-i686-macosx.tar.bz2" ]
then
    rm -fr odb-2.4.0-i686-macosx/

    if wget http://www.codesynthesis.com/download/odb/2.4/odb-2.4.0-i686-macosx.tar.bz2
    then
        echo "downloaded ODB compiler"
    else
        rm odb-2.4.0-i686-macosx.tar.bz2
        exit 1
    fi
fi

if [ ! -e "odb-2.4.0-i686-macosx" ]
then
    if tar -xjvf odb-2.4.0-i686-macosx.tar.bz2
    then
        echo "export PATH=$(pwd)/odb-2.4.0-i686-macosx/bin:\$PATH" >> ~/.bash_profile
        # http://www.joshstaiger.org/archives/2005/07/bash_profile_vs.html
        source ~/.bash_profile
    else
        echo "Failed to extract ODB compiler odb-2.4.0-i686-macosx.tar.bz2"
        rm odb-2.4.0-i686-macosx.tar.bz2
        rm -fr odb-2.4.0-i686-macosx
        exit 1
    fi
fi

# ODB Common Runtime Library
if [ ! -e "libodb-2.4.0.tar.bz2" ]
then
    rm -fr libodb-2.4.0/

    if wget http://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2
    then
        echo "Downloaded ODB common runtime library"
    else
        echo "Failed to download ODB common runtime library"
        rm libodb-2.4.0.tar.bz2
        exit 1
    fi
fi

if [ ! -e "libodb-2.4.0" ]
then
    if tar -xjvf libodb-2.4.0.tar.bz2
    then
        cd libodb-2.4.0
        ./configure
        if [ $? -ne 0 ]; then
            echo "Failed to configure libodb"
            cd ../
            rm -fr libodb-2.4.0
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Failed to Build libodb"
            cd ../
            rm -fr libodb-2.4.0
            exit 1
        fi
        make install
        if [ $? -ne 0 ]; then
            echo "Failed to install libodb"
            cd ../
            rm -fr libodb-2.4.0
            exit 1
        fi
        cd ..
    else
        echo "failed to extract libodb-2.4.0.tar.bz2"
        rm libodb-2.4.0.tar.bz2
        rm -fr libodb-2.4.0
        exit 1
    fi
fi

# ODB Database Runtime Library
if [ ! -e "libodb-sqlite-2.4.0.tar.bz2" ]
then
    rm -fr libodb-sqlite/

    if wget http://www.codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.tar.bz2
    then
        echo "Downloaded libodb-sqlite-2.4.0.tar.bz2"
    else
        echo "Failed to download libodb-sqlite-2.4.0.tar.bz2"
        rm libodb-sqlite-2.4.0.tar.bz2
        exit 1
    fi
fi

if [ ! -e "libodb-sqlite-2.4.0" ]
then
    if tar -xjvf libodb-sqlite-2.4.0.tar.bz2
    then
        cd libodb-sqlite-2.4.0
        ./configure
        if [ $? -ne 0 ]; then
            echo "Failed to configure libodb-sqlite"
            cd ../
            rm -fr libodb-sqlite-2.4.0
            exit 1
        fi
        make
        if [ $? -ne 0 ]; then
            echo "Failed to build libodb-sqlite"
            cd ../
            rm -fr libodb-sqlite-2.4.0
            exit 1
        fi
        make install
        if [ $? -ne 0 ]; then
            echo "Failed to install libodb-sqlite"
            cd ../
            rm -fr libodb-sqlite-2.4.0
            exit 1
        fi
    else
        echo "Failed to extract libodb-sqlite-2.4.0.tar.bz2"
        rm libodb-sqlite-2.4.0.tar.bz2
        rm -fr libodb-sqlite-2.4.0
        exit 1
    fi
fi
popd

pushd src
if [ ! -e "${LIBPNG_TARBALL}" ]
then
    rm -fr ${LIBPNG_VERSION}

    if wget -O ${LIBPNG_TARBALL} "ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng16/${LIBPNG_TARBALL}"
    then
        echo "Download Successful"
    else
        echo "Downloading ${LIBPNG_TARBALL} failed"
        rm ${LIBPNG_TARBALL}
        exit 1
    fi
fi

if [ ! -e "${LIBPNG_VERSION}" ]
then
    if tar -xzvf "${LIBPNG_TARBALL}"
    then
        cd "${LIBPNG_VERSION}"/
        CFLAGS=-mmacosx-version-min=10.7 ./configure
        make
        make install
    else
        echo "Failed to build libpng"
        exit 1
    fi
fi
popd

#mSIGNA (bulding only required libraries for JoyStream)
pushd src
if [ ! -e "mSIGNA" ]
then
    if git clone https://github.com/JoyStream/mSIGNA
    then
        echo "Cloned mSIGNA Successfuly"
    else
        echo "Failed to clone mSIGNA repo"
        rm -fr mSIGNA
        exit 1
    fi
fi

cd mSIGNA/
git checkout joystream-master
if [ $? -ne 0 ]; then
    echo "Local modification of mSIGNA branch, please resolve and try again"
fi
git pull origin joystream-master
if [ $? -ne 0 ]; then
    echo "Problem getting latest mSIGNA commits, please resolve and try again"
fi
cd deps/qrencode-3.4.3
CFLAGS=-mmacosx-version-min=10.7 ./configure
make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA qrencode"
    exit 1
fi
make install
cd ../stdutils
SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA stdutils"
    exit 1
fi
cd ../logger
make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA logger"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../Signals
SYSROOT=../../sysroot make install
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA Signals"
    exit 1
fi
cd ../CoinCore
make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinCore"
    exit 1
fi
SYSROOT=../../sysroot make install
cd ../CoinQ
make
if [ $? -ne 0 ]; then
    echo "Failed to build mSIGNA CoinQ"
    exit 1
fi
SYSROOT=../../sysroot make install

#optionally build full mSIGNA app
#cd ../../
#./build-all.sh osx
