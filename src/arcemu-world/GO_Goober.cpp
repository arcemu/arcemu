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
	GO_Goober::GO_Goober() : GameObject(){
	}

	GO_Goober::GO_Goober(uint64 GUID) : GameObject(GUID){
		spell = NULL;
	}

	GO_Goober::~GO_Goober(){
	}

	void GO_Goober::InitAI(){
		GameObject::InitAI();

		if(pInfo->goober.linkedTrapId != 0){
			GameObjectInfo *i = GameObjectNameStorage.LookupEntry(pInfo->goober.linkedTrapId);

			if (i != NULL){
				if (i->trap.spellId != 0)
					spell = dbcSpell.LookupEntryForced(i->trap.spellId);
			}
		}
	}

	void GO_Goober::Open(){
		SetState(GAMEOBJECT_STATE_OPEN);

		if(pInfo->goober.autoCloseTime != 0)
			sEventMgr.AddEvent(this, &GO_Goober::Close, EVENT_GAMEOBJECT_CLOSE, pInfo->goober.autoCloseTime, 1, 0);
	}

	void GO_Goober::Close(){
		sEventMgr.RemoveEvents(this, EVENT_GAMEOBJECT_CLOSE);
		SetState(GAMEOBJECT_STATE_CLOSED);
	}

	void GO_Goober::Use(uint64 GUID){
		if(GetState() == GAMEOBJECT_STATE_CLOSED){
			Open();

			if (spell != NULL){
				CastSpell(GUID, spell);
			}
		}else{
			Close();
		}
	}
}
