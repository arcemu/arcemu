################################################################################
#
# ArcEmu MMORPG Server
# Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
# Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
################################################################################
FROM ubuntu:bionic

RUN apt-get update
RUN apt-get install -y libssl1.0.0 libmysqlclient20
RUN mkdir -p /srv/arcemu_world
RUN mkdir -p /srv/arcemu_world/etc
RUN mkdir -p /srv/arcemu_world/plugins
RUN mkdir -p /srv/arcemu_world/DBC
RUN mkdir -p /srv/arcemu_world/maps
RUN mkdir -p /srv/arcemu_world/log
RUN mkdir -p /srv/arcemu_world/scripts

COPY srv/arcemu_world/world /srv/arcemu_world/world
COPY srv/arcemu_world/plugins/* /srv/arcemu_world/plugins/

EXPOSE 8129

WORKDIR /srv/arcemu_world
CMD ["./world"]
