/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#ifndef __AREATRIGGERHANDLER_H
#define __AREATRIGGERHANDLER_H


enum AreaTriggerType
{
	ATTYPE_NULL		 = 0,
	ATTYPE_INSTANCE	 = 1,
	ATTYPE_QUESTTRIGGER = 2,
	ATTYPE_INN		  = 3,
	ATTYPE_TELEPORT	 = 4,
	ATTYPE_SPELL		= 5,
	ATTYPE_BATTLEGROUND = 6,
};

#pragma pack(push,1)
typedef struct AreaTrigger
{
	uint32 AreaTriggerID;
	uint8 Type;
	uint32 Mapid;
	uint32 PendingScreen;
	char * Name;
	float x;
	float y;
	float z;
	float o;
	uint32 required_honor_rank;
	uint32 required_level;
}AreaTrigger;
#pragma pack(pop)



#endif
