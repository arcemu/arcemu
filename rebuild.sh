#!/bin/sh

DEST=~/wowserver/test
OPTS="--prefix=$DEST"

autoreconf -fi
export LIBS=-ldl; ./configure $OPTS
make clean
make -j2
make install
