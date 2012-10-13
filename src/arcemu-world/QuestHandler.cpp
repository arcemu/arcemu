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
initialiseSingleton(QuestMgr);

void WorldSession::HandleQuestgiverStatusQueryOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_STATUS_QUERY.");

	if(_player->IsInBg())
		return; //Added in 3.0.2, quests can be shared anywhere besides a BG

	uint64 guid;
	WorldPacket data(SMSG_QUESTGIVER_STATUS, 12);
	Object* qst_giver = NULL;

	recv_data >> guid;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);
	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;

		if(!quest_giver->isQuestGiver())
		{
			LOG_DEBUG("WORLD: Creature is not a questgiver.");
			return;
		}
	}
	else if(guidtype == HIGHGUID_TYPE_ITEM)
	{
		Item* quest_giver = GetPlayer()->GetItemInterface()->GetItemByGUID(guid);
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
	}

	if(!qst_giver)
	{
		LOG_DEBUG("WORLD: Invalid questgiver GUID " I64FMT ".", guid);
		return;
	}

	data << guid << sQuestMgr.CalcStatus(qst_giver, GetPlayer());
	SendPacket(&data);
}

void WorldSession::HandleQuestgiverHelloOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_HELLO.");

	uint64 guid;

	recv_data >> guid;

	Creature* qst_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));

	if(!qst_giver)
	{
		LOG_DEBUG("WORLD: Invalid questgiver GUID.");
		return;
	}

	if(!qst_giver->isQuestGiver())
	{
		LOG_DEBUG("WORLD: Creature is not a questgiver.");
		return;
	}

	/*if(qst_giver->GetAIInterface()) // NPC Stops moving for 3 minutes
		qst_giver->GetAIInterface()->StopMovement(180000);*/

	//qst_giver->Emote(EMOTE_ONESHOT_TALK); // this doesn't work
	sQuestMgr.OnActivateQuestGiver(qst_giver, GetPlayer());
}

void WorldSession::HandleQuestGiverQueryQuestOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_QUERY_QUEST.");

	WorldPacket data;
	uint64 guid;
	uint32 quest_id;
	uint32 status = 0;
	uint8 unk;

	recv_data >> guid;
	recv_data >> quest_id;
	recv_data >> unk;

	Object* qst_giver = NULL;

	bool bValid = false;
	Quest* qst = QuestStorage.LookupEntry(quest_id);

	if(!qst)
	{
		LOG_DEBUG("WORLD: Invalid quest ID.");
		return;
	}

	uint32 guidtype = GET_TYPE_FROM_GUID(guid);
	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		if(quest_giver->isQuestGiver())
		{
			bValid = true;
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
		}
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		bValid = quest_giver->isQuestGiver();
		if(quest_giver->isQuestGiver())
		{
			bValid = true;
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
		}
	}
	else if(guidtype == HIGHGUID_TYPE_ITEM)
	{
		Item* quest_giver = GetPlayer()->GetItemInterface()->GetItemByGUID(guid);
//------cebernic: added it for script engine
		if(!quest_giver) return;
		ItemPrototype* itemProto = quest_giver->GetProto();

		if(itemProto->Bonding != ITEM_BIND_ON_USE || quest_giver->IsSoulbound())     // SoulBind item will be used after SoulBind()
		{
			if(sScriptMgr.CallScriptedItem(quest_giver, GetPlayer()))
				return;
		}

		if(itemProto->Bonding == ITEM_BIND_ON_USE)
			quest_giver->SoulBind();
//---------------------------------------------
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		bValid = true;
		status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, 1, false);
	}

	if(!qst_giver)
	{
		LOG_DEBUG("WORLD: Invalid questgiver GUID.");
		return;
	}

	if(!bValid)
	{
		LOG_DEBUG("WORLD: object is not a questgiver.");
		return;
	}


	if((status == QMGR_QUEST_AVAILABLE) || (status == QMGR_QUEST_REPEATABLE) || (status == QMGR_QUEST_CHAT))
	{
		sQuestMgr.BuildQuestDetails(&data, qst, qst_giver, 1, language, _player);	 // 0 because we want goodbye to function
		SendPacket(&data);
		LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS.");

		if(qst->HasFlag(QUEST_FLAGS_AUTO_ACCEPT))
			_player->AcceptQuest(qst_giver->GetGUID(), qst->id);
	}

	else if(status == QMGR_QUEST_NOT_FINISHED || status == QMGR_QUEST_FINISHED)
	{
		sQuestMgr.BuildRequestItems(&data, qst, qst_giver, status, language);
		SendPacket(&data);
		LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS.");
	}
}

void WorldSession::HandleQuestgiverAcceptQuestOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_ACCEPT_QUEST");

	uint64 guid;
	uint32 quest_id;

	recv_data >> guid;
	recv_data >> quest_id;

	_player->AcceptQuest(guid, quest_id);

}

void WorldSession::HandleQuestgiverCancelOpcode(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);

	LOG_DEBUG("WORLD: Sent SMSG_GOSSIP_COMPLETE");
}

void WorldSession::HandleQuestlogRemoveQuestOpcode(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTLOG_REMOVE_QUEST");

	uint8 quest_slot;
	recvPacket >> quest_slot;
	if(quest_slot >= 25)
		return;

	QuestLogEntry* qEntry = GetPlayer()->GetQuestLogInSlot(quest_slot);
	if(!qEntry)
	{
		LOG_DEBUG("WORLD: No quest in slot %d.", quest_slot);
		return;
	}
	Quest* qPtr = qEntry->GetQuest();
	CALL_QUESTSCRIPT_EVENT(qEntry, OnQuestCancel)(GetPlayer());
	qEntry->Finish();

	// Remove all items given by the questgiver at the beginning
	for(uint32 i = 0; i < 4; ++i)
	{
		if(qPtr->receive_items[i])
			GetPlayer()->GetItemInterface()->RemoveItemAmt(qPtr->receive_items[i], 1);
	}

	if( qPtr->srcitem && qPtr->srcitem != qPtr->receive_items[0] )
	{
		ItemPrototype *itemProto = ::ItemPrototypeStorage.LookupEntry( qPtr->srcitem );
		if( itemProto != NULL )
			if( itemProto->QuestId != qPtr->id )
				_player->GetItemInterface()->RemoveItemAmt(qPtr->srcitem, qPtr->srcitemcount ? qPtr->srcitemcount : 1);
	}
	//remove all quest items (but not trade goods) collected and required only by this quest
	for(uint32 i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
	{
		if(qPtr->required_item[i] != 0)
		{
			ItemPrototype* itemProto = ItemPrototypeStorage.LookupEntry(qPtr->required_item[i]);
			if(itemProto != NULL && itemProto->Class == ITEM_CLASS_QUEST)
				GetPlayer()->GetItemInterface()->RemoveItemAmt(qPtr->required_item[i], qPtr->required_itemcount[i]);
		}
	}

	GetPlayer()->UpdateNearbyGameObjects();

	sHookInterface.OnQuestCancelled(_player, qPtr);
}

void WorldSession::HandleQuestQueryOpcode(WorldPacket & recv_data)
{

	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUEST_QUERY");

	uint32 quest_id;

	recv_data >> quest_id;

	Quest* qst = QuestStorage.LookupEntry(quest_id);

	if(!qst)
	{
		LOG_DEBUG("WORLD: Invalid quest ID.");
		return;
	}

	WorldPacket* pkt = BuildQuestQueryResponse(qst);
	SendPacket(pkt);
	delete pkt;

	LOG_DEBUG("WORLD: Sent SMSG_QUEST_QUERY_RESPONSE.");
}

void WorldSession::HandleQuestgiverRequestRewardOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_REQUESTREWARD_QUEST.");

	uint64 guid;
	uint32 quest_id;

	recv_data >> guid;
	recv_data >> quest_id;

	bool bValid = false;
	Quest* qst = NULL;
	Object* qst_giver = NULL;
	uint32 status = 0;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		if(quest_giver->isQuestGiver())
		{
			bValid = true;
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			if(!qst)
				qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_START);

			/*if(!qst)
				qst = QuestStorage.LookupEntry(quest_id);*/
			if(!qst)
			{
				LOG_ERROR("WARNING: Cannot get reward for quest %u, as it doesn't exist at Unit %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
		}
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return; // oops..
		bValid = quest_giver->isQuestGiver();
		if(bValid)
		{
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			/*if(!qst) sQuestMgr.FindQuest(quest_id);*/
			if(!qst)
			{
				LOG_ERROR("WARNING: Cannot get reward for quest %u, as it doesn't exist at GO %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
		}
	}

	if(!qst_giver)
	{
		LOG_DEBUG("WORLD: Invalid questgiver GUID.");
		return;
	}

	if(!bValid || qst == NULL)
	{
		LOG_DEBUG("WORLD: Creature is not a questgiver.");
		return;
	}

	if(status == QMGR_QUEST_FINISHED)
	{
		WorldPacket data;
		sQuestMgr.BuildOfferReward(&data, qst, qst_giver, 1, language, _player);
		SendPacket(&data);
		LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS.");
	}

	// if we got here it means we're cheating
}

void WorldSession::HandleQuestgiverCompleteQuestOpcode(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_COMPLETE_QUEST.");

	uint64 guid;
	uint32 quest_id;

	recvPacket >> guid;
	recvPacket >> quest_id;

	bool bValid = false;
	Quest* qst = NULL;
	Object* qst_giver = NULL;
	uint32 status = 0;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		if(quest_giver->isQuestGiver())
		{
			bValid = true;
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			/*if(!qst)
				sQuestMgr.FindQuest(quest_id);*/
			if(!qst)
			{
				LOG_ERROR("WARNING: Cannot complete quest %u, as it doesn't exist at Unit %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
		}
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return; // oops..
		bValid = quest_giver->isQuestGiver();
		if(bValid)
		{
			qst = quest_giver->FindQuest(quest_id, QUESTGIVER_QUEST_END);
			/*if(!qst) sQuestMgr.FindQuest(quest_id);*/
			if(!qst)
			{
				LOG_ERROR("WARNING: Cannot complete quest %u, as it doesn't exist at GO %u.", quest_id, quest_giver->GetEntry());
				return;
			}
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), qst, (uint8)quest_giver->GetQuestRelation(qst->id), false);
		}
	}

	if(!qst_giver)
	{
		LOG_DEBUG("WORLD: Invalid questgiver GUID.");
		return;
	}

	if(!bValid || qst == NULL)
	{
		LOG_DEBUG("WORLD: Creature is not a questgiver.");
		return;
	}

	if(status == QMGR_QUEST_NOT_FINISHED || status == QMGR_QUEST_REPEATABLE)
	{
		WorldPacket data;
		sQuestMgr.BuildRequestItems(&data, qst, qst_giver, status, language);
		SendPacket(&data);
		LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS.");
	}

	if(status == QMGR_QUEST_FINISHED)
	{
		WorldPacket data;
		sQuestMgr.BuildOfferReward(&data, qst, qst_giver, 1, language, _player);
		SendPacket(&data);
		LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS.");
	}

	sHookInterface.OnQuestFinished(_player, qst, qst_giver);
}

void WorldSession::HandleQuestgiverChooseRewardOpcode(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG("WORLD: Received CMSG_QUESTGIVER_CHOOSE_REWARD.");

	uint64 guid;
	uint32 quest_id;
	uint32 reward_slot;

	recvPacket >> guid;
	recvPacket >> quest_id;
	recvPacket >> reward_slot;

	if(reward_slot >= 6)
		return;

	bool bValid = false;
	Quest* qst = NULL;
	Object* qst_giver = NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* quest_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		if(quest_giver->isQuestGiver())
		{
			bValid = true;
			qst = QuestStorage.LookupEntry(quest_id);
		}
	}
	else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* quest_giver = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(quest_giver)
			qst_giver = quest_giver;
		else
			return;
		//bValid = quest_giver->isQuestGiver();
		//if(bValid)
		bValid = true;
		qst = QuestStorage.LookupEntry(quest_id);
	}

	if(!qst_giver)
	{
		LOG_DEBUG("WORLD: Invalid questgiver GUID.");
		return;
	}

	if(!bValid || qst == NULL)
	{
		LOG_DEBUG("WORLD: Creature is not a questgiver.");
		return;
	}

	//FIX ME: Some Quest givers talk in the end of the quest.
	//   qst_giver->SendChatMessage(CHAT_MSG_MONSTER_SAY,LANG_UNIVERSAL,qst->GetQuestEndMessage().c_str());
	QuestLogEntry* qle = _player->GetQuestLogForEntry(quest_id);

	if(!qle && !qst->is_repeatable)
	{
		LOG_DEBUG("WORLD: QuestLogEntry not found.");
		return;
	}

	if(qle && !qle->CanBeFinished())
	{
		LOG_DEBUG("WORLD: Quest not finished.");
		return;
	}

	// remove icon
	/*if(qst_giver->GetTypeId() == TYPEID_UNIT)
	{
		qst_giver->BuildFieldUpdatePacket(GetPlayer(), UNIT_DYNAMIC_FLAGS, qst_giver->GetUInt32Value(UNIT_DYNAMIC_FLAGS));
	}*/

	//check for room in inventory for all items
	if(!sQuestMgr.CanStoreReward(GetPlayer(), qst, reward_slot))
	{
		sQuestMgr.SendQuestFailed(FAILED_REASON_INV_FULL, qst, GetPlayer());
		return;
	}


	sQuestMgr.OnQuestFinished(GetPlayer(), qst, qst_giver, reward_slot);
	//if(qst_giver->GetTypeId() == TYPEID_UNIT) qst->LUA_SendEvent(TO< Creature* >( qst_giver ),GetPlayer(),ON_QUEST_COMPLETEQUEST);

	if(qst->next_quest_id)
	{
		WorldPacket data(12);
		data.Initialize(CMSG_QUESTGIVER_QUERY_QUEST);
		data << guid;
		data << qst->next_quest_id;
		HandleQuestGiverQueryQuestOpcode(data);
	}
}

void WorldSession::HandlePushQuestToPartyOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 questid;
	recv_data >> questid;

	LOG_DETAIL("WORLD: Received CMSG_PUSHQUESTTOPARTY quest = %u", questid);

	Quest* pQuest = QuestStorage.LookupEntry(questid);
	if(pQuest)
	{
		Group* pGroup = _player->GetGroup();
		if(pGroup)
		{
			uint32 pguid = _player->GetLowGUID();
			SubGroup* sgr = _player->GetGroup() ?
			                _player->GetGroup()->GetSubGroup(_player->GetSubGroup()) : 0;

			if(sgr)
			{
				_player->GetGroup()->Lock();
				GroupMembersSet::iterator itr;
				for(itr = sgr->GetGroupMembersBegin(); itr != sgr->GetGroupMembersEnd(); ++itr)
				{
					Player* pPlayer = (*itr)->m_loggedInPlayer;
					if(pPlayer && pPlayer->GetGUID() !=  pguid)
					{
						WorldPacket data(MSG_QUEST_PUSH_RESULT, 9);
						data << uint64(pPlayer->GetGUID());
						data << uint8(QUEST_SHARE_MSG_SHARING_QUEST);
						_player->GetSession()->SendPacket(&data);

						uint8 response = QUEST_SHARE_MSG_SHARING_QUEST;
						uint32 status = sQuestMgr.PlayerMeetsReqs(pPlayer, pQuest, false);

						// Checks if the player has the quest
						if(pPlayer->GetQuestLogForEntry(questid))
						{
							response = QUEST_SHARE_MSG_HAVE_QUEST;
						}
						// Checks if the player has finished the quest
						else if(pPlayer->HasFinishedQuest(questid))
						{
							response = QUEST_SHARE_MSG_FINISH_QUEST;
						}
						// Checks if the player is able to take the quest
						else if(status != QMGR_QUEST_AVAILABLE && status != QMGR_QUEST_CHAT)
						{
							response = QUEST_SHARE_MSG_CANT_TAKE_QUEST;
						}
						// Checks if the player has room in his/her questlog
						else if(pPlayer->GetOpenQuestSlot() == -1)
						{
							response = QUEST_SHARE_MSG_LOG_FULL;
						}
						// Checks if the player is dueling
						else if(pPlayer->DuelingWith)   // || pPlayer->GetQuestSharer() ) //VLack: A possible lock up can occur if we don't zero out questsharer, because sometimes the client does not send the reply packet.. This of course eliminates the check on it, so it is possible to spam group members with quest sharing, but hey, they are YOUR FRIENDS, and better than not being able to receive quest sharing requests at all!
						{
							response = QUEST_SHARE_MSG_BUSY;
						}

						//VLack: The quest giver player has to be visible for pPlayer, or else the client will show a non-functional "complete quest" panel instead of the "accept quest" one!
						//We could either push a full player create for pPlayer that would cause problems later (because they are still out of range and this would have to be handled somehow),
						//or create a fake bad response, as we no longer have an out of range response. I'll go with the latter option and send that the other player is busy...
						//Also, pPlayer's client can send a busy response automatically even if the players see each other, but they are still too far away.
						//But sometimes nothing happens on pPlayer's client (near the end of mutual visibility line), no quest window and no busy response either. This has to be solved later, maybe a distance check here...
						if(response == QUEST_SHARE_MSG_SHARING_QUEST && !pPlayer->IsVisible(_player->GetGUID()))
						{
							response = QUEST_SHARE_MSG_BUSY;
						}

						if(response != QUEST_SHARE_MSG_SHARING_QUEST)
						{
							sQuestMgr.SendPushToPartyResponse(_player, pPlayer, response);
							continue;
						}

						data.clear();
						sQuestMgr.BuildQuestDetails(&data, pQuest, _player, 1, pPlayer->GetSession()->language, pPlayer);
						pPlayer->SetQuestSharer(pguid); //VLack: better to set this _before_ sending out the packet, so no race conditions can happen on heavily loaded servers.
						pPlayer->GetSession()->SendPacket(&data);
					}
				}
				_player->GetGroup()->Unlock();
			}
		}
	}
}

void WorldSession::HandleQuestPushResult(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint8 msg;
	recvPacket >> guid;
	uint32 questid = 0;
	if(recvPacket.size() >= 13)  //VLack: The client can send a 13 byte packet, where the result message is the 13th byte, and we have some data before it... Usually it is the quest id, but I have seen this as uint32(0) too.
		recvPacket >> questid;
	recvPacket >> msg;

	LOG_DETAIL("WORLD: Received MSG_QUEST_PUSH_RESULT");

	if(GetPlayer()->GetQuestSharer())
	{
		Player* pPlayer = objmgr.GetPlayer(GetPlayer()->GetQuestSharer());
		if(pPlayer)
		{
			WorldPacket data(MSG_QUEST_PUSH_RESULT, 9);
			if(recvPacket.size() >= 13)  //VLack: In case the packet was the longer one, its guid is the quest giver player, thus in the response we have to tell him that _this_ player reported the particular state. I think this type of response could also eliminate our SetQuestSharer/GetQuestSharer mess and its possible lock up conditions...
				data << uint64(_player->GetGUID());
			else
				data << uint64(guid);
			data << uint8(msg);
			pPlayer->GetSession()->SendPacket(&data);
			GetPlayer()->SetQuestSharer(0);
		}
	}
}
