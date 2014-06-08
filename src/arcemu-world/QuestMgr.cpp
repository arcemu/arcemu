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

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, QuestRelation* qst)
{
	return CalcQuestStatus(quest_giver, plr, qst->qst, qst->type, false);
}

bool QuestMgr::isRepeatableQuestFinished(Player* plr, Quest* qst)
{
	uint32 i;

	for(i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
	{
		if(qst->required_item[i])
		{
			if(plr->GetItemInterface()->GetItemCount(qst->required_item[i]) < qst->required_itemcount[i])
			{
				return false;
			}
		}
	}

	return true;
}

uint32 QuestMgr::PlayerMeetsReqs(Player* plr, Quest* qst, bool skiplevelcheck)
{
	std::list<uint32>::iterator itr;
	uint32 status;

	if(!sQuestMgr.IsQuestRepeatable(qst) && !sQuestMgr.IsQuestDaily(qst))
		status = QMGR_QUEST_AVAILABLE;
	else
	{
		status = QMGR_QUEST_REPEATABLE;
		if(qst->is_repeatable == arcemu_QUEST_REPEATABLE_DAILY && plr->HasFinishedDaily(qst->id))
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	if(plr->getLevel() < qst->min_level && !skiplevelcheck)
		return QMGR_QUEST_AVAILABLELOW_LEVEL;

	if(qst->required_class)
		if(!(qst->required_class & plr->getClassMask()))
			return QMGR_QUEST_NOT_AVAILABLE;

	if(qst->required_races)
	{
		if(!(qst->required_races & plr->getRaceMask()))
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	if(qst->required_tradeskill)
	{
		if(!plr->_HasSkillLine(qst->required_tradeskill))
			return QMGR_QUEST_NOT_AVAILABLE;
		if(qst->required_tradeskill_value && plr->_GetSkillLineCurrent(qst->required_tradeskill) < qst->required_tradeskill_value)
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	// Check reputation
	if(qst->required_rep_faction && qst->required_rep_value)
		if(plr->GetStanding(qst->required_rep_faction) < (int32)qst->required_rep_value)
			return QMGR_QUEST_NOT_AVAILABLE;

	if(plr->HasFinishedQuest(qst->id) && !sQuestMgr.IsQuestRepeatable(qst) && !sQuestMgr.IsQuestDaily(qst))
		return QMGR_QUEST_NOT_AVAILABLE;

	// Check One of Quest Prequest
	bool questscompleted = false;
	if(!qst->quest_list.empty())
	{
		set<uint32>::iterator iter = qst->quest_list.begin();
		for(; iter != qst->quest_list.end(); ++iter)
		{
			Quest* questcheck = QuestStorage.LookupEntry((*iter));
			if(questcheck)
			{
				if(plr->HasFinishedQuest((*iter)))
				{
					questscompleted = true;
					break;
				}
			}
		}
		if(!questscompleted)   // If none of listed quests is done, next part isn't available.
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->required_quests[i] > 0 && !plr->HasFinishedQuest(qst->required_quests[i]))
		{
			return QMGR_QUEST_NOT_AVAILABLE;
		}
	}

	// check quest level
	if(static_cast< int32 >(plr->getLevel()) >= (qst->questlevel + 5) && (status != QMGR_QUEST_REPEATABLE))
		return QMGR_QUEST_CHAT;

	return status;
}

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, Quest* qst, uint8 type, bool skiplevelcheck)
{
	QuestLogEntry* qle;

	qle = plr->GetQuestLogForEntry(qst->id);

	if(!qle)
	{
		if(type & QUESTGIVER_QUEST_START)
		{
			return PlayerMeetsReqs(plr, qst, skiplevelcheck);
		}
	}
	else
	{
		if( qle->HasFailed() )
			return QMGR_QUEST_NOT_FINISHED;

		if(type & QUESTGIVER_QUEST_END)
		{
			if(!qle->CanBeFinished())
			{
				if(qst->is_repeatable)
				{
					return QMGR_QUEST_REPEATABLE;
				}
				else
				{
					return QMGR_QUEST_NOT_FINISHED;
				}
			}
			else
			{
				return QMGR_QUEST_FINISHED;
			}
		}
	}

	return QMGR_QUEST_NOT_AVAILABLE;
}

uint32 QuestMgr::CalcQuestStatus(Player* plr, uint32 qst)
{
	QuestLogEntry* qle = plr->GetQuestLogForEntry(qst);
	if(qle != NULL)
	{
		if(!qle->CanBeFinished())
		{
			return QMGR_QUEST_NOT_FINISHED;
		}
		else
		{
			return QMGR_QUEST_FINISHED;
		}
	}

	return QMGR_QUEST_NOT_AVAILABLE;
}


uint32 QuestMgr::CalcStatus(Object* quest_giver, Player* plr)
{
	uint32 status = QMGR_QUEST_NOT_AVAILABLE;
	std::list<QuestRelation*>::const_iterator itr;
	std::list<QuestRelation*>::const_iterator q_begin;
	std::list<QuestRelation*>::const_iterator q_end;
	bool bValid = false;

	if(quest_giver->IsGameObject())
	{
		bValid = TO< GameObject* >(quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = TO< GameObject* >(quest_giver)->QuestsBegin();
			q_end = TO< GameObject* >(quest_giver)->QuestsEnd();
		}
	}
	else if(quest_giver->IsCreature())
	{
		bValid = TO< Creature* >(quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = TO< Creature* >(quest_giver)->QuestsBegin();
			q_end = TO< Creature* >(quest_giver)->QuestsEnd();
		}
	}
	else if(quest_giver->IsItem())
	{
		if(TO< Item* >(quest_giver)->GetProto()->QuestId)
			bValid = true;
	}
	//This will be handled at quest share so nothing important as status
	else if(quest_giver->IsPlayer())
	{
		status = QMGR_QUEST_AVAILABLE;
	}

	if(!bValid)
	{
		//annoying message that is not needed since all objects don't exactly have quests
		//LOG_DEBUG("QUESTS: Warning, invalid NPC " I64FMT " specified for CalcStatus. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
		return status;
	}

	if(quest_giver->IsItem())
	{
		Quest* pQuest = QuestStorage.LookupEntry(TO< Item* >(quest_giver)->GetProto()->QuestId);
		QuestRelation qr;
		qr.qst = pQuest;
		qr.type = 1;

		uint32 tmp_status = CalcQuestStatus(quest_giver, plr, &qr);
		if(tmp_status > status)
			status = tmp_status;
	}

	for(itr = q_begin; itr != q_end; ++itr)
	{
		uint32 tmp_status = CalcQuestStatus(quest_giver, plr, *itr);	// save a call
		if(tmp_status > status)
			status = tmp_status;
	}

	return status;
}

uint32 QuestMgr::ActiveQuestsCount(Object* quest_giver, Player* plr)
{
	std::list<QuestRelation*>::const_iterator itr;
	map<uint32, uint8> tmp_map;
	uint32 questCount = 0;

	std::list<QuestRelation*>::const_iterator q_begin;
	std::list<QuestRelation*>::const_iterator q_end;
	bool bValid = false;

	if(quest_giver->IsGameObject())
	{
		bValid = TO< GameObject* >(quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = TO< GameObject* >(quest_giver)->QuestsBegin();
			q_end   = TO< GameObject* >(quest_giver)->QuestsEnd();

		}
	}
	else if(quest_giver->IsCreature())
	{
		bValid = TO< Creature* >(quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = TO< Creature* >(quest_giver)->QuestsBegin();
			q_end   = TO< Creature* >(quest_giver)->QuestsEnd();
		}
	}

	if(!bValid)
	{
		LOG_DEBUG("QUESTS: Warning, invalid NPC " I64FMT " specified for ActiveQuestsCount. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
		return 0;
	}

	for(itr = q_begin; itr != q_end; ++itr)
	{
		if(CalcQuestStatus(quest_giver, plr, *itr) >= QMGR_QUEST_CHAT)
		{
			if(tmp_map.find((*itr)->qst->id) == tmp_map.end())
			{
				tmp_map.insert(std::map<uint32, uint8>::value_type((*itr)->qst->id, 1));
				questCount++;
			}
		}
	}

	return questCount;
}

void QuestMgr::BuildOfferReward(WorldPacket* data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr)
{
	LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->id, language) : NULL;
	ItemPrototype* it;
	uint32 i = 0;
	data->SetOpcode(SMSG_QUESTGIVER_OFFER_REWARD);
	*data << uint64(qst_giver->GetGUID());
	*data << uint32(qst->id);

	if(lq)
	{
		*data << lq->Title;
		*data << lq->CompletionText;
	}
	else
	{
		*data << qst->title;
		*data << qst->completiontext;
	}

	//uint32 a = 0, b = 0, c = 1, d = 0, e = 1;

	*data << (qst->next_quest_id ? uint8(1) : uint8(0));  // next quest shit
	*data << qst->quest_flags;
	*data << qst->suggestedplayers;

	*data << qst->completionemotecount;
	for(i = 0; i < qst->completionemotecount; i++)
	{
		*data << qst->completionemote[i];
		*data << qst->completionemotedelay[i];
	}

	*data << qst->count_reward_choiceitem;
	if(qst->count_reward_choiceitem)
	{
		for(i = 0; i < 6; ++i)
		{
			if(qst->reward_choiceitem[i])
			{
				*data << qst->reward_choiceitem[i];
				*data << qst->reward_choiceitemcount[i];
				it = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[i]);
				*data << (it ? it->DisplayInfoID : uint32(0));
			}
		}
	}

	*data << qst->count_reward_item;
	if(qst->count_reward_item)
	{
		for(i = 0; i < 4; ++i)
		{
			if(qst->reward_item[i])
			{
				*data << qst->reward_item[i];
				*data << qst->reward_itemcount[i];
				it = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
				*data << (it ? it->DisplayInfoID : uint32(0));
			}
		}
	}

	*data << uint32(0);
	uint32 xp = 0;
	if(plr->getLevel() < plr->GetMaxLevel())
	{
		xp = float2int32(GenerateQuestXP(plr, qst) * sWorld.getRate(RATE_QUESTXP));
	}
	*data << uint32(xp); //VLack: The quest will give you this amount of XP

	*data << (qst->bonushonor * 10);
	*data << float(0);
	*data << uint32(0);
	*data << qst->reward_spell;
	*data << qst->effect_on_player;
	*data << qst->rewardtitleid;
	*data << qst->rewardtalents;
	*data << qst->bonusarenapoints;
	*data << uint32(0);
	for(i = 0; i < 5; ++i)              // reward factions ids
		*data << uint32(0);
	for(i = 0; i < 5; ++i)              // columnid in QuestFactionReward.dbc (zero based)?
		*data << uint32(0);
	for(i = 0; i < 5; ++i)              // reward reputation override?
		*data << uint32(0);
}

void QuestMgr::BuildQuestDetails(WorldPacket* data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr)
{
	LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->id, language) : NULL;
	std::map<uint32, uint8>::const_iterator itr;

	data->SetOpcode(SMSG_QUESTGIVER_QUEST_DETAILS);

	*data << qst_giver->GetGUID();			// npc guid
	*data << uint64(qst_giver->IsPlayer() ? qst_giver->GetGUID() : 0);						// (questsharer?) guid
	*data << qst->id;						// quest id

	if(lq)
	{
		*data << lq->Title;
		*data << lq->Details;
		*data << lq->Objectives;
	}
	else
	{
		*data <<  qst->title;
		*data <<  qst->details;
		*data <<  qst->objectives;
	}

	*data << uint8(1);						// Activate accept
	*data << qst->quest_flags;
	*data << qst->suggestedplayers;			// "Suggested players"
	*data << uint8(0);						// MANGOS: IsFinished? value is sent back to server in quest accept packet

	ItemPrototype* ip;
	uint32 i;

	*data << qst->count_reward_choiceitem;
	for(i = 0; i < 6; ++i)
	{
		if(!qst->reward_choiceitem[i])
			continue;

		*data << qst->reward_choiceitem[i];
		*data << qst->reward_choiceitemcount[i];
		ip = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[i]);
		*data << (ip ? ip->DisplayInfoID : uint32(0));

	}

	*data << qst->count_reward_item;
	for(i = 0; i < 4; ++i)
	{
		if(!qst->reward_item[i])
			continue;

		*data << qst->reward_item[i];
		*data << qst->reward_itemcount[i];
		ip = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
		*data << (ip ? ip->DisplayInfoID : uint32(0));
	}

	*data << GenerateRewardMoney(plr, qst);	// Money reward
	*data << uint32(0); //New 3.3 - this is the XP you'll see on the quest reward panel too, but I think it is fine not to show it, because it can change if the player levels up before completing the quest.
	*data << (qst->bonushonor * 10);					// Honor reward
	*data << float(0); //New 3.3
	*data << qst->reward_spell;					// this is the spell (id) the quest finisher teaches you, or the icon of the spell if effect_on_player is not 0
	*data << qst->effect_on_player;				// this is the spell (id) the quest finisher casts on you as a reward
	*data << qst->rewardtitleid;				// Title reward (ID)
	*data << qst->rewardtalents;				// Talent reward
	*data << qst->bonusarenapoints;				// Arena Points reward
	*data << uint32(0);							// new 3.3.0
	for(i = 0; i < 5; ++i)
		*data << uint32(0);
	for(i = 0; i < 5; ++i)
		*data << uint32(0);
	for(i = 0; i < 5; ++i)
		*data << uint32(0);


	*data << qst->detailemotecount;				// Amount of emotes (4?)
	for(i = 0; i < qst->detailemotecount; i++)
	{
		*data << qst->detailemote[i];			// Emote ID
		*data << qst->detailemotedelay[i];		// Emote Delay
	}
}

void QuestMgr::BuildRequestItems(WorldPacket* data, Quest* qst, Object* qst_giver, uint32 status, uint32 language)
{
	LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->id, language) : NULL;
	ItemPrototype* it;
	data->SetOpcode(SMSG_QUESTGIVER_REQUEST_ITEMS);

	*data << qst_giver->GetGUID();
	*data << qst->id;

	if(lq)
	{
		*data << lq->Title;
		*data << ((lq->IncompleteText[0]) ? lq->IncompleteText : lq->Details);
	}
	else
	{
		*data << qst->title;
		*data << (qst->incompletetext[0] ? qst->incompletetext : qst->details);
	}

	*data << uint32(0);

	if(status == QMGR_QUEST_NOT_FINISHED)
		*data << qst->incompleteemote;
	else
		*data << qst->completeemote;

	*data << uint32(0);
	*data << qst->quest_flags;
	*data << qst->suggestedplayers;
	*data << uint32(qst->reward_money < 0 ? -qst->reward_money : 0);	     // Required Money

	// item count
	*data << qst->count_required_item;

	// (loop for each item)
	for(uint32 i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
	{
		if(qst->required_item[i] != 0)
		{
			*data << qst->required_item[i];
			*data << qst->required_itemcount[i];
			it = ItemPrototypeStorage.LookupEntry(qst->required_item[i]);
			*data << (it ? it->DisplayInfoID : uint32(0));
		}
		else
		{
			*data << uint32(0);
			*data << uint32(0);
			*data << uint32(0);
		}
	}

	// wtf is this?
	if(status == QMGR_QUEST_NOT_FINISHED)
	{
		*data << uint32(0); //incomplete button
	}
	else
	{
		*data << uint32(3);
	}

	*data << uint32(4);
	*data << uint32(8);
	*data << uint32(10);
}

void QuestMgr::BuildQuestComplete(Player* plr, Quest* qst)
{
	uint32 xp ;
	uint32 currtalentpoints = plr->GetCurrentTalentPoints();
	uint32 rewardtalents = qst->rewardtalents;
	uint32 playerlevel = plr->getLevel();

	if(playerlevel >= plr->GetMaxLevel())
	{
		xp = 0;
	}
	else
	{
		xp = float2int32(GenerateQuestXP(plr, qst) * sWorld.getRate(RATE_QUESTXP));
		plr->GiveXP(xp, 0, false);
	}

	if( currtalentpoints <= ( playerlevel - 9 - rewardtalents ) )
		plr->AddTalentPointsToAllSpec( rewardtalents );

	// Reward title
	if(qst->rewardtitleid > 0)
		plr->SetKnownTitle(static_cast< RankTitles >(qst->rewardtitleid), true);

	WorldPacket data(SMSG_QUESTGIVER_QUEST_COMPLETE, 72);

	data << uint32(qst->id);
	data << uint32(xp);
	data << uint32(GenerateRewardMoney(plr, qst));
	data << uint32(qst->bonushonor * 10);
	data << uint32(rewardtalents);
	data << uint32(qst->bonusarenapoints);
	data << uint32(qst->count_reward_item);   //Reward item count

	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->reward_item[i])
		{
			data << uint32(qst->reward_item[i]);
			data << uint32(qst->reward_itemcount[i]);
		}
	}
	plr->SendPacket(&data);
}

void QuestMgr::BuildQuestList(WorldPacket* data, Object* qst_giver, Player* plr, uint32 language)
{
	if(!plr || !plr->GetSession()) return;
	uint32 status;
	list<QuestRelation*>::iterator it;
	list<QuestRelation*>::iterator st;
	list<QuestRelation*>::iterator ed;
	map<uint32, uint8> tmp_map;

	data->Initialize(SMSG_QUESTGIVER_QUEST_LIST);

	*data << qst_giver->GetGUID();
	*data << plr->GetSession()->LocalizedWorldSrv(70);//"How can I help you?"; //Hello line
	*data << uint32(1);//Emote Delay
	*data << uint32(1);//Emote

	bool bValid = false;
	if(qst_giver->IsGameObject())
	{
		bValid = TO< GameObject* >(qst_giver)->HasQuests();
		if(bValid)
		{
			st = TO< GameObject* >(qst_giver)->QuestsBegin();
			ed = TO< GameObject* >(qst_giver)->QuestsEnd();
		}
	}
	else if(qst_giver->IsCreature())
	{
		bValid = TO< Creature* >(qst_giver)->HasQuests();
		if(bValid)
		{
			st = TO< Creature* >(qst_giver)->QuestsBegin();
			ed = TO< Creature* >(qst_giver)->QuestsEnd();
		}
	}

	if(!bValid)
	{
		*data << uint8(0);
		return;
	}

	*data << uint8(sQuestMgr.ActiveQuestsCount(qst_giver, plr));

	for(it = st; it != ed; ++it)
	{
		status = sQuestMgr.CalcQuestStatus(qst_giver, plr, *it);
		if(status >= QMGR_QUEST_CHAT)
		{
			if(tmp_map.find((*it)->qst->id) == tmp_map.end())
			{
				tmp_map.insert(std::map<uint32, uint8>::value_type((*it)->qst->id, 1));
				LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest((*it)->qst->id, language) : NULL;

				*data << (*it)->qst->id;
				/**data << sQuestMgr.CalcQuestStatus(qst_giver, plr, *it);
				*data << uint32(0);*/

				switch(status)
				{
					case QMGR_QUEST_NOT_FINISHED:
						*data << uint32(4);
						break;

					case QMGR_QUEST_FINISHED:
						*data << uint32(4);
						break;

					case QMGR_QUEST_CHAT:
						*data << uint32(QMGR_QUEST_AVAILABLE);
						break;

					default:
						*data << status;
				}
				*data << int32((*it)->qst->questlevel);
				*data << uint32((*it)->qst->quest_flags);
				*data << uint8((*it)->qst->is_repeatable); // According to MANGOS: "changes icon: blue question or yellow exclamation"

				if(lq)
					*data << lq->Title;
				else
					*data << (*it)->qst->title;
			}
		}
	}
}

void QuestMgr::BuildQuestUpdateAddItem(WorldPacket* data, uint32 itemid, uint32 count)
{
	data->Initialize(SMSG_QUESTUPDATE_ADD_ITEM);
	*data << itemid << count;
}

void QuestMgr::SendQuestUpdateAddKill(Player* plr, uint32 questid, uint32 entry, uint32 count, uint32 tcount, uint64 guid)
{
	WorldPacket data(32);
	data.SetOpcode(SMSG_QUESTUPDATE_ADD_KILL);
	data << questid << entry << count << tcount << guid;
	plr->GetSession()->SendPacket(&data);
}

void QuestMgr::BuildQuestUpdateComplete(WorldPacket* data, Quest* qst)
{
	data->Initialize(SMSG_QUESTUPDATE_COMPLETE);

	*data << qst->id;
}

void QuestMgr::SendPushToPartyResponse(Player* plr, Player* pTarget, uint8 response)
{
	WorldPacket data(MSG_QUEST_PUSH_RESULT, 9);
	data << uint64(pTarget->GetGUID());
	data << uint8(response);
	plr->GetSession()->SendPacket(&data);
}

bool QuestMgr::OnGameObjectActivate(Player* plr, GameObject* go)
{
	uint32 i, j;
	QuestLogEntry* qle;
	uint32 entry = go->GetEntry();
	Quest* qst;

	for(i = 0; i < 25; ++i)
	{
		qle = plr->GetQuestLogInSlot(i);
		if(qle != NULL)
		{
			qst = qle->GetQuest();
			// don't waste time on quests without mobs
			if(qst->count_required_mob == 0)
				continue;

			for(j = 0; j < 4; ++j)
			{
				if(qst->required_mob[j] == static_cast<int32>(entry) &&
				        qst->required_mobtype[j] == QUEST_MOB_TYPE_GAMEOBJECT &&
				        qle->m_mobcount[j] < qst->required_mobcount[j])
				{
					// add another kill.
					// (auto-dirty's it)
					qle->IncrementMobCount(j);
					qle->SendUpdateAddKill(j);
					CALL_QUESTSCRIPT_EVENT(qle, OnGameObjectActivate)(entry, plr, qle);

					if(qle->CanBeFinished())
						qle->SendQuestComplete();

					qle->UpdatePlayerFields();
					return true;
				}
			}
		}
	}
	return false;
}

void QuestMgr::OnPlayerKill(Player* plr, Creature* victim, bool IsGroupKill)
{
	uint32 entry = victim->GetEntry();
	_OnPlayerKill(plr, entry, IsGroupKill);

	// Extra credit (yay we wont have to script this anymore) - Shauren
	for(uint8 i = 0; i < 2; ++i)
	{
		uint32 extracredit = victim->GetCreatureInfo()->killcredit[i];
		if(extracredit != 0)
		{
			if(CreatureNameStorage.LookupEntry(extracredit))
				_OnPlayerKill(plr, extracredit, IsGroupKill);
		}
	}
}

void QuestMgr::_OnPlayerKill(Player* plr, uint32 entry, bool IsGroupKill)
{
	if(!plr)
		return;

	uint32 i, j;
	QuestLogEntry* qle;
	Quest* qst;

	if(plr->HasQuestMob(entry))
	{
		for(i = 0; i < 25; ++i)
		{
			qle = plr->GetQuestLogInSlot(i);
			if(qle != NULL)
			{
				qst = qle->GetQuest();
				for(j = 0; j < 4; ++j)
				{
					if(qst->required_mob[j] == 0)
						continue;

					if(qst->required_mob[j] == static_cast<int32>(entry) &&
					        qst->required_mobtype[j] == QUEST_MOB_TYPE_CREATURE &&
					        qle->m_mobcount[j] < qst->required_mobcount[j])
					{
						// add another kill.(auto-dirty's it)
						qle->IncrementMobCount(j);
						qle->SendUpdateAddKill(j);
						CALL_QUESTSCRIPT_EVENT(qle, OnCreatureKill)(entry, plr, qle);
						qle->UpdatePlayerFields();
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
			}
		}
	}

	if(IsGroupKill)
	{
		// Shared kills
		Player* gplr = NULL;

		if(plr->InGroup())
		{
			if(Group* pGroup = plr->GetGroup())
			{
				//removed by Zack How the hell will healers get the kills then ?
				//if(pGroup->GetGroupType() != GROUP_TYPE_PARTY)
				//	return;  // Raid's don't get shared kills.

				GroupMembersSet::iterator gitr;
				pGroup->Lock();
				for(uint32 k = 0; k < pGroup->GetSubGroupCount(); k++)
				{
					for(gitr = pGroup->GetSubGroup(k)->GetGroupMembersBegin(); gitr != pGroup->GetSubGroup(k)->GetGroupMembersEnd(); ++gitr)
					{
						gplr = (*gitr)->m_loggedInPlayer;
						if(gplr && gplr != plr && plr->isInRange(gplr, 300) && gplr->HasQuestMob(entry)) // don't double kills also don't give kills to party members at another side of the world
						{
							for(i = 0; i < 25; ++i)
							{
								qle = gplr->GetQuestLogInSlot(i);
								if(qle != NULL)
								{
									qst = qle->GetQuest();
									for(j = 0; j < 4; ++j)
									{
										if(qst->required_mob[j] == 0)
											continue;

										if(qst->required_mob[j] == static_cast<int32>(entry) &&
										        qst->required_mobtype[j] == QUEST_MOB_TYPE_CREATURE &&
										        qle->m_mobcount[j] < qst->required_mobcount[j])
										{
											// add another kill.
											// (auto-dirty's it)
											qle->IncrementMobCount(j);
											qle->SendUpdateAddKill(j);
											CALL_QUESTSCRIPT_EVENT(qle, OnCreatureKill)(entry, plr, qle);
											qle->UpdatePlayerFields();

											if(qle->CanBeFinished())
												qle->SendQuestComplete();
											break;
										}
									}
								}
							}
						}
					}
				}
				pGroup->Unlock();
			}
		}
	}
}

void QuestMgr::OnPlayerCast(Player* plr, uint32 spellid, uint64 & victimguid)
{
	if(!plr || !plr->HasQuestSpell(spellid))
		return;

	Unit* victim = plr->GetMapMgr() ? plr->GetMapMgr()->GetUnit(victimguid) : NULL;

	uint32 i, j;
	uint32 entry = (victim) ? victim->GetEntry() : 0;
	QuestLogEntry* qle;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i)) != 0)
		{
			// don't waste time on quests without casts
			if(!qle->IsCastQuest())
				continue;

			Quest* qst = qle->GetQuest();
			for(j = 0; j < 4; ++j)
			{
				if(qst->required_mob[j])
				{
					if(victim && qst->required_mob[j] == static_cast<int32>(entry) && qst->required_spell[j] == spellid && (qle->m_mobcount[j] < qst->required_mobcount[j] || qle->m_mobcount[j] == 0) && !qle->IsUnitAffected(victim))
					{
						qle->AddAffectedUnit(victim);
						qle->IncrementMobCount(j);
						qle->SendUpdateAddKill(j);
						qle->UpdatePlayerFields();
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
				// Some quests, like druid's Trial of the Lake (28/29), don't have a required target for spell cast
				else
				{
					if(qst->required_spell[j] == spellid)
					{
						qle->IncrementMobCount(j);
						qle->UpdatePlayerFields();
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
			}
		}
	}
}



void QuestMgr::OnPlayerItemPickup(Player* plr, Item* item)
{
	uint32 i, j;
	uint32 pcount;
	uint32 entry = item->GetEntry();
	QuestLogEntry* qle;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i))  != 0)
		{
			if(qle->GetQuest()->count_required_item == 0)
				continue;

			for(j = 0; j < MAX_REQUIRED_QUEST_ITEM; ++j)
			{
				if(qle->GetQuest()->required_item[j] == entry)
				{
					pcount = plr->GetItemInterface()->GetItemCount(entry, true);
					CALL_QUESTSCRIPT_EVENT(qle, OnPlayerItemPickup)(entry, pcount, plr, qle);
					if(pcount < qle->GetQuest()->required_itemcount[j])
					{
						WorldPacket data(8);
						data.SetOpcode(SMSG_QUESTUPDATE_ADD_ITEM);
						data << qle->GetQuest()->required_item[j] << uint32(1);
						plr->GetSession()->SendPacket(&data);
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
			}
		}
	}
}

void QuestMgr::OnPlayerExploreArea(Player* plr, uint32 AreaID)
{
	uint32 i, j;
	QuestLogEntry* qle;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i)) != 0)
		{
			// don't waste time on quests without triggers
			if(qle->GetQuest()->count_requiredtriggers == 0)
				continue;

			for(j = 0; j < 4; ++j)
			{
				if(qle->GetQuest()->required_triggers[j] == AreaID &&
				        !qle->m_explored_areas[j])
				{
					qle->SetTrigger(j);
					CALL_QUESTSCRIPT_EVENT(qle, OnExploreArea)(qle->m_explored_areas[j], plr, qle);
					qle->UpdatePlayerFields();
					if(qle->CanBeFinished())
						qle->SendQuestComplete();
					break;
				}
			}
		}
	}
}

void QuestMgr::AreaExplored(Player* plr, uint32 QuestID)
{
	uint32 i, j;
	QuestLogEntry* qle;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i)) != 0)
		{
			// search for quest
			if(qle->GetQuest()->id == QuestID)
				for(j = 0; j < 4; ++j)
				{
					if(qle->GetQuest()->required_triggers[j] &&
					        !qle->m_explored_areas[j])
					{
						qle->SetTrigger(j);
						CALL_QUESTSCRIPT_EVENT(qle, OnExploreArea)(qle->m_explored_areas[j], plr, qle);
						qle->UpdatePlayerFields();
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
		}
	}
}

void QuestMgr::GiveQuestRewardReputation(Player* plr, Quest* qst, Object* qst_giver)
{
	// Reputation reward
	for(int z = 0; z < 6; z++)
	{
		uint32 fact = 19;   // default to 19 if no factiondbc
		int32 amt  = float2int32(GenerateQuestXP(plr, qst) * 0.1f);      // guess
		if(!qst->reward_repfaction[z])
		{
			if(z >= 1)
				break;

			// Let's do this properly. Determine the faction of the creature, and give reputation to his faction.
			if(qst_giver->IsCreature())
				if(TO< Creature* >(qst_giver)->m_factionDBC != NULL)
					fact = TO< Creature* >(qst_giver)->m_factionDBC->ID;
			if(qst_giver->IsGameObject())
				fact = TO< GameObject* >(qst_giver)->GetFaction();
		}
		else
		{
			fact = qst->reward_repfaction[z];
			if(qst->reward_repvalue[z])
				amt = qst->reward_repvalue[z];
		}

		if(qst->reward_replimit)
			if(plr->GetStanding(fact) >= (int32)qst->reward_replimit)
				continue;

		amt = float2int32(amt * sWorld.getRate(RATE_QUESTREPUTATION));     // reputation rewards
		plr->ModStanding(fact, amt);
	}
}

void QuestMgr::OnQuestAccepted(Player* plr, Quest* qst, Object* qst_giver)
{

}

void QuestMgr::OnQuestFinished(Player* plr, Quest* qst, Object* qst_giver, uint32 reward_slot)
{
	//Re-Check for Gold Requirement (needed for possible xploit) - reward money < 0 means required money
	if(qst->reward_money < 0 && plr->GetGold() < uint32(-qst->reward_money))
		return;

	// Check they don't have more than the max gold
	if(sWorld.GoldCapEnabled && (plr->GetGold() + qst->reward_money) > sWorld.GoldLimit)
	{
		plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
		return;
	}

	QuestLogEntry* qle = NULL;
	qle = plr->GetQuestLogForEntry(qst->id);
	if(!qle)
		return;

	BuildQuestComplete(plr, qst);
	CALL_QUESTSCRIPT_EVENT(qle, OnQuestComplete)(plr, qle);
	for(uint32 x = 0; x < 4; x++)
	{
		if(qst->required_spell[x] != 0)
		{
			if(plr->HasQuestSpell(qst->required_spell[x]))
				plr->RemoveQuestSpell(qst->required_spell[x]);
		}
		else if(qst->required_mob[x] != 0)
		{
			if(plr->HasQuestMob(qst->required_mob[x]))
				plr->RemoveQuestMob(qst->required_mob[x]);
		}
	}
	qle->ClearAffectedUnits();
	qle->Finish();


	if(qst_giver->IsCreature())
	{
		if(!TO< Creature* >(qst_giver)->HasQuest(qst->id, 2))
		{
			//sCheatLog.writefromsession(plr->GetSession(), "tried to finish quest from invalid npc.");
			plr->GetSession()->Disconnect();
			return;
		}
	}

	//details: hmm as i can remember, repeatable quests give faction rep still after first completion
	if(IsQuestRepeatable(qst) || IsQuestDaily(qst))
	{
		// Reputation reward
		GiveQuestRewardReputation(plr, qst, qst_giver);
		// Static Item reward
		for(uint32 i = 0; i < 4; ++i)
		{
			if(qst->reward_item[i])
			{
				ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
				if(!proto)
				{
					LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
				}
				else
				{
					Item* add;
					SlotResult slotresult;
					add = plr->GetItemInterface()->FindItemLessMax(qst->reward_item[i], qst->reward_itemcount[i], false);
					if(!add)
					{
						slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
						if(!slotresult.Result)
						{
							plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						}
						else
						{
							Item* itm = objmgr.CreateItem(qst->reward_item[i], plr);
							if(itm == NULL)
								return;

							itm->SetStackCount(uint32(qst->reward_itemcount[i]));
							if(!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
								itm->DeleteMe();
						}
					}
					else
					{
						add->SetStackCount(add->GetStackCount() + qst->reward_itemcount[i]);
						add->m_isDirty = true;
					}
				}
			}
		}

		// Choice Rewards
		if(qst->reward_choiceitem[reward_slot])
		{
			ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
			if(!proto)
			{
				LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
			}
			else
			{
				Item* add;
				SlotResult slotresult;
				add = plr->GetItemInterface()->FindItemLessMax(qst->reward_choiceitem[reward_slot], qst->reward_choiceitemcount[reward_slot], false);
				if(!add)
				{
					slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					}
					else
					{
						Item* itm = objmgr.CreateItem(qst->reward_choiceitem[reward_slot], plr);
						if(itm == NULL)
							return;

						itm->SetStackCount(uint32(qst->reward_choiceitemcount[reward_slot]));
						if(!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
							itm->DeleteMe();

					}
				}
				else
				{
					add->SetStackCount(add->GetStackCount() + qst->reward_choiceitemcount[reward_slot]);
					add->m_isDirty = true;
				}
			}
		}

		// Remove items
		for(uint32 i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
		{
			if(qst->required_item[i]) plr->GetItemInterface()->RemoveItemAmt(qst->required_item[i], qst->required_itemcount[i]);
		}

		// Remove srcitem
		if(qst->srcitem && qst->srcitem != qst->receive_items[0])
			plr->GetItemInterface()->RemoveItemAmt(qst->srcitem, qst->srcitemcount ? qst->srcitemcount : 1);

		// cast Effect Spell
		if(qst->effect_on_player)
		{
			SpellEntry*   inf = dbcSpell.LookupEntryForced(qst->effect_on_player);
			if(inf)
			{
				Spell* spe = sSpellFactoryMgr.NewSpell(plr, inf, true, NULL);
				SpellCastTargets tgt;
				tgt.m_unitTarget = plr->GetGUID();
				spe->prepare(&tgt);
			}
		}

		plr->ModGold(GenerateRewardMoney(plr, qst));

		// if daily then append to finished dailies
		if(qst->is_repeatable == arcemu_QUEST_REPEATABLE_DAILY)
			plr->PushToFinishedDailies(qst->id);
	}
	else
	{
		plr->ModGold(GenerateRewardMoney(plr, qst));

		// Reputation reward
		GiveQuestRewardReputation(plr, qst, qst_giver);
		// Static Item reward
		for(uint32 i = 0; i < 4; ++i)
		{
			if(qst->reward_item[i])
			{
				ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
				if(!proto)
				{
					LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
				}
				else
				{
					Item* add;
					SlotResult slotresult;
					add = plr->GetItemInterface()->FindItemLessMax(qst->reward_item[i], qst->reward_itemcount[i], false);
					if(!add)
					{
						slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
						if(!slotresult.Result)
						{
							plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						}
						else
						{
							Item* itm = objmgr.CreateItem(qst->reward_item[i], plr);
							if(itm == NULL)
								return;

							itm->SetStackCount(uint32(qst->reward_itemcount[i]));
							if(!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
								itm->DeleteMe();
						}
					}
					else
					{
						add->SetStackCount(add->GetStackCount() + qst->reward_itemcount[i]);
						add->m_isDirty = true;
					}
				}
			}
		}

		// Choice Rewards
		if(qst->reward_choiceitem[reward_slot])
		{
			ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
			if(!proto)
			{
				LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
			}
			else
			{
				Item* add;
				SlotResult slotresult;
				add = plr->GetItemInterface()->FindItemLessMax(qst->reward_choiceitem[reward_slot], qst->reward_choiceitemcount[reward_slot], false);
				if(!add)
				{
					slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					}
					else
					{
						Item* itm = objmgr.CreateItem(qst->reward_choiceitem[reward_slot], plr);
						if(itm == NULL)
							return;

						itm->SetStackCount(uint32(qst->reward_choiceitemcount[reward_slot]));
						if(!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
							itm->DeleteMe();
					}
				}
				else
				{
					add->SetStackCount(add->GetStackCount() + qst->reward_choiceitemcount[reward_slot]);
					add->m_isDirty = true;
				}
			}
		}

		// Remove items
		for(uint32 i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
		{
			if(qst->required_item[i]) plr->GetItemInterface()->RemoveItemAmt(qst->required_item[i], qst->required_itemcount[i]);
		}

		// Remove srcitem
		if(qst->srcitem && qst->srcitem != qst->receive_items[0])
			plr->GetItemInterface()->RemoveItemAmt(qst->srcitem, qst->srcitemcount ? qst->srcitemcount : 1);

		// cast learning spell
		if(qst->reward_spell && !qst->effect_on_player) // qst->reward_spell is the spell the quest finisher teaches you, OR the icon of the spell if effect_on_player is not 0
		{
			if(!plr->HasSpell(qst->reward_spell))
			{
				// "Teaching" effect
				WorldPacket data(SMSG_SPELL_START, 42);
				data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
				data << uint32(7763);
				data << uint8(0);
				data << uint16(0);
				data << uint32(0);
				data << uint16(2);
				data << plr->GetGUID();
				plr->GetSession()->SendPacket(&data);

				data.Initialize(SMSG_SPELL_GO);
				data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
				data << uint32(7763);		   // spellID
				data << uint8(0) << uint8(1);   // flags
				data << uint8(1);			   // amount of targets
				data << plr->GetGUID();		 // target
				data << uint8(0);
				data << uint16(2);
				data << plr->GetGUID();
				plr->GetSession()->SendPacket(&data);

				// Teach the spell
				plr->addSpell(qst->reward_spell);
			}
		}

		// cast Effect Spell
		if(qst->effect_on_player)
		{
			SpellEntry*   inf = dbcSpell.LookupEntryForced(qst->effect_on_player);
			if(inf)
			{
				Spell* spe = sSpellFactoryMgr.NewSpell(plr, inf, true, NULL);
				SpellCastTargets tgt;
				tgt.m_unitTarget = plr->GetGUID();
				spe->prepare(&tgt);
			}
		}

		//Add to finished quests
		plr->AddToFinishedQuests(qst->id);
		if(qst->bonusarenapoints != 0)
		{
			plr->m_arenaPoints += qst->bonusarenapoints;
			plr->RecalculateHonor();
		}

#ifdef ENABLE_ACHIEVEMENTS
		plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT, 1, 0, 0);
		if(qst->reward_money)
			plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD, qst->reward_money, 0, 0);
		plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE, qst->zone_id, 0, 0);
		plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST, qst->id, 0, 0);
#endif
		// Remove quests that are listed to be removed on quest complete.
		set<uint32>::iterator iter = qst->remove_quest_list.begin();
		for(; iter != qst->remove_quest_list.end(); ++iter)
		{
			if(!plr->HasFinishedQuest((*iter)))
				plr->AddToFinishedQuests((*iter));
		}
	}

	if( qst->MailTemplateId != 0 )
	{
		MailTemplateEntry * mail = dbcMailTemplateEntry.LookupEntryForced( qst->MailTemplateId );
		if( mail != NULL )
		{
			int mailType = NORMAL;
			uint64 itemGuid = 0;
			if(qst_giver->IsCreature())
				mailType = CREATURE;
			else if(qst_giver->IsGameObject())
				mailType = GAMEOBJECT;

			if( qst->MailSendItem != 0 )
			{
				// the way it's done in World::PollMailboxInsertQueue
				Item * pItem = objmgr.CreateItem(qst->MailSendItem, NULL);
				if(pItem != NULL)
				{
					pItem->SetStackCount(1);
					pItem->SaveToDB(0, 0, true, NULL);
					itemGuid = pItem->GetGUID();
					pItem->DeleteMe();
				}
			}

			sMailSystem.SendAutomatedMessage( mailType, qst_giver->GetGUID(), plr->GetGUID(), mail->subject, mail->content, 0, 0, itemGuid, MAIL_STATIONERY_TEST1, MAIL_CHECK_MASK_HAS_BODY, qst->MailDelaySecs);
		}
	}
}

/////////////////////////////////////
//		Quest Management		 //
/////////////////////////////////////

void QuestMgr::LoadNPCQuests(Creature* qst_giver)
{
	qst_giver->SetQuestList(GetCreatureQuestList(qst_giver->GetEntry()));
}

void QuestMgr::LoadGOQuests(GameObject* go)
{
	go->SetQuestList(GetGOQuestList(go->GetEntry()));
}

QuestRelationList* QuestMgr::GetGOQuestList(uint32 entryid)
{
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* > &olist = _GetList<GameObject>();
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
	return (itr == olist.end()) ? 0 : itr->second;
}

QuestRelationList* QuestMgr::GetCreatureQuestList(uint32 entryid)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation*>* > &olist = _GetList<Creature>();
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
	return (itr == olist.end()) ? 0 : itr->second;
}

template <class T> void QuestMgr::_AddQuest(uint32 entryid, Quest* qst, uint8 type)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation*>* > &olist = _GetList<T>();
	std::list<QuestRelation*>* nlist;
	QuestRelation* ptr = NULL;

	if(olist.find(entryid) == olist.end())
	{
		nlist = new std::list<QuestRelation*>;

		olist.insert(HM_NAMESPACE::hash_map<uint32, list<QuestRelation*>* >::value_type(entryid, nlist));
	}
	else
	{
		nlist = olist.find(entryid)->second;
	}

	list<QuestRelation*>::iterator it;
	for(it = nlist->begin(); it != nlist->end(); ++it)
	{
		if((*it)->qst == qst)
		{
			ptr = (*it);
			break;
		}
	}

	if(ptr == NULL)
	{
		ptr = new QuestRelation;
		ptr->qst = qst;
		ptr->type = type;

		nlist->push_back(ptr);
	}
	else
	{
		ptr->type |= type;
	}
}



void QuestMgr::_CleanLine(std::string* str)
{
	_RemoveChar((char*)"\r", str);
	_RemoveChar((char*)"\n", str);

	while(str->c_str()[0] == 32)
	{
		str->erase(0, 1);
	}
}

void QuestMgr::_RemoveChar(char* c, std::string* str)
{
	string::size_type pos = str->find(c, 0);

	while(pos != string::npos)
	{
		str->erase(pos, 1);
		pos = str->find(c, 0);
	}
}

uint32 QuestMgr::GenerateQuestXP(Player* plr, Quest* qst)
{
	if(qst->is_repeatable != 0)
		return 0;

	// Leaving this for compatibility reason for the old system + custom quests ^^
	if(qst->reward_xp != 0)
	{
		float modifier = 0.0f;
		uint32 playerlevel = plr->getLevel();
		int32 questlevel = qst->questlevel;

		if(static_cast< int32 >(playerlevel) < (questlevel + 6))
			return qst->reward_xp;

		if(static_cast< int32 >(playerlevel) > (questlevel +  9))
			return 0;

		if(static_cast< int32 >(playerlevel) == (questlevel +  6))
			modifier = 0.8f;

		if(static_cast< int32 >(playerlevel) == (questlevel +  7))
			modifier = 0.6f;

		if(static_cast< int32 >(playerlevel) == (questlevel +  8))
			modifier = 0.4f;

		if(static_cast< int32 >(playerlevel) == (questlevel +  9))
			modifier = 0.2f;


		return static_cast< uint32 >(modifier * qst->reward_xp);

	}
	else
	{
		// new quest reward xp calculation mechanism based on DBC values + index taken from DB

		uint32 realXP = 0;
		uint32 xpMultiplier = 0;
		int32 baseLevel = 0;
		int32 playerLevel = plr->getLevel();
		int32 QuestLevel = qst->questlevel;

		if(QuestLevel != -1)
			baseLevel = QuestLevel;

		if(((baseLevel - playerLevel) + 10) * 2 > 10)
		{
			baseLevel = playerLevel;

			if(QuestLevel != -1)
				baseLevel = QuestLevel;

			if(((baseLevel - playerLevel) + 10) * 2 <= 10)
			{
				if(QuestLevel == -1)
					baseLevel = playerLevel;

				xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
			}
			else
			{
				xpMultiplier = 10;
			}
		}
		else
		{
			baseLevel = playerLevel;

			if(QuestLevel != -1)
				baseLevel = QuestLevel;

			if(((baseLevel - playerLevel) + 10) * 2 >= 1)
			{
				baseLevel = playerLevel;

				if(QuestLevel != -1)
					baseLevel = QuestLevel;

				if(((baseLevel - playerLevel) + 10) * 2 <= 10)
				{
					if(QuestLevel == -1)
						baseLevel = playerLevel;

					xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
				}
				else
				{
					xpMultiplier = 10;
				}
			}
			else
			{
				xpMultiplier = 1;
			}
		}

		if(const QuestXP* pXPData = dbcQuestXP.LookupEntry(baseLevel))
		{
			uint32 rawXP = xpMultiplier * pXPData->xpIndex[ qst->RewXPId ] / 10;

			realXP = static_cast< uint32 >(Arcemu::round(static_cast< double >(rawXP)));
		}
		return realXP;
	}
}

uint32 QuestMgr::GenerateRewardMoney(Player* plr, Quest* qst)
{
//	if ( plr == NULL || !plr->IsInWorld() || plr->getLevel() >= plr->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL) || ( !plr->GetSession()->HasFlag( ACCOUNT_FLAG_XPACK_01 ) && plr->getLevel() != 60 ) || plr->getLevel() != 70 || qst->is_repeatable != 0 )
	{
		return qst->reward_money;
	}
//	else
	{
//		return qst->reward_money + float2int32( GenerateQuestXP( plr, qst ) * sWorld.getRate( RATE_QUESTXP ) ) * 6;
	}
}
/*
#define XP_INC 50
#define XP_DEC 10
#define XP_INC100 15
#define XP_DEC100 5
	double xp, pxp, mxp, mmx;

	// hack fix
	xp  = qst->max_level * XP_INC;
	if(xp <= 0)
		xp = 1;

	pxp  = xp + (xp / 100) * XP_INC100;

	xp   = XP_DEC;

	mxp  = xp + (xp / 100) * XP_DEC100;

	mmx = (pxp - mxp);

	if(qst->quest_flags & QUEST_FLAG_SPEAKTO)
		mmx *= 0.6;
	if(qst->quest_flags & QUEST_FLAG_TIMED)
		mmx *= 1.1;
	if(qst->quest_flags & QUEST_FLAG_EXPLORATION)
		mmx *= 1.2;

	if(mmx < 0)
		return 1;

	mmx *= sWorld.getRate(RATE_QUESTXP);
	return (int)mmx;*/


void QuestMgr::SendQuestInvalid(INVALID_REASON reason, Player* plyr)
{
	if(!plyr)
		return;
	plyr->GetSession()->OutPacket(SMSG_QUESTGIVER_QUEST_INVALID, 4, &reason);

	LOG_DEBUG("WORLD:Sent SMSG_QUESTGIVER_QUEST_INVALID");
}

void QuestMgr::SendQuestFailed(FAILED_REASON failed, Quest* qst, Player* plyr)
{
	if(!plyr)
		return;

	WorldPacket data(8);
	data.Initialize(SMSG_QUESTGIVER_QUEST_FAILED);
	data << uint32(qst->id);
	data << failed;
	plyr->GetSession()->SendPacket(&data);
	LOG_DEBUG("WORLD:Sent SMSG_QUESTGIVER_QUEST_FAILED");
}

void QuestMgr::SendQuestUpdateFailedTimer(Quest* pQuest, Player* plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILEDTIMER, 4, &pQuest->id);
	LOG_DEBUG("WORLD:Sent SMSG_QUESTUPDATE_FAILEDTIMER");
}

void QuestMgr::SendQuestUpdateFailed(Quest* pQuest, Player* plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILED, 4, &pQuest->id);
	LOG_DEBUG("WORLD:Sent SMSG_QUESTUPDATE_FAILED");
}

void QuestMgr::SendQuestLogFull(Player* plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTLOG_FULL);
	LOG_DEBUG("WORLD:Sent QUEST_LOG_FULL_MESSAGE");
}

uint32 QuestMgr::GetGameObjectLootQuest(uint32 GO_Entry)
{
	HM_NAMESPACE::hash_map<uint32, uint32>::iterator itr = m_ObjectLootQuestList.find(GO_Entry);
	if(itr == m_ObjectLootQuestList.end()) return 0;

	return itr->second;
}

void QuestMgr::SetGameObjectLootQuest(uint32 GO_Entry, uint32 Item_Entry)
{
	if(m_ObjectLootQuestList.find(GO_Entry) != m_ObjectLootQuestList.end())
	{
		//LOG_ERROR("WARNING: Gameobject %d has more than 1 quest item allocated in it's loot template!", GO_Entry);
	}

	// Find the quest that has that item
	uint32 QuestID = 0;
	uint32 i;
	StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		Quest* qst = itr->Get();
		for(i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
		{
			if(qst->required_item[i] == Item_Entry)
			{
				QuestID = qst->id;
				m_ObjectLootQuestList[GO_Entry] = QuestID;
				itr->Destruct();
				return;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	//LOG_ERROR("WARNING: No coresponding quest was found for quest item %d", Item_Entry);
}

void QuestMgr::BuildQuestFailed(WorldPacket* data, uint32 questid)
{
	data->Initialize(SMSG_QUESTUPDATE_FAILEDTIMER);
	*data << questid;
}

bool QuestMgr::OnActivateQuestGiver(Object* qst_giver, Player* plr)
{
	if(qst_giver->IsGameObject() && !TO< GameObject* >(qst_giver)->HasQuests())
		return false;

	uint32 questCount = sQuestMgr.ActiveQuestsCount(qst_giver, plr);
	WorldPacket data(1004);

	if(questCount == 0)
	{
		LOG_DEBUG("WORLD: Invalid NPC for CMSG_QUESTGIVER_HELLO.");
		return false;
	}
	else if(questCount == 1)
	{
		std::list<QuestRelation*>::const_iterator itr;
		std::list<QuestRelation*>::const_iterator q_begin;
		std::list<QuestRelation*>::const_iterator q_end;

		bool bValid = false;

		if(qst_giver->IsGameObject())
		{
			bValid = TO< GameObject* >(qst_giver)->HasQuests();
			if(bValid)
			{
				q_begin = TO< GameObject* >(qst_giver)->QuestsBegin();
				q_end   = TO< GameObject* >(qst_giver)->QuestsEnd();
			}
		}
		else if(qst_giver->IsCreature())
		{
			bValid = TO< Creature* >(qst_giver)->HasQuests();
			if(bValid)
			{
				q_begin = TO< Creature* >(qst_giver)->QuestsBegin();
				q_end   = TO< Creature* >(qst_giver)->QuestsEnd();
			}
		}

		if(!bValid)
		{
			LOG_DEBUG("QUESTS: Warning, invalid NPC " I64FMT " specified for OnActivateQuestGiver. TypeId: %d.", qst_giver->GetGUID(), qst_giver->GetTypeId());
			return false;
		}

		for(itr = q_begin; itr != q_end; ++itr)
			if(sQuestMgr.CalcQuestStatus(qst_giver, plr, *itr) >= QMGR_QUEST_CHAT)
				break;

		if(sQuestMgr.CalcStatus(qst_giver, plr) < QMGR_QUEST_CHAT)
			return false;

		ARCEMU_ASSERT(itr != q_end);

		uint32 status = sQuestMgr.CalcStatus(qst_giver, plr);

		if((status == QMGR_QUEST_AVAILABLE) || (status == QMGR_QUEST_REPEATABLE) || (status == QMGR_QUEST_CHAT))
		{
			sQuestMgr.BuildQuestDetails(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr);		// 1 because we have 1 quest, and we want goodbye to function
			plr->GetSession()->SendPacket(&data);
			LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS.");

			if((*itr)->qst->HasFlag(QUEST_FLAGS_AUTO_ACCEPT))
				plr->AcceptQuest(qst_giver->GetGUID(), (*itr)->qst->id);
		}
		else if(status == QMGR_QUEST_FINISHED)
		{
			sQuestMgr.BuildOfferReward(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr);
			plr->GetSession()->SendPacket(&data);
			//ss
			LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD.");
		}
		else if(status == QMGR_QUEST_NOT_FINISHED)
		{
			sQuestMgr.BuildRequestItems(&data, (*itr)->qst, qst_giver, status, plr->GetSession()->language);
			plr->GetSession()->SendPacket(&data);
			LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS.");
		}
	}
	else
	{
		sQuestMgr.BuildQuestList(&data, qst_giver , plr, plr->GetSession()->language);
		plr->GetSession()->SendPacket(&data);
		LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST.");
	}
	return true;
}

QuestMgr::~QuestMgr()
{
	HM_NAMESPACE::hash_map<uint32, Quest*>::iterator itr1;
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation*>* >::iterator itr2;
	list<QuestRelation*>::iterator itr3;
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation*>* >::iterator itr4;
	list<QuestAssociation*>::iterator itr5;

	// clear relations
	for(itr2 = m_obj_quests.begin(); itr2 != m_obj_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete(*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}

	m_obj_quests.clear();

	for(itr2 = m_npc_quests.begin(); itr2 != m_npc_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete(*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}

	m_npc_quests.clear();

	for(itr2 = m_itm_quests.begin(); itr2 != m_itm_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete(*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_itm_quests.clear();
	for(itr4 = m_quest_associations.begin(); itr4 != m_quest_associations.end(); ++itr4)
	{
		if(!itr4->second)
			continue;

		itr5 = itr4->second->begin();
		for(; itr5 != itr4->second->end(); ++itr5)
		{
			delete(*itr5);
		}
		itr4->second->clear();
		delete itr4->second;
	}
	// NTY.
	m_quest_associations.clear();
}


bool QuestMgr::CanStoreReward(Player* plyr, Quest* qst, uint32 reward_slot)
{
	uint32 available_slots = 0;
	uint32 slotsrequired = 0;
	available_slots = plyr->GetItemInterface()->CalculateFreeSlots(NULL);
	// Static Item reward
	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->reward_item[i])
		{
			slotsrequired++;
			ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
			if(!proto)
				LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
			else if(plyr->GetItemInterface()->CanReceiveItem(proto, qst->reward_itemcount[i]))
				return false;
		}
	}

	// Choice Rewards
	if(qst->reward_choiceitem[reward_slot])
	{
		slotsrequired++;
		ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
		if(!proto)
			LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
		else if(plyr->GetItemInterface()->CanReceiveItem(proto, qst->reward_choiceitemcount[reward_slot]))
			return false;
	}
	if(available_slots < slotsrequired)
	{
		return false;
	}

	return true;
}

void QuestMgr::LoadExtraQuestStuff()
{
	StorageContainerIterator<Quest> * it = QuestStorage.MakeIterator();
	Quest* qst;
	while(!it->AtEnd())
	{
		qst = it->Get();

		// 0 them out
		qst->count_required_item = 0;
		qst->count_required_mob = 0;
		qst->count_requiredtriggers = 0;
		qst->count_receiveitems = 0;
		qst->count_reward_item = 0;
		qst->count_reward_choiceitem = 0;

		qst->required_mobtype[0] = 0;
		qst->required_mobtype[1] = 0;
		qst->required_mobtype[2] = 0;
		qst->required_mobtype[3] = 0;

		qst->count_requiredquests = 0;

		if(it->Get()->x_or_y_quest_string)
		{
			it->Get()->quest_list.clear();
			string quests = string(it->Get()->x_or_y_quest_string);
			vector<string> qsts = StrSplit(quests, " ");
			for(vector<string>::iterator iter = qsts.begin(); iter != qsts.end(); ++iter)
			{
				uint32 id = atol((*iter).c_str());
				if(id)
					it->Get()->quest_list.insert(id);
			}
		}

		if(it->Get()->remove_quests)
		{
			string quests = string(it->Get()->remove_quests);
			vector<string> qsts = StrSplit(quests, " ");
			for(vector<string>::iterator iter = qsts.begin(); iter != qsts.end(); ++iter)
			{
				uint32 id = atol((*iter).c_str());
				if(id)
					it->Get()->remove_quest_list.insert(id);
			}
		}

		for(int i = 0 ; i < 4; ++i)
		{
			if(qst->required_mob[i] != 0)
			{
				if(qst->required_mob[i] < 0)
				{
					GameObjectInfo* go_info = GameObjectNameStorage.LookupEntry(qst->required_mob[i] * -1);
					if(go_info)
					{
						qst->required_mobtype[i] = QUEST_MOB_TYPE_GAMEOBJECT;
						qst->required_mob[i] *= -1;
					}
					else
					{
						// if quest has neither valid gameobject, log it.
						LOG_DEBUG("Quest %lu has required_mobtype[%d]==%lu, it's not a valid GameObject.", qst->id, i, qst->required_mob[i]);
					}
				}
				else
				{
					CreatureInfo*   c_info  = CreatureNameStorage.LookupEntry(qst->required_mob[i]);
					if(c_info)
						qst->required_mobtype[i] = QUEST_MOB_TYPE_CREATURE;
					else
					{
						// if quest has neither valid creature, log it.
						LOG_DEBUG("Quest %lu has required_mobtype[%d]==%lu, it's not a valid Creature.", qst->id, i, qst->required_mob[i]);
					}
				}

				qst->count_required_mob++;
			}

			if(qst->reward_item[i])
				qst->count_reward_item++;

			if(qst->required_triggers[i])
				qst->count_requiredtriggers++;

			if(qst->receive_items[i])
				qst->count_receiveitems++;

			if(qst->required_quests[i])
				qst->count_requiredquests++;
		}

		for( int i = 0; i < MAX_REQUIRED_QUEST_ITEM; i++ )
			if( qst->required_item[ i ] != 0 )
				qst->count_required_item++;

		for(int i = 0; i < 6; ++i)
		{
			if(qst->reward_choiceitem[i])
				qst->count_reward_choiceitem++;
		}

		qst->pQuestScript = NULL;

		if(!it->Inc())
			break;
	}

	it->Destruct();

	// load creature starters
	uint32 creature, quest;

	QueryResult* pResult = WorldDatabase.Query("SELECT * FROM creature_quest_starter");
	uint32 pos = 0;
	uint32 total;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do
		{
			Field* data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				Log.Error("ObjectMgr", "Tried to add starter to npc %d for non-existent quest %d.", creature, quest);
			}
			else
			{
				_AddQuest<Creature>(creature, qst, 1);  // 1 = starter
			}
		}
		while(pResult->NextRow());
		delete pResult;
	}

	pResult = WorldDatabase.Query("SELECT * FROM creature_quest_finisher");
	pos = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do
		{
			Field* data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				Log.Error("ObjectMgr", "Tried to add finisher to npc %d for non-existent quest %d.", creature, quest);
			}
			else
			{
				_AddQuest<Creature>(creature, qst, 2);  // 1 = starter
			}
		}
		while(pResult->NextRow());
		delete pResult;
	}

	pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_starter");
	pos = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do
		{
			Field* data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				Log.Error("ObjectMgr", "Tried to add starter to go %d for non-existent quest %d.", creature, quest);
			}
			else
			{
				_AddQuest<GameObject>(creature, qst, 1);  // 1 = starter
			}
		}
		while(pResult->NextRow());
		delete pResult;
	}

	pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_finisher");
	pos = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do
		{
			Field* data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				Log.Error("ObjectMgr", "Tried to add finisher to go %d for non-existent quest %d.", creature, quest);
			}
			else
			{
				_AddQuest<GameObject>(creature, qst, 2);  // 2 = finish
			}
		}
		while(pResult->NextRow());
		delete pResult;
	}
	objmgr.ProcessGameobjectQuests();

	//load item quest associations
	uint32 item;
	uint8 item_count;

	pResult = WorldDatabase.Query("SELECT * FROM item_quest_association");
	pos = 0;
	if(pResult != NULL)
	{
		total = pResult->GetRowCount();
		do
		{
			Field* data = pResult->Fetch();
			item = data[0].GetUInt32();
			quest = data[1].GetUInt32();
			item_count = data[2].GetUInt8();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				Log.Error("ObjectMgr", "Tried to add association to item %d for non-existent quest %d.", item, quest);
			}
			else
			{
				AddItemQuestAssociation(item, qst, item_count);
			}
		}
		while(pResult->NextRow());
		delete pResult;
	}

	m_QuestPOIMap.clear();

	QueryResult* result = WorldDatabase.Query("SELECT questId, poiId, objIndex, mapId, mapAreaId, floorId, unk3, unk4 FROM quest_poi");
	if(result != NULL)
	{
		uint32 count = 0;

		do
		{

			Field* fields = result->Fetch();

			uint32 questId          = fields[ 0 ].GetUInt32();
			uint32 poiId            = fields[ 1 ].GetUInt32();
			int32  objIndex         = fields[ 2 ].GetInt32();
			uint32 mapId            = fields[ 3 ].GetUInt32();
			uint32 mapAreaId        = fields[ 4 ].GetUInt32();
			uint32 floorId          = fields[ 5 ].GetUInt32();
			uint32 unk3             = fields[ 6 ].GetUInt32();
			uint32 unk4             = fields[ 7 ].GetUInt32();

			QuestPOI POI(poiId, objIndex, mapId, mapAreaId, floorId, unk3, unk4);

			m_QuestPOIMap[ questId ].push_back(POI);

			count++;

		}
		while(result->NextRow());

		delete result;

		Log.Notice("QuestMgr", "Point Of Interest (POI) data loaded for %u quests.", count);



		QueryResult* points = WorldDatabase.Query("SELECT questId, poiId, x, y FROM quest_poi_points");
		if(points != NULL)
		{
			count = 0;

			do
			{

				Field* pointFields  = points->Fetch();

				uint32 questId      = pointFields[ 0 ].GetUInt32();
				uint32 poiId        = pointFields[ 1 ].GetUInt32();
				int32  x            = pointFields[ 2 ].GetInt32();
				int32  y            = pointFields[ 3 ].GetInt32();

				QuestPOIVector & vect = m_QuestPOIMap[ questId ];

				for(QuestPOIVector::iterator itr = vect.begin(); itr != vect.end(); ++itr)
				{

					if(itr->PoiId != poiId)
						continue;

					QuestPOIPoint point(x, y);

					itr->points.push_back(point);

					break;
				}

				count++;

			}
			while(points->NextRow());

			delete points;
			Log.Success("QuestMgr", "%u quest Point Of Interest points loaded.", count);
		}

	}
}

void QuestMgr::AddItemQuestAssociation(uint32 itemId, Quest* qst, uint8 item_count)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation*>* > &associationList = GetQuestAssociationList();
	std::list<QuestAssociation*>* tempList;
	QuestAssociation* ptr = NULL;

	// look for the item in the associationList
	if(associationList.find(itemId) == associationList.end())
	{
		// not found. Create a new entry and QuestAssociationList
		tempList = new std::list<QuestAssociation*>;

		associationList.insert(HM_NAMESPACE::hash_map<uint32, list<QuestAssociation*>* >::value_type(itemId, tempList));
	}
	else
	{
		// item found, now we'll search through its QuestAssociationList
		tempList = associationList.find(itemId)->second;
	}

	// look through this item's QuestAssociationList for a matching quest entry
	list<QuestAssociation*>::iterator it;
	for(it = tempList->begin(); it != tempList->end(); ++it)
	{
		if((*it)->qst == qst)
		{
			// matching quest found
			ptr = (*it);
			break;
		}
	}

	// did we find a matching quest?
	if(ptr == NULL)
	{
		// nope, create a new QuestAssociation for this item and quest
		ptr = new QuestAssociation;
		ptr->qst = qst;
		ptr->item_count = item_count;

		tempList->push_back(ptr);
	}
	else
	{
		// yep, update the QuestAssociation with the new item_count information
		ptr->item_count = item_count;
		LOG_DEBUG("WARNING: Duplicate entries found in item_quest_association, updating item #%d with new item_count: %d.", itemId, item_count);
	}
}

QuestAssociationList* QuestMgr::GetQuestAssociationListForItemId(uint32 itemId)
{
	HM_NAMESPACE::hash_map<uint32, QuestAssociationList* > &associationList = GetQuestAssociationList();
	HM_NAMESPACE::hash_map<uint32, QuestAssociationList* >::iterator itr = associationList.find(itemId);
	if(itr == associationList.end())
	{
		return 0;
	}
	else
	{
		return itr->second;
	}
}

void QuestMgr::OnPlayerEmote(Player* plr, uint32 emoteid, uint64 & victimguid)
{
	if(!plr || !emoteid || !victimguid)
		return;

	Unit* victim = plr->GetMapMgr() ? plr->GetMapMgr()->GetUnit(victimguid) : NULL;

	uint32 i, j;
	uint32 entry = (victim) ? victim->GetEntry() : 0;
	QuestLogEntry* qle;
	for(i = 0; i < 25; ++i)
	{
		if((qle = plr->GetQuestLogInSlot(i)) != 0)
		{
			// dont waste time on quests without emotes
			if(!qle->IsEmoteQuest())
			{
				continue;
			}

			Quest* qst = qle->GetQuest();
			for(j = 0; j < 4; ++j)
			{
				if(qst->required_mob[j])
				{
					if(victim && qst->required_mob[j] == static_cast<int32>(entry) && qst->required_emote[j] == emoteid && (qle->m_mobcount[j] < qst->required_mobcount[j] || qle->m_mobcount[j] == 0) && !qle->IsUnitAffected(victim))
					{
						qle->AddAffectedUnit(victim);
						qle->IncrementMobCount(j);
						if(qst->id == 11224)   // Show progress for quest "Send Them Packing"
						{
							qle->SendUpdateAddKill(j);
						}
						qle->UpdatePlayerFields();
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
				// in case some quest doesn't have a required target for the emote..
				else
				{
					if(qst->required_emote[j] == emoteid)
					{
						qle->IncrementMobCount(j);
						qle->UpdatePlayerFields();
						if(qle->CanBeFinished())
							qle->SendQuestComplete();
						break;
					}
				}
			}
		}
	}
}

void QuestMgr::BuildQuestPOIResponse(WorldPacket & data, uint32 questid)
{
	Quest* q = QuestStorage.LookupEntry(questid);

	if(q != NULL)
	{
		QuestPOIVector const* POI = NULL;

		QuestPOIMap::iterator itr = m_QuestPOIMap.find(questid);

		if(itr != m_QuestPOIMap.end())
			POI = &(itr->second);

		if(POI != NULL)
		{

			data << uint32(questid);
			data << uint32(POI->size());

			for(QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
			{

				data << uint32(itr->PoiId);
				data << int32(itr->ObjectiveIndex);
				data << uint32(itr->MapId);
				data << uint32(itr->MapAreaId);
				data << uint32(itr->FloorId);
				data << uint32(itr->Unk3);
				data << uint32(itr->Unk4);
				data << uint32(itr->points.size());

				for(std::vector< QuestPOIPoint >::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
				{

					data << int32(itr2->x);
					data << int32(itr2->y);
				}
			}

		}
		else
		{
			data << uint32(questid);
			data << uint32(0);
		}

	}
	else
	{
		data << uint32(questid);
		data << uint32(0);
	}
}

void QuestMgr::FillQuestMenu(Creature* giver, Player* plr, Arcemu::Gossip::Menu & menu)
{
	uint32 status;
	uint8 icon;
	if(giver->isQuestGiver() && giver->HasQuests())
	{
		for(std::list<QuestRelation*>::iterator itr = giver->QuestsBegin(); itr != giver->QuestsEnd(); ++itr)
		{
			status = sQuestMgr.CalcQuestStatus(giver, plr, *itr);
			if(status >= QMGR_QUEST_CHAT)
			{
				switch(status)
				{
					case QMGR_QUEST_NOT_FINISHED:
						icon = QMGR_QUEST_REPEATABLE_LOWLEVEL;
						break;

					case QMGR_QUEST_FINISHED:
						icon = QMGR_QUEST_REPEATABLE_LOWLEVEL;
						break;

					case QMGR_QUEST_CHAT:
						icon = QMGR_QUEST_AVAILABLE;
						break;

					default:
						icon = (uint8)status;
						break;
				}
				menu.AddQuest((*itr)->qst, icon);
			}
		}
	}
}
