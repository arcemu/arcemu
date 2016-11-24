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
	GO_Chest::GO_Chest() : GO_Lootable(){
	}

	GO_Chest::GO_Chest(uint64 GUID) : GO_Lootable(GUID){
	}

	GO_Chest::~GO_Chest(){
	}

	void GO_Chest::InitAI(){
		GameObject::InitAI();

		if(pInfo->chest.linkedTrapId != 0){
			GameObjectInfo *i = GameObjectNameStorage.LookupEntry(pInfo->chest.linkedTrapId);

			if(i != NULL){
				if(i->trap.spellId != 0)
					spell = dbcSpell.LookupEntryForced(i->trap.spellId);
			}
		}
	}

	bool GO_Chest::HasLoot(){
		if (loot.gold > 0)
			return true;

		for (vector< __LootItem >::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr){
			if((itr->item.itemproto->Bonding == ITEM_BIND_QUEST) || (itr->item.itemproto->Bonding == ITEM_BIND_QUEST2))
				continue;

			if (itr->iItemsCount > 0)
				return true;
		}
		return false;
	}

	void GO_Chest::Open(){
		SetState(GAMEOBJECT_STATE_OPEN);
	}

	void GO_Chest::Close(){
		SetState(GAMEOBJECT_STATE_CLOSED);
		}

	void GO_Chest::Use(uint64 GUID){
		if(GetState() == GAMEOBJECT_STATE_CLOSED){
			Open();

			if(spell != NULL)
				CastSpell(GUID, spell);
		}else{
			Close();
		}
	}
}