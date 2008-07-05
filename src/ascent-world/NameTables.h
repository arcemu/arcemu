/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
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
