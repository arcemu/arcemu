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

#ifndef __STATS_H
#define __STATS_H

#include "Unit.h"
#include "UpdateMask.h"
#include "ItemInterface.h"

enum Stats
{
	STAT_STRENGTH,
	STAT_AGILITY,
	STAT_STAMINA,
	STAT_INTELLECT,
	STAT_SPIRIT,
};

SERVER_DECL uint32 getConColor(uint16 AttackerLvl, uint16 VictimLvl);
SERVER_DECL uint32 CalculateXpToGive(Unit *pVictim, Unit *pAttacker);
SERVER_DECL uint32 CalculateStat(uint16 level,double a3, double a2, double a1, double a0);
SERVER_DECL uint32 CalculateDamage( Unit* pAttacker, Unit* pVictim, uint32 weapon_damage_type, uint64 spellgroup, SpellEntry* ability );
SERVER_DECL uint32 GainStat(uint16 level, uint8 playerclass,uint8 Stat);
SERVER_DECL bool isEven (int num);

#endif
