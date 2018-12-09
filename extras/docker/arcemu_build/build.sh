#!/bin/bash

git clone https://github.com/arcemu/arcemu.git src
if [ $? -ne 0 ]
then
    exit 1
fi

mkdir build
if [ $? -ne 0 ]
then
    exit 1
fi

cd build
cmake -DCMAKE_INSTALL_PREFIX=/srv/build/arcemu/bin ../src/cmake
if [ $? -ne 0 ]
then
    exit 1
fi

make
if [ $? -ne 0 ]
then
    exit 1
fi

make install
if [ $? -ne 0 ]
then
    exit 1
fi

exit 0
