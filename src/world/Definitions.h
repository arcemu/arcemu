/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WOWSERVER_DEFINITION_H
#define WOWSERVER_DEFINITION_H


const unsigned NUM_MAPS = 800;
const unsigned NUM_INSTANCE_MODES = 4;


// Sorry...need this enumeration in Player.*
enum INSTANCE_MODE
{
    MODE_NORMAL = 0,
    MODE_HEROIC = 1
};

enum RAID_MODE
{
    MODE_NORMAL_10MEN	=	0,
    MODE_NORMAL_25MEN	=	1,
    MODE_HEROIC_10MEN	=	2,
    MODE_HEROIC_25MEN	=	3,
    TOTAL_RAID_MODES    =   4
};


#endif
