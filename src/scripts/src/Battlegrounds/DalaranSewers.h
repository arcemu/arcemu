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
static LocationVector ArenaStartLocation[]=
{
    {1363.3609f, 817.3569f, 14.8128f},  // green team
    {1219.5115f, 765.0264f, 14.8253f}   // gold team
}

// repop position
const LocationVector arena_repop_pos = LocationVector(1292.51f, 792.05f, 9.34f);

struct ArenaObjectSpawn{
    uint32 entry;
    uint8 mapid;
    float: x, y, z, orientation;
    uint32 flags;
    uint32 faction;
    float scale;
    uint8 state;
    anim_progress;
};

const* ArenaObjectSpawn DalaranSewersGO(191877, 617, 1291.974487f, 791.844666f, 9.339742f, 3.116816f, 32, 1375, 1.0f);

static ArenaObjectSpawn DalaranSewersGates[] =
{
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
