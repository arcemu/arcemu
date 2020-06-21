/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#ifndef OBJECTCONSTANTS_H_
#define OBJECTCONSTANTS_H_

enum HIGHGUID_TYPE
{
    HIGHGUID_TYPE_PLAYER			= 0x00000000,
    HIGHGUID_TYPE_CORPSE			= 0x30000000,
    HIGHGUID_TYPE_ITEM				= 0x40000000,
    HIGHGUID_TYPE_CONTAINER			= 0x50000000,			// confirm this pl0x
    HIGHGUID_TYPE_DYNAMICOBJECT		= 0x60000000,
    HIGHGUID_TYPE_WAYPOINT			= 0x10000000,
    HIGHGUID_TYPE_TRANSPORTER		= 0x1FC00000,
    HIGHGUID_TYPE_GAMEOBJECT		= 0xF1100000,
    HIGHGUID_TYPE_UNIT				= 0xF1300000,
    HIGHGUID_TYPE_PET				= 0xF1400000,
    HIGHGUID_TYPE_VEHICLE			= 0xF1500000,
//===============================================
    HIGHGUID_TYPE_MASK				= 0xFFF00000,
    LOWGUID_ENTRY_MASK				= 0x00FFFFFF,
};

#define GET_TYPE_FROM_GUID(x) ( Arcemu::Util::GUID_HIPART( (x) ) & HIGHGUID_TYPE_MASK )
#define GET_LOWGUID_PART(x) ( Arcemu::Util::GUID_LOPART( (x) ) & LOWGUID_ENTRY_MASK )

#define MAX_INTERACTION_RANGE 5.0f

// TODO: fix that type mess

enum TYPE
{
    TYPE_OBJECT		 = 1,
    TYPE_ITEM		   = 2,
    TYPE_CONTAINER	  = 4,
    TYPE_UNIT		   = 8,
    TYPE_PLAYER		 = 16,
    TYPE_GAMEOBJECT	 = 32,
    TYPE_DYNAMICOBJECT  = 64,
    TYPE_CORPSE		 = 128,
    TYPE_AIGROUP		= 256,
    TYPE_AREATRIGGER	= 512,
};

enum TYPEID
{
    TYPEID_OBJECT		= 0,
    TYPEID_ITEM		  = 1,
    TYPEID_CONTAINER	 = 2,
    TYPEID_UNIT		  = 3,
    TYPEID_PLAYER		= 4,
    TYPEID_GAMEOBJECT	= 5,
    TYPEID_DYNAMICOBJECT = 6,
    TYPEID_CORPSE		= 7,
    TYPEID_AIGROUP	   = 8,
    TYPEID_AREATRIGGER   = 9,
};

enum OBJECT_UPDATE_TYPE
{
    UPDATETYPE_VALUES = 0,
    //  8 bytes - GUID
    //  Goto Update Block
    UPDATETYPE_MOVEMENT = 1,
    //  8 bytes - GUID
    //  Goto Position Update
    UPDATETYPE_CREATE_OBJECT = 2,
    //  8 bytes - GUID
    //  1 byte - Object Type (*)
    //  Goto Position Update
    //  Goto Update Block
    UPDATETYPE_CREATE_YOURSELF = 3, // Looks like 3 & 4 do the same thing
    //  4 bytes - Count
    //  Loop Count Times:
    //  8 bytes - GUID
    UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4 // <- this is correct, not sure about 3
                                      //  4 bytes - Count
                                      //  Loop Count Times:
                                      //  8 bytes - GUID

};

#endif
