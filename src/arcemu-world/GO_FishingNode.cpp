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

	void GO_FishingNode::OnPushToWorld(){
		uint32 zone = GetMapMgr()->GetAreaID(GetPositionX(), GetPositionY());
		if(zone == 0)
			zone = GetZoneId();

		if(lootmgr.IsFishable(zone)){ // Only set a 'splash' if there is any loot in this area / zone
			uint32 seconds[] = { 0, 4, 10, 14 };
			uint32 rnd = RandomUInt(3);
			sEventMgr.AddEvent(this, &Arcemu::GO_FishingNode::EventFishHooked, EVENT_GAMEOBJECT_FISH_HOOKED, seconds[rnd] * 1000, 1, 0);
		}

			sEventMgr.AddEvent(this, &Arcemu::GO_FishingNode::EndFishing, true, EVENT_GAMEOBJECT_END_FISHING, 17 * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	bool GO_FishingNode::UseNode(){
		sEventMgr.RemoveEvents(this);

		// Clicking on the bobber before something is hooked
		if(!FishHooked){
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