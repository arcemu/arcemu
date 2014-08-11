/*
* ArcEmu MMORPG Server
* Copyright(C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
* Copyright(C) 2008 - 2014 <http://www.ArcEmu.org/>
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "StdAfx.h"
namespace Arcemu{
	GO_FishingNode::GO_FishingNode() : GO_Lootable(){
		FishHooked = false;
	}

	GO_FishingNode::GO_FishingNode(uint64 GUID) : GO_Lootable(GUID){
		FishHooked = false;
	}

	GO_FishingNode::~GO_FishingNode(){
	}

	bool GO_FishingNode::UseNode(){
		sEventMgr.RemoveEvents(this);

		// Clicking on the bobber before something is hooked
		if (!FishHooked){
			EndFishing(true);
			return false;
		}
		return true;
	}

	void GO_FishingNode::EndFishing(bool abort){
		if (!abort)
			sEventMgr.AddEvent(TO_GAMEOBJECT(this), &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, 10 * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else
			ExpireAndDelete();
	}

	void GO_FishingNode::EventFishHooked(){

		WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM, 12);

		data << uint64(GetGUID());
		data << uint32(0); // value < 4

		SendMessageToSet(&data, false, false);

		FishHooked = true;
	}

	bool GO_FishingNode::HasLoot(){
		for (vector< __LootItem >::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr){
			if ((itr->item.itemproto->Bonding == ITEM_BIND_QUEST) || (itr->item.itemproto->Bonding == ITEM_BIND_QUEST2))
				continue;
			if (itr->iItemsCount > 0)
				return true;
		}
		return false;
	}
}