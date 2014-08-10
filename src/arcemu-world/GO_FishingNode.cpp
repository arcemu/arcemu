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
	GO_FishingNode::GO_FishingNode() : GO_Lootable(){
	}
	GO_FishingNode::GO_FishingNode(uint64 GUID) : GO_Lootable(GUID){
	}
	GO_FishingNode::~GO_FishingNode(){
	}
	void GO_FishingNode::UseFishingNode(Player *player){
		sEventMgr.RemoveEvents(this);
		// Clicking on the bobber before something is hooked
		if ((GetFlags() & 32) == 0){
			player->GetSession()->OutPacket(SMSG_FISH_NOT_HOOKED);
			EndFishing(player, true);
			return;
		}
		uint32 zone = player->GetAreaID();
		if (zone == 0) // If the player's area ID is 0, use the zone ID instead
			zone = player->GetZoneId();
		FishingZoneEntry *entry = FishingZoneStorage.LookupEntry(zone);
		if (entry == NULL){
			sLog.outError("ERROR: Fishing zone information for zone %d not found!", zone);
			EndFishing(player, true);
			return;
		}
		uint32 maxskill = entry->MaxSkill;
		uint32 minskill = entry->MinSkill;
		if (player->_GetSkillLineCurrent(SKILL_FISHING, false) < maxskill)
			player->_AdvanceSkillLine(SKILL_FISHING, float2int32(1.0f * sWorld.getRate(RATE_SKILLRATE)));
		GameObject *go = NULL;
		Arcemu::GO_FishingHole *school = NULL;
		for (std::set< Object* >::iterator itr = this->m_objectsInRange.begin(); itr != m_objectsInRange.end(); ++itr){
			Object *o = *itr;
			if (!o->IsGameObject())
				continue;
			go = TO_GAMEOBJECT(o);
			if (go->GetType() != GAMEOBJECT_TYPE_FISHINGHOLE)
				continue;
			school = static_cast< Arcemu::GO_FishingHole* >(go);
			if (!isInRange(school, static_cast< float >(school->GetInfo()->fishinghole.radius))){
				school = NULL;
				continue;
			}
			else
				break;
		}
		if (school != NULL){ // open school loot if school exists
			if (school->GetMapMgr() != NULL)
				lootmgr.FillGOLoot(&school->loot, school->GetInfo()->raw.sound1, school->GetMapMgr()->iInstanceMode);
			else
				lootmgr.FillGOLoot(&school->loot, school->GetInfo()->raw.sound1, 0);
			player->SendLoot(school->GetGUID(), LOOT_FISHING, school->GetMapId());
			EndFishing(player, false);
			school->CatchFish();
			if (!school->CanFish())
				sEventMgr.AddEvent(TO_GAMEOBJECT(school), &GameObject::Despawn, (uint32)0, (1800000 + RandomUInt(3600000)), EVENT_GAMEOBJECT_EXPIRE, 10000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); // respawn in 30 - 90 minutes
		}
		else if (Rand(((player->_GetSkillLineCurrent(SKILL_FISHING, true) - minskill) * 100) / maxskill)){ // Open loot on success, otherwise FISH_ESCAPED.
			lootmgr.FillFishingLoot(&loot, zone);
			player->SendLoot(GetGUID(), LOOT_FISHING, GetMapId());
			EndFishing(player, false);
		}
		else{ // Failed
			player->GetSession()->OutPacket(SMSG_FISH_ESCAPED);
			EndFishing(player, true);
		}
	}
	void GO_FishingNode::EndFishing(Player *player, bool abort){
		Spell * spell = player->GetCurrentSpell();
		if (spell != NULL){
			if (abort){
				// abort because of a reason - Roe vs. Wade
				//FIX ME: here 'failed' should appear over progress bar
				spell->SendChannelUpdate(0);
				spell->finish(false);
			}
			else{
				// spell ended
				spell->SendChannelUpdate(0);
				spell->finish();
			}
		}
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
		SetFlags(GetFlags() | 32);
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