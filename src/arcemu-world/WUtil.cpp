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

#include "StdAfx.h"
#include <iostream>

uint32 Arcemu::Util::GUID_HIPART(uint64 GUID)
{
	uint32 higuid = *(reinterpret_cast< const uint32* >(&GUID) + 1);

	return higuid;
}

uint32 Arcemu::Util::GUID_LOPART(uint64 GUID)
{
	uint32 loguid = *(reinterpret_cast< const uint32* >(&GUID));

	return loguid;
}

void Arcemu::Util::ArcemuAssert(bool condition)
{
	if(!condition)
	{
		LOG_ERROR("Assertion failed. Please submit the callstack on ArcEmu IssueTracker (unless you are using a repack).");
		sLog.Close();

		// bogus null function call to make sure we stop and make a core dump / crash dump
		((void(*)())0)();
	}
}

uint64 Arcemu::Util::MAKE_PET_GUID(uint32 entry, uint32 lowGUID)
{
	uint64 val = 0;

	val = uint64(HIGHGUID_TYPE_PET) << 32;
	val = val | (uint64(entry) << 24);
	val = val | lowGUID;

	return val;
}

uint64 Arcemu::Util::MAKE_ITEM_GUID(uint32 lowguid)
{
	uint64 GUID = 0;

	uint32* u = reinterpret_cast< uint32* >(&GUID);

	u[ 0 ] = lowguid;
	u[ 1 ] = HIGHGUID_TYPE_ITEM;

	return GUID;
}

uint32 Arcemu::Util::GET_CREATURE_ENTRY_FROM_GUID(uint64 guid)
{
	return ( guid >> 24) & 0x0FFFFFFF;
}

uint32 Arcemu::Util::MAKE_UNIT_ACTION_BUTTON( uint32 spell, uint32 unk ){
	return ( uint32( spell ) | ( uint32( unk ) << 24 ) );
}

/* Some minor documentation about the time field
// MOVE THIS DOCUMENTATION TO THE WIKI

minute's = 0x0000003F                  00000000000000000000000000111111
hour's   = 0x000007C0                  00000000000000000000011111000000
weekdays = 0x00003800                  00000000000000000011100000000000
days     = 0x000FC000                  00000000000011111100000000000000
months   = 0x00F00000                  00000000111100000000000000000000
years    = 0x1F000000                  00011111000000000000000000000000
unk	     = 0xE0000000                  11100000000000000000000000000000
*/

#define MINUTE_BITMASK      0x0000003F
#define HOUR_BITMASK        0x000007C0
#define WEEKDAY_BITMASK     0x00003800
#define DAY_BITMASK         0x000FC000
#define MONTH_BITMASK       0x00F00000
#define YEAR_BITMASK        0x1F000000
#define UNK_BITMASK         0xE0000000

#define MINUTE_SHIFTMASK    0
#define HOUR_SHIFTMASK      6
#define WEEKDAY_SHIFTMASK   11
#define DAY_SHIFTMASK       14
#define MONTH_SHIFTMASK     20
#define YEAR_SHIFTMASK      24
#define UNK_SHIFTMASK       29

uint32 Arcemu::Util::MAKE_GAME_TIME()
{
	uint32 gameTime = 0;

	time_t basetime = UNIXTIME;
	uint32 DayOfTheWeek;
	if(localtime(&basetime)->tm_wday == 0)
		DayOfTheWeek = 6;
	else
		DayOfTheWeek = localtime(&basetime)->tm_wday - 1;

	uint32 DayOfTheMonth = localtime(&basetime)->tm_mday - 1;
	uint32 CurrentMonth = localtime(&basetime)->tm_mon;
	uint32 CurrentYear = localtime(&basetime)->tm_year - 100;
	uint32 hours = localtime(&basetime)->tm_hour;
	uint32 minutes = localtime(&basetime)->tm_min;

	gameTime = ((minutes << MINUTE_SHIFTMASK) & MINUTE_BITMASK);
	gameTime |= ((hours << HOUR_SHIFTMASK) & HOUR_BITMASK);
	gameTime |= ((DayOfTheWeek << WEEKDAY_SHIFTMASK) & WEEKDAY_BITMASK);
	gameTime |= ((DayOfTheMonth << DAY_SHIFTMASK) & DAY_BITMASK);
	gameTime |= ((CurrentMonth << MONTH_SHIFTMASK) & MONTH_BITMASK);
	gameTime |= ((CurrentYear << YEAR_SHIFTMASK) & YEAR_BITMASK);
	
	return gameTime;
}