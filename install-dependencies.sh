#!/bin/sh

#NPROC=$(nproc)

sudo apt-get install -y automake autoconf libtool perl ninja-build build-essential libssl-dev

#wget https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0.tar.gz

#tar -zxvf cmake-3.20.0.tar.gz

#cd cmake-3.20.0 || exit

#./bootstrap

#make $NPROC

#sudo make install $NPROC

cmake --version
