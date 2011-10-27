/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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