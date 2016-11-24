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
	GO_FishingHole::GO_FishingHole() : GO_Lootable(){
	}

	GO_FishingHole::GO_FishingHole(uint64 GUID) : GO_Lootable(GUID){
		usage_remaining = 0;
	}

	GO_FishingHole::~GO_FishingHole(){
	}

	void GO_FishingHole::InitAI(){
		CalcFishRemaining(true);
	}

	bool GO_FishingHole::CanFish(){
		if (usage_remaining > 0)
			return true;
		else
			return false;
	}

	void GO_FishingHole::CatchFish(){
		ARCEMU_ASSERT(usage_remaining > 0);
		usage_remaining--;

		if (usage_remaining == 0)
			sEventMgr.AddEvent(TO_GAMEOBJECT(this), &GameObject::Despawn, uint32(0), (1800000 + RandomUInt(3600000)), EVENT_GAMEOBJECT_EXPIRE, 10000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); // respawn in 30 - 90 minutes
	}

	void GO_FishingHole::CalcFishRemaining(bool force){
		if (force || (usage_remaining == 0))
			usage_remaining = pInfo->raw.sound2 + RandomUInt(pInfo->raw.sound3 - pInfo->raw.sound2) - 1;
	}

	bool GO_FishingHole::HasLoot(){
		for (vector<__LootItem>::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr){
			if (itr->item.itemproto->Bonding == ITEM_BIND_QUEST || itr->item.itemproto->Bonding == ITEM_BIND_QUEST2)
				continue;
			if (itr->iItemsCount > 0)
				return true;
		}
		return false;
	}
}