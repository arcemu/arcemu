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

#include "RingOfValor.h"

RingOfValor::RingOfValor( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side ) : 
Arena( mgr, id, lgroup, t, players_per_side ){
}

RingOfValor::~RingOfValor(){
}

void RingOfValor::OnCreate(){
	GameObject *obj = NULL;
	
	obj = SpawnGameObject(194030, 618, 763.93f, -295.0f, 26.0f, 0.0f, 40, 1375, 1.0f);
	obj->PushToWorld(m_mapMgr);
	
	obj = SpawnGameObject(194031, 618, 763.93f, -274.0f, 26.0f, 0.0f, 40, 1375, 1.0f);
	obj->PushToWorld(m_mapMgr);
	
	obj = SpawnGameObject(193458, 618, 763.630f, -261.783f, 26.0f, 0.0f, 40, 1375, 1.0f);
	obj->PushToWorld(m_mapMgr);
	
	// These GOs freeze the client, probably db issue tho, however I will leave it commented for now
	// -dfighter
	//obj = SpawnGameObject(193459, 618, 763.761f, -306.230f, 26.0f, 0.0f, 40, 1375, 1.0f);
	//obj->PushToWorld(m_mapMgr);
	//obj = SpawnGameObject(193460, 618, 802.313f, -284.349f, 24.6f, 0.0f, 40, 1375, 1.0f);
	//obj->PushToWorld(m_mapMgr);
	
	obj = SpawnGameObject(193461, 618, 723.522f, -284.428f, 24.6f, 0.0f, 40, 1375, 1.0f);
	obj->PushToWorld(m_mapMgr);
	
	obj = SpawnGameObject(192392, 618, 763.93f, -295.0f, 27.0f, 0.0f, 32, 1375, 1.0f);
	obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
	obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	m_gates.insert(obj);
	
	obj = SpawnGameObject(192391, 618, 763.93f, -274.0f, 27.0f, 0.0f, 32, 1375, 1.0f);
	obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
	obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	m_gates.insert(obj);

	Arena::OnCreate();
}

LocationVector RingOfValor::GetStartingCoords( uint32 Team ){
	if(Team)
		return LocationVector(763.6011f, -294.3227f, 28.4f);
	else
		return LocationVector(763.9755f, -274.0825f, 28.4f);
}

bool RingOfValor::HookHandleRepop( Player *plr ){
	LocationVector dest;
	dest.ChangeCoords(762.91f, -284.28f, 28.28f);
	plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), dest);
	return true;
}

