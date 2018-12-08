FROM ubuntu:xenial

RUN apt-get update
RUN apt-get install -y libssl1.0.0 libmysqlclient20
RUN mkdir -p /srv/arcemu_world
RUN mkdir -p /srv/arcemu_world/etc
RUN mkdir -p /srv/arcemu_world/modules
RUN mkdir -p /srv/arcemu_world/DBC
RUN mkdir -p /srv/arcemu_world/maps
RUN mkdir -p /srv/arcemu_world/log

COPY srv/arcemu_world/etc/world.conf /srv/arcemu_world/etc/world.conf
COPY srv/arcemu_world/etc/realms.conf /srv/arcemu_world/etc/realms.conf
COPY srv/arcemu_world/etc/optional.conf /srv/arcemu_world/etc/optional.conf

COPY srv/arcemu_world/world /srv/arcemu_world/world
COPY srv/arcemu_world/modules/* /srv/arcemu_world/modules/

EXPOSE 8129

WORKDIR /srv/arcemu_world
CMD ["./world"]
