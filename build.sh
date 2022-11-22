#!/bin/sh

NPROC=$(nproc)

mkdir build

cd build || exit

cmake -DCMAKE_BUILD_TYPE=Release -DOATPP_BUILD_TESTS=OFF -DLIBRESSL_APPS=OFF -DLIBRESSL_TESTS=OFF  ..

cmake --build . --target HttpServer -- -j $NPROC
