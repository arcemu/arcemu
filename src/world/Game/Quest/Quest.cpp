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

//Pakcet Building
/////////////////

WorldPacket* WorldSession::BuildQuestQueryResponse(Quest* qst)
{
	// 2048 bytes should be more than enough. The fields cost ~200 bytes.
	// better to allocate more at startup than have to realloc the buffer later on.

	WorldPacket* data = new WorldPacket(SMSG_QUEST_QUERY_RESPONSE, 248);
	LocalizedQuest* lci = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->id, language) : NULL;
	uint32 i;

	*data << uint32(qst->id);						// Quest ID
	*data << uint32(2);								// Unknown, always seems to be 2
	*data << int32(qst->questlevel);				// Quest level
	*data << uint32(qst->min_level);				// Quest required level

	if(qst->quest_sort > 0)
		*data << int32(-(int32)qst->quest_sort);	// Negative if pointing to a sort.
	else
		*data << uint32(qst->zone_id);				// Positive if pointing to a zone.

	*data << uint32(qst->type);						// Info ID / Type
	*data << qst->suggestedplayers;					// suggested players
	*data << uint32(qst->required_rep_faction);		// Faction ID
	*data << uint32(qst->required_rep_value);		// Faction Amount
	*data << uint32(0);								// Unknown (always 0)
	*data << uint32(0);								// Unknown (always 0)
	*data << uint32(qst->next_quest_id);			// Next Quest ID
	*data << uint32(0);								// Column id +1 from QuestXp.dbc, entry is quest level
	*data << uint32(sQuestMgr.GenerateRewardMoney(_player, qst));	// Copper reward
	*data << uint32(qst->reward_money < 0 ? -qst->reward_money : 0);		// Required Money
	*data << uint32(qst->reward_spell);                                 // Spell added to spellbook upon completion
	*data << uint32(qst->effect_on_player);			// Spell casted on player upon completion
	*data << uint32(qst->bonushonor);				// 2.3.0 - bonus honor
	*data << float(0);								// 3.3.0 - some multiplier for honor
	*data << uint32(qst->srcitem);					// Item given at the start of a quest (srcitem)
	*data << uint32(qst->quest_flags);				// Quest Flags
	*data << qst->rewardtitleid;					// 2.4.0 unk
	*data << uint32(0);								// playerkillcount
	*data << qst->rewardtalents;
	*data << uint32(0);								// 3.3.0 Unknown
	*data << uint32(0);								// 3.3.0 Unknown

	// (loop 4 times)
	for(i = 0; i < MAX_REWARD_ITEMS; ++i)
	{
		*data << qst->reward_item[i];				// Forced Reward Item [i]
		*data << qst->reward_itemcount[i];			// Forced Reward Item Count [i]
	}

	// (loop 6 times)
	for(i = 0; i < MAX_REWARD_CHOICE_ITEMS; ++i)
	{
		*data << qst->reward_choiceitem[i];			// Choice Reward Item [i]
		*data << qst->reward_choiceitemcount[i];	// Choice Reward Item Count [i]
	}

	// (loop 5 times) - these 3 loops are here to allow displaying rep rewards in client (not handled in core yet)
	for(i = 0; i < MAX_REWARD_REPFACTION; ++i)
	{
		*data << uint32(qst->reward_repfaction[i]);	// reward factions ids
	}

	for(i = 0; i < MAX_REWARD_REPFACTION; ++i)
	{
		*data << uint32(0);							// column index in QuestFactionReward.dbc but use unknown
	}

	for(i = 0; i < 5; ++i)					// Unknown
	{
		*data << uint32(0);
	}

	*data << qst->point_mapid;						// Unknown
	*data << qst->point_x;							// Unknown
	*data << qst->point_y;							// Unknown
	*data << qst->point_opt;						// Unknown

	if(lci)
	{
		*data << lci->Title;
		*data << lci->Objectives;
		*data << lci->Details;
		*data << lci->EndText;
		*data << uint8(0);
	}
	else
	{
		*data << qst->title;						// Title / name of quest
		*data << qst->objectives;					// Objectives / description
		*data << qst->details;						// Details
		*data << qst->eventobjective;				// Describes the event that should be completed ( e.g.: Scourge Leader identified )
		*data << qst->objectivescompletetext;		// Shown when the quest objectives have been completed and the quest ready to be turned in. ( e.g.: Return to xyz )
	}

	for(i = 0; i < MAX_REQUIRED_MOBS; ++i)
	{
		*data << qst->required_mob[i];				// Kill mob entry ID [i]
		*data << qst->required_mobcount[i];			// Kill mob count [i]
		*data << uint32(0);							// Unknown
		*data << uint32(0);							// 3.3.0 Unknown
	}

	for(i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
	{
		*data << qst->required_item[i];				// Collect item [i]
		*data << qst->required_itemcount[i];		// Collect item count [i]
	}


	for( i = 0; i < MAX_OBJECTIVE_TEXTS; ++i )
	{
		if( lci != NULL )
		{
			*data << lci->ObjectiveText[ i ];
		}
		else
		{
			*data << qst->objectivetexts[ i ];
		}
	}

	return data;
}


/*****************
* QuestLogEntry *
*****************/
QuestLogEntry::QuestLogEntry()
{
	mInitialized = false;
	m_quest = NULL;
	mDirty = false;
	m_slot = -1;
	completed = 0;
}

QuestLogEntry::~QuestLogEntry()
{

}

void QuestLogEntry::Init(Quest* quest, Player* plr, uint32 slot)
{
	ARCEMU_ASSERT(quest != NULL);
	ARCEMU_ASSERT(plr != NULL);

	m_quest = quest;
	m_plr = plr;
	m_slot = slot;

	iscastquest = false;
	isemotequest = false;

	for( int i = 0; i < MAX_REQUIRED_SPELLS; i++ )
	{
		if(quest->required_spell[i] != 0)
		{
			iscastquest = true;
			if(!plr->HasQuestSpell(quest->required_spell[i]))
				plr->quest_spells.insert(quest->required_spell[i]);
		}
	}

	for( int i = 0; i < MAX_REQUIRED_EMOTES; i++ )
	{
		if(quest->required_emote[i] != 0)
		{
			isemotequest = true;
		}
	}

	for( int i = 0; i < MAX_REQUIRED_MOBS; i++ )
	{
		if(quest->required_mob[i] != 0)
		{
			if(!plr->HasQuestMob(quest->required_mob[i]))
				plr->quest_mobs.insert(quest->required_mob[i]);
		}
	}

	// update slot
	plr->SetQuestLogSlot(this, slot);

	mDirty = true;

	for( int i = 0; i < MAX_REQUIRED_MOBS; i++ )
		m_mobcount[ i ] = 0;

	for( int i = 0; i < MAX_REQUIRED_TRIGGERS; i++ )
		m_explored_areas[ i ] = 0;

	if( m_quest->time > 0 )
		expirytime = UNIXTIME + m_quest->time / 1000;
	else
		expirytime = 0;

	if(!plr->GetSession()->m_loggingInPlayer)  //quest script should not be called on login
		CALL_QUESTSCRIPT_EVENT(this, OnQuestStart)(plr, this);
}

void QuestLogEntry::ClearAffectedUnits()
{
	if(m_affected_units.size() > 0)
		m_affected_units.clear();
}
void QuestLogEntry::AddAffectedUnit(Unit* target)
{
	if(!target)
		return;
	if(!IsUnitAffected(target))
		m_affected_units.insert(target->GetGUID());
}
bool QuestLogEntry::IsUnitAffected(Unit* target)
{
	if(!target)
		return true;
	if(m_affected_units.find(target->GetGUID()) != m_affected_units.end())
		return true;
	return false;
}

void QuestLogEntry::SaveToDB(QueryBuffer* buf)
{
	ARCEMU_ASSERT(m_slot != -1);
	if(!mDirty)
		return;

	std::stringstream ss;

	ss << "DELETE FROM questlog WHERE player_guid = ";
	ss << m_plr->GetLowGUID();
	ss << " AND quest_id = ";
	ss << m_quest->id;
	ss << ";";

	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());

	ss.rdbuf()->str("");

	ss << "INSERT INTO questlog VALUES(";
	ss << m_plr->GetLowGUID() << "," << m_quest->id << "," << m_slot << "," << expirytime;
	for(int i = 0; i < 4; ++i)
		ss << "," << m_explored_areas[i];

	for(int i = 0; i < 4; ++i)
		ss << "," << m_mobcount[i];

	ss << "," << uint32(completed);

	ss << ")";

	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

bool QuestLogEntry::LoadFromDB(Field* fields)
{
	// playerguid,questid,timeleft,area0,area1,area2,area3,kill0,kill1,kill2,kill3
	int f = 3;
	ARCEMU_ASSERT(m_plr && m_quest);
	expirytime = fields[f].GetUInt32();
	f++;
	for(int i = 0; i < MAX_REQUIRED_TRIGGERS; ++i)
	{
		m_explored_areas[i] = fields[f].GetUInt32();
		f++;
		CALL_QUESTSCRIPT_EVENT(this, OnExploreArea)(m_quest->required_triggers[i], m_plr, this);
	}

	for(int i = 0; i < MAX_REQUIRED_MOBS; ++i)
	{
		m_mobcount[i] = fields[f].GetUInt32();
		f++;
		if(GetQuest()->required_mobtype[i] == QUEST_MOB_TYPE_CREATURE)
		{
			CALL_QUESTSCRIPT_EVENT(this, OnCreatureKill)(GetQuest()->required_mob[i], m_plr, this);
		}
		else
		{
			CALL_QUESTSCRIPT_EVENT(this, OnGameObjectActivate)(GetQuest()->required_mob[i], m_plr, this);
		}
	}

	completed = fields[f].GetUInt32();

	mDirty = false;
	return true;
}

bool QuestLogEntry::CanBeFinished()
{
	uint32 i;

	if(m_quest->iscompletedbyspelleffect && ( completed == QUEST_INCOMPLETE ) )
		return false;

	if( completed == QUEST_FAILED )
		return false;
	else
	if( completed == QUEST_COMPLETE )
		return true;


	for(i = 0; i < MAX_REQUIRED_MOBS; ++i)
	{
		if(m_quest->required_mob[i] != 0)
		{
			if(m_mobcount[i] < m_quest->required_mobcount[i])
			{
				return false;
			}
		}
	}

	for( i = 0; i < MAX_REQUIRED_SPELLS; i++ )
	{
		if(m_quest->required_spell[i] != 0)   // requires spell cast, with no required target
		{
			if(m_mobcount[i] == 0 || m_mobcount[i] < m_quest->required_mobcount[i])
			{
				return false;
			}
		}
	}

	for( i = 0; i < MAX_REQUIRED_EMOTES; i++ )
	{
		if(m_quest->required_emote[i] != 0)   // requires emote, with no required target
		{
			if(m_mobcount[i] == 0 || m_mobcount[i] < m_quest->required_mobcount[i])
			{
				return false;
			}
		}
	}

	for(i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
	{
		if(m_quest->required_item[i])
		{
			if(m_plr->GetItemInterface()->GetItemCount(m_quest->required_item[i]) < m_quest->required_itemcount[i])
			{
				return false;
			}
		}
	}

	//Check for Gold & AreaTrigger Requirements
	if(m_quest->reward_money < 0 && m_plr->GetGold() < uint32(-m_quest->reward_money))
		return false;

	for(i = 0; i < MAX_REQUIRED_TRIGGERS; ++i)
	{
		if(m_quest->required_triggers[i])
		{
			if(m_explored_areas[i] == 0)
				return false;
		}
	}

	return true;
}

void QuestLogEntry::SetMobCount(uint32 i, uint32 count)
{
	ARCEMU_ASSERT(i < MAX_REQUIRED_MOBS);
	m_mobcount[i] = count;
	mDirty = true;
}

void QuestLogEntry::IncrementMobCount(uint32 i)
{
	ARCEMU_ASSERT(i < MAX_REQUIRED_MOBS);
	++m_mobcount[i];
	mDirty = true;
}

void QuestLogEntry::SetTrigger(uint32 i)
{
	ARCEMU_ASSERT(i < MAX_REQUIRED_TRIGGERS);
	m_explored_areas[i] = 1;
	mDirty = true;
}

void QuestLogEntry::SetSlot(int32 i)
{
	ARCEMU_ASSERT(i != -1);
	m_slot = i;
}

void QuestLogEntry::Finish()
{
	sEventMgr.RemoveEvents( m_plr, EVENT_TIMED_QUEST_EXPIRE );

	uint32 base = GetBaseField(m_slot);
	m_plr->SetUInt32Value(base + QUEST_FIELD_OFFSET_ID, 0);
	m_plr->SetUInt32Value(base + QUEST_FIELD_OFFSET_STATE, QUEST_STATE_NONE);
	m_plr->SetUInt64Value(base + QUEST_FIELD_OFFSET_COUNTS, 0);
	m_plr->SetUInt32Value(base + QUEST_FIELD_OFFSET_TIMER, 0);

	// clear from player log
	m_plr->SetQuestLogSlot(NULL, m_slot);
	m_plr->PushToRemovedQuests(m_quest->id);
	m_plr->UpdateNearbyGameObjects();
	// delete ourselves

	delete this;
}

void QuestLogEntry::MarkObjectiveComplete( const unsigned int objective, uint64 guid )
{
	SetMobCount( objective, GetQuest()->required_mobcount[ objective ] );
	SendUpdateAddKill( objective, guid );
	
	if( CanBeFinished() )
	{
		SendQuestComplete();
	}

	UpdatePlayerFields();
}

void QuestLogEntry::Fail( bool timerexpired ){
	sEventMgr.RemoveEvents( m_plr, EVENT_TIMED_QUEST_EXPIRE );

	completed = QUEST_FAILED;
	expirytime = 0;
	mDirty = true;
	
	uint32 base = GetBaseField( m_slot );
	m_plr->SetUInt32Value( base + QUEST_FIELD_OFFSET_STATE, QUEST_STATE_FAILED );

	if( timerexpired )
		sQuestMgr.SendQuestUpdateFailedTimer( m_quest, m_plr );
	else
		sQuestMgr.SendQuestUpdateFailed( m_quest, m_plr );
}

void QuestLogEntry::UpdatePlayerFields()
{
	if(!m_plr)
		return;

	uint32 base = GetBaseField(m_slot);
	uint32 state = 0;
	uint64 counts = 0;
	uint8* p = (uint8*)&counts;

	/// Assume the quest is complete, try to prove otherwise
	bool objectivesComplete = true;

	for( int i = 0; i < MAX_REQUIRED_TRIGGERS; i++ )
	{
		if( m_quest->required_triggers[ i ] != 0 )
		{
			if( m_explored_areas[ i ] != 1 )
			{
				objectivesComplete = false;
			}
		}
	}

	for( int i = 0; i < MAX_REQUIRED_SPELLS; i++ )
	{
		if( m_quest->required_spell[ i ] != 0 )
		{
			p[2 * i] |= (uint8)m_mobcount[ i ];

			if( m_quest->required_mobcount[ i ] > m_mobcount[ i ] )
			{
				objectivesComplete = false;
			}
		}
	}

	for( int i = 0; i < MAX_REQUIRED_EMOTES; i++ )
	{
		if( m_quest->required_emote[ i ] != 0 )
		{
			p[2 * i] |= (uint8)m_mobcount[ i ];

			if( m_quest->required_mobcount[ i ] > m_mobcount[ i ] )
			{
				objectivesComplete = false;
			}
		}
	}

	for( int i = 0; i < MAX_REQUIRED_MOBS; i++ )
	{
		if( m_quest->required_mob[ i ] != 0 )
		{
			p[2 * i] |= (uint8)m_mobcount[ i ];

			if( m_quest->required_mobcount[ i ] > m_mobcount[ i ] )
			{
				objectivesComplete = false;
			}
		}
	}
	
	if( ( m_quest->time != 0 ) && ( expirytime < UNIXTIME ) )
	{
		state = QUEST_STATE_FAILED;
	}
	else
	{
		if( objectivesComplete )
		{
			state = QUEST_STATE_COMPLETE;
		}
	}

	m_plr->SetUInt32Value(base + QUEST_FIELD_OFFSET_ID, m_quest->id);
	m_plr->SetUInt32Value(base + QUEST_FIELD_OFFSET_STATE, state);
	m_plr->SetUInt64Value(base + QUEST_FIELD_OFFSET_COUNTS, counts);

	if( ( m_quest->time != 0 ) && ( completed != QUEST_FAILED ) ){
		m_plr->SetUInt32Value( base + QUEST_FIELD_OFFSET_TIMER, expirytime );
		sEventMgr.AddEvent( m_plr, &Player::EventTimedQuestExpire, m_quest->id, EVENT_TIMED_QUEST_EXPIRE, ( expirytime - UNIXTIME ) * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	}else
		m_plr->SetUInt32Value( base + QUEST_FIELD_OFFSET_TIMER, 0 );
}

void QuestLogEntry::SendQuestComplete()
{
	WorldPacket data(4);
	data.SetOpcode(SMSG_QUESTUPDATE_COMPLETE);
	data << m_quest->id;
	m_plr->GetSession()->SendPacket(&data);
	m_plr->UpdateNearbyGameObjects();
	CALL_QUESTSCRIPT_EVENT(this, OnQuestComplete)(m_plr, this);
}

void QuestLogEntry::SendUpdateAddKill(uint32 i, uint64 guid)
{
	sQuestMgr.SendQuestUpdateAddKill(m_plr, m_quest->id, m_quest->required_mob[i], m_mobcount[i], m_quest->required_mobcount[i], guid);
}

void QuestLogEntry::Complete()
{
	completed = QUEST_COMPLETE;
}

