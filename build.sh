#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra"
LIBS="-lX11"

g++ -std=c++17 $CFLAGS -O2 -o example.bin linux.cpp $LIBS

