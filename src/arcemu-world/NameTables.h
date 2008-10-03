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

//
// NameTables.h
//

#ifndef __NAMETABLES_H
#define __NAMETABLES_H

struct NameTableEntry
{
	uint32 id;
	const char *name;
};

static inline const char* LookupName(uint32 id, NameTableEntry *table)
{
	for(uint32 i = 0; table[i].name != 0; i++)
	{
		if (table[i].id == id)
			return table[i].name;
	}

	return "UNKNOWN";
}

extern NameTableEntry g_worldOpcodeNames[];
extern NameTableEntry g_logonOpcodeNames[];
extern NameTableEntry g_pluginOpcodeNames[];

#endif
