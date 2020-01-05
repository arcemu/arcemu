FROM ubuntu:bionic

RUN apt-get update
RUN apt-get install -y libssl1.0.0 libmysqlclient20
RUN mkdir -p /srv/arcemu_logon
RUN mkdir -p /srv/arcemu_logon/etc
RUN mkdir -p /srv/arcemu_logon/log

COPY srv/arcemu_logon/logon /srv/arcemu_logon/logon

EXPOSE 3724

WORKDIR /srv/arcemu_logon
CMD ["./logon"]
