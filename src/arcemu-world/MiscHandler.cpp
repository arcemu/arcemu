/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

void WorldSession::HandleRepopRequestOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Recvd CMSG_REPOP_REQUEST Message");
	if(_player->getDeathState() != JUST_DIED)
		return;
	if(_player->m_CurrentTransporter)
		_player->m_CurrentTransporter->RemovePlayer(_player);

	GetPlayer()->RepopRequestedPlayer();
}

void WorldSession::HandleAutostoreLootItemOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 itemid = 0;
	uint32 amt = 1;
	uint8 lootSlot = 0;
	uint8 error = 0;
	SlotResult slotresult;

	Item* add;
	Loot* pLoot = NULL;

	if(_player->IsCasting())
		_player->InterruptSpell();
	GameObject* pGO = NULL;
	Creature* pCreature = NULL;
	Item *lootSrcItem = NULL;

	uint32 guidtype = GET_TYPE_FROM_GUID(_player->GetLootGUID());
	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(GetPlayer()->GetLootGUID()));
		if(!pCreature)return;
		pLoot = &pCreature->loot;
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		pGO = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(GetPlayer()->GetLootGUID()));
		if(!pGO)return;
		pLoot = &pGO->loot;
	}
	else if(guidtype == HIGHGUID_TYPE_ITEM)
	{
		Item* pItem = _player->GetItemInterface()->GetItemByGUID(_player->GetLootGUID());
		if(!pItem)
			return;
		lootSrcItem = pItem;
		pLoot = pItem->loot;
	}
	else if(guidtype == HIGHGUID_TYPE_PLAYER)
	{
		Player* pl = _player->GetMapMgr()->GetPlayer((uint32)GetPlayer()->GetLootGUID());
		if(!pl) return;
		pLoot = &pl->loot;
	}

	if(!pLoot) return;

	recv_data >> lootSlot;
	if(lootSlot >= pLoot->items.size())
	{
		LOG_DEBUG("Player %s might be using a hack! (slot %d, size %d)",
		          GetPlayer()->GetName(), lootSlot, pLoot->items.size());
		return;
	}

	if( pLoot->items[ lootSlot ].looted ){
		LOG_DEBUG( "Player %s GUID %u tried to loot an already looted item.", _player->GetName(), _player->GetLowGUID() );
		return;
	}

	amt = pLoot->items.at(lootSlot).iItemsCount;
	if(pLoot->items.at(lootSlot).roll != NULL)
		return;

	if(!pLoot->items.at(lootSlot).ffa_loot)
	{
		if(!amt) //Test for party loot
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ALREADY_LOOTED);
			return;
		}
	}
	else
	{
		//make sure this player can still loot it in case of ffa_loot
		LooterSet::iterator itr = pLoot->items.at(lootSlot).has_looted.find(_player->GetLowGUID());

		if(pLoot->items.at(lootSlot).has_looted.end() != itr)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ALREADY_LOOTED);
			return;
		}
	}

	itemid = pLoot->items.at(lootSlot).item.itemproto->ItemId;
	ItemPrototype* it = pLoot->items.at(lootSlot).item.itemproto;

	if((error = _player->GetItemInterface()->CanReceiveItem(it, 1)) != 0)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		return;
	}

	if(pGO)
	{
		CALL_GO_SCRIPT_EVENT(pGO, OnLootTaken)(_player, it);
	}
	else if(pCreature)
		CALL_SCRIPT_EVENT(pCreature, OnLootTaken)(_player, it);

	add = GetPlayer()->GetItemInterface()->FindItemLessMax(itemid, amt, false);
	sHookInterface.OnLoot(_player, pCreature, 0, itemid);
	if(!add)
	{
		slotresult = GetPlayer()->GetItemInterface()->FindFreeInventorySlot(it);
		if(!slotresult.Result)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
			return;
		}

		LOG_DEBUG("AutoLootItem MISC");
		Item* item = objmgr.CreateItem(itemid, GetPlayer());
		if(item == NULL)
			return;

		item->SetStackCount(amt);
		if(pLoot->items.at(lootSlot).iRandomProperty != NULL)
		{
			item->SetItemRandomPropertyId(pLoot->items.at(lootSlot).iRandomProperty->ID);
			item->ApplyRandomProperties(false);
		}
		else if(pLoot->items.at(lootSlot).iRandomSuffix != NULL)
		{
			item->SetRandomSuffix(pLoot->items.at(lootSlot).iRandomSuffix->id);
			item->ApplyRandomProperties(false);
		}

		if(GetPlayer()->GetItemInterface()->SafeAddItem(item, slotresult.ContainerSlot, slotresult.Slot))
		{
			sQuestMgr.OnPlayerItemPickup(GetPlayer(), item);
			_player->SendItemPushResult(false, true, true, true, slotresult.ContainerSlot, slotresult.Slot, 1, item->GetEntry(), item->GetItemRandomSuffixFactor(), item->GetItemRandomPropertyId(), item->GetStackCount());
#ifdef ENABLE_ACHIEVEMENTS
			_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item->GetEntry(), 1, 0);
#endif
		}
		else
			item->DeleteMe();
	}
	else
	{
		add->SetStackCount(add->GetStackCount() + amt);
		add->m_isDirty = true;

		sQuestMgr.OnPlayerItemPickup(GetPlayer(), add);
		_player->SendItemPushResult(false, false, true, false, (uint8)_player->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()), 0xFFFFFFFF, amt , add->GetEntry(), add->GetItemRandomSuffixFactor(), add->GetItemRandomPropertyId(), add->GetStackCount());
#ifdef ENABLE_ACHIEVEMENTS
		_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, add->GetEntry(), 1, 0);
#endif
	}

	//in case of ffa_loot update only the player who receives it.
	if(!pLoot->items.at(lootSlot).ffa_loot)
	{
		pLoot->items.at(lootSlot).iItemsCount = 0;

		// this gets sent to all looters
		WorldPacket data(1);
		data.SetOpcode(SMSG_LOOT_REMOVED);
		data << lootSlot;
		Player* plr;
		for(LooterSet::iterator itr = pLoot->looters.begin(); itr != pLoot->looters.end(); ++itr)
		{
			if((plr = _player->GetMapMgr()->GetPlayer(*itr)) != 0)
				plr->GetSession()->SendPacket(&data);
		}
	}
	else
	{
		pLoot->items.at(lootSlot).has_looted.insert(_player->GetLowGUID());
		WorldPacket data(1);
		data.SetOpcode(SMSG_LOOT_REMOVED);
		data << lootSlot;
		_player->GetSession()->SendPacket(&data);
	}

	if( lootSrcItem != NULL ){
		pLoot->items[ lootSlot ].looted = true;
	}

	/* any left yet? (for fishing bobbers) */
	if(pGO && pGO->GetEntry() == GO_FISHING_BOBBER)
	{
		int count = 0;
		for(vector<__LootItem>::iterator itr = pLoot->items.begin(); itr != pLoot->items.end(); ++itr)
			count += (*itr).iItemsCount;
		if(!count)
			pGO->ExpireAndDelete();
	}
}

void WorldSession::HandleLootMoneyOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	Loot* pLoot = NULL;
	uint64 lootguid = GetPlayer()->GetLootGUID();
	if(!lootguid)
		return;   // dunno why this happens

	if(_player->IsCasting())
		_player->InterruptSpell();

	WorldPacket pkt;
	Unit* pt = 0;
	uint32 guidtype = GET_TYPE_FROM_GUID(lootguid);

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(lootguid));
		if(!pCreature)return;
		pLoot = &pCreature->loot;
		pt = pCreature;
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(lootguid));
		if(!pGO)return;
		pLoot = &pGO->loot;
	}
	else if(guidtype == HIGHGUID_TYPE_CORPSE)
	{
		Corpse* pCorpse = objmgr.GetCorpse((uint32)lootguid);
		if(!pCorpse)return;
		pLoot = &pCorpse->loot;
	}
	else if(guidtype == HIGHGUID_TYPE_PLAYER)
	{
		Player* pPlayer = _player->GetMapMgr()->GetPlayer((uint32)lootguid);
		if(!pPlayer) return;
		pLoot = &pPlayer->loot;
		pPlayer->bShouldHaveLootableOnCorpse = false;
		pt = pPlayer;
	}
	else if(guidtype == HIGHGUID_TYPE_ITEM)
	{
		Item* pItem = _player->GetItemInterface()->GetItemByGUID(lootguid);
		if(!pItem)
			return;
		pLoot = pItem->loot;
	}

	if(!pLoot)
	{
		//bitch about cheating maybe?
		return;
	}

	uint32 money = pLoot->gold;

	pLoot->gold = 0;
	WorldPacket data(1);
	data.SetOpcode(SMSG_LOOT_CLEAR_MONEY);
	// send to all looters
	Player* plr;
	for(LooterSet::iterator itr = pLoot->looters.begin(); itr != pLoot->looters.end(); ++itr)
	{
		if((plr = _player->GetMapMgr()->GetPlayer(*itr)) != 0)
			plr->GetSession()->SendPacket(&data);
	}

	if(!_player->InGroup())
	{
		if(money)
		{
			// Check they don't have more than the max gold
			if(sWorld.GoldCapEnabled && (GetPlayer()->GetGold() + money) > sWorld.GoldLimit)
			{
				GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
			}
			else
			{
				GetPlayer()->ModGold(money);
#ifdef ENABLE_ACHIEVEMENTS
				GetPlayer()->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, money, 0, 0);
#endif
			}
			sHookInterface.OnLoot(_player, pt, money, 0);
		}
	}
	else
	{
		//this code is wrong must be party not raid!
		Group* party = _player->GetGroup();
		if(party)
		{
			/*uint32 share = money/party->MemberCount();*/
			vector<Player*> targets;
			targets.reserve(party->MemberCount());

			GroupMembersSet::iterator itr;
			SubGroup* sgrp;
			party->getLock().Acquire();
			for(uint32 i = 0; i < party->GetSubGroupCount(); i++)
			{
				sgrp = party->GetSubGroup(i);
				for(itr = sgrp->GetGroupMembersBegin(); itr != sgrp->GetGroupMembersEnd(); ++itr)
				{
					if((*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetZoneId() == _player->GetZoneId() && _player->GetInstanceID() == (*itr)->m_loggedInPlayer->GetInstanceID())
						targets.push_back((*itr)->m_loggedInPlayer);
				}
			}
			party->getLock().Release();

			if(!targets.size())
				return;

			uint32 share = money / uint32(targets.size());

			pkt.SetOpcode(SMSG_LOOT_MONEY_NOTIFY);
			pkt << share;

			for(vector<Player*>::iterator itr2 = targets.begin(); itr2 != targets.end(); ++itr2)
			{
				// Check they don't have more than the max gold
				if(sWorld.GoldCapEnabled && ((*itr2)->GetGold() + share) > sWorld.GoldLimit)
				{
					(*itr2)->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
				}
				else
				{
					(*itr2)->ModGold(share);
					(*itr2)->GetSession()->SendPacket(&pkt);
#ifdef ENABLE_ACHIEVEMENTS
					(*itr2)->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, share, 0, 0);
#endif
				}
			}
		}
	}
}

void WorldSession::HandleLootOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	if(guid == 0)
		return;

	if(_player->IsDead())    // If the player is dead they can't loot!
		return;

	if(_player->IsStealth())    // Check if the player is stealthed
		_player->RemoveStealth(); // cebernic:RemoveStealth on looting. Blizzlike

	if(_player->IsCasting())    // Check if the player is casting
		_player->InterruptSpell(); // Cancel spell casting

	if(_player->IsInvisible())    // Check if the player is invisible for what ever reason
		_player->RemoveInvisibility(); // Remove all invisibility


	if(_player->InGroup() && !_player->m_bg)
	{
		Group* party = _player->GetGroup();
		if(party)
		{
			if(party->GetMethod() == PARTY_LOOT_MASTER)
			{
				WorldPacket data(SMSG_LOOT_MASTER_LIST, 330);  // wont be any larger
				data << (uint8)party->MemberCount();
				uint32 real_count = 0;
				SubGroup* s;
				GroupMembersSet::iterator itr;
				party->Lock();
				for(uint32 i = 0; i < party->GetSubGroupCount(); ++i)
				{
					s = party->GetSubGroup(i);
					for(itr = s->GetGroupMembersBegin(); itr != s->GetGroupMembersEnd(); ++itr)
					{
						if((*itr)->m_loggedInPlayer && _player->GetZoneId() == (*itr)->m_loggedInPlayer->GetZoneId())
						{
							data << (*itr)->m_loggedInPlayer->GetGUID();
							++real_count;
						}
					}
				}
				party->Unlock();
				*(uint8*)&data.contents()[0] = static_cast<uint8>(real_count);

				party->SendPacketToAll(&data);
			}
		}
	}
	_player->SendLoot(guid, LOOT_CORPSE, _player->GetMapId());
}


void WorldSession::HandleLootReleaseOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;
	WorldPacket data(SMSG_LOOT_RELEASE_RESPONSE, 9);
	data << guid << uint8(1);
	SendPacket(&data);

	_player->SetLootGUID(0);
	_player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
	_player->m_currentLoot = 0;

	if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_UNIT)
	{
		Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(pCreature == NULL)
			return;
		// remove from looter set
		pCreature->loot.looters.erase(_player->GetLowGUID());
		if(pCreature->loot.gold <= 0)
		{
			for(std::vector<__LootItem>::iterator i = pCreature->loot.items.begin(); i != pCreature->loot.items.end(); ++i)
				if(i->iItemsCount > 0)
				{
					ItemPrototype* proto = i->item.itemproto;
					if(proto->Class != 12)
						return;
					if(_player->HasQuestForItem(i->item.itemproto->ItemId))
						return;
				}
			pCreature->BuildFieldUpdatePacket(_player, UNIT_DYNAMIC_FLAGS, 0);

			if(!pCreature->Skinned)
			{
				if(lootmgr.IsSkinnable(pCreature->GetEntry()))
				{
					pCreature->BuildFieldUpdatePacket(_player, UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
				}
			}
		}
	}
	else if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = _player->GetMapMgr()->GetGameObject((uint32)guid);
		if(pGO == NULL)
			return;

		switch(pGO->GetType())
		{
			case GAMEOBJECT_TYPE_FISHINGNODE:
				{
					pGO->loot.looters.erase(_player->GetLowGUID());
					if(pGO->IsInWorld())
					{
						pGO->RemoveFromWorld(true);
					}
					delete pGO;
				}
				break;
			case GAMEOBJECT_TYPE_CHEST:
				{
					pGO->loot.looters.erase(_player->GetLowGUID());
					//check for locktypes

					bool despawn = false;
					if( pGO->GetInfo()->sound3 == 1 )
						despawn = true;

					Lock* pLock = dbcLock.LookupEntryForced(pGO->GetInfo()->SpellFocus);
					if(pLock)
					{
						for(uint32 i = 0; i < LOCK_NUM_CASES; i++)
						{
							if(pLock->locktype[i])
							{
								if(pLock->locktype[i] == 1)   //Item or Quest Required;
								{
									if( despawn )
										pGO->Despawn(0, (sQuestMgr.GetGameObjectLootQuest(pGO->GetEntry()) ? 180000 + (RandomUInt(180000)) : 900000 + (RandomUInt(600000))));
									else
										pGO->SetByte( GAMEOBJECT_BYTES_1, 0, 1 );

									return;
								}
								else if(pLock->locktype[i] == 2)   //locktype;
								{
									//herbalism and mining;
									if(pLock->lockmisc[i] == LOCKTYPE_MINING || pLock->lockmisc[i] == LOCKTYPE_HERBALISM)
									{
										//we still have loot inside.
										if(pGO->HasLoot()  || !despawn )
										{
											pGO->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
											// TODO : redo this temporary fix, because for some reason hasloot is true even when we loot everything
											// my guess is we need to set up some even that rechecks the GO in 10 seconds or something
											//pGO->Despawn( 600000 + ( RandomUInt( 300000 ) ) );
											return;
										}

										if(pGO->CanMine())
										{
											pGO->loot.items.clear();
											pGO->UseMine();
											return;
										}
										else
										{
											pGO->CalcMineRemaining(true);
											pGO->Despawn(0, 900000 + (RandomUInt(600000)));
											return;
										}
									}
									else
									{
										if(pGO->HasLoot()  || !despawn )
										{
											pGO->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
											return;
										}
										pGO->Despawn(0, sQuestMgr.GetGameObjectLootQuest(pGO->GetEntry()) ? 180000 + (RandomUInt(180000)) : (IS_INSTANCE(pGO->GetMapId()) ? 0 : 900000 + (RandomUInt(600000))));
										return;
									}
								}
								else //other type of locks that i don't bother to split atm ;P
								{
									if(pGO->HasLoot()  || !despawn )
									{
										pGO->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
										return;
									}
									pGO->Despawn(0, sQuestMgr.GetGameObjectLootQuest(pGO->GetEntry()) ? 180000 + (RandomUInt(180000)) : (IS_INSTANCE(pGO->GetMapId()) ? 0 : 900000 + (RandomUInt(600000))));
									return;
								}
							}
						}
					}
					else
					{
						if(pGO->HasLoot()  || !despawn )
						{
							pGO->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
							return;
						}
						pGO->Despawn(0, sQuestMgr.GetGameObjectLootQuest(pGO->GetEntry()) ? 180000 + (RandomUInt(180000)) : (IS_INSTANCE(pGO->GetMapId()) ? 0 : 900000 + (RandomUInt(600000))));

						return;

					}
				}
			default:
				break;
		}
	}
	else if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_CORPSE)
	{
		Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
		if(pCorpse)
			pCorpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 0);
	}
	else if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_PLAYER)
	{
		Player* plr = objmgr.GetPlayer((uint32)guid);
		if(plr)
		{
			plr->bShouldHaveLootableOnCorpse = false;
			plr->loot.items.clear();
			plr->RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
		}
	}
	else if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_ITEM)     // Loot from items, eg. sacks, milling, prospecting...
	{
		Item* item = _player->GetItemInterface()->GetItemByGUID(guid);
		if(item == NULL)
			return;

		// delete current loot, so the next one can be filled
		if(item->loot != NULL)
		{
			uint32 itemsNotLooted = 
				std::count_if( item->loot->items.begin(), item->loot->items.end(), ItemIsNotLooted() );

			if( ( itemsNotLooted == 0 ) && ( item->loot->gold == 0 ) ){
				delete item->loot;
				item->loot = NULL;
			}
		}

		// remove loot source items
		if( item->loot == NULL )
			_player->GetItemInterface()->RemoveItemAmtByGuid( guid, 1 );
	}
	else
		LOG_DEBUG("Unhandled loot source object type in HandleLootReleaseOpcode");
}

void WorldSession::HandleWhoOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 min_level;
	uint32 max_level;
	uint32 class_mask;
	uint32 race_mask;
	uint32 zone_count;
	uint32* zones = 0;
	uint32 name_count;
	string* names = 0;
	string chatname;
	string guildname;
	bool cname = false;
	bool gname = false;
	uint32 i;

	recv_data >> min_level >> max_level;
	recv_data >> chatname >> guildname >> race_mask >> class_mask;
	recv_data >> zone_count;

	if(zone_count > 0 && zone_count < 10)
	{
		zones = new uint32[zone_count];

		for(i = 0; i < zone_count; ++i)
			recv_data >> zones[i];
	}
	else
	{
		zone_count = 0;
	}

	recv_data >> name_count;
	if(name_count > 0 && name_count < 10)
	{
		names = new string[name_count];

		for(i = 0; i < name_count; ++i)
			recv_data >> names[i];
	}
	else
	{
		name_count = 0;
	}

	if(chatname.length() > 0)
		cname = true;

	if(guildname.length() > 0)
		gname = true;

	LOG_DEBUG("WORLD: Recvd CMSG_WHO Message with %u zones and %u names", zone_count, name_count);

	bool gm = false;
	uint32 team = _player->GetTeam();
	if(HasGMPermissions())
		gm = true;

	uint32 sent_count = 0;
	uint32 total_count = 0;

	PlayerStorageMap::const_iterator itr, iend;
	Player* plr;
	uint32 lvl;
	bool add;
	WorldPacket data;
	data.SetOpcode(SMSG_WHO);
	data << uint64(0);

	objmgr._playerslock.AcquireReadLock();
	iend = objmgr._players.end();
	itr = objmgr._players.begin();
	while(itr != iend && sent_count < 49)   // WhoList should display 49 names not including your own
	{
		plr = itr->second;
		++itr;

		if(!plr->GetSession() || !plr->IsInWorld())
			continue;

		if(!sWorld.show_gm_in_who_list && !HasGMPermissions())
		{
			if(plr->GetSession()->HasGMPermissions())
				continue;
		}

		// Team check
		if(!gm && plr->GetTeam() != team && !plr->GetSession()->HasGMPermissions() && !sWorld.interfaction_misc)
			continue;

		++total_count;

		// Add by default, if we don't have any checks
		add = true;

		// Chat name
		if(cname && chatname != *plr->GetNameString())
			continue;

		// Guild name
		if(gname)
		{
			if(!plr->GetGuild() || strcmp(plr->GetGuild()->GetGuildName(), guildname.c_str()) != 0)
				continue;
		}

		// Level check
		lvl = plr->getLevel();

		if(min_level && max_level)
		{
			// skip players outside of level range
			if(lvl < min_level || lvl > max_level)
				continue;
		}

		// Zone id compare
		if(zone_count)
		{
			// people that fail the zone check don't get added
			add = false;
			for(i = 0; i < zone_count; ++i)
			{
				if(zones[i] == plr->GetZoneId())
				{
					add = true;
					break;
				}
			}
		}

		if(!((class_mask >> 1) & plr->getClassMask()) || !((race_mask >> 1) & plr->getRaceMask()))
			add = false;

		// skip players that fail zone check
		if(!add)
			continue;

		// name check
		if(name_count)
		{
			// people that fail name check don't get added
			add = false;
			for(i = 0; i < name_count; ++i)
			{
				if(!strnicmp(names[i].c_str(), plr->GetName(), names[i].length()))
				{
					add = true;
					break;
				}
			}
		}

		if(!add)
			continue;

		// if we're here, it means we've passed all testing
		// so add the names :)
		data << plr->GetName();
		if(plr->m_playerInfo->guild)
			data << plr->m_playerInfo->guild->GetGuildName();
		else
			data << uint8(0);	   // Guild name

		data << plr->getLevel();
		data << uint32(plr->getClass());
		data << uint32(plr->getRace());
		data << plr->getGender();
		data << uint32(plr->GetZoneId());
		++sent_count;
	}
	objmgr._playerslock.ReleaseReadLock();
	data.wpos(0);
	data << sent_count;
	data << sent_count;

	SendPacket(&data);

	// free up used memory
	if(zones)
		delete [] zones;
	if(names)
		delete [] names;
}

void WorldSession::HandleLogoutRequestOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	Player* pPlayer = GetPlayer();
	WorldPacket data(SMSG_LOGOUT_RESPONSE, 5);

	LOG_DEBUG("WORLD: Recvd CMSG_LOGOUT_REQUEST Message");

	if(pPlayer)
	{
		if(!sHookInterface.OnLogoutRequest(pPlayer))
		{
			// Declined Logout Request
			data << uint32(1) << uint8(0);
			SendPacket(&data);
			return;
		}

		if(GetPermissionCount() > 0)
		{
			//Logout on NEXT sessionupdate to preserve processing of dead packets (all pending ones should be processed)
			SetLogoutTimer(1);
			return;
		}

		if(pPlayer->CombatStatus.IsInCombat() ||	//can't quit still in combat
		        pPlayer->DuelingWith != NULL)			//can't quit still dueling or attacking
		{
			data << uint32(1) << uint8(0);
			SendPacket(&data);
			return;
		}

		if(pPlayer->m_isResting ||	  // We are resting so log out instantly
		        pPlayer->GetTaxiState())  // or we are on a taxi
		{
			//Logout on NEXT sessionupdate to preserve processing of dead packets (all pending ones should be processed)
			SetLogoutTimer(1);
			return;
		}

		data << uint32(0); //Filler
		data << uint8(0); //Logout accepted
		SendPacket(&data);

		//stop player from moving
		pPlayer->SetMovement(MOVE_ROOT, 1);
		LoggingOut = true;
		// Set the "player locked" flag, to prevent movement
		pPlayer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

		//make player sit
		pPlayer->SetStandState(STANDSTATE_SIT);
		SetLogoutTimer(20000);
	}
	/*
	> 0 = You can't Logout Now
	*/
}

void WorldSession::HandlePlayerLogoutOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Recvd CMSG_PLAYER_LOGOUT Message");
	if(!HasGMPermissions())
	{
		// send "You do not have permission to use this"
		SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
	}
	else
	{
		LogoutPlayer(true);
	}
}

void WorldSession::HandleLogoutCancelOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	LOG_DEBUG("WORLD: Recvd CMSG_LOGOUT_CANCEL Message");

	Player* pPlayer = GetPlayer();
	if(!pPlayer)
		return;
	if(!LoggingOut)
		return;
	LoggingOut = false;

	//Cancel logout Timer
	SetLogoutTimer(0);

	//tell client about cancel
	OutPacket(SMSG_LOGOUT_CANCEL_ACK);

	//unroot player
	pPlayer->SetMovement(MOVE_UNROOT, 5);

	// Remove the "player locked" flag, to allow movement
	pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	//make player stand
	pPlayer->SetStandState(STANDSTATE_STAND);

	LOG_DEBUG("WORLD: sent SMSG_LOGOUT_CANCEL_ACK Message");
}

void WorldSession::HandleZoneUpdateOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	uint32 newZone;

	recv_data >> newZone;

	if(GetPlayer()->GetZoneId() == newZone)
		return;

	sWeatherMgr.SendWeather(GetPlayer());
	_player->ZoneUpdate(newZone);

	//clear buyback
	_player->GetItemInterface()->EmptyBuyBack();
}

void WorldSession::HandleSetSelectionOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;
	_player->SetSelection(guid);

	if(_player->m_comboPoints)
		_player->UpdateComboPoints();

	_player->SetTargetGUID(guid);
	if(guid == 0) // deselected target
	{

		if(_player->IsInWorld())
			_player->CombatStatusHandler_ResetPvPTimeout();
	}
}

void WorldSession::HandleStandStateChangeOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint8 animstate;
	recv_data >> animstate;

	_player->SetStandState(animstate);
}

void WorldSession::HandleBugOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 suggestion, contentlen;
	std::string content;
	uint32 typelen;
	std::string type;

	recv_data >> suggestion >> contentlen >> content >> typelen >> type;

	if(suggestion == 0)
		LOG_DEBUG("WORLD: Received CMSG_BUG [Bug Report]");
	else
		LOG_DEBUG("WORLD: Received CMSG_BUG [Suggestion]");

	uint64 AccountId = GetAccountId();
	uint32 TimeStamp = uint32(UNIXTIME);
	uint32 ReportID = objmgr.GenerateReportID();

	std::stringstream ss;

	ss << "INSERT INTO playerbugreports VALUES('";
	ss << ReportID << "','";
	ss << AccountId << "','";
	ss << TimeStamp << "','";
	ss << suggestion << "','";
	ss << CharacterDatabase.EscapeString(type) << "','";
	ss << CharacterDatabase.EscapeString(content) << "')";

	CharacterDatabase.ExecuteNA(ss.str().c_str());
}

void WorldSession::HandleCorpseReclaimOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	LOG_DETAIL("WORLD: Received CMSG_RECLAIM_CORPSE");

	uint64 guid;
	recv_data >> guid;

	if(guid == 0)
		return;

	Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
	if(pCorpse == NULL)	return;

	// Check that we're reviving from a corpse, and that corpse is associated with us.
	if(GET_LOWGUID_PART(pCorpse->GetOwner()) != _player->GetLowGUID() && pCorpse->GetUInt32Value(CORPSE_FIELD_FLAGS) == 5)
	{
		WorldPacket data(SMSG_RESURRECT_FAILED, 4);
		data << uint32(1); // this is a real guess!
		SendPacket(&data);
		return;
	}

	// Check we are actually in range of our corpse
	if(pCorpse->GetDistance2dSq(_player) > CORPSE_MINIMUM_RECLAIM_RADIUS_SQ)
	{
		WorldPacket data(SMSG_RESURRECT_FAILED, 4);
		data << uint32(1);
		SendPacket(&data);
		return;
	}

	// Check death clock before resurrect they must wait for release to complete
	// cebernic: changes for better logic
	if(time(NULL) < pCorpse->GetDeathClock() + CORPSE_RECLAIM_TIME)
	{
		WorldPacket data(SMSG_RESURRECT_FAILED, 4);
		data << uint32(1);
		SendPacket(&data);
		return;
	}

	GetPlayer()->ResurrectPlayer();
	GetPlayer()->SetHealth(GetPlayer()->GetMaxHealth() / 2);
}

void WorldSession::HandleResurrectResponseOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DETAIL("WORLD: Received CMSG_RESURRECT_RESPONSE");

	if(_player->isAlive())
		return;

	uint64 guid;
	uint8 status;
	recv_data >> guid;
	recv_data >> status;

	// need to check guid
	Player* pl = _player->GetMapMgr()->GetPlayer((uint32)guid);
	if(pl == NULL)
		pl = objmgr.GetPlayer((uint32)guid);

	// checking valid resurrecter fixes exploits
	if(pl == NULL || status != 1 || !_player->m_resurrecter || _player->m_resurrecter != guid)
	{
		_player->m_resurrectHealth = 0;
		_player->m_resurrectMana = 0;
		_player->m_resurrecter = 0;
		return;
	}

	_player->ResurrectPlayer();
	_player->SetMovement(MOVE_UNROOT, 1);
}

void WorldSession::HandleUpdateAccountData(WorldPacket & recv_data)
{
	LOG_DETAIL("WORLD: Received CMSG_UPDATE_ACCOUNT_DATA");

	uint32 uiID;
	if(!sWorld.m_useAccountData)
		return;

	recv_data >> uiID;

	if(uiID > 8)
	{
		// Shit..
		LOG_ERROR("WARNING: Accountdata > 8 (%d) was requested to be updated by %s of account %d!", uiID, GetPlayer()->GetName(), this->GetAccountId());
		return;
	}

	uint32 uiDecompressedSize;
	recv_data >> uiDecompressedSize;
	uLongf uid = uiDecompressedSize;

	// client wants to 'erase' current entries
	if(uiDecompressedSize == 0)
	{
		SetAccountData(uiID, NULL, false, 0);
		WorldPacket rdata(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE, 4 + 4); //VLack: seen this in a 3.1.1 packet dump as response
		rdata << uint32(uiID);
		rdata << uint32(0);
		SendPacket(&rdata);
		return;
	}

	/*
	if(uiDecompressedSize>100000)
	{
		Disconnect();
		return;
	}
	*/

	if(uiDecompressedSize >= 65534)
	{
		// BLOB fields can't handle any more than this.
		return;
	}

	size_t ReceivedPackedSize = recv_data.size() - 8;
	char* data = new char[uiDecompressedSize + 1];
	memset(data, 0, uiDecompressedSize + 1);	/* fix umr here */

	if(uiDecompressedSize > ReceivedPackedSize) // if packed is compressed
	{
		int32 ZlibResult;

		ZlibResult = uncompress((uint8*)data, &uid, recv_data.contents() + 8, (uLong)ReceivedPackedSize);

		switch(ZlibResult)
		{
			case Z_OK:				  //0 no error decompression is OK
				SetAccountData(uiID, data, false, uiDecompressedSize);
				LOG_DETAIL("WORLD: Successfully decompressed account data %d for %s, and updated storage array.", uiID, GetPlayer()->GetName());
				break;

			case Z_ERRNO:			   //-1
			case Z_STREAM_ERROR:		//-2
			case Z_DATA_ERROR:		  //-3
			case Z_MEM_ERROR:		   //-4
			case Z_BUF_ERROR:		   //-5
			case Z_VERSION_ERROR:	   //-6
				{
					delete [] data;
					LOG_ERROR("WORLD WARNING: Decompression of account data %d for %s FAILED.", uiID, GetPlayer()->GetName());
					break;
				}

			default:
				delete [] data;
				LOG_ERROR("WORLD WARNING: Decompression gave a unknown error: %x, of account data %d for %s FAILED.", ZlibResult, uiID, GetPlayer()->GetName());
				break;
		}
	}
	else
	{
		memcpy(data, recv_data.contents() + 8, uiDecompressedSize);
		SetAccountData(uiID, data, false, uiDecompressedSize);
	}
	WorldPacket rdata(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE, 4 + 4); //VLack: seen this in a 3.1.1 packet dump as response
	rdata << uint32(uiID);
	rdata << uint32(0);
	SendPacket(&rdata);
}

void WorldSession::HandleRequestAccountData(WorldPacket & recv_data)
{
	LOG_DETAIL("WORLD: Received CMSG_REQUEST_ACCOUNT_DATA");

	uint32 id;
	if(!sWorld.m_useAccountData)
		return;
	recv_data >> id;

	if(id > 8)
	{
		// Shit..
		LOG_ERROR("WARNING: Accountdata > 8 (%d) was requested by %s of account %d!", id, GetPlayer()->GetName(), this->GetAccountId());
		return;
	}

	AccountDataEntry* res = GetAccountData(id);
	WorldPacket data ;
	data.SetOpcode(SMSG_UPDATE_ACCOUNT_DATA);
	data << id;
	// if red does not exists if ID == 7 and if there is no data send 0
	if(!res || !res->data)  // if error, send a NOTHING packet
	{
		data << (uint32)0;
	}
	else
	{
		data << res->sz;
		uLongf destsize;
		if(res->sz > 200)
		{
			data.resize(res->sz + 800);  // give us plenty of room to work with..

			if((compress(const_cast<uint8*>(data.contents()) + (sizeof(uint32) * 2), &destsize, (const uint8*)res->data, res->sz)) != Z_OK)
			{
				LOG_ERROR("Error while compressing ACCOUNT_DATA");
				return;
			}

			data.resize(destsize + 8);
		}
		else
			data.append(res->data, res->sz);
	}

	SendPacket(&data);
}

void WorldSession::HandleSetActionButtonOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_SET_ACTION_BUTTON");
	uint8 button, misc, type;
	uint16 action;
	recv_data >> button >> action >> misc >> type;
	LOG_DEBUG("BUTTON: %u ACTION: %u TYPE: %u MISC: %u", button, action, type, misc);
	if(action == 0)
	{
		LOG_DEBUG("MISC: Remove action from button %u", button);
		//remove the action button from the db
		GetPlayer()->setAction(button, 0, 0, 0);
	}
	else
	{
		if(button >= PLAYER_ACTION_BUTTON_COUNT)
			return;

		if(type == 64 || type == 65)
		{
			LOG_DEBUG("MISC: Added Macro %u into button %u", action, button);
			GetPlayer()->setAction(button, action, misc, type);
		}
		else if(type == 128)
		{
			LOG_DEBUG("MISC: Added Item %u into button %u", action, button);
			GetPlayer()->setAction(button, action, misc, type);
		}
		else if(type == 0)
		{
			LOG_DEBUG("MISC: Added Spell %u into button %u", action, button);
			GetPlayer()->setAction(button, action, type, misc);
		}
	}

}

void WorldSession::HandleSetWatchedFactionIndexOpcode(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	uint32 factionid;
	recvPacket >> factionid;
	GetPlayer()->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, factionid);
}

void WorldSession::HandleTogglePVPOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	_player->PvPToggle();
}

void WorldSession::HandleAmmoSetOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 ammoId;
	recv_data >> ammoId;

	if(!ammoId)
		return;

	ItemPrototype* xproto = ItemPrototypeStorage.LookupEntry(ammoId);
	if(!xproto)
		return;

	if(xproto->Class != ITEM_CLASS_PROJECTILE || GetPlayer()->GetItemInterface()->GetItemCount(ammoId) == 0)
	{
		sCheatLog.writefromsession(GetPlayer()->GetSession(), "Definitely cheating. tried to add %u as ammo.", ammoId);
		GetPlayer()->GetSession()->Disconnect();
		return;
	}

	if(xproto->RequiredLevel)
	{
		if(GetPlayer()->getLevel() < xproto->RequiredLevel)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ITEM_RANK_NOT_ENOUGH);
			_player->SetAmmoId(0);
			_player->CalcDamage();
			return;
		}
	}
	if(xproto->RequiredSkill)
	{
		if(!GetPlayer()->_HasSkillLine(xproto->RequiredSkill))
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ITEM_RANK_NOT_ENOUGH);
			_player->SetAmmoId(0);
			_player->CalcDamage();
			return;
		}

		if(xproto->RequiredSkillRank)
		{
			if(_player->_GetSkillLineCurrent(xproto->RequiredSkill, false) < xproto->RequiredSkillRank)
			{
				GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ITEM_RANK_NOT_ENOUGH);
				_player->SetAmmoId(0);
				_player->CalcDamage();
				return;
			}
		}
	}
	switch(_player->getClass())
	{
		case PRIEST:  // allowing priest, warlock, mage to equip ammo will mess up wand shoot. stop it.
		case WARLOCK:
		case MAGE:
		case SHAMAN: // these don't get messed up since they don't use wands, but they don't get to use bows/guns/crossbows anyways
		case DRUID:  // we wouldn't want them cheating extra stats from ammo, would we?
		case PALADIN:
		case DEATHKNIGHT:
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM); // good error message?
			_player->SetAmmoId(0);
			_player->CalcDamage();
			return;
		default:
			_player->SetAmmoId(ammoId);
			_player->CalcDamage();
			break;
	}
}

#define OPEN_CHEST 11437

void WorldSession::HandleBarberShopResult(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: CMSG_ALTER_APPEARANCE ");

	uint32 hair, haircolor, facialhairorpiercing;
	recv_data >> hair >> haircolor >> facialhairorpiercing;
	uint32 oldhair = _player->GetByte(PLAYER_BYTES, 2);
	uint32 oldhaircolor = _player->GetByte(PLAYER_BYTES, 3);
	uint32 oldfacial = _player->GetByte(PLAYER_BYTES_2, 0);
	uint32 newhair, newhaircolor, newfacial;
	uint32 cost = 0;
	BarberShopStyleEntry* bbse;

	bbse = dbcBarberShopStyleStore.LookupEntryForced(hair);
	if(!bbse)		return;
	newhair = bbse->type;

	newhaircolor = haircolor;

	bbse = dbcBarberShopStyleStore.LookupEntryForced(facialhairorpiercing);
	if(!bbse)		return;
	newfacial = bbse->type;

	uint32 level = _player->getLevel();
	if(level >= 100)
		level = 100;
	gtFloat* cutcosts = dbcBarberShopPrices.LookupEntryForced(level - 1);
	if(!cutcosts)
		return;

	// hair style cost = cutcosts
	// hair color cost = cutcosts * 0.5 or free if hair style changed
	// facial hair cost = cutcosts * 0.75
	if(newhair != oldhair)
	{
		cost += (uint32)cutcosts->val;
	}
	else if(newhaircolor != oldhaircolor)
	{
		cost += (uint32)(cutcosts->val) >> 1;
	}
	if(newfacial != oldfacial)
	{
		cost += (uint32)(cutcosts->val * 0.75f);
	}

	if(!_player->HasGold(cost))
	{
		WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
		data << uint32(1);                                  // no money
		SendPacket(&data);
		return;
	}
	WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
	data << uint32(0);                                  // ok
	SendPacket(&data);

	_player->SetByte(PLAYER_BYTES, 2, static_cast<uint8>(newhair));
	_player->SetByte(PLAYER_BYTES, 3, static_cast<uint8>(newhaircolor));
	_player->SetByte(PLAYER_BYTES_2, 0, static_cast<uint8>(newfacial));
	_player->ModGold(-(int32)cost);

	_player->SetStandState(0);                              // stand up
#ifdef ENABLE_ACHIEVEMENTS
	_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1, 0, 0);
	_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER, cost, 0, 0);
#endif
}

void WorldSession::HandleGameObjectUse(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;
	SpellCastTargets targets;
	Spell* spell = NULL;
	SpellEntry* spellInfo = NULL;
	LOG_DEBUG("WORLD: CMSG_GAMEOBJ_USE: [GUID %d]", guid);

	GameObject* obj = _player->GetMapMgr()->GetGameObject((uint32)guid);
	if(!obj) return;
	GameObjectInfo* goinfo = obj->GetInfo();
	if(!goinfo) return;

	Player* plyr = GetPlayer();

	CALL_GO_SCRIPT_EVENT(obj, OnActivate)(_player);
	CALL_INSTANCE_SCRIPT_EVENT(_player->GetMapMgr(), OnGameObjectActivate)(obj, _player);

	_player->RemoveStealth(); // cebernic:RemoveStealth due to GO was using. Blizzlike


	uint32 type = obj->GetType();
	switch(type)
	{
		case GAMEOBJECT_TYPE_CHAIR:
			{

				/*WorldPacket data(MSG_MOVE_HEARTBEAT, 66);
				data << plyr->GetNewGUID();
				data << uint8(0);
				data << uint64(0);
				data << obj->GetPositionX() << obj->GetPositionY() << obj->GetPositionZ() << obj->GetOrientation();
				plyr->SendMessageToSet(&data, true);*/
				plyr->SafeTeleport(plyr->GetMapId(), plyr->GetInstanceID(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation());
				plyr->SetStandState(STANDSTATE_SIT_MEDIUM_CHAIR);
				plyr->m_lastRunSpeed = 0; //counteract mount-bug; reset speed to zero to force update SetPlayerSpeed in next line.
				//plyr->SetSpeeds(RUN,plyr->m_base_runSpeed); <--cebernic : Oh No,this could be wrong. If I have some mods existed,this just on baserunspeed as a fixed value?
				plyr->UpdateSpeed();
			}
			break;
		case GAMEOBJECT_TYPE_BARBER_CHAIR:
			{
				plyr->SafeTeleport(plyr->GetMapId(), plyr->GetInstanceID(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation());
				plyr->m_lastRunSpeed = 0; //counteract mount-bug; reset speed to zero to force update SetPlayerSpeed in next line.
				plyr->UpdateSpeed();
				//send barber shop menu to player
				WorldPacket data(SMSG_ENABLE_BARBER_SHOP, 0);
				SendPacket(&data);
				plyr->SetStandState(STANDSTATE_SIT_HIGH_CHAIR);
				//Zack : no idea if this phaseshift is even required
//			WorldPacket data2(SMSG_SET_PHASE_SHIFT, 4);
//			data2 << uint32(0x00000200);
//			plyr->SendMessageToSet(&data2, true);
			}
			break;
		case GAMEOBJECT_TYPE_CHEST://cast da spell
			{
				spellInfo = dbcSpell.LookupEntry(OPEN_CHEST);
				spell = sSpellFactoryMgr.NewSpell(plyr, spellInfo, true, NULL);
				_player->m_currentSpell = spell;
				targets.m_unitTarget = obj->GetGUID();
				spell->prepare(&targets);
			}
			break;
		case GAMEOBJECT_TYPE_FISHINGNODE:
			{
				obj->UseFishingNode(plyr);
			}
			break;
		case GAMEOBJECT_TYPE_DOOR:
			{
				// cebernic modified this state = 0 org =1
				if((obj->GetByte(GAMEOBJECT_BYTES_1, 0) == 0))  //&& (obj->GetUInt32Value(GAMEOBJECT_FLAGS) == 33) )
					obj->EventCloseDoor();
				else
				{
					obj->SetFlag(GAMEOBJECT_FLAGS, 1);   // lock door
					obj->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
					sEventMgr.AddEvent(obj, &GameObject::EventCloseDoor, EVENT_GAMEOBJECT_DOOR_CLOSE, 20000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
			break;
		case GAMEOBJECT_TYPE_FLAGSTAND:
			{
				// battleground/warsong gulch flag
				if(plyr->m_bg)
					plyr->m_bg->HookFlagStand(plyr, obj);

			}
			break;
		case GAMEOBJECT_TYPE_FLAGDROP:
			{
				// Dropped flag
				if(plyr->m_bg)
					plyr->m_bg->HookFlagDrop(plyr, obj);

			}
			break;
		case GAMEOBJECT_TYPE_QUESTGIVER:
			{
				// Questgiver
				if(obj->HasQuests())
				{
					sQuestMgr.OnActivateQuestGiver(obj, plyr);
				}
			}
			break;
		case GAMEOBJECT_TYPE_SPELLCASTER:
			{
				if(obj->m_summoner != NULL && obj->m_summoner->IsPlayer() && plyr != TO< Player* >(obj->m_summoner))
				{
					if(TO< Player* >(obj->m_summoner)->GetGroup() == NULL)
						break;
					else if(TO< Player* >(obj->m_summoner)->GetGroup() != plyr->GetGroup())
						break;
				}

				SpellEntry* info = dbcSpell.LookupEntryForced(goinfo->SpellFocus);
				if(!info)
					break;
				spell = sSpellFactoryMgr.NewSpell(plyr, info, false, NULL);
				//spell->SpellByOther = true;
				targets.m_targetMask |= TARGET_FLAG_UNIT;
				targets.m_unitTarget = plyr->GetGUID();
				spell->prepare(&targets);
				if(obj->charges > 0 && !--obj->charges)
					obj->ExpireAndDelete();
			}
			break;
		case GAMEOBJECT_TYPE_RITUAL:
			{
				// store the members in the ritual, cast sacrifice spell, and summon.
				uint32 i = 0;
				if(!obj->m_ritualmembers || !obj->m_ritualspell || !obj->m_ritualcaster /*|| !obj->m_ritualtarget*/)
					return;

				for(i = 0; i < goinfo->SpellFocus; i++)
				{
					if(!obj->m_ritualmembers[i])
					{
						obj->m_ritualmembers[i] = plyr->GetLowGUID();
						plyr->SetChannelSpellTargetGUID(obj->GetGUID());
						plyr->SetChannelSpellId(obj->m_ritualspell);
						break;
					}
					else if(obj->m_ritualmembers[i] == plyr->GetLowGUID())
					{
						// we're deselecting :(
						obj->m_ritualmembers[i] = 0;
						plyr->SetChannelSpellId(0);
						plyr->SetChannelSpellTargetGUID(0);
						return;
					}
				}

				if(i == goinfo->SpellFocus - 1)
				{
					obj->m_ritualspell = 0;
					Player* plr;
					for(i = 0; i < goinfo->SpellFocus; i++)
					{
						plr = _player->GetMapMgr()->GetPlayer(obj->m_ritualmembers[i]);
						if(plr)
						{
							plr->SetChannelSpellTargetGUID(0);
							plr->SetChannelSpellId(0);
						}
					}

					SpellEntry* info = NULL;
					if(goinfo->ID == 36727 || goinfo->ID == 194108)   // summon portal
					{
						if(!obj->m_ritualtarget)
							return;
						info = dbcSpell.LookupEntryForced(goinfo->sound1);
						if(!info)
							break;
						Player* target = objmgr.GetPlayer(obj->m_ritualtarget);
						if(target == NULL || !target->IsInWorld())
							return;
						spell = sSpellFactoryMgr.NewSpell(_player->GetMapMgr()->GetPlayer(obj->m_ritualcaster), info, true, NULL);
						targets.m_unitTarget = target->GetGUID();
						spell->prepare(&targets);
					}
					else if(goinfo->ID == 177193)    // doom portal
					{
						Player* psacrifice = NULL;

						// kill the sacrifice player
						psacrifice = _player->GetMapMgr()->GetPlayer(obj->m_ritualmembers[(int)(rand() % (goinfo->SpellFocus - 1))]);
						Player* pCaster = obj->GetMapMgr()->GetPlayer(obj->m_ritualcaster);
						if(!psacrifice || !pCaster)
							return;

						info = dbcSpell.LookupEntryForced(goinfo->sound4);
						if(!info)
							break;
						spell = sSpellFactoryMgr.NewSpell(psacrifice, info, true, NULL);
						targets.m_unitTarget = psacrifice->GetGUID();
						spell->prepare(&targets);

						// summons demon
						info = dbcSpell.LookupEntry(goinfo->sound1);
						spell = sSpellFactoryMgr.NewSpell(pCaster, info, true, NULL);
						SpellCastTargets targets2;
						targets2.m_unitTarget = pCaster->GetGUID();
						spell->prepare(&targets2);
					}
					else if(goinfo->ID == 179944)    // Summoning portal for meeting stones
					{
						plr = _player->GetMapMgr()->GetPlayer(obj->m_ritualtarget);
						if(!plr)
							return;

						Player* pleader = _player->GetMapMgr()->GetPlayer(obj->m_ritualcaster);
						if(!pleader)
							return;

						info = dbcSpell.LookupEntry(goinfo->sound1);
						spell = sSpellFactoryMgr.NewSpell(pleader, info, true, NULL);
						SpellCastTargets targets2(plr->GetGUID());
						spell->prepare(&targets2);

						/* expire the gameobject */
						obj->ExpireAndDelete();
					}
					else if(goinfo->ID == 186811 || goinfo->ID == 181622)
					{
						info = dbcSpell.LookupEntryForced(goinfo->sound1);
						if(info == NULL)
							return;
						spell = sSpellFactoryMgr.NewSpell(_player->GetMapMgr()->GetPlayer(obj->m_ritualcaster), info, true, NULL);
						SpellCastTargets targets2(obj->m_ritualcaster);
						spell->prepare(&targets2);
						obj->ExpireAndDelete();
					}
				}
			}
			break;
		case GAMEOBJECT_TYPE_GOOBER:
			{
				plyr->CastSpell(guid, goinfo->Unknown1, false);

				// show page
				if(goinfo->sound7)
				{
					WorldPacket data(SMSG_GAMEOBJECT_PAGETEXT, 8);
					data << obj->GetGUID();
					plyr->GetSession()->SendPacket(&data);
				}
			}
			break;
		case GAMEOBJECT_TYPE_CAMERA://eye of azora
			{
				/*WorldPacket pkt(SMSG_TRIGGER_CINEMATIC,4);
				pkt << (uint32)1;//i ve found only on such item,id =1
				SendPacket(&pkt);*/

				if(goinfo->Unknown1)
				{
					uint32 cinematicid = goinfo->sound1;
					plyr->GetSession()->OutPacket(SMSG_TRIGGER_CINEMATIC, 4, &cinematicid);
				}
			}
			break;
		case GAMEOBJECT_TYPE_MEETINGSTONE:	// Meeting Stone
			{
				/* Use selection */
				Player* pPlayer = objmgr.GetPlayer((uint32)_player->GetSelection());
				if(!pPlayer || _player->GetGroup() != pPlayer->GetGroup() || !_player->GetGroup())
					return;

				GameObjectInfo* info = GameObjectNameStorage.LookupEntry(179944);
				if(!info)
					return;

				/* Create the summoning portal */
				GameObject* pGo = _player->GetMapMgr()->CreateGameObject(179944);
				pGo->CreateFromProto(179944, _player->GetMapId(), _player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), 0);
				pGo->m_ritualcaster = _player->GetLowGUID();
				pGo->m_ritualtarget = pPlayer->GetLowGUID();
				pGo->m_ritualspell = 18540;	// meh
				pGo->PushToWorld(_player->GetMapMgr());

				/* member one: the (w00t) caster */
				pGo->m_ritualmembers[0] = _player->GetLowGUID();
				_player->SetChannelSpellTargetGUID(pGo->GetGUID());
				_player->SetChannelSpellId(pGo->m_ritualspell);

				/* expire after 2mins*/
				sEventMgr.AddEvent(pGo, &GameObject::_Expire, EVENT_GAMEOBJECT_EXPIRE, 120000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
			break;
	}
}

void WorldSession::HandleTutorialFlag(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 iFlag;
	recv_data >> iFlag;

	uint32 wInt = (iFlag / 32);
	uint32 rInt = (iFlag % 32);

	if(wInt >= 7)
	{
		Disconnect();
		return;
	}

	uint32 tutflag = GetPlayer()->GetTutorialInt(wInt);
	tutflag |= (1 << rInt);
	GetPlayer()->SetTutorialInt(wInt, tutflag);

	LOG_DEBUG("Received Tutorial Flag Set {%u}.", iFlag);
}

void WorldSession::HandleTutorialClear(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	for(uint32 iI = 0; iI < 8; iI++)
		GetPlayer()->SetTutorialInt(iI, 0xFFFFFFFF);
}

void WorldSession::HandleTutorialReset(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	for(uint32 iI = 0; iI < 8; iI++)
		GetPlayer()->SetTutorialInt(iI, 0x00000000);
}

void WorldSession::HandleSetSheathedOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 active;
	recv_data >> active;
	_player->SetByte(UNIT_FIELD_BYTES_2, 0, (uint8)active);
}

void WorldSession::HandlePlayedTimeOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 playedt = (uint32)UNIXTIME - _player->m_playedtime[2];
	uint8 displayinui = 0;

	///////////////////////////////////////////////////////////////////////////////////////////
	// As of 3.2.0a this is what the client sends to poll the /played time
	//
	// {CLIENT} Packet: (0x01CC) CMSG_PLAYED_TIME PacketSize = 1 TimeStamp = 691943484
	// 01
	//
	// Structure:
	// uint8 displayonui   -  1 when it should be printed on the screen, 0 when it shouldn't
	//
	//////////////////////////////////////////////////////////////////////////////////////////

	recv_data >> displayinui;

	LOG_DEBUG("Recieved CMSG_PLAYED_TIME.");
	LOG_DEBUG("displayinui: %lu", displayinui);

	if(playedt)
	{
		_player->m_playedtime[0] += playedt;
		_player->m_playedtime[1] += playedt;
		_player->m_playedtime[2] += playedt;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// As of 3.2.0a the server sends this as a response to the client /played time packet
	//
	//  {SERVER} Packet: (0x01CD) SMSG_PLAYED_TIME PacketSize = 9 TimeStamp = 691944000
	//  FE 0C 00 00 FE 0C 00 00 01
	//
	//
	// Structure:
	//
	// uint32 playedtotal      -   total time played in seconds
	// uint32 playedlevel      -   time played on this level in seconds
	// uint32 displayinui      -   1 when it should be printed on the screen, 0 when it shouldn't
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	WorldPacket data(SMSG_PLAYED_TIME, 9); //VLack: again, an Aspire trick, with an uint8(0) -- I hate packet structure changes...
	data << (uint32)_player->m_playedtime[1];
	data << (uint32)_player->m_playedtime[0];
	data << uint8(displayinui);
	SendPacket(&data);

	LOG_DEBUG("Sent SMSG_PLAYED_TIME.");
	LOG_DEBUG(" total: %lu level: %lu", _player->m_playedtime[1], _player->m_playedtime[0]);
}

void WorldSession::HandleInspectOpcode(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 8);
	CHECK_INWORLD_RETURN;

	uint64 guid;
	ByteBuffer m_Packed_GUID;
	recv_data >> guid;
	Player* player = _player->GetMapMgr()->GetPlayer((uint32)guid);
	if(player == NULL)
	{
		LOG_ERROR("HandleInspectOpcode: guid was null");
		return;
	}

	_player->SetTargetGUID(guid);
	_player->SetSelection(guid);
	if(_player->m_comboPoints)
		_player->UpdateComboPoints();

	WorldPacket data(SMSG_INSPECT_TALENT, 1000);
	m_Packed_GUID.appendPackGUID(player->GetGUID());
	uint32 guid_size = (uint32)m_Packed_GUID.size();
	data.append(m_Packed_GUID);

	//data.appendPackGUID(guid);
	//data.appendPackGUID(player->GetGUID());
	//data << player->GetNewGUID();
#ifdef SAVE_BANDWIDTH
	PlayerSpec *currSpec = &player->m_specs[player->m_talentActiveSpec];
	data << uint32(currSpec->GetTP());
	data << uint8(1) << uint8(0);
	data << uint8(currSpec->talents.size()); //fake value, will be overwritten at the end
	for(std::map<uint32, uint8>::iterator itr = currSpec->talents.begin(); itr != currSpec->talents.end(); itr++)
		data << itr->first << itr->second;
	data << uint8(0); // Send Glyph info
#else
	data << uint32(player->m_specs[player->m_talentActiveSpec].GetTP());
	data << uint8(player->m_talentSpecsCount);
	data << uint8(player->m_talentActiveSpec);
	for(uint8 s = 0; s < player->m_talentSpecsCount; s++)
	{
		PlayerSpec spec = player->m_specs[s];

		int32 talent_max_rank;
		uint32 talent_tab_id;

		uint8 talent_count = 0;
		size_t pos = data.wpos();
		data << uint8(talent_count); //fake value, will be overwritten at the end

		for(uint32 i = 0; i < 3; ++i)
		{
			talent_tab_id = sWorld.InspectTalentTabPages[player->getClass()][i];

			for(uint32 j = 0; j < dbcTalent.GetNumRows(); ++j)
			{
				TalentEntry const* talent_info = dbcTalent.LookupRowForced(j);

				//LOG_DEBUG( "HandleInspectOpcode: i(%i) j(%i)", i, j );

				if(talent_info == NULL)
					continue;

				//LOG_DEBUG( "HandleInspectOpcode: talent_info->TalentTree(%i) talent_tab_id(%i)", talent_info->TalentTree, talent_tab_id );

				if(talent_info->TalentTree != talent_tab_id)
					continue;

				talent_max_rank = -1;
				for(int32 k = 4; k > -1; --k)
				{
					//LOG_DEBUG( "HandleInspectOpcode: k(%i) RankID(%i) HasSpell(%i) TalentTree(%i) Tab(%i)", k, talent_info->RankID[k - 1], player->HasSpell( talent_info->RankID[k - 1] ), talent_info->TalentTree, talent_tab_id );
					if(talent_info->RankID[k] != 0 && player->HasSpell(talent_info->RankID[k]))
					{
						talent_max_rank = k;
						break;
					}
				}

				//LOG_DEBUG( "HandleInspectOpcode: RankID(%i) talent_max_rank(%i)", talent_info->RankID[talent_max_rank-1], talent_max_rank );

				if(talent_max_rank < 0)
					continue;

				data << uint32(talent_info->TalentID);
				data << uint8(talent_max_rank);

				++talent_count;

				//LOG_DEBUG( "HandleInspectOpcode: talent(%i) talent_max_rank(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i) mask(%i)", talent_info->TalentID, talent_max_rank, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset , mask);
			}
		}

		data.put<uint8>(pos, talent_count);

		// Send Glyph info
		data << uint8(GLYPHS_COUNT);
		for(uint8 i = 0; i < GLYPHS_COUNT; i++)
			data << uint16(spec.glyphs[i]);

	}
#endif

	// ----[ Build the item list with their enchantments ]----
	uint32 slot_mask = 0;
	size_t slot_mask_pos = data.wpos();
	data << uint32(slot_mask);   // VLack: 3.1, this is a mask field, if we send 0 we can skip implementing this for now; here should come the player's enchantments from its items (the ones you would see on the character sheet).

	ItemInterface* iif = player->GetItemInterface();

	for(uint32 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)   // Ideally this goes from 0 to 18 (EQUIPMENT_SLOT_END is 19 at the moment)
	{
		Item* item = iif->GetInventoryItem(static_cast<uint16>(i));

		if(!item)
			continue;

		slot_mask |= (1 << i);

		data << uint32(item->GetEntry());

		uint16 enchant_mask = 0;
		size_t enchant_mask_pos = data.wpos();

		data << uint16(enchant_mask);

		for(uint32 Slot = 0; Slot < MAX_ENCHANTMENT_SLOT; ++Slot) // In UpdateFields.h we have ITEM_FIELD_ENCHANTMENT_1_1 to ITEM_FIELD_ENCHANTMENT_12_1, iterate on them...
		{
			uint32 enchantId = item->GetEnchantmentId(Slot);   // This calculation has to be in sync with Item.cpp line ~614, at the moment it is:    uint32 EnchantBase = Slot * 3 + ITEM_FIELD_ENCHANTMENT_1_1;

			if(!enchantId)
				continue;

			enchant_mask |= (1 << Slot);
			data << uint16(enchantId);
		}

		data.put<uint16>(enchant_mask_pos, enchant_mask);

		data << uint16(0);   // UNKNOWN
		FastGUIDPack(data, item->GetCreatorGUID());  // Usually 0 will do, but if your friend created that item for you, then it is nice to display it when you get inspected.
		data << uint32(0);   // UNKNOWN
	}
	data.put<uint32>(slot_mask_pos, slot_mask);

	SendPacket(&data);
}

void WorldSession::HandleSetActionBarTogglesOpcode(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	uint8 cActionBarId;
	recvPacket >> cActionBarId;
	LOG_DEBUG("Received CMSG_SET_ACTIONBAR_TOGGLES for actionbar id %d.", cActionBarId);

	GetPlayer()->SetByte(PLAYER_FIELD_BYTES, 2, cActionBarId);
}

// Handlers for acknowledgement opcodes (removes some 'unknown opcode' flood from the logs)
void WorldSession::HandleAcknowledgementOpcodes(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	switch(recv_data.GetOpcode())
	{
		case CMSG_MOVE_WATER_WALK_ACK:
			_player->m_waterwalk = _player->m_setwaterwalk;
			break;

		case CMSG_MOVE_SET_CAN_FLY_ACK:
			_player->FlyCheat = _player->m_setflycheat;
			break;
	}
}

void WorldSession::HandleSelfResurrectOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 self_res_spell = _player->GetUInt32Value(PLAYER_SELF_RES_SPELL);
	if(self_res_spell)
	{
		SpellEntry* sp = dbcSpell.LookupEntry(self_res_spell);
		Spell* s = sSpellFactoryMgr.NewSpell(_player, sp, true, NULL);
		SpellCastTargets tgt;
		tgt.m_unitTarget = _player->GetGUID();
		s->prepare(&tgt);
	}
}

void WorldSession::HandleRandomRollOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 min, max;
	recv_data >> min >> max;

	LOG_DETAIL("WORLD: Received MSG_RANDOM_ROLL: %u-%u", min, max);

	WorldPacket data(20);
	data.SetOpcode(MSG_RANDOM_ROLL);
	data << min << max;

	uint32 roll;

	if(max > RAND_MAX)
		max = RAND_MAX;

	if(min > max)
		min = max;


	// generate number
	roll = RandomUInt(max - min) + min;

	// append to packet, and guid
	data << roll << _player->GetGUID();

	// send to set
	if(_player->InGroup())
		_player->GetGroup()->SendPacketToAll(&data);
	else
		SendPacket(&data);
}

void WorldSession::HandleLootMasterGiveOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

//	uint8 slot = 0;
	uint32 itemid = 0;
	uint32 amt = 1;
	uint8 error = 0;
	SlotResult slotresult;

	Creature* pCreature = NULL;
	GameObject* pGameObject = NULL; //cebernic added it
	Object* pObj = NULL;
	Loot* pLoot = NULL;
	/* struct:
	{CLIENT} Packet: (0x02A3) CMSG_LOOT_MASTER_GIVE PacketSize = 17
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|39 23 05 00 81 02 27 F0 01 7B FC 02 00 00 00 00 |9#....'..{......|
	|00											  |.			   |
	-------------------------------------------------------------------

		uint64 creatureguid
		uint8  slotid
		uint64 target_playerguid

	*/
	uint64 creatureguid, target_playerguid;
	uint8 slotid;
	recv_data >> creatureguid >> slotid >> target_playerguid;

	if(_player->GetGroup() == NULL || _player->GetGroup()->GetLooter() != _player->m_playerInfo)
		return;

	Player* player = _player->GetMapMgr()->GetPlayer((uint32)target_playerguid);
	if(!player)
		return;

	// cheaterz!
	if(_player->GetLootGUID() != creatureguid)
		return;

	//now its time to give the loot to the target player
	if(GET_TYPE_FROM_GUID(GetPlayer()->GetLootGUID()) == HIGHGUID_TYPE_UNIT)
	{
		pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(creatureguid));
		if(!pCreature)return;
		pLoot = &pCreature->loot;
	}
	else if(GET_TYPE_FROM_GUID(GetPlayer()->GetLootGUID()) == HIGHGUID_TYPE_GAMEOBJECT) // cebernic added it support gomastergive
	{
		pGameObject = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(creatureguid));
		if(!pGameObject)return;
		pGameObject->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		pLoot = &pGameObject->loot;
	}


	if(!pLoot)
		return;
	if(pCreature)
		pObj = pCreature;
	else
		pObj = pGameObject;

	if(!pObj)
		return;

	if(slotid >= pLoot->items.size())
	{
		LOG_DEBUG("AutoLootItem: Player %s might be using a hack! (slot %d, size %d)",
		          GetPlayer()->GetName(), slotid, pLoot->items.size());
		return;
	}

	amt = pLoot->items.at(slotid).iItemsCount;

	if(!pLoot->items.at(slotid).ffa_loot)
	{
		if(!amt) //Test for party loot
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ALREADY_LOOTED);
			return;
		}
	}
	else
	{
		//make sure this player can still loot it in case of ffa_loot
		LooterSet::iterator itr = pLoot->items.at(slotid).has_looted.find(player->GetLowGUID());

		if(pLoot->items.at(slotid).has_looted.end() != itr)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ALREADY_LOOTED);
			return;
		}
	}

	itemid = pLoot->items.at(slotid).item.itemproto->ItemId;
	ItemPrototype* it = pLoot->items.at(slotid).item.itemproto;

	if((error = player->GetItemInterface()->CanReceiveItem(it, 1)) != 0)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		return;
	}

	if(pCreature)
		CALL_SCRIPT_EVENT(pCreature, OnLootTaken)(player, it);


	slotresult = player->GetItemInterface()->FindFreeInventorySlot(it);
	if(!slotresult.Result)
	{
		GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
		return;
	}

	Item* item = objmgr.CreateItem(itemid, player);
	if(item == NULL)
		return;

	item->SetStackCount(amt);
	if(pLoot->items.at(slotid).iRandomProperty != NULL)
	{
		item->SetItemRandomPropertyId(pLoot->items.at(slotid).iRandomProperty->ID);
		item->ApplyRandomProperties(false);
	}
	else if(pLoot->items.at(slotid).iRandomSuffix != NULL)
	{
		item->SetRandomSuffix(pLoot->items.at(slotid).iRandomSuffix->id);
		item->ApplyRandomProperties(false);
	}

	if(player->GetItemInterface()->SafeAddItem(item, slotresult.ContainerSlot, slotresult.Slot))
	{
		player->SendItemPushResult(false, true, true, true, slotresult.ContainerSlot, slotresult.Slot, 1 , item->GetEntry(), item->GetItemRandomSuffixFactor(), item->GetItemRandomPropertyId(), item->GetStackCount());
		sQuestMgr.OnPlayerItemPickup(player, item);
#ifdef ENABLE_ACHIEVEMENTS
		_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item->GetEntry(), 1, 0);
#endif
	}
	else
		item->DeleteMe();

	pLoot->items.at(slotid).iItemsCount = 0;

	// this gets sent to all looters
	if(!pLoot->items.at(slotid).ffa_loot)
	{
		pLoot->items.at(slotid).iItemsCount = 0;

		// this gets sent to all looters
		WorldPacket data(1);
		data.SetOpcode(SMSG_LOOT_REMOVED);
		data << slotid;
		Player* plr;
		for(LooterSet::iterator itr = pLoot->looters.begin(); itr != pLoot->looters.end(); ++itr)
		{
			if((plr = _player->GetMapMgr()->GetPlayer(*itr)) != 0)
				plr->GetSession()->SendPacket(&data);
		}
	}
	else
	{
		pLoot->items.at(slotid).has_looted.insert(player->GetLowGUID());
	}
}

void WorldSession::HandleLootRollOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	/* struct:

	{CLIENT} Packet: (0x02A0) CMSG_LOOT_ROLL PacketSize = 13
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|11 4D 0B 00 BD 06 01 F0 00 00 00 00 02		  |.M...........   |
	-------------------------------------------------------------------

	uint64 creatureguid
	uint21 slotid
	uint8  choice

	*/
	uint64 creatureguid;
	uint32 slotid;
	uint8 choice;
	recv_data >> creatureguid >> slotid >> choice;

	LootRoll* li = NULL;

	uint32 guidtype = GET_TYPE_FROM_GUID(creatureguid);
	if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = _player->GetMapMgr()->GetGameObject((uint32)creatureguid);
		if(!pGO)
			return;
		if(slotid >= pGO->loot.items.size() || pGO->loot.items.size() == 0)
			return;
		if(pGO->GetInfo() && pGO->GetInfo()->Type == GAMEOBJECT_TYPE_CHEST)
			li = pGO->loot.items[slotid].roll;
	}
	else if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		// Creatures
		Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(creatureguid));
		if(!pCreature)
			return;

		if(slotid >= pCreature->loot.items.size() || pCreature->loot.items.size() == 0)
			return;

		li = pCreature->loot.items[slotid].roll;
	}
	else
		return;

	if(!li)
		return;

	li->PlayerRolled(_player, choice);
}

void WorldSession::HandleOpenItemOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 2);
	int8 slot, containerslot;
	recv_data >> containerslot >> slot;

	Item* pItem = _player->GetItemInterface()->GetInventoryItem(containerslot, slot);
	if(!pItem)
		return;

	// gift wrapping handler
	if(pItem->GetGiftCreatorGUID() && pItem->wrapped_item_id)
	{
		ItemPrototype* it = ItemPrototypeStorage.LookupEntry(pItem->wrapped_item_id);
		if(it == NULL)
			return;

		pItem->SetGiftCreatorGUID(0);
		pItem->SetEntry(pItem->wrapped_item_id);
		pItem->wrapped_item_id = 0;
		pItem->SetProto(it);

		if(it->Bonding == ITEM_BIND_ON_PICKUP)
			pItem->SoulBind();
		else
			pItem->ClearFlags();

		if(it->MaxDurability)
		{
			pItem->SetDurability(it->MaxDurability);
			pItem->SetDurabilityMax(it->MaxDurability);
		}

		pItem->m_isDirty = true;
		pItem->SaveToDB(containerslot, slot, false, NULL);
		return;
	}

	Lock* lock = dbcLock.LookupEntryForced(pItem->GetProto()->LockId);

	uint32 removeLockItems[LOCK_NUM_CASES] = {0, 0, 0, 0, 0, 0, 0, 0};

	if(lock) // locked item
	{
		for(int i = 0; i < LOCK_NUM_CASES; i++)
		{
			if(lock->locktype[i] == 1 && lock->lockmisc[i] > 0)
			{
				int16 slot2 = _player->GetItemInterface()->GetInventorySlotById(lock->lockmisc[i]);
				if(slot2 != ITEM_NO_SLOT_AVAILABLE && slot2 >= INVENTORY_SLOT_ITEM_START && slot2 < INVENTORY_SLOT_ITEM_END)
				{
					removeLockItems[i] = lock->lockmisc[i];
				}
				else
				{
					_player->GetItemInterface()->BuildInventoryChangeError(pItem, NULL, INV_ERR_ITEM_LOCKED);
					return;
				}
			}
			else if(lock->locktype[i] == 2 && pItem->locked)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(pItem, NULL, INV_ERR_ITEM_LOCKED);
				return;
			}
		}
		for(int i = 0; i < LOCK_NUM_CASES; i++)
			if(removeLockItems[i])
				_player->GetItemInterface()->RemoveItemAmt(removeLockItems[i], 1);
	}

	// fill loot
	_player->SetLootGUID(pItem->GetGUID());
	if(!pItem->loot)
	{
		pItem->loot = new Loot;
		lootmgr.FillItemLoot(pItem->loot, pItem->GetEntry());
	}
	_player->SendLoot(pItem->GetGUID(), LOOT_DISENCHANTING, _player->GetMapId());
}

void WorldSession::HandleCompleteCinematic(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	// when a Cinematic is started the player is going to sit down, when its finished its standing up.
	_player->SetStandState(STANDSTATE_STAND);
};

void WorldSession::HandleResetInstanceOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	sInstanceMgr.ResetSavedInstances(_player);
}

void WorldSession::HandleToggleCloakOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	//////////////////////////
	//	PLAYER_FLAGS									   = 3104 / 0x00C20 / 0000000000000000000110000100000
	//																							 ^
	// This bit, on = toggled OFF, off = toggled ON.. :S

	//uint32 SetBit = 0 | (1 << 11);

	if(_player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_NOCLOAK))
		_player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_NOCLOAK);
	else
		_player->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_NOCLOAK);
}

void WorldSession::HandleToggleHelmOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	//////////////////////////
	//	PLAYER_FLAGS									   = 3104 / 0x00C20 / 0000000000000000000110000100000
	//																							  ^
	// This bit, on = toggled OFF, off = toggled ON.. :S

	//uint32 SetBit = 0 | (1 << 10);

	if(_player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_NOHELM))
		_player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_NOHELM);
	else
		_player->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_NOHELM);
}

void WorldSession::HandleDungeonDifficultyOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 data;
	recv_data >> data;

	// Set dungeon difficulty for us
	_player->iInstanceType = data;
	sInstanceMgr.ResetSavedInstances(_player);

	Group* m_Group = _player->GetGroup();

	// If we have a group and we are the leader then set it for the entire group as well
	if(m_Group && _player->IsGroupLeader())
		m_Group->SetDungeonDifficulty(data);
}

void WorldSession::HandleRaidDifficultyOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 data;
	recv_data >> data;

	// set the raid difficulty for us
	_player->SetRaidDifficulty(data);
	sInstanceMgr.ResetSavedInstances(_player);

	Group* m_Group = _player->GetGroup();

	// if we have a group and we are the leader then set it for the entire group as well
	if(m_Group && _player->IsGroupLeader())
		m_Group->SetRaidDifficulty(data);
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 SummonerGUID;
	uint8 IsClickOk;

	recv_data >> SummonerGUID >> IsClickOk;

	if(!IsClickOk)
		return;
	if(!_player->m_summoner)
	{
		SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
		return;
	}

	if(_player->CombatStatus.IsInCombat())
		return;

	_player->SafeTeleport(_player->m_summonMapId, _player->m_summonInstanceId, _player->m_summonPos);

	_player->m_summoner = _player->m_summonInstanceId = _player->m_summonMapId = 0;
}

void WorldSession::HandleDismountOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	LOG_DEBUG("WORLD: Received CMSG_DISMOUNT");

	if(_player->GetTaxiState())
		return;

	_player->Dismount();
}

void WorldSession::HandleSetAutoLootPassOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 on;
	recv_data >> on;

	if(_player->IsInWorld())
		_player->BroadcastMessage(_player->GetSession()->LocalizedWorldSrv(67), on ? _player->GetSession()->LocalizedWorldSrv(68) : _player->GetSession()->LocalizedWorldSrv(69));

	_player->m_passOnLoot = (on != 0) ? true : false;
}

void WorldSession::HandleRemoveGlyph(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 glyphNum;
	recv_data >> glyphNum;
	if(glyphNum > 5)
		return; // Glyph doesn't exist
	// Get info
	uint32 glyphId = _player->GetGlyph(glyphNum);
	if(glyphId == 0)
		return;
	GlyphPropertyEntry* glyph = dbcGlyphProperty.LookupEntryForced(glyphId);
	if(!glyph)
		return;
	_player->SetGlyph(glyphNum, 0);
	_player->RemoveAllAuras(glyph->SpellID, 0);
	_player->m_specs[_player->m_talentActiveSpec].glyphs[glyphNum] = 0;
	_player->smsg_TalentsInfo(false);
}

void WorldSession::HandleGameobjReportUseOpCode(WorldPacket & recv_data)    // CMSG_GAMEOBJ_REPORT_USE
{
	CHECK_INWORLD_RETURN;
	uint64 guid;
	recv_data >> guid;
	GameObject* gameobj = _player->GetMapMgr()->GetGameObject((uint32)guid);
	if(gameobj == NULL)
		return;
	sQuestMgr.OnGameObjectActivate(_player, gameobj);
#ifdef ENABLE_ACHIEVEMENTS
	_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, gameobj->GetEntry(), 0, 0);
#endif
	return;
}

void WorldSession::HandleWorldStateUITimerUpdate(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WorldPacket data(SMSG_WORLD_STATE_UI_TIMER_UPDATE, 4);
	data << (uint32)UNIXTIME;
	SendPacket(&data);
}

void WorldSession::HandleSetTaxiBenchmarkOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	CHECK_PACKET_SIZE(recv_data, 1);

	uint8 mode;
	recv_data >> mode;

	LOG_DEBUG("Client used \"/timetest %d\" command", mode);
}

void WorldSession::HandleRealmSplitOpcode(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 4);

	LOG_DEBUG("WORLD: Received CMSG_REALM_SPLIT");

	uint32 unk;
	std::string split_date = "01/01/01";
	recv_data >> unk;

	WorldPacket data(SMSG_REALM_SPLIT, 4 + 4 + split_date.size() + 1);
	data << unk;
	data << uint32(0);   // realm split state
	// split states:
	// 0x0 realm normal
	// 0x1 realm split
	// 0x2 realm split pending
	data << split_date;
	SendPacket(&data);
}