/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
#include "Game/Entities/Update/UpdateBuilder.h"
#include "Game/Network/Messenger.h"

#define ARCEMU_NORMAL_GAMESPEED 0.0166666669777748f

void Player::SendLevelupInfo(uint32 level, uint32 Hp, uint32 Mana, uint32 Stat0, uint32 Stat1, uint32 Stat2, uint32 Stat3, uint32 Stat4)
{
	WorldPacket data(SMSG_LEVELUP_INFO, 14 * 4);

	data << uint32(level);
	data << uint32(Hp);
	data << uint32(Mana);

	for(int i = 0; i < 6; ++i)
		data << uint32(0);

	data << uint32(Stat0);
	data << uint32(Stat1);
	data << uint32(Stat2);
	data << uint32(Stat3);
	data << uint32(Stat4);

	m_session->SendPacket(&data);
}

void Player::SendSpellModifier(uint8 spellgroup, uint8 spelltype, int32 v, bool is_pct)
{

	WorldPacket data((uint16)(is_pct ? SMSG_SET_PCT_SPELL_MODIFIER : SMSG_SET_FLAT_SPELL_MODIFIER), 48);

	data << uint8(spellgroup);
	data << uint8(spelltype);
	data << uint32(v);

	m_session->SendPacket(&data);
}

void Player::SendItemPushResult(bool created, bool recieved, bool sendtoset, bool newitem, uint8 destbagslot, uint32 destslot, uint32 count, uint32 entry, uint32 suffix, uint32 randomprop, uint32 stack)
{

	WorldPacket data(SMSG_ITEM_PUSH_RESULT, 8 + 4 + 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 4);

	data << uint64(GetGUID());

	if(recieved)
		data << uint32(1);
	else
		data << uint32(0);

	if(created)
		data << uint32(1);
	else
		data << uint32(0);

	data << uint32(1);
	data << uint8(destbagslot);

	if(newitem)
		data << uint32(destslot);
	else
		data << uint32(-1);

	data << uint32(entry);
	data << uint32(suffix);
	data << uint32(randomprop);
	data << uint32(count);
	data << uint32(stack);

	if(sendtoset && InGroup())
		GetGroup()->SendPacketToAll(&data);
	else
		m_session->SendPacket(&data);

}

void Player::SendDungeonDifficulty()
{

	WorldPacket data(MSG_SET_DUNGEON_DIFFICULTY, 12);

	data << uint32(iInstanceType);
	data << uint32(1);
	data << uint32(InGroup());

	m_session->SendPacket(&data);
}

void Player::SendRaidDifficulty()
{
	WorldPacket data(MSG_SET_RAID_DIFFICULTY, 12);

	data << uint32(m_RaidDifficulty);
	data << uint32(1);
	data << uint32(InGroup());

	m_session->SendPacket(&data);
}

void Player::SendInstanceDifficulty(uint32 difficulty)
{
	WorldPacket data(SMSG_INSTANCE_DIFFICULTY, 8);

	data << uint32(difficulty);
	data << uint32(0);

	m_session->SendPacket(&data);
}

/*Loot type MUST be
1-corpse, go
2-skinning/herbalism/minning
3-Fishing
*/
void Player::SendLoot(uint64 guid, uint8 loot_type, uint32 mapid)
{
	Group* m_Group = m_playerInfo->m_Group;

	if(!IsInWorld())
		return;

	Loot* pLoot = NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	int8 loot_method;

	if(m_Group != NULL)
		loot_method = m_Group->GetMethod();
	else
		loot_method = PARTY_LOOT_FFA;

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* pCreature = GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!pCreature)return;
		pLoot = &pCreature->loot;
		m_currentLoot = pCreature->GetGUID();

	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(!pGO)return;
		pGO->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		pLoot = &pGO->loot;
		m_currentLoot = pGO->GetGUID();
	}
	else if((guidtype == HIGHGUID_TYPE_PLAYER))
	{
		Player* p = GetMapMgr()->GetPlayer((uint32)guid);
		if(!p)return;
		pLoot = &p->loot;
		m_currentLoot = p->GetGUID();
	}
	else if((guidtype == HIGHGUID_TYPE_CORPSE))
	{
		Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
		if(!pCorpse)return;
		pLoot = &pCorpse->loot;
		m_currentLoot = pCorpse->GetGUID();
	}
	else if((guidtype == HIGHGUID_TYPE_ITEM))
	{
		Item* pItem = GetItemInterface()->GetItemByGUID(guid);
		if(!pItem)
			return;
		pLoot = pItem->loot;
		m_currentLoot = pItem->GetGUID();
	}

	if(!pLoot)
	{
		// something whack happened.. damn cheaters..
		return;
	}

	// add to looter set
	pLoot->looters.insert(GetLowGUID());

	WorldPacket data, data2(32);
	data.SetOpcode(SMSG_LOOT_RESPONSE);


	m_lootGuid = guid;


	data << uint64(guid);
	data << uint8(loot_type);  //loot_type;
	data << uint32(pLoot->gold);
	data << uint8(0);   //loot size reserve


	std::vector<__LootItem>::iterator iter = pLoot->items.begin();
	uint32 count = 0;
	uint8 slottype = 0;

	for(uint32 x = 0; iter != pLoot->items.end(); iter++, x++)
	{
		if(iter->iItemsCount == 0)
			continue;

		LooterSet::iterator itr = iter->has_looted.find(GetLowGUID());
		if(iter->has_looted.end() != itr)
			continue;

		ItemPrototype* itemProto = iter->item.itemproto;
		if(!itemProto)
			continue;

		// check if it's on ML if so only quest items and ffa loot should be shown based on mob
		if(loot_method == PARTY_LOOT_MASTER && m_Group && m_Group->GetLooter() != m_playerInfo)
			// pass on all ffa_loot and the grey / white items
			if(!iter->ffa_loot && !(itemProto->Quality < m_Group->GetThreshold()))
				continue;

		// team check
		if( itemProto->HasFlag2(ITEM_FLAG2_HORDE_ONLY) && IsTeamAlliance() ) 
			continue; 

		if( itemProto->HasFlag2(ITEM_FLAG2_ALLIANCE_ONLY) && IsTeamHorde() ) 
			continue;

		//quest items check. type 4/5
		//quest items that don't start quests.
		if((itemProto->Bonding == ITEM_BIND_QUEST) && !(itemProto->QuestId) && !HasQuestForItem(itemProto->ItemId))
			continue;
		if((itemProto->Bonding == ITEM_BIND_QUEST2) && !(itemProto->QuestId) && !HasQuestForItem(itemProto->ItemId))
			continue;

		//quest items that start quests need special check to avoid drops all the time.
		if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
			continue;
		if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
			continue;

		if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
			continue;
		if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
			continue;

		//check for starting item quests that need questlines.
		if((itemProto->QuestId && itemProto->Bonding != ITEM_BIND_QUEST && itemProto->Bonding != ITEM_BIND_QUEST2))
		{
			Quest* pQuest = QuestStorage.LookupEntry(itemProto->QuestId);
			if(pQuest)
			{
				uint32 finishedCount = 0;

				//check if its a questline.
				for(uint32 i = 0; i < pQuest->count_requiredquests; i++)
				{
					if(pQuest->required_quests[i])
					{
						if(!HasFinishedQuest(pQuest->required_quests[i]) || GetQuestLogForEntry(pQuest->required_quests[i]))
						{

						}
						else
						{
							finishedCount++;
						}
					}
				}
			}
		}

		slottype = 0;
		if(m_Group != NULL && loot_type < 2)
		{
			switch(loot_method)
			{
				case PARTY_LOOT_MASTER:
					slottype = 2;
					break;
				case PARTY_LOOT_GROUP:
				case PARTY_LOOT_RR:
				case PARTY_LOOT_NBG:
					slottype = 1;
					break;
				default:
					slottype = 0;
					break;
			}
			// only quality items are distributed
			if(itemProto->Quality < m_Group->GetThreshold())
			{
				slottype = 0;
			}

			// if all people passed anyone can loot it? :P
			if(iter->passed)
				slottype = 0;					// All players passed on the loot

			//if it is ffa loot and not an masterlooter
			if(iter->ffa_loot)
				slottype = 0;
		}

		data << uint8(x);
		data << uint32(itemProto->ItemId);
		data << uint32(iter->iItemsCount);  //nr of items of this type
		data << uint32(iter->item.displayid);

		if(iter->iRandomSuffix)
		{
			data << uint32(Item::GenerateRandomSuffixFactor(itemProto));
			data << uint32(-int32(iter->iRandomSuffix->id));
		}
		else if(iter->iRandomProperty)
		{
			data << uint32(0);
			data << uint32(iter->iRandomProperty->ID);
		}
		else
		{
			data << uint32(0);
			data << uint32(0);
		}

		data << slottype;   // "still being rolled for" flag

		if(slottype == 1)
		{
			if(iter->roll == NULL && !iter->passed)
			{
				int32 ipid = 0;
				uint32 factor = 0;
				if(iter->iRandomProperty)
					ipid = iter->iRandomProperty->ID;
				else if(iter->iRandomSuffix)
				{
					ipid = -int32(iter->iRandomSuffix->id);
					factor = Item::GenerateRandomSuffixFactor(iter->item.itemproto);
				}

				if(iter->item.itemproto)
				{
					iter->roll = new LootRoll(60000, (m_Group != NULL ? m_Group->MemberCount() : 1),  guid, x, itemProto->ItemId, factor, uint32(ipid), GetMapMgr());

					data2.Initialize(SMSG_LOOT_START_ROLL);
					data2 << guid;
					data2 << uint32(mapid);
					data2 << uint32(x);
					data2 << uint32(itemProto->ItemId);
					data2 << uint32(factor);
					if(iter->iRandomProperty)
						data2 << uint32(iter->iRandomProperty->ID);
					else if(iter->iRandomSuffix)
						data2 << uint32(ipid);
					else
						data2 << uint32(0);

					data2 << uint32(iter->iItemsCount);
					data2 << uint32(60000);	// countdown
					data2 << uint8(7);		// some sort of flags that require research
				}

				Group* pGroup = m_playerInfo->m_Group;
				if(pGroup)
				{
					pGroup->Lock();
					for(uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i)
					{
						for(GroupMembersSet::iterator itr2 = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr2 != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr2)
						{

							PlayerInfo* pinfo = *itr2;

							if(pinfo->m_loggedInPlayer && pinfo->m_loggedInPlayer->GetItemInterface()->CanReceiveItem(itemProto, iter->iItemsCount) == 0)
							{
								if(pinfo->m_loggedInPlayer->m_passOnLoot)
									iter->roll->PlayerRolled(pinfo->m_loggedInPlayer, 3);		// passed
								else
									pinfo->m_loggedInPlayer->SendPacket(&data2);
							}
						}
					}
					pGroup->Unlock();
				}
				else
				{
					m_session->SendPacket(&data2);
				}
			}
		}
		count++;
	}
	data.wpos(13);
	data << uint8(count);

	m_session->SendPacket(&data);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
}

void Player::SendInitialLogonPackets()
{
	// Initial Packets... they seem to be re-sent on port.
	//m_session->OutPacket(SMSG_SET_REST_START_OBSOLETE, 4, &m_timeLogoff); // Seem to be unused by client

	Messenger::SendBindPointUpdate( this, LocationVector( m_bind_pos_x, m_bind_pos_y, m_bind_pos_z ), m_bind_mapid, m_bind_zoneid );

	//Proficiencies
	Messenger::SendSetItemProficiency( this, 4, armor_proficiency);
	Messenger::SendSetItemProficiency( this, 2, weapon_proficiency);

	//Tutorial Flags
	WorldPacket data( 32 );
	data.Initialize(SMSG_TUTORIAL_FLAGS);

	for(int i = 0; i < 8; i++)
		data << uint32(m_Tutorials[i]);

	m_session->SendPacket(&data);

	smsg_TalentsInfo(false);
	smsg_InitialSpells();

	data.Initialize(SMSG_SEND_UNLEARN_SPELLS);
	data << uint32(0); // count, for(count) uint32;
	GetSession()->SendPacket(&data);

	SendInitialActions();
	smsg_InitialFactions();


	Messenger::SendLoginSetTimeSpeed( this, Arcemu::Util::MAKE_GAME_TIME(), ARCEMU_NORMAL_GAMESPEED );

	// cebernic for speedhack bug
	m_lastRunSpeed = 0;
	UpdateSpeed();

	Messenger::SendWorldStateUpdate( this, 0xF3D, sWorld.Arena_Season );
	Messenger::SendWorldStateUpdate( this, 0xC77, sWorld.Arena_Progress );

	LOG_DETAIL("WORLD: Sent initial logon packets for %s.", GetName());
}

void Player::SendLootUpdate(Object* o)
{

	if(!IsVisible(o->GetGUID()))
		return;

	// Build the actual update.
	ByteBuffer buf(500);

	uint32 Flags = o->GetUInt32Value(UNIT_DYNAMIC_FLAGS);

	Flags |= U_DYN_FLAG_LOOTABLE;
	Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

	UpdateBuilder::BuildFieldUpdatePacket(&buf, o, UNIT_DYNAMIC_FLAGS, Flags );

	PushUpdateData(&buf, 1);
}

void Player::SendUpdateDataToSet(ByteBuffer* groupbuf, ByteBuffer* nongroupbuf, bool sendtoself)
{

	/////////////////////////// first case we need to send to both grouped and ungrouped players in the set /////////////////////////////
	if(groupbuf != NULL && nongroupbuf != NULL)
	{

		for(std::set< Object* >::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
		{
			Player* p = TO< Player* >(*itr);

			if(p->GetGroup() != NULL && GetGroup() != NULL && p->GetGroup()->GetID() == GetGroup()->GetID())
				p->PushUpdateData(groupbuf, 1);
			else
				p->PushUpdateData(nongroupbuf, 1);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	else

		////////////////////////////////////////////// second case we send to group only ///////////////////////////////////////////////////
		if(groupbuf != NULL && nongroupbuf == NULL)
		{

			for(std::set< Object* >::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
			{
				Player* p = TO< Player* >(*itr);

				if(p->GetGroup() != NULL && GetGroup() != NULL && p->GetGroup()->GetID() == GetGroup()->GetID())
					p->PushUpdateData(groupbuf, 1);
			}
		}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		else

			////////////////////////////////////////// Last case we send to nongroup only ////////////////////////////////////////////////////
			if(groupbuf == NULL && nongroupbuf != NULL)
			{

				for(std::set< Object* >::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
				{
					Player* p = TO< Player* >(*itr);

					if(p->GetGroup() == NULL || p->GetGroup()->GetID() != GetGroup()->GetID())
						p->PushUpdateData(nongroupbuf, 1);
				}
			}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if(sendtoself)
		PushUpdateData(groupbuf, 1);
}

void Player::TagUnit(Object* o)
{

	// For new players who get a create object
	uint32 Flags = o->GetUInt32Value(UNIT_DYNAMIC_FLAGS);
	Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

	// Update existing players.
	ByteBuffer buf(500);
	ByteBuffer buf1(500);

	UpdateBuilder::BuildFieldUpdatePacket(&buf1, o, UNIT_DYNAMIC_FLAGS, Flags );
	UpdateBuilder::BuildFieldUpdatePacket(&buf, o, UNIT_DYNAMIC_FLAGS, o->GetUInt32Value(UNIT_DYNAMIC_FLAGS) );

	SendUpdateDataToSet(&buf1, &buf, true);
}

void Player::SendEquipmentSetList()
{

	WorldPacket data(SMSG_EQUIPMENT_SET_LIST, 1000);

	m_ItemInterface->m_EquipmentSets.FillEquipmentSetListPacket(data);

	m_session->SendPacket(&data);

	LOG_DEBUG("Sent SMSG_EQUIPMENT_SET_LIST.");
}

void Player::SendEquipmentSetSaved(uint32 setID, uint32 setGUID)
{
	WorldPacket data(SMSG_EQUIPMENT_SET_SAVED, 12);

	data << uint32(setID);
	data << WoWGuid(uint64(setGUID));

	m_session->SendPacket(&data);

	LOG_DEBUG("Sent SMSG_EQUIPMENT_SET_SAVED.");
}

void Player::SendEquipmentSetUseResult(uint8 result)
{
	WorldPacket data(SMSG_EQUIPMENT_SET_USE_RESULT, 1);

	data << uint8(result);

	m_session->SendPacket(&data);

	LOG_DEBUG("SMSG_EQUIPMENT_SET_USE_RESULT sent.");
}

void Player::SendInitialWorldstates(){
	WorldPacket data( SMSG_INIT_WORLD_STATES, 100 );
	m_mapMgr->GetWorldStatesHandler().BuildInitWorldStatesForZone( m_zoneId, m_AreaID, data );
	m_session->SendPacket( &data );
}
