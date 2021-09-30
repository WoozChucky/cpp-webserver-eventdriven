#!/bin/sh

NPROC=$(nproc)

cd LibreSSL || exit

sudo sh autogen.sh

sudo ./configure

mkdir build

cd build || exit

cmake -DCMAKE_BUILD_TYPE=Release -DLIBRESSL_APPS=OFF -DLIBRESSL_TESTS=OFF ..

make -j $NPROC

sudo make install -j $NPROC

cd .. # build
cd .. # root libressl
