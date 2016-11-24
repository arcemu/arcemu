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

#ifndef DALARAN_SEWERS_H
#define DALARAN_SEWERS_H

#include "StdAfx.h"

// start locations
static LocationVector DalaranArenaStartLocation[] =
{
    LocationVector(1363.3609f, 817.3569f, 14.8128f),  // green team
    LocationVector(1219.5115f, 765.0264f, 14.8253f)   // gold team
};

// repop position
const LocationVector dalaran_arena_repop_pos = LocationVector(1292.51f, 792.05f, 9.34f);

struct DalaranArenaObjectSpawn{
    uint32 entry;
    uint16 mapid;
    float x, y, z, orientation;
    uint32 flags;
    uint32 faction;
    float scale;
    uint8 state;
    uint32 anim_progress;
};

const DalaranArenaObjectSpawn DalaranSewersFountain = { 191877, 617, 1291.974487f, 791.844666f, 9.339742f, 3.116816f, 32, 1375, 1.0f };

static DalaranArenaObjectSpawn DalaranSewersGates[] =
{
    { 192643, 617, 1232.11f, 764.699f, 20.3f, 0.0f, 32, 1375, 2.0f, 1, 100 },
    { 192642, 617, 1350.02f, 817.502f, 19.1398f, 0.0f, 32, 1375, 2.0f, 100 }
};

class DalaranSewers : public Arena{
public:
	DalaranSewers( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side );
	~DalaranSewers();

	static CBattleground* Create( MapMgr* m, uint32 i, uint32 l, uint32 t, uint32 players_per_side ){
		return new DalaranSewers( m, i, l, t, players_per_side );
	}

	void OnCreate();
	LocationVector GetStartingCoords( uint32 Team );
	bool HookHandleRepop( Player *plr );
};

#endif
