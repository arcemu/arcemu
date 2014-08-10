/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "StdAfx.h"
namespace Arcemu{

	GO_SpellFocus::GO_SpellFocus() : GameObject(){
	}

	GO_SpellFocus::GO_SpellFocus(uint64 GUID) : GameObject(GUID){
	}

	GO_SpellFocus::~GO_SpellFocus(){
	}

	void GO_SpellFocus::OnPushToWorld(){
		GameObject::OnPushToWorld();
		SpawnLinkedTrap();
	}

	void GO_SpellFocus::SpawnLinkedTrap(){
		uint32 trapid = pInfo->spellFocus.linkedTrapId;

		if (trapid == 0)
			return;

		GameObject *go = m_mapMgr->CreateGameObject(trapid);
		if (go == NULL){
			sLog.outError("Failed to create linked trap for GameObject %u ( %s ).", pInfo->ID, pInfo->Name);
			return;
		}

		if (!go->CreateFromProto(trapid, m_mapId, m_position.x, m_position.y, m_position.z, m_position.o)){
			sLog.outError("Failed CreateFromProto for linked trap of GameObject %u ( %s ).", pInfo->ID, pInfo->Name);
			return;
		}

		go->SetFaction(GetFaction());
		go->SetUInt64Value(OBJECT_FIELD_CREATED_BY, GetGUID());
		go->PushToWorld(m_mapMgr);
	}
}