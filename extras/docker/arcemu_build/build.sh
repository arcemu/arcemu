FROM ubuntu:xenial

RUN apt-get update
RUN apt-get install -y git cmake make g++
RUN apt-get install -y zlib1g-dev libssl-dev libpcre3-dev libbz2-dev libmysqlclient-dev libmysql++-dev

RUN mkdir -p /srv/build/arcemu
RUN mkdir -p /srv/build/arcemu/bin
COPY build.sh /srv/build/arcemu/build.sh
RUN chmod a+x /srv/build/arcemu/build.sh

WORKDIR /srv/build/arcemu
CMD ["./build.sh"]

root@ubuntu-xenial:~/docker# ^C
root@ubuntu-xenial:~/docker# cat build.sh
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
