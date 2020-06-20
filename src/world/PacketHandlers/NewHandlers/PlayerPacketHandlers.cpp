/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"
#include "PlayerPacketHandlers.h"

DEFINE_PACKET_HANDLER_METHOD( UnlearnSkillPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN
	
	uint32 skill_line;
	uint32 points_remaining = _player->GetPrimaryProfessionPoints();
	recv_data >> skill_line;

	// Cheater detection
	// if(!_player->HasSkillLine(skill_line)) return;

	// Remove any spells within that line that the player has
	_player->RemoveSpellsFromLine(skill_line);

	// Finally, remove the skill line.
	_player->_RemoveSkillLine(skill_line);

	// added by Zack : This is probably wrong or already made elsewhere :
	// restore skill learnability
	if(points_remaining == _player->GetPrimaryProfessionPoints())
	{
		// we unlearned a skill so we enable a new one to be learned
		skilllineentry* sk = dbcSkillLine.LookupEntryForced(skill_line);
		if(!sk)
			return;
		if(sk->type == SKILL_TYPE_PROFESSION && points_remaining < 2)
			_player->SetPrimaryProfessionPoints(points_remaining + 1);
	}
}

DEFINE_PACKET_HANDLER_METHOD( LearnMultipleTalentsPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN
	uint32 talentcount;
	uint32 talentid;
	uint32 rank;

	LOG_DEBUG("Recieved packet CMSG_LEARN_TALENTS_MULTIPLE.");

	recv_data >> talentcount;

	for(uint32 i = 0; i < talentcount; ++i)
	{
		recv_data >> talentid;
		recv_data >> rank;

		_player->LearnTalent(talentid, rank, true);
	}
}

DEFINE_PACKET_HANDLER_METHOD( LearnTalentPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN
		
	uint32 talent_id, requested_rank, unk;
	recv_data >> talent_id >> requested_rank >> unk;
	_player->LearnTalent(talent_id, requested_rank);
}

DEFINE_PACKET_HANDLER_METHOD( UnlearnTalentsPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN
	uint32 price = _player->CalcTalentResetCost(_player->GetTalentResetTimes());
	if(_player->HasGold(price))
		return;

	_player->SetTalentResetTimes(_player->GetTalentResetTimes() + 1);
	_player->ModGold(-(int32) price);
	_player->Reset_Talents();
}

DEFINE_PACKET_HANDLER_METHOD( DissmissCritterPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	uint64 GUID;

	recv_data >> GUID;

	if(_player->GetSummonedCritterGUID() == 0)
	{
		LOG_ERROR
		("Player %u sent dismiss companion packet, but player has no companion",
		 _player->GetLowGUID());
		return;
	}

	if(_player->GetSummonedCritterGUID() != GUID)
	{
		LOG_ERROR
		("Player %u sent dismiss companion packet, but it doesn't match player's companion",
		 _player->GetLowGUID());
		return;
	}

	Unit* companion = _player->GetMapMgr()->GetUnit(GUID);
	if(companion != NULL)
	{
		companion->Delete();
	}

	_player->SetSummonedCritterGUID(0);
}

DEFINE_PACKET_HANDLER_METHOD( QueryQuestPOIPacketHandler )
{
	Player *_player = session.GetPlayer();

	CHECK_INWORLD_RETURN
	LOG_DEBUG("Received CMSG_QUEST_POI_QUERY");

	uint32 count = 0;
	recv_data >> count;

	if(count > MAX_QUEST_LOG_SIZE)
	{
		LOG_DEBUG
		("Client sent Quest POI query for more than MAX_QUEST_LOG_SIZE quests.");

		count = MAX_QUEST_LOG_SIZE;
	}

	WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4 + (4 + 4) * count);

	data << uint32(count);

	for(uint32 i = 0; i < count; i++)
	{
		uint32 questId;

		recv_data >> questId;

		sQuestMgr.BuildQuestPOIResponse(data, questId);
	}

	session.SendPacket(&data);

	LOG_DEBUG("Sent SMSG_QUEST_POI_QUERY_RESPONSE");
}

DEFINE_PACKET_HANDLER_METHOD( MirrorImagePacketHandler )
{
	Player *_player = session.GetPlayer();
	
	CHECK_INWORLD_RETURN

	LOG_DEBUG("Received CMG_GET_MIRRORIMAGE_DATA");

	uint64 GUID;

	recv_data >> GUID;

	Unit* Image = _player->GetMapMgr()->GetUnit(GUID);
	if(Image == NULL)
		return;					// ups no unit found with that GUID on the
	// map. Spoofed packet?

	if(Image->GetCreatedByGUID() == 0)
		return;

	uint64 CasterGUID = Image->GetCreatedByGUID();
	Unit* Caster = _player->GetMapMgr()->GetUnit(CasterGUID);
	if(Caster == NULL)
		return;					// apperantly this mirror image mirrors
	// nothing, poor lonely soul :(
	// Maybe it's the Caster's ghost called Casper

	WorldPacket data(SMSG_MIRRORIMAGE_DATA, 68);

	data << uint64(GUID);
	data << uint32(Caster->GetDisplayId());
	data << uint8(Caster->getRace());

	if(Caster->IsPlayer())
	{
		Player* pcaster = static_cast < Player* >(Caster);

		data << uint8(pcaster->getGender());
		data << uint8(pcaster->getClass());

		// facial features, like big nose, piercings, bonehead, etc
		data << uint8(pcaster->GetByte(PLAYER_BYTES, 0));	// skin color
		data << uint8(pcaster->GetByte(PLAYER_BYTES, 1));	// face
		data << uint8(pcaster->GetByte(PLAYER_BYTES, 2));	// hair style
		data << uint8(pcaster->GetByte(PLAYER_BYTES, 3));	// hair color
		data << uint8(pcaster->GetByte(PLAYER_BYTES_2, 0));	// facial hair

		if(pcaster->IsInGuild())
			data << uint32(pcaster->GetGuildId());
		else
			data << uint32(0);

		static const uint32 imageitemslots[] =
		{
			EQUIPMENT_SLOT_HEAD,
			EQUIPMENT_SLOT_SHOULDERS,
			EQUIPMENT_SLOT_BODY,
			EQUIPMENT_SLOT_CHEST,
			EQUIPMENT_SLOT_WAIST,
			EQUIPMENT_SLOT_LEGS,
			EQUIPMENT_SLOT_FEET,
			EQUIPMENT_SLOT_WRISTS,
			EQUIPMENT_SLOT_HANDS,
			EQUIPMENT_SLOT_BACK,
			EQUIPMENT_SLOT_TABARD
		};

		for(uint32 i = 0; i < 11; ++i)
		{
			Item* item =
			    pcaster->GetItemInterface()->GetInventoryItem(static_cast <
			            int16 >
			            (imageitemslots
			             [i]));

			if(item != NULL)
				data << uint32(item->GetProto()->DisplayInfoID);
			else
				data << uint32(0);
		}
	}

	session.SendPacket(&data);

	LOG_DEBUG("Sent: SMSG_MIRRORIMAGE_DATA");
}

DEFINE_PACKET_HANDLER_METHOD( UseEquipmentSetPacketHandler )
{
	LOG_DEBUG("Received CMSG_EQUIPMENT_SET_USE");

	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	WoWGuid GUID;
	int8 SrcBagID;
	uint8 SrcSlotID;
	uint8 result = 0;

	for(int8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
	{
		uint64 ItemGUID = 0;

		GUID.Clear();

		recv_data >> GUID;
		recv_data >> SrcBagID;
		recv_data >> SrcSlotID;

		ItemGUID = GUID.GetOldGuid();

		// Let's see if we even have this item
		Item* item = _player->GetItemInterface()->GetItemByGUID(ItemGUID);
		if(item == NULL)
		{
			// Nope we don't probably WPE hack :/
			result = 1;
			continue;
		}

		int8 dstslot = i;
		int8 dstbag = static_cast<int8>(INVALID_BACKPACK_SLOT);

		// This is the best case, we already have the item equipped
		if((SrcBagID == dstbag) && (SrcSlotID == dstslot))
			continue;

		// Let's see if we have an item in the destination slot
		Item* dstslotitem =
		    _player->GetItemInterface()->GetInventoryItem(dstslot);

		if(dstslotitem == NULL)
		{
			// we have no item equipped in the slot, so let's equip
			AddItemResult additemresult;
			int8 EquipError =
			    _player->GetItemInterface()->CanEquipItemInSlot(dstbag,
			            dstslot,
			            item->
			            GetProto(),
			            false, false);

			if(EquipError == INV_ERR_OK)
			{
				dstslotitem =
				    _player->GetItemInterface()->
				    SafeRemoveAndRetreiveItemFromSlot(SrcBagID, SrcSlotID,
				                                      false);
				additemresult =
				    _player->GetItemInterface()->SafeAddItem(item, dstbag,
				            dstslot);


				if(additemresult != ADD_ITEM_RESULT_OK)
				{
					// We failed for w/e reason, so let's revert
					_player->GetItemInterface()->SafeAddItem(item, SrcBagID,
					        SrcSlotID);
					result = 1;
				}


			}
			else
			{
				result = 1;
			}

		}
		else
		{
			// There is something equipped so we need to swap
			if(!_player->GetItemInterface()->
			        SwapItems(INVALID_BACKPACK_SLOT, dstslot, SrcBagID, SrcSlotID))
				result = 1;
		}

	}

	_player->SendEquipmentSetUseResult(result);
}

DEFINE_PACKET_HANDLER_METHOD( SaveEquipmentSetPacketHandler )
{
	LOG_DEBUG("Received CMSG_EQUIPMENT_SET_SAVE");

	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	WoWGuid GUID;
	uint32 setGUID;

	recv_data >> GUID;

	setGUID = Arcemu::Util::GUID_LOPART(GUID.GetOldGuid());

	if(setGUID == 0)
		setGUID = objmgr.GenerateEquipmentSetID();

	Arcemu::EquipmentSet* set = new Arcemu::EquipmentSet();

	set->SetGUID = setGUID;

	recv_data >> set->SetID;
	recv_data >> set->SetName;
	recv_data >> set->IconName;

	for(uint32 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
	{
		GUID.Clear();
		recv_data >> GUID;
		set->ItemGUID[i] = Arcemu::Util::GUID_LOPART(GUID.GetOldGuid());
	}


	bool success;
	success =
	    _player->GetItemInterface()->m_EquipmentSets.AddEquipmentSet(set->
	            SetGUID,
	            set);

	if(success)
	{
		LOG_DEBUG("Player %u successfully stored equipment set %u at slot %u ",
		          _player->GetLowGUID(), set->SetGUID, set->SetID);
		_player->SendEquipmentSetSaved(set->SetID, set->SetGUID);
	}
	else
	{
		LOG_DEBUG("Player %u couldn't store equipment set %u at slot %u ",
		          _player->GetLowGUID(), set->SetGUID, set->SetID);
	}
}

DEFINE_PACKET_HANDLER_METHOD( DeleteEquipmentSetPacketHandler )
{
	LOG_DEBUG("Received CMSG_EQUIPMENT_SET_DELETE");

	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	WoWGuid setGUID;
	bool success;

	recv_data >> setGUID;

	uint32 GUID = Arcemu::Util::GUID_LOPART(setGUID.GetOldGuid());

	success = _player->GetItemInterface()->m_EquipmentSets.DeleteEquipmentSet(GUID);

	if(success)
	{
		LOG_DEBUG("Equipmentset with GUID %u was successfully deleted.", GUID);
	}
	else
	{
		LOG_DEBUG("Equipmentset with GUID %u couldn't be deleted.", GUID);
	}
}

