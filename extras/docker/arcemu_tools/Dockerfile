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
RUN mkdir /srv/arcemu_tools
RUN mkdir /srv/arcemu_tools/data

COPY srv/arcemu_tools/* /srv/arcemu_tools/

WORKDIR /srv/arcemu_tools/data
CMD ["bash","/srv/arcemu_tools/runtools.sh"]
