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

/* Achievement Working List:

	- ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL
	- ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM
	- ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM // Partial - need also to check item's that player's have already.

*/

#include "StdAfx.h"

AchievementMgr::AchievementMgr(Player *player)
{
	m_player = player;
}

AchievementMgr::~AchievementMgr()
{
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
		delete iter->second;
	m_criteriaProgress.clear();
} 
void AchievementMgr::SaveToDB()
{
	if(!m_completedAchievements.empty())
	{
		std::ostringstream ss;
		ss << "REPLACE INTO character_achievement (guid, achievement, date) VALUES ";
		bool first = true;
		for(CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); iter++)
		{
			if (ss.str().length() >= 16000)
			{
				// SQL query length is limited to 16384 characters
				CharacterDatabase.Query( ss.str().c_str() );
				ss.str("");
				ss << "REPLACE INTO character_achievement (guid, achievement, date) VALUES ";
				first = true;
			}
			if(!first)
				ss << ", ";
			else
				first = false;
			ss << "("<<GetPlayer()->GetUInt32Value(OBJECT_FIELD_GUID) << ", " << iter->first << ", " << iter->second << ")";
		}
		CharacterDatabase.Query( ss.str().c_str() );
	}
 
	if(!m_criteriaProgress.empty())
	{
		std::ostringstream ss;
		ss << "REPLACE INTO character_achievement_progress (guid, criteria, counter, date) VALUES ";
		bool first = true;
		for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
		{
			if (ss.str().length() >= 16000)
			{
				// SQL query length is limited to 16384 characters
				CharacterDatabase.Query( ss.str().c_str() );
				ss.str("");
				ss << "REPLACE INTO character_achievement_progress (guid, criteria, counter, date) VALUES ";
				first = true;
			}
			if(!first)
				ss << ", ";
			else
				first = false;
			ss << "(" << GetPlayer()->GetUInt32Value(OBJECT_FIELD_GUID) << ", " << iter->first << ", " << iter->second->counter << ", " << iter->second->date << ")";
		}
		CharacterDatabase.Query( ss.str().c_str() );
	}
} 

void AchievementMgr::LoadFromDB(QueryResult *achievementResult, QueryResult *criteriaResult)
{
	if(achievementResult)
	{
		do
		{
			Field *fields = achievementResult->Fetch();
			m_completedAchievements[fields[0].GetUInt32()] = fields[1].GetUInt32();
		} while(achievementResult->NextRow());
		delete achievementResult;
	}
 
	if(criteriaResult)
	{
		do
		{
			Field *fields = criteriaResult->Fetch();
			CriteriaProgress *progress = new CriteriaProgress(fields[0].GetUInt32(), fields[1].GetUInt32(), fields[2].GetUInt64());
			m_criteriaProgress[progress->id] = progress;
		} while(criteriaResult->NextRow());
		delete criteriaResult;
	} 
} 
void AchievementMgr::SendAchievementEarned(AchievementEntry const* achievement)
{
	const char *msg = "|Hplayer:$N|h[$N]|h has earned the achievement $a!";

	if(GetPlayer()->IsInGuild())
	{
		Guild* guild = objmgr.GetGuildByGuildName(GetPlayer()->m_playerInfo->guild->GetGuildName());
		WorldPacket data(SMSG_MESSAGECHAT, 200);
		data << uint8(CHAT_MSG_ACHIEVEMENT);
		data << uint8(CHAT_MSG_GUILD_ACHIEVEMENT);
		data << uint32(LANG_UNIVERSAL);
		data << GetPlayer()->GetNewGUID();
		data << uint32(5);
		data << GetPlayer()->GetNewGUID();
		data << uint32(strlen(msg)+1);
		data << msg;
		data << uint8(0);
		data << uint32(achievement->ID);
		GetPlayer()->GetSession()->SendPacket(&data);     
	}
	if(achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_KILL|ACHIEVEMENT_FLAG_REALM_FIRST_REACH))
	{
		WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, strlen(GetPlayer()->GetName())+1+8+4+4);
		data << GetPlayer()->GetName();
		data << GetPlayer()->GetNewGUID();
		data << uint32(achievement->ID);
		data << uint32(0);
		GetPlayer()->GetSession()->SendPacket(&data);
	}
	else
	{
		WorldPacket data(SMSG_MESSAGECHAT, 200);
		data << uint8(CHAT_MSG_ACHIEVEMENT);
		data << uint32(LANG_UNIVERSAL);
		data << GetPlayer()->GetNewGUID();
		data << uint32(5 );
		data << GetPlayer()->GetNewGUID();
		data << uint32(strlen(msg)+1);
		data << msg;
		data << uint8(0);
		data << uint32(achievement->ID);
		GetPlayer()->GetSession()->SendPacket(&data);
 	}

	WorldPacket data( SMSG_ACHIEVEMENT_EARNED, 30);
	data << GetPlayer()->GetNewGUID();      
	data << uint32( achievement->ID );
	data << uint32( secsToTimeBitFields(UNIXTIME) );
	data << uint32(0);
	GetPlayer()->GetSession()->SendPacket(&data);
}

void AchievementMgr::SendCriteriaUpdate(CriteriaProgress *progress)
{
	WorldPacket data(SMSG_CRITERIA_UPDATE, 32);
	data << uint32(progress->id);

	data.appendPackGUID(progress->counter);

	data << GetPlayer()->GetNewGUID();   
	data << uint32(0);
	data << uint32(secsToTimeBitFields(progress->date));
	data << uint32(0);  // timer 1
	data << uint32(0);  // timer 2
	GetPlayer()->GetSession()->SendPacket(&data);
}
void AchievementMgr::CheckAllAchievementCriteria()
{
	for(uint32 i=0; i<ACHIEVEMENT_CRITERIA_TYPE_TOTAL; i++)
		UpdateAchievementCriteria(AchievementCriteriaTypes(i));
}

void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscvalue1, uint32 miscvalue2, uint32 time)
{
	AchievementCriteriaEntryList const& achievementCriteriaList = objmgr.GetAchievementCriteriaByType(type);
	for(AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i!=achievementCriteriaList.end(); ++i)
	{
		AchievementCriteriaEntry const *achievementCriteria = (*i);

		if(IsCompletedCriteria(achievementCriteria))
			continue;

		if((achievementCriteria->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP) && GetPlayer()->GetGroup())
			continue;

		AchievementEntry const *achievement = dbcAchievementStore.LookupEntry(achievementCriteria->referredAchievement);
		if(!achievement)
			continue;

		if(achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE && GetPlayer()->GetTeam() != 1 ||
			achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && GetPlayer()->GetTeam() != 0 )
			continue;

		switch (type)
		{
		//Start of Achievement List
			case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
				SetCriteriaProgress(achievementCriteria, GetPlayer()->getLevel());
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
			case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
				if (achievementCriteria->loot_item.itemID == miscvalue1)
					UpdateCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
				if (GetPlayer()->HasOverlayUncovered(achievementCriteria->explore_area.areaReference))
					SetCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
				UpdateCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
				if(achievementCriteria->complete_quests_in_zone.zoneID == miscvalue1)
					UpdateCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
				if(achievementCriteria->complete_quest.questID == miscvalue1)
					UpdateCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
				UpdateCriteriaProgress(achievementCriteria, miscvalue1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
				UpdateCriteriaProgress(achievementCriteria, miscvalue1);
				break;
			//End of Achievement List
			default:
				return;
		}
		if(IsCompletedCriteria(achievementCriteria))
			CompletedCriteria(achievementCriteria);
	}
}

void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type)
{
	AchievementCriteriaEntryList const& achievementCriteriaList = objmgr.GetAchievementCriteriaByType(type);
	for(AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i!=achievementCriteriaList.end(); ++i)
	{
		AchievementCriteriaEntry const *achievementCriteria = (*i);

		AchievementEntry const *achievement = dbcAchievementStore.LookupEntry(achievementCriteria->referredAchievement);
		if(!achievement)
			continue;

		if(IsCompletedCriteria(achievementCriteria))
			continue;

		if(achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE && GetPlayer()->GetTeam() != 1 ||
			achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && GetPlayer()->GetTeam() != 0 )
			continue;

		switch (type)
		{
		//Start of Achievement List
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
			SetCriteriaProgress(achievementCriteria, GetPlayer()->getLevel());
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
			if (GetPlayer()->HasOverlayUncovered(achievementCriteria->explore_area.areaReference))
				SetCriteriaProgress(achievementCriteria, 1);
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
			if (m_completedAchievements.find(achievementCriteria->complete_achievement.linkedAchievement) != m_completedAchievements.end())
				SetCriteriaProgress(achievementCriteria, 1);
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
			SetCriteriaProgress(achievementCriteria, (uint32)GetPlayer()->m_finishedQuests.size());
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
			{
				uint32 qcinzone = 0;
				set<uint32>::iterator qc = GetPlayer()->m_finishedQuests.begin();
				for(; qc!=GetPlayer()->m_finishedQuests.end(); ++qc)
				{
					QueryResult* result = WorldDatabase.Query("SELECT ZoneId FROM quests WHERE entry=%u;", (*qc));
					if(result)
					{
						Field* field = result->Fetch();
						if(field->GetUInt32() == achievementCriteria->complete_quests_in_zone.zoneID)
						{
							++qcinzone;
						}
					}
					delete result;
				}
				SetCriteriaProgress(achievementCriteria, qcinzone);
			}
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
			{
				//uint32 completed = achievementCriteria->complete_quest.questCount;
				uint32 completed = 0;
				set<uint32>::iterator it = GetPlayer()->m_finishedQuests.find(achievementCriteria->complete_quest.questID);
				if( it!=GetPlayer()->m_finishedQuests.end() )
				{
					++completed;
				}
				SetCriteriaProgress(achievementCriteria, completed);
			}
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
			{
				uint32 qrmoney = 0;
				set<uint32>::iterator fq = GetPlayer()->m_finishedQuests.begin();
				for(; fq!=GetPlayer()->m_finishedQuests.end(); ++fq)
				{
					QueryResult* result = WorldDatabase.Query("SELECT RewMoney FROM quests WHERE entry=%u;", (*fq));
					if(result)
					{
						Field* field = result->Fetch();
						qrmoney += field->GetUInt32();
					}
					delete result;
				}
				SetCriteriaProgress(achievementCriteria, qrmoney);
			}
			break;
			//End of Achievement List
		}
		if(IsCompletedCriteria(achievementCriteria))
			CompletedCriteria(achievementCriteria);
	}
}


bool AchievementMgr::IsCompletedCriteria(AchievementCriteriaEntry const* achievementCriteria)
{
	AchievementEntry const* achievement = dbcAchievementStore.LookupEntry(achievementCriteria->referredAchievement);
	if(!achievement)
	{
		return false;
	}

	if(achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
	{
		return false;
	}

	if(achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
	{
		if(objmgr.allCompletedAchievements.find(achievement->ID)!=objmgr.allCompletedAchievements.end())
			return false;
	}

	CriteriaProgressMap::iterator itr = m_criteriaProgress.find(achievementCriteria->ID);
	if(itr == m_criteriaProgress.end())
	{
		return false;
	}

	CriteriaProgress *progress = itr->second;

	switch(achievementCriteria->requiredType)
	{
	//Start of Achievement List
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
			if(achievement->ID == 467 && GetPlayer()->getClass() != SHAMAN ||
				achievement->ID == 466 && GetPlayer()->getClass() != DRUID ||
				achievement->ID == 465 && GetPlayer()->getClass() != PALADIN ||
				achievement->ID == 464 && GetPlayer()->getClass() != PRIEST ||
				achievement->ID == 463 && GetPlayer()->getClass() != WARLOCK ||
				achievement->ID == 462 && GetPlayer()->getClass() != HUNTER ||
				achievement->ID == 461 && GetPlayer()->getClass() != DEATHKNIGHT ||
				achievement->ID == 460 && GetPlayer()->getClass() != MAGE ||
				achievement->ID == 459 && GetPlayer()->getClass() != WARRIOR ||
				achievement->ID == 458 && GetPlayer()->getClass() != ROGUE ||
				achievement->ID == 1404 && GetPlayer()->getRace() != RACE_GNOME ||
				achievement->ID == 1405 && GetPlayer()->getRace() != RACE_BLOODELF ||
				achievement->ID == 1406 && GetPlayer()->getRace() != RACE_DRAENEI ||
				achievement->ID == 1407 && GetPlayer()->getRace() != RACE_DWARF ||
				achievement->ID == 1408 && GetPlayer()->getRace() != RACE_HUMAN ||
				achievement->ID == 1409 && GetPlayer()->getRace() != RACE_NIGHTELF ||
				achievement->ID == 1410 && GetPlayer()->getRace() != RACE_ORC ||
				achievement->ID == 1411 && GetPlayer()->getRace() != RACE_TAUREN ||
				achievement->ID == 1412 && GetPlayer()->getRace() != RACE_TROLL ||
				achievement->ID == 1413 && GetPlayer()->getRace() != RACE_UNDEAD )
				return false;
			return progress->counter >= achievementCriteria->reach_level.level;
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
		case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
			return progress->counter >= achievementCriteria->loot_item.itemCount;
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
			return progress->counter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
			return progress->counter >= achievementCriteria->loot_money.goldInCopper;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
			return progress->counter >= achievementCriteria->complete_quest_count.totalQuestCount;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
			return progress->counter >= achievementCriteria->complete_quests_in_zone.questCount;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
			return progress->counter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
			return progress->counter >= achievementCriteria->quest_reward_money.goldInCopper;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
			return m_completedAchievements.find(achievementCriteria->complete_achievement.linkedAchievement) != m_completedAchievements.end();
		//End of Achievement List
	}
	return false;
}

void AchievementMgr::CompletedCriteria(AchievementCriteriaEntry const* criteria)
{
	AchievementEntry const* achievement = dbcAchievementStore.LookupEntry(criteria->referredAchievement);
	if(!achievement)
	{
		return;
	}

	if(achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
	{
		return;
	}

	if(criteria->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL || GetAchievementCompletionState(achievement)==ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED)
	{
		CompletedAchievement(achievement);
	}
}

AchievementCompletionState AchievementMgr::GetAchievementCompletionState(AchievementEntry const* entry)
{
	if(m_completedAchievements.find(entry->ID)!=m_completedAchievements.end())
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_STORED;
	}

	bool foundOutstanding = false;
	for ( uint32 rowId = 0; rowId<dbcAchievementCriteriaStore.GetNumRows(); ++rowId )
	{
		AchievementCriteriaEntry const* criteria = dbcAchievementCriteriaStore.LookupRow(rowId);
		if( !criteria || criteria->referredAchievement!= entry->ID )
		{
			continue;
		}

		if( IsCompletedCriteria(criteria) && criteria->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL )
		{
			return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
		}

		if( !IsCompletedCriteria(criteria) )
		{
			foundOutstanding = true;
		}
	}
	if( foundOutstanding )
	{
		return ACHIEVEMENT_COMPLETED_NONE;
	}
	else
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
	}
}

void AchievementMgr::SetCriteriaProgress(AchievementCriteriaEntry const* entry, uint32 newValue, bool relative)
{
	CriteriaProgress *progress = NULL;

	if( m_criteriaProgress.find(entry->ID) == m_criteriaProgress.end() )
	{
		progress = new CriteriaProgress(entry->ID, newValue);
		m_criteriaProgress[entry->ID]=progress;
	}
	else
	{
		progress = m_criteriaProgress[entry->ID];
			if(progress->counter == newValue)
				return;
		progress->counter = newValue;
	}
	SendCriteriaUpdate( progress );
}

void AchievementMgr::UpdateCriteriaProgress(AchievementCriteriaEntry const* entry, uint32 updateByValue)
{
	CriteriaProgress *progress = NULL;

	if( m_criteriaProgress.find(entry->ID) == m_criteriaProgress.end() )
	{
		progress = new CriteriaProgress(entry->ID, updateByValue);
		m_criteriaProgress[entry->ID]=progress;
	}
	else
	{
		progress = m_criteriaProgress[entry->ID];
		progress->counter += updateByValue;
	}
	SendCriteriaUpdate( progress );
}

void AchievementMgr::CompletedAchievement(AchievementEntry const* achievement)
{
	if( achievement->flags & ACHIEVEMENT_FLAG_COUNTER || m_completedAchievements.find(achievement->ID)!=m_completedAchievements.end() )
	{
		return;
	}

	SendAchievementEarned( achievement );
	m_completedAchievements[achievement->ID] = time(NULL);

	if( ! ( achievement->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL ) )
	{
		objmgr.allCompletedAchievements.insert( achievement->ID );
		UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT );
	}
}

void AchievementMgr::SendAllAchievementData(Player* player)
{
	WorldPacket data( SMSG_ALL_ACHIEVEMENT_DATA,4*3+m_completedAchievements.size()*4*2+m_criteriaProgress.size()*7*4 );
	BuildAllDataPacket(&data);
	player->GetSession()->SendPacket(&data);
}

void AchievementMgr::SendRespondInspectAchievements(Player* player)
{
	WorldPacket data( SMSG_ALL_ACHIEVEMENT_DATA,4+4*3+m_completedAchievements.size()*4*2+m_criteriaProgress.size()*7*4 );
	data.append(GetPlayer()->GetGUID());
	BuildAllDataPacket(&data);
	player->GetSession()->SendPacket(&data);
}

void AchievementMgr::BuildAllDataPacket(WorldPacket *data)
{
	for( CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); ++iter )
	{
		*data << uint32(iter->first);
		*data << uint32(secsToTimeBitFields(iter->second));
	}
	*data << int32(-1);
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
	{
		*data << uint32(iter->second->id);
		data->appendPackGUID(iter->second->counter);
		*data << GetPlayer()->GetNewGUID(); 
		*data << uint32(0);
		*data << uint32(secsToTimeBitFields(iter->second->date));
		*data << uint32(0);
		*data << uint32(0);
	}
	*data << int32(-1);
}
