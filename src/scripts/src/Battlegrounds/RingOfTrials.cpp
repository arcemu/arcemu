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

#include "RingOfTrials.h"

RingOfTrials::RingOfTrials( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side ) : 
Arena( mgr, id, lgroup, t, players_per_side ){
}

RingOfTrials::~RingOfTrials(){
}

void RingOfTrials::OnCreate(){

	if (GameObject *obj = SpawnGameObject(183979, 559, 4090.064453f, 2858.437744f, 10.236313f, 0.492805f, 32, 1375, 1.0f))
	{
        obj->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
        obj->SetParentRotation(2, 0.243916f);
        obj->SetParentRotation(3, 0.969796f);
        obj->PushToWorld(m_mapMgr);
	}
    else
        Log.Error("Arena", "Failed to spawn 183979 object in Ring of Trials arena (map id 559)");

	if (GameObject *obj = SpawnGameObject(183980, 559, 4081.178955f, 2874.970459f, 12.391714f, 0.492805f, 32, 1375, 1.0f))
    {
        obj->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
        obj->SetParentRotation(2, 0.243916f);
        obj->SetParentRotation(3, 0.969796f);
        m_gates.insert(obj);
	}
    else
        Log.Error("Arena", "Failed to spawn 183980 object in Ring of Trials arena (map id 559)");

	if (GameObject *obj = SpawnGameObject(183977, 559, 4023.709473f, 2981.776611f, 10.701169f, -2.648788f, 32, 1375, 1.0f))
    {
        obj->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
        obj->SetParentRotation(2, 0.969796f);
        obj->SetParentRotation(3, -0.243916f);
        obj->PushToWorld(m_mapMgr);
	}
    else
        Log.Error("Arena", "Failed to spawn 183977 object in Ring of Trials arena (map id 559)");

	if (GameObject *obj = SpawnGameObject(183978, 559, 4031.854248f, 2966.833496f, 12.646200f, -2.648788f, 32, 1375, 1.0f))
    {
        obj->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
        obj->SetParentRotation(2, 0.969796f);
        obj->SetParentRotation(3, -0.243916f);
        m_gates.insert(obj);
    }
    else
        Log.Error("Arena", "Failed to spawn 183978 object in Ring of Trials arena (map id 559)");

	Arena::OnCreate();
}

void RingOfTrials::HookOnShadowSight(){
	if (m_buffs[0] = SpawnGameObject(184664, 559, 4011.113232f, 2896.879980f, 12.523950f, 0.486944f, 32, 1375, 1.0f))
    {
        m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
        m_buffs[0]->SetParentRotation(2, 0.904455f);
        m_buffs[0]->SetParentRotation(3, -0.426569f);
        m_buffs[0]->SetType(GAMEOBJECT_TYPE_TRAP);
        m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
        m_buffs[0]->PushToWorld(m_mapMgr);
    }
    else
        Log.Error("Arena", "Failed to spawn 184664 object in Ring of Trials arena (map id 559)");

	if (m_buffs[1] = SpawnGameObject(184664, 559, 4102.111426f, 2945.843262f, 12.662578f, 3.628544f, 32, 1375, 1.0f))
    {
        m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
        m_buffs[1]->SetParentRotation(2, 0.90445f);
        m_buffs[1]->SetParentRotation(3, -0.426569f);
        m_buffs[1]->SetType(GAMEOBJECT_TYPE_TRAP);
        m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
        m_buffs[1]->PushToWorld(m_mapMgr);
    }
    else
        Log.Error("Arena", "Failed to spawn 184664 object in Ring of Trials arena (map id 559)");
}

LocationVector RingOfTrials::GetStartingCoords( uint32 Team ){
	return Team == GOLD_TEAM ? LocationVector(4027.004883f, 2976.964844f, 11.600499f) : LocationVector(4085.861328f, 2866.750488f, 12.417445f);
}

bool RingOfTrials::HookHandleRepop( Player *plr ){
	LocationVector dest;
	dest.ChangeCoords(4057.042725f, 2918.686523f, 13.051933f);
	plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), dest);
	return true;
}

