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
