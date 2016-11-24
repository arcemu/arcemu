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

#include "DalaranSewers.h"

DalaranSewers::DalaranSewers( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side ) : 
Arena( mgr, id, lgroup, t, players_per_side )
{
}

DalaranSewers::~DalaranSewers()
{
}

void DalaranSewers::OnCreate()
{
    for (uint8 i = 0; i < 2; i++)
    {
        if (GameObject* obj = SpawnGameObject(DalaranSewersGates[i].entry, DalaranSewersGates[i].mapid, DalaranSewersGates[i].x, 
        DalaranSewersGates[i].y, DalaranSewersGates[i].z, DalaranSewersGates[i].orientation, DalaranSewersGates[i].flags, 
        DalaranSewersGates[i].faction, DalaranSewersGates[i].scale))
        {
            obj->SetState(DalaranSewersGates[i].state);
            obj->SetAnimProgress(DalaranSewersGates[i].anim_progress);
        }
        else
            Log.Error("Arena", "Failed to create object (entry: %u) in Dalaran Severs arena (map id 617)");
    }

    // fountain
    if (GameObject* obj = SpawnGameObject(191877, 617, 1291.974487f, 791.844666f, 9.339742f, 3.116816f, 32, 1375, 1.0f))
        obj->PushToWorld(m_mapMgr);
    else
        Log.Error("Arena", "Failed to create object 191877 in Dalaran Severs arena (map id 617)");

    Arena::OnCreate();
}

LocationVector DalaranSewers::GetStartingCoords(uint32 Team)
{
    return DalaranArenaStartLocation[Team];
}

bool DalaranSewers::HookHandleRepop(Player* plr)
{
    plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), dalaran_arena_repop_pos);
    return true;
}
