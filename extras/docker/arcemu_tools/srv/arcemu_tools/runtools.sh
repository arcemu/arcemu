#!/bin/bash
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
TOOLS_PATH=`dirname $0`
WORKDIR=`pwd`

echo "Running Arcemu extractor tools..."
echo "Tools path: $TOOLS_PATH"
echo "Current directory: $WORKDIR"

echo "Running DBC & maps extraction..."
$TOOLS_PATH/ad
if [ $? -ne 0 ]
then
    exit 1
fi


echo "Running creature data extraction..."
$TOOLS_PATH/creature_data
if [ $? -ne 0 ]
then
    exit 1
fi

echo "Creating vmaps directory..."
mkdir vmaps
if [ $? -ne 0 ]
then
    echo "Failed to create directory 'vmaps'. Exiting.."
    exit 1
fi

echo "Running vmap extraction and assembly. This will take a long while..."

echo "Running vmap extractor..."
$TOOLS_PATH/vmap_extractor
if [ $? -ne 0 ]
then
    exit 1
fi

echo "Running vmap assembly..."
$TOOLS_PATH/vmap_assembler Buildings vmaps
if [ $? -ne 0 ]
then
    exit 1
fi


echo "All tools finished successfully."
exit 0
