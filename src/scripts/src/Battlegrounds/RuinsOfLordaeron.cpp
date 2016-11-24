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

#include "RuinsOfLordaeron.h"

RuinsOfLordaeron::RuinsOfLordaeron( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side ) : 
Arena( mgr, id, lgroup, t, players_per_side ){
}

RuinsOfLordaeron::~RuinsOfLordaeron(){
}

void RuinsOfLordaeron::OnCreate(){
	GameObject *obj = NULL;

	obj = SpawnGameObject(185917, 572, 1278.647705f, 1730.556641f, 31.605574f, 1.684245f, 32, 1375, 1.0f);
	obj->SetState(1);
	obj->SetParentRotation(2, 0.746058f);
	obj->SetParentRotation(3, 0.665881f);
	m_gates.insert(obj);
	
	obj = SpawnGameObject(185918, 572, 1293.560791f, 1601.937988f, 31.605574f, -1.457349f, 32, 1375, 1.0f);
	obj->SetState(1);
	obj->SetParentRotation(2, -0.665881f);
	obj->SetParentRotation(3, 0.746058f);
	m_gates.insert(obj);

	Arena::OnCreate();
}

void RuinsOfLordaeron::HookOnShadowSight(){
	m_buffs[0] = SpawnGameObject(184664, 572, 1328.729268f, 1632.738403f, 34.838585f, 2.611449f, 32, 1375, 1.0f);
	m_buffs[0]->SetState(1);
	m_buffs[0]->SetParentRotation(2, 0.904455f);
	m_buffs[0]->SetParentRotation(3, -0.426569f);
	m_buffs[0]->SetType(GAMEOBJECT_TYPE_TRAP);
	m_buffs[0]->SetAnimProgress(100);
	m_buffs[0]->PushToWorld(m_mapMgr);
	
	m_buffs[1] = SpawnGameObject(184664, 572, 1243.306763f, 1699.334351f, 34.837566f, 5.713773f, 32, 1375, 1.0f);
	m_buffs[1]->SetState(1);
	m_buffs[1]->SetParentRotation(2, 0.90445f);
	m_buffs[1]->SetParentRotation(3, -0.426569f);
	m_buffs[1]->SetType(GAMEOBJECT_TYPE_TRAP);
	m_buffs[1]->SetAnimProgress(100);
	m_buffs[1]->PushToWorld(m_mapMgr);

}

LocationVector RuinsOfLordaeron::GetStartingCoords( uint32 Team ){
	if(Team)
		return LocationVector(1277.105103f, 1743.956177f, 31.603209f);
	else
		return LocationVector(1295.322388f, 1585.953369f, 31.605387f);

}

bool RuinsOfLordaeron::HookHandleRepop( Player *plr ){
	LocationVector dest(0, 0, 0, 0);
	dest.ChangeCoords(1286.112061f, 1668.334961f, 39.289127f);
	plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), dest);
	return true;
}

