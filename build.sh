#!/usr/bin/env bash
set -eu

TARGET=main
# TARGET=test

SRC=${TARGET}.c
OUT=${TARGET}.out

glibc_install=`pwd`/glibc/build/install

clang -Os -g2 -march=native \
  -L "${glibc_install}/lib" \
  -I "${glibc_install}/include" \
  -Wl,--rpath="${glibc_install}/lib" \
  -Wl,--dynamic-linker="${glibc_install}/lib/ld-linux-x86-64.so.2" \
  -std=c11 \
  -o ${OUT} \
  ${SRC} \

