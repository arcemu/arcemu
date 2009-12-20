#!/bin/sh

DEST=~/realm
OPTS="--prefix=$DEST"

autoreconf -fi
./configure $OPTS
make clean
make -j3
make install
