#!/bin/bash

set -e

echo "There must not be any @ in path"
sleep 2

git clone https://sourceware.org/git/glibc.git --branch release/2.36/master --depth 1
mkdir -p glibc/build/install

cd glibc/build

export glibc_install=$(pwd)/install

../configure --prefix "$glibc_install"

make -j `nproc`
make install -j `nproc`

