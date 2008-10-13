/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
initialiseSingleton( QuestMgr );

void WorldSession::HandleQuestgiverStatusQueryOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_STATUS_QUERY." );
	if(!_player) return;
    if(!_player->IsInWorld()) return;

	uint64 guid;
	WorldPacket data(SMSG_QUESTGIVER_STATUS, 12);
    Object *qst_giver = NULL;

	recv_data >> guid;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);
    if(guidtype==HIGHGUID_TYPE_UNIT)
    {
        Creature *quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
        if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;

        if (!quest_giver->isQuestGiver())
	    {
		    sLog.outDebug("WORLD: Creature is not a questgiver.");
		    return;
	    }
    }
    else if(guidtype==HIGHGUID_TYPE_ITEM)
	{
		Item *quest_giver = GetPlayer()->GetItemInterface()->GetItemByGUID(guid);
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
	}
    else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject *quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
	}

	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID "I64FMT".", guid);
		return;
	}

	data << guid << sQuestMgr.CalcStatus(qst_giver, GetPlayer());
	SendPacket( &data );
}

void WorldSession::HandleQuestgiverHelloOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_HELLO." );
    if(!_player) return;
	if(!_player->IsInWorld()) return;

	uint64 guid;

	recv_data >> guid;

	Creature *qst_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));

	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID.");
		return;
	}

	if (!qst_giver->isQuestGiver())
	{
		sLog.outDebug("WORLD: Creature is not a questgiver.");
		return;
	}

	/*if(qst_giver->GetAIInterface()) // NPC Stops moving for 3 minutes
		qst_giver->GetAIInterface()->StopMovement(180000);*/

	//qst_giver->Emote(EMOTE_ONESHOT_TALK); // this doesnt work
	sQuestMgr.OnActivateQuestGiver(qst_giver, GetPlayer());
}

void WorldSession::HandleQuestGiverQueryQuestOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_QUERY_QUEST." );
    if(!_player) return;
	if(!_player->IsInWorld()) return;

	WorldPacket data;
	uint64 guid;
	uint32 quest_id;
	uint32 status = 0;

	recv_data >> guid;
	recv_data >> quest_id;

	Object *qst_giver = NULL;

	bool bValid = false;
	Quest* qst = QuestStorage.LookupEntry(quest_id);
	
	if (!qst)
	{
		sLog.outDebug("WORLD: Invalid quest ID.");
		return;
	}

	uint32 guidtype = GET_TYPE_FROM_GUID(guid);
	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		if(bValid)
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
	} 
	else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject *quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		if(bValid)
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
	} 
	else if(guidtype==HIGHGUID_TYPE_ITEM)
	{
		Item *quest_giver = GetPlayer()->GetItemInterface()->GetItemByGUID(guid);
//------cebernic: added it for script engine
    if ( !quest_giver ) return;
    ItemPrototype *itemProto = quest_giver->GetProto();
	  if(!itemProto)
		  return;

    if ( itemProto->Bonding != ITEM_BIND_ON_USE || quest_giver->IsSoulbound() ){ // SoulBind item will be used after SoulBind()
      if(sScriptMgr.CallScriptedItem(quest_giver,GetPlayer()))
		    return;
    }

    if(itemProto->Bonding == ITEM_BIND_ON_USE)
		  quest_giver->SoulBind();
//---------------------------------------------
    if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = true;
		status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, 1, false);
	}
	
	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID.");
		return;
	}

	if (!bValid)
	{
		sLog.outDebug("WORLD: object is not a questgiver.");
		return;
	}

	/*if (!qst_giver->FindQuest(quest_id, QUESTGIVER_QUEST_START | QUESTGIVER_QUEST_END))
	{
		sLog.outDebug("WORLD: QuestGiver doesn't have that quest.");
		return;
	}*/	// bleh.. not needed.. maybe for antihack later on would be a good idea though
	
	if ((status == QMGR_QUEST_AVAILABLE) || (status == QMGR_QUEST_REPEATABLE) || (status == QMGR_QUEST_CHAT))
	{
		sQuestMgr.BuildQuestDetails(&data, qst,qst_giver,1, language, _player);	 // 0 because we want goodbye to function
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS." );
	}
	/*else if (status == QMGR_QUEST_FINISHED)
	{
		sQuestMgr.BuildOfferReward(&data, qst, qst_giver, 1);
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD." );
	}*/
	else if (status == QMGR_QUEST_NOT_FINISHED || status == QMGR_QUEST_FINISHED)
	{
		sQuestMgr.BuildRequestItems(&data, qst, qst_giver, status, language);
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS." );
	}
}

void WorldSession::HandleQuestgiverAcceptQuestOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_ACCEPT_QUEST" );
    if(!_player) return;
	if(!_player->IsInWorld()) return;

	//WorldPacket data;

	uint64 guid;
	uint32 quest_id;

	recv_data >> guid;
	recv_data >> quest_id;

	bool bValid = false;
	bool hasquest = true;
	bool bSkipLevelCheck = false;
	Quest *qst = NULL;
	Object *qst_giver = NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		hasquest = quest_giver->HasQuest(quest_id, 1);
		if(bValid)
			qst = QuestStorage.LookupEntry(quest_id);
	} 
	else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject *quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		//bValid = quest_giver->isQuestGiver();
		//if(bValid)
		bValid = true;
			qst = QuestStorage.LookupEntry(quest_id);
	} 
	else if(guidtype==HIGHGUID_TYPE_ITEM)
	{
		Item *quest_giver = GetPlayer()->GetItemInterface()->GetItemByGUID(guid);
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = true;
		bSkipLevelCheck=true;
		qst = QuestStorage.LookupEntry(quest_id);
	}
	else if(guidtype==HIGHGUID_TYPE_PLAYER)
	{
		Player *quest_giver = _player->GetMapMgr()->GetPlayer((uint32)guid);
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = true;
		qst = QuestStorage.LookupEntry(quest_id);
	}

	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID.");
		return;
	}

	if( !bValid || qst == NULL )
	{
		sLog.outDebug("WORLD: Creature is not a questgiver.");
		return;
	}

	if( _player->GetQuestLogForEntry( qst->id ) )
		return;

	if( qst_giver->GetTypeId() == TYPEID_UNIT && static_cast< Creature* >( qst_giver )->m_escorter != NULL )
	{
		SystemMessage("You cannot accept this quest at this time.");
		return;
	}

	// Check the player hasn't already taken this quest, or
	// it isn't available.
	uint32 status = sQuestMgr.CalcQuestStatus(qst_giver, _player,qst,3, bSkipLevelCheck);

	if((!sQuestMgr.IsQuestRepeatable(qst) && _player->HasFinishedQuest(qst->id)) || ( status != QMGR_QUEST_AVAILABLE && status != QMGR_QUEST_REPEATABLE && status != QMGR_QUEST_CHAT )
		|| !hasquest)
	{
		// We've got a hacker. Disconnect them.
		//sCheatLog.writefromsession(this, "tried to accept incompatible quest %u from %u.", qst->id, qst_giver->GetEntry());
		//Disconnect();
		return;
	}

	int32 log_slot = GetPlayer()->GetOpenQuestSlot();

	if (log_slot == -1)
	{
		sQuestMgr.SendQuestLogFull(GetPlayer());
		return;
	}

	//FIXME
	/*if(Player Has Timed quest && qst->HasFlag(QUEST_FLAG_TIMED))
		sQuestMgr.SendQuestInvalid(INVALID_REASON_HAVE_TIMED_QUEST);*/

	if(qst->count_receiveitems || qst->srcitem)
	{
		uint32 slots_required = qst->count_receiveitems;

		if(GetPlayer()->GetItemInterface()->CalculateFreeSlots(NULL) < slots_required)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_BAG_FULL);
			sQuestMgr.SendQuestFailed(FAILED_REASON_INV_FULL, qst, GetPlayer());
			return;
		}
	}	
	
/*	if(qst_giver->GetTypeId() == TYPEID_UNIT && !ScriptSystem->OnQuestRequireEvent(qst, static_cast< Creature* >( qst_giver ), _player, QUEST_EVENT_CAN_ACCEPT))
		return;*/

	QuestLogEntry *qle = new QuestLogEntry();
	qle->Init(qst, _player, log_slot);
	qle->UpdatePlayerFields();

	// If the quest should give any items on begin, give them the items.
	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->receive_items[i])
		{
			Item *item = objmgr.CreateItem( qst->receive_items[i], GetPlayer());
			if(item == NULL)
				continue;
			if(!GetPlayer()->GetItemInterface()->AddItemToFreeSlot(item))
			{
				item->DeleteMe();
			}
			else
				SendItemPushResult(item, false, true, false, true, 
				_player->GetItemInterface()->LastSearchItemBagSlot(), _player->GetItemInterface()->LastSearchItemSlot(),
				1);
		}
	}

	if(qst->srcitem && qst->srcitem != qst->receive_items[0])
	{
		Item * item = objmgr.CreateItem( qst->srcitem, _player );
		if(item)
		{
			item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, qst->srcitemcount ? qst->srcitemcount : 1);
			if(!_player->GetItemInterface()->AddItemToFreeSlot(item))
				item->DeleteMe();
		}
	}

	// Timed quest handler.
	if(qst->time > 0)
	{
		//Start Quest Timer Event Here
		//sEventMgr.AddEvent(GetPlayer(), &Player::EventTimedQuestExpire, qst, qle, static_cast<uint32>(log_slot), EVENT_TIMED_QUEST_EXPIRE, qst->time * 1000, 1);
		//uint32 qtime = static_cast<uint32>(time(NULL) + qst->time);
		//GetPlayer()->SetUInt32Value(log_slot+2, qtime);
		//GetPlayer()->SetUInt32Value(PLAYER_QUEST_LOG_1_01 + (log_slot * 3), qtime);
		//GetPlayer()->timed_quest_slot = log_slot;
	}

	if(qst->count_required_item || qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)	// gameobject quests deactivate
		GetPlayer()->UpdateNearbyGameObjects();

	//ScriptSystem->OnQuestEvent(qst, static_cast< Creature* >( qst_giver ), _player, QUEST_EVENT_ON_ACCEPT);

	sQuestMgr.OnQuestAccepted(_player,qst,qst_giver);

	sLog.outDebug("WORLD: Added new QLE.");
	sHookInterface.OnQuestAccept(_player, qst, qst_giver);
}

void WorldSession::HandleQuestgiverCancelOpcode(WorldPacket& recvPacket)
{
    OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);

	sLog.outDebug("WORLD: Sent SMSG_GOSSIP_COMPLETE");
}

void WorldSession::HandleQuestlogRemoveQuestOpcode(WorldPacket& recvPacket)
{
	sLog.outDebug( "WORLD: Received CMSG_QUESTLOG_REMOVE_QUEST" );
    if(!_player) return;
	if(!_player->IsInWorld()) return;

	uint8 quest_slot;
	recvPacket >> quest_slot;
	if(quest_slot >= 25)
		return;

	QuestLogEntry *qEntry = GetPlayer()->GetQuestLogInSlot(quest_slot);
	if (!qEntry)
	{
		sLog.outDebug("WORLD: No quest in slot %d.", quest_slot);
		return;		
	}
	Quest *qPtr = qEntry->GetQuest();
	CALL_QUESTSCRIPT_EVENT(qEntry, OnQuestCancel)(GetPlayer());
	qEntry->Finish();

	// Remove all items given by the questgiver at the beginning
	for(uint32 i = 0; i < 4; ++i)
	{
		if(qPtr->receive_items[i])
			GetPlayer()->GetItemInterface()->RemoveItemAmt( qPtr->receive_items[i], 1 );
	}
	if(qPtr->time > 0)
	{
		GetPlayer()->timed_quest_slot = 0;
	}
	GetPlayer()->UpdateNearbyGameObjects();

	sHookInterface.OnQuestCancelled(_player, qPtr);
}

void WorldSession::HandleQuestQueryOpcode( WorldPacket & recv_data )
{
    if(!_player) return;
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_QUEST_QUERY" );

	uint32 quest_id;

	recv_data >> quest_id;

	Quest *qst = QuestStorage.LookupEntry(quest_id);

	if (!qst)
	{
		sLog.outDebug("WORLD: Invalid quest ID.");
		return;
	}

	WorldPacket *pkt = BuildQuestQueryResponse(qst);
	SendPacket(pkt);
	delete pkt;

	sLog.outDebug( "WORLD: Sent SMSG_QUEST_QUERY_RESPONSE." );
}

void WorldSession::HandleQuestgiverRequestRewardOpcode( WorldPacket & recv_data )
{
    if(!_player) return;
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_REQUESTREWARD_QUEST." );

	uint64 guid;
	uint32 quest_id;

	recv_data >> guid;
	recv_data >> quest_id;

	bool bValid = false;
	Quest *qst = NULL;
	Object *qst_giver = NULL;
	uint32 status = 0;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		if(bValid)
		{
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			if(!qst)
				qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_START);

			/*if(!qst) 
				qst = QuestStorage.LookupEntry(quest_id);*/
			if(!qst)
			{
				sLog.outError("WARNING: Cannot get reward for quest %u, as it doesn't exist at Unit %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id),false);
		}
	} 
	else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject *quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return; // oops..
		bValid = quest_giver->isQuestGiver();
		if(bValid)
		{
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			/*if(!qst) sQuestMgr.FindQuest(quest_id);*/
			if(!qst)
			{
				sLog.outError("WARNING: Cannot get reward for quest %u, as it doesn't exist at GO %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id),false);
		}
	}

	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID.");
		return;
	}

	if (!bValid || qst == NULL)
	{
		sLog.outDebug("WORLD: Creature is not a questgiver.");
		return;
	}

	if (status == QMGR_QUEST_FINISHED)
	{
        WorldPacket data;
		sQuestMgr.BuildOfferReward(&data, qst, qst_giver, 1, language, _player);
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS." );
	}

	// if we got here it means we're cheating
}

void WorldSession::HandleQuestgiverCompleteQuestOpcode( WorldPacket & recvPacket )
{
    if(!_player) return;
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_COMPLETE_QUEST." );

	uint64 guid;
	uint32 quest_id;

	recvPacket >> guid;
	recvPacket >> quest_id;

	bool bValid = false;
	Quest *qst = NULL;
	Object *qst_giver = NULL;
	uint32 status = 0;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		if(bValid)
		{
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			/*if(!qst) 
				sQuestMgr.FindQuest(quest_id);*/
			if(!qst)
			{
				sLog.outError("WARNING: Cannot complete quest %u, as it doesn't exist at Unit %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id),false);
		}
	} 
	else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject *quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return; // oops..
		bValid = quest_giver->isQuestGiver();
		if(bValid)
		{
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			/*if(!qst) sQuestMgr.FindQuest(quest_id);*/
			if(!qst)
			{
				sLog.outError("WARNING: Cannot complete quest %u, as it doesn't exist at GO %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id),false);
		}
	}

	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID.");
		return;
	}

	if (!bValid || qst == NULL)
	{
		sLog.outDebug("WORLD: Creature is not a questgiver.");
		return;
	}

	if (status == QMGR_QUEST_NOT_FINISHED || status == QMGR_QUEST_REPEATABLE)
	{
        WorldPacket data;
		sQuestMgr.BuildRequestItems(&data, qst, qst_giver, status, language);
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS." );
	}

	if (status == QMGR_QUEST_FINISHED)
	{
		WorldPacket data;
		sQuestMgr.BuildOfferReward(&data, qst, qst_giver, 1, language, _player);
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS." );
	}
	
	sHookInterface.OnQuestFinished(_player, qst, qst_giver);
}

void WorldSession::HandleQuestgiverChooseRewardOpcode(WorldPacket& recvPacket)
{
    if(!_player) return;
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_QUESTGIVER_CHOOSE_REWARD." );

	uint64 guid;
	uint32 quest_id;
	uint32 reward_slot;

	recvPacket >> guid;
	recvPacket >> quest_id;
	recvPacket >> reward_slot;

	if( reward_slot >= 6 )
		return;

	bool bValid = false;
	Quest *qst = NULL;
	Object *qst_giver = NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		if(bValid)
			qst = QuestStorage.LookupEntry(quest_id);
	} 
	else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject *quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = (Object*)quest_giver;
		else
			return;
		//bValid = quest_giver->isQuestGiver();
		//if(bValid)
		bValid = true;
			qst = QuestStorage.LookupEntry(quest_id);
	}

	if (!qst_giver)
	{
		sLog.outDebug("WORLD: Invalid questgiver GUID.");
		return;
	}

	if (!bValid || qst == NULL)
	{
		sLog.outDebug("WORLD: Creature is not a questgiver.");
		return;
	}

	//FIXME: Some Quest givers talk in the end of the quest.
	//   qst_giver->SendChatMessage(CHAT_MSG_MONSTER_SAY,LANG_UNIVERSAL,qst->GetQuestEndMessage().c_str());
	QuestLogEntry *qle = _player->GetQuestLogForEntry(quest_id);

    if (!qle && !qst->is_repeatable)
	{
		sLog.outDebug("WORLD: QuestLogEntry not found.");
		return;
	}

	if (qle && !qle->CanBeFinished())
	{
		sLog.outDebug("WORLD: Quest not finished.");
		return;
	}

	// remove icon
	/*if(qst_giver->GetTypeId() == TYPEID_UNIT)
	{
		qst_giver->BuildFieldUpdatePacket(GetPlayer(), UNIT_DYNAMIC_FLAGS, qst_giver->GetUInt32Value(UNIT_DYNAMIC_FLAGS));
	}*/

    //check for room in inventory for all items
	if(!sQuestMgr.CanStoreReward(GetPlayer(),qst,reward_slot))
    {
        sQuestMgr.SendQuestFailed(FAILED_REASON_INV_FULL, qst, GetPlayer());
        return;
    }

	
	sQuestMgr.OnQuestFinished(GetPlayer(), qst, qst_giver, reward_slot);
	//if(qst_giver->GetTypeId() == TYPEID_UNIT) qst->LUA_SendEvent(static_cast< Creature* >( qst_giver ),GetPlayer(),ON_QUEST_COMPLETEQUEST);

	if(qst->next_quest_id)
	{
        WorldPacket data(12);
		data.Initialize(CMSG_QUESTGIVER_QUERY_QUEST);
		data << guid;
		data << qst->next_quest_id;
		HandleQuestGiverQueryQuestOpcode(data);
	}
	if(qst->time > 0)
	{
		GetPlayer()->timed_quest_slot = 0;
	}
}

void WorldSession::HandlePushQuestToPartyOpcode(WorldPacket &recv_data)
{
    if(!_player) return;
	if(!_player->IsInWorld()) return;
	uint32 questid, status;
	recv_data >> questid;

	sLog.outDetail( "WORLD: Received CMSG_PUSHQUESTTOPARTY quest = %u", questid );

	Quest *pQuest = QuestStorage.LookupEntry(questid);
	if(pQuest)
	{
		Group *pGroup = _player->GetGroup();
		if(pGroup)
		{
			uint32 pguid = _player->GetLowGUID();
			SubGroup * sgr = _player->GetGroup() ?
				_player->GetGroup()->GetSubGroup(_player->GetSubGroup()) : 0;

			if(sgr)
			{
				_player->GetGroup()->Lock();
				GroupMembersSet::iterator itr;
				for(itr = sgr->GetGroupMembersBegin(); itr != sgr->GetGroupMembersEnd(); ++itr)
				{
					Player *pPlayer = (*itr)->m_loggedInPlayer;
					if(pPlayer && pPlayer->GetGUID() !=  pguid)
					{
						WorldPacket data( MSG_QUEST_PUSH_RESULT, 13 );
						data << pPlayer->GetGUID();
						data << uint32(QUEST_SHARE_MSG_SHARING_QUEST);
						data << uint8(0);
						_player->GetSession()->SendPacket(&data);

						uint32 response = 0;
						if(_player->GetDistance2dSq(pPlayer) > 100)
						{
							response = QUEST_SHARE_MSG_TOO_FAR;
							continue;
						}
						QuestLogEntry *qst = pPlayer->GetQuestLogForEntry(questid);
						if(qst)
						{
							response = QUEST_SHARE_MSG_HAVE_QUEST;
							continue;
						}
						status = sQuestMgr.PlayerMeetsReqs(pPlayer, pQuest, false);
						if(status != QMGR_QUEST_AVAILABLE && status != QMGR_QUEST_CHAT)
						{
							response = QUEST_SHARE_MSG_CANT_TAKE_QUEST;
							continue;
						}
						if(pPlayer->HasFinishedQuest(questid))
						{
							response = QUEST_SHARE_MSG_FINISH_QUEST;
							continue;
						}
						if(pPlayer->GetOpenQuestSlot() == -1)
						{
							response = QUEST_SHARE_MSG_LOG_FULL;
							continue;
						}
						//Anything more?
						if(pPlayer->DuelingWith)
						{
							response = QUEST_SHARE_MSG_BUSY;
							continue;
						}
						if(response > 0)
							sQuestMgr.SendPushToPartyResponse(_player, pPlayer, response);

						data.clear();
						sQuestMgr.BuildQuestDetails(&data, pQuest, pPlayer, 1, pPlayer->GetSession()->language, _player);
						pPlayer->GetSession()->SendPacket(&data);
						pPlayer->SetQuestSharer(pguid);
					}
				}
				_player->GetGroup()->Unlock();
			}
		}
	}
}

void WorldSession::HandleQuestPushResult(WorldPacket& recvPacket)
{
    if(!_player) return;
	if(!_player->IsInWorld()) return;
	uint64 guid;
	uint8 msg;
	recvPacket >> guid >> msg;

	sLog.outDetail( "WORLD: Received MSG_QUEST_PUSH_RESULT " );

	if(GetPlayer()->GetQuestSharer())
	{
		Player *pPlayer = objmgr.GetPlayer(GetPlayer()->GetQuestSharer());
		if(pPlayer)
		{
			WorldPacket data(MSG_QUEST_PUSH_RESULT, 13);
			data << guid;
			data << uint32(msg);
			data << uint8(0);
			pPlayer->GetSession()->SendPacket(&data);
			GetPlayer()->SetQuestSharer(0);
		}
	}
}
