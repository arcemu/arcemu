/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

/*
	Achievement Working List:
	- ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL
	- ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM
	- ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM
	- ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA
	- ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY
	- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT
	- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE
	- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST
	- ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD
	- ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION
	- ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION
	- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT
	- ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL
	- ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE
	- ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL
	- ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL
	- ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM
	- ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM
	- ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS
	- ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE (partial)
	- ACHIEVEMENT_CRITERIA_TYPE_KILLING_BLOW (most)
	- ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE
	- ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM

	Achievement Rewards Working List:
	- Titles
	- Spells
	- Items (goes to inventory?)
	As long as the achievement type is handled, the reward -should- work too.

	What's Not Working Yet:
	- Several achievement types
	- Time limits on achievements
	- Realm-First achievements (everyone gets them)
*/

#include "StdAfx.h"

bool SendAchievementProgress(const CriteriaProgress* c)
{
	if(c->counter <= 0) // achievement not started yet, don't send progress
		return false;
	AchievementCriteriaEntry const* acEntry = dbcAchievementCriteriaStore.LookupEntry(c->id);
	if(acEntry->requiredType==ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION) // Exalted with X faction (don't send 12323/42000 progress, it's not shown anyway)
		return false;
	if(acEntry->requiredType==ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL) // Reach level (don't send 7/80 progress, it's not shown anyway)
		return false;
	return true;
}

bool IsStatisticAchievement(const AchievementEntry* a)
{
	switch(a->categoryId)
	{
		case    21: // Total deaths from other players, Total deaths from opposite faction
		case   122: // Total deaths
		case   125: // Total raid and dungeon deaths (and for each dungeon/raid type)
		case   128: // Total kills, Total kills that grant experience or honor
		case   131: // Total facepalms, etc. (Social)
		case   136: // Total Honorable Kills, World Honorable Kills, Continent with the most Honorable Kills, ...
		case   137: // Total Killing Blows, World Killing Blows, Continent with the most Killing Blows, ...
		case   140: // Total gold acquired, Average gold earned per day, Gold looted, Gold from quest rewards, ...
		case   141: // Total damage done, Total damage received, Total healing done, Total healing received, Largest hit dealt, ...
		case   147: // Total factions encountered, Most factions at Exalted, Most factions at Revered or higher, ...
		case 14807: // Total 5-player dungeons entered, Total 10-player raids entered, Total 25-player raids entered, ...
			return true;
		default:
			break;
	}
	return false;
}

bool SaveAchievementProgressToDB(const CriteriaProgress* c)
{
	if(c->counter <= 0) // don't save it if it's not started yet
		return false;
	AchievementCriteriaEntry const* acEntry = dbcAchievementCriteriaStore.LookupEntry(c->id);
	switch(acEntry->requiredType)
	{
		// these get updated when character logs on, don't save to character progress db
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
		case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
			return false;
		default:
			break;
	}
	return true;
}

AchievementMgr::AchievementMgr(Player *player)
:
m_player(player)
{

}

AchievementMgr::~AchievementMgr()
{
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
		delete iter->second;
	m_criteriaProgress.clear();
	m_completedAchievements.clear();
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
			if(SaveAchievementProgressToDB(iter->second)) // only save some progresses, others will be updated when character logs in
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
		}
		if(!first) // don't execute query if there's no entries to save
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
		//Guild* guild = objmgr.GetGuildByGuildName(GetPlayer()->m_playerInfo->guild->GetGuildName());
		Guild * guild = GetPlayer()->getPlayerInfo()->guild;
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

void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type, int32 miscvalue1, int32 miscvalue2, uint32 time)
{
	uint64 selectedGUID;
	if(type==ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE)
	{
		selectedGUID = GetPlayer()->GetSelection();
	}
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
					UpdateCriteriaProgress(achievementCriteria, miscvalue2);
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
			case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
				if(achievementCriteria->gain_reputation.factionID == miscvalue1)
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
				UpdateCriteriaProgress(achievementCriteria, miscvalue1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
				if(achievementCriteria->learn_spell.spellID == miscvalue1)
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
				// Vanity pets owned - miscvalue1==778
				// Number of mounts  - miscvalue1==777
				if(achievementCriteria->number_of_mounts.unknown == miscvalue1)
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
				if(achievementCriteria->kill_creature.creatureID == miscvalue1)
				switch(achievement->ID)
				{
					case 2556: // Pest Control
						if (  (miscvalue1== 3300 && achievementCriteria->index== 1)   // Adder
							|| (miscvalue1==32261 && achievementCriteria->index== 2)   // Crystal Spider
							|| (miscvalue1==24270 && achievementCriteria->index== 3)   // Devouring Maggot
							|| (miscvalue1== 9699 && achievementCriteria->index== 4)   // Fire Beetle
							|| (miscvalue1==24174 && achievementCriteria->index== 5)   // Fjord Rat
							|| (miscvalue1==32258 && achievementCriteria->index== 6)   // Gold Beetle
							|| (miscvalue1==16068 && achievementCriteria->index== 7)   // Larva
							|| (miscvalue1==16030 && achievementCriteria->index== 8)   // Maggot
							|| (miscvalue1== 4953 && achievementCriteria->index== 9)   // Moccasin
							|| (miscvalue1== 6271 && achievementCriteria->index==10)   // Mouse
							|| (miscvalue1== 4075 && achievementCriteria->index==11)   // Rat
							|| (miscvalue1== 4076 && achievementCriteria->index==12)   // Roach
							|| (miscvalue1==15476 && achievementCriteria->index==13)   // Scorpion
							|| (miscvalue1== 2914 && achievementCriteria->index==14)   // Snake
							|| (miscvalue1==14881 && achievementCriteria->index==15)   // Spider
							|| (miscvalue1==32428 && achievementCriteria->index==16)   // Underbelly Rat
							|| (miscvalue1==28202 && achievementCriteria->index==17) ) // Zul'Drak Rat
							{
								SetCriteriaProgress(achievementCriteria, 1);
							}
						break;
					default:
						UpdateCriteriaProgress(achievementCriteria, miscvalue2);
						break;
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
				if(achievementCriteria->reach_skill_level.skillID == miscvalue1)
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
				if(achievementCriteria->learn_skill_level.skillID == miscvalue1)
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
				if(achievementCriteria->equip_item.itemID == miscvalue1)
					SetCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
				// Achievement ID:556 description Equip an epic item in every slot with a minimum item level of 213.
				// "213" value not found in achievement or criteria entries (dbc), have to hard-code it here? :(
				// Achievement ID:557 description Equip a superior item in every slot with a minimum item level of 187.
				// "187" value not found in achievement or criteria entries (dbc), have to hard-code it here? :(
				// AchievementType for both Achievement ID:556 (Equip epic items) and ID:557 (Equip superior items)
				//    is the same (47) ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM
				// Going to send item slot in miscvalue1 and item quality in miscvalue2 when calling UpdateAchievementCriteria.
				if(achievementCriteria->equip_epic_item.itemSlot == miscvalue1)
				{
					if((achievementCriteria->referredAchievement == 556) && (miscvalue2 == ITEM_QUALITY_EPIC_PURPLE))
					{
						SetCriteriaProgress(achievementCriteria, 1);
					}
					else if((achievementCriteria->referredAchievement == 557) && (miscvalue2 == ITEM_QUALITY_RARE_BLUE))
					{
						SetCriteriaProgress(achievementCriteria, 1);
					}
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
				if(achievementCriteria->do_emote.emoteID == miscvalue1)
				{
					// emote matches, check the achievement target ... (if required)
					Unit* pUnit = GetPlayer()->GetMapMgr()->GetUnit(selectedGUID);
					switch(achievement->ID)
					{
						case 1206: // To All The Squirrels I've Loved Before
							if(pUnit) // requires a target
							{
								uint32 ent = pUnit->GetEntry();
								if (  (ent== 1412 && achievementCriteria->index== 1)   // Squirrel
									|| (ent==25679 && achievementCriteria->index== 2)   // Steam Frog
									|| (ent==25677 && achievementCriteria->index== 3)   // Borean Frog
									|| (ent== 6368 && achievementCriteria->index== 4)   // Cat
									|| (ent==  620 && achievementCriteria->index== 5)   // Chicken
									|| (ent== 2442 && achievementCriteria->index== 6)   // Cow
									|| (ent== 6827 && achievementCriteria->index== 7)   // Crab
									|| (ent==  883 && achievementCriteria->index== 8)   // Deer
									|| (ent==19665 && achievementCriteria->index== 9)   // Ewe
									|| (ent==  890 && achievementCriteria->index==10)   // Fawn
									|| (ent==13321 && achievementCriteria->index==11)   // Frog
									|| (ent== 4166 && achievementCriteria->index==12)   // Gazelle
									|| (ent== 5951 && achievementCriteria->index==13)   // Hare
									|| (ent== 9600 && achievementCriteria->index==14)   // Parrot
									|| (ent==  721 && achievementCriteria->index==15)   // Rabbit
									|| (ent== 2098 && achievementCriteria->index==16)   // Ram
									|| (ent== 1933 && achievementCriteria->index==17)   // Sheep
									|| (ent==17467 && achievementCriteria->index==18)   // Skunk
									|| (ent==10685 && achievementCriteria->index==19)   // Swine
									|| (ent== 1420 && achievementCriteria->index==20)   // Toad
									|| (ent== 2620 && achievementCriteria->index==21) ) // Prairie Dog
								{
									SetCriteriaProgress(achievementCriteria, 1);
								}
							}
							break;
						case 2557: // To All The Squirrels Who Shared My Life
							if(pUnit) // requires a target
							{
								uint32 ent = pUnit->GetEntry();
								if (  (ent==29328 && achievementCriteria->index== 1)   // Arctic Hare
									|| (ent==31685 && achievementCriteria->index== 2)   // Borean Marmot
									|| (ent==28407 && achievementCriteria->index== 3)   // Fjord Penguin
									|| (ent==24746 && achievementCriteria->index== 4)   // Fjord Turkey
									|| (ent==32498 && achievementCriteria->index== 5)   // Glacier Penguin (not in db?)
									|| (ent==31889 && achievementCriteria->index== 6)   // Grizzly Squirrel
									|| (ent== 6653 && achievementCriteria->index== 7)   // Huge Toad
									|| (ent== 9700 && achievementCriteria->index== 8)   // Lava Crab
									|| (ent==31890 && achievementCriteria->index== 9)   // Mountain Skunk
									|| (ent==26503 && achievementCriteria->index==10)   // Scalawag Frog
									|| (ent==28093 && achievementCriteria->index==11)   // Sholazar Tickbird
									|| (ent==28440 && achievementCriteria->index==12) ) // Tundra Penguin
								{
									SetCriteriaProgress(achievementCriteria, 1);
								}
							}
							break;
						case 247: // Make Love, Not Warcraft
							{
								Player* pTarget = objmgr.GetPlayer((uint32)selectedGUID);
								if( pTarget && pTarget->IsPlayer() && pTarget->isDead() && isHostile(pTarget, GetPlayer()) )
								{
									UpdateCriteriaProgress(achievementCriteria, 1);
								}
							}
							break;
						case 293: // TODO:  Disturbing the Peace
							// While wearing 3 pieces of Brewfest clothing, get completely smashed and dance in Dalaran.
							break;
						case 1280: // TODO: Flirt With Disaster
							// Get completely smashed, put on your best perfume, throw a handful of rose petals on Jeremiah Payson and then kiss him. You'll regret it in the morning.
							break;
						case 1279: // TODO: Flirt With Disaster
							// Get completely smashed, put on your best perfume, throw a handful of rose petals on Sraaz and then kiss him. You'll regret it in the morning.
							break;
						case 1690: // TODO: A Frosty Shake
							// During the Feast of Winter Veil, use your Winter Veil Disguise kit to become a snowman and then dance with another snowman in Dalaran.
							break;
						case 1704: // TODO: I Pitied The Fool
							// Pity the Love Fool in the locations specified below.
							// Wintergrasp (achievementCriteria->index==1)
							// Battle Ring of Gurubashi Arena (achievementCriteria->index==2)
							// Arathi Basin Blacksmith (achievementCriteria->index==3)
							// The Culling of Stratholme (achievementCriteria->index==4)
							// Naxxramas (achievementCriteria->index==5)
							break;
						case 1042: // Number of Hugs
						case 1045: // Total cheers
						case 1047: // Total facepalms
						case 1065: // Total waves
						case 1066: // Total times LOL'd (laugh, guffaw, rofl, giggle, chuckle)
						case 1067: // Total times playing world's smallest violin
							UpdateCriteriaProgress(achievementCriteria, 1);
							break;
						default:
							break;
					}
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_KILLING_BLOW:
				// miscvalue1 contain Map ID
				switch(achievementCriteria->referredAchievement)
				{
					case 231:
						if(   ( (miscvalue1 ==  30) && (achievementCriteria->index==1) )   // Alterac Valley
							|| ( (miscvalue1 == 529) && (achievementCriteria->index==2) )   // Arathi Basin
							|| ( (miscvalue1 == 566) && (achievementCriteria->index==3) )   // Eye of the Storm
							|| ( (miscvalue1 == 489) && (achievementCriteria->index==4) )   // Warsong Gulch
							|| ( (miscvalue1 == 607) && (achievementCriteria->index==5) ) ) // Strand of the Ancients
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 233: // TODO: Berserking killing blow
						break;
					case 1487: // Total Killing Blows
						UpdateCriteriaProgress(achievementCriteria, 1);
						break;
					case 1488:
						if(   ( (miscvalue1 ==   0) && (achievementCriteria->index==1) )   // Eastern Kingdoms
							|| ( (miscvalue1 ==   1) && (achievementCriteria->index==2) )   // Kalimdor
							|| ( (miscvalue1 == 530) && (achievementCriteria->index==3) )   // Burning Crusade Areas
							|| ( (miscvalue1 == 571) && (achievementCriteria->index==4) ) ) // Northrend
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1490:
						if(   ( (miscvalue1 == 559) && (achievementCriteria->index==1) )   // Nagrand Arena
							|| ( (miscvalue1 == 562) && (achievementCriteria->index==2) )   // Blade's Edge Arena
							|| ( (miscvalue1 == 572) && (achievementCriteria->index==3) )   // Ruins of Lordaeron
							|| ( (miscvalue1 == 617) && (achievementCriteria->index==4) )   // Dalaran Sewers
							|| ( (miscvalue1 == 618) && (achievementCriteria->index==5) ) ) // Ring of Valor
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1491:
						if(   ( (miscvalue1 ==  30) && (achievementCriteria->index==1) )   // Alterac Valley
							|| ( (miscvalue1 == 529) && (achievementCriteria->index==2) )   // Arathi Basin
							|| ( (miscvalue1 == 489) && (achievementCriteria->index==3) )   // Warsong Gulch
							|| ( (miscvalue1 == 566) && (achievementCriteria->index==4) )   // Eye of the Storm
							|| ( (miscvalue1 == 607) && (achievementCriteria->index==5) ) ) // Strand of the Ancients
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1492: // TODO: 2v2 Arena Killing Blows
						break;
					case 1493: // TODO: 3v3 Arena Killing Blows
						break;
					case 1494: // TODO: 5v5 Arena Killing Blows
						break;
					case 1495: // Alterac Valley Killing Blows
						if(miscvalue1 == 30)
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1496: // Arathi Basin Killing Blows
						if(miscvalue1 == 529)
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1497: // Warsong Gulch Killing Blows
						if(miscvalue1 == 489)
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1498: // Eye of the Storm Killing Blows
						if(miscvalue1 == 566)
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1499: // Strand of the Ancients Killing Blows
						if(miscvalue1 == 607)
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 2148: // TODO: Deliver a killing blow to a Scion of Eternity while riding on a hover disk
						break;
					case 2149: // TODO: Deliver a killing blow to a Scion of Eternity while riding on a hover disk
						break;
					default:
						break;
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
				if(achievementCriteria->use_item.itemID == miscvalue1)
					UpdateCriteriaProgress(achievementCriteria, 1);
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
				// Total NPC kills, Kill an NPC that yeilds XP, Beasts, Critters killed, Demons, Dragonkin ...
				// miscvalue1 = killed creature high GUID
				// miscvalue2 = killed creature low GUID
				{
					uint64 crGUID = miscvalue1;
					crGUID <<= 32; // shift to high 32-bits
					crGUID += miscvalue2;
					Unit* pUnit = GetPlayer()->GetMapMgr()->GetUnit(crGUID);
					if(pUnit)
					{
						uint8 crTypeId = pUnit->GetTypeId();
						uint32 crType = NOUNITTYPE;
						bool crTotem = false;
						bool yieldXP = CalculateXpToGive( pUnit, GetPlayer() )  > 0;
						if(crTypeId == TYPEID_UNIT)
						{
							crTotem = ((Creature*)pUnit)->IsTotem();
							if(((Creature*)pUnit)->GetCreatureInfo())
							{
								crType = ((Creature*)pUnit)->GetCreatureInfo()->Type;
							}
							if(     (achievementCriteria->ID == 4944)                             // Total NPC kills              refAch==1197
								|| ( (achievementCriteria->ID == 4946) && (yieldXP)            )   // Kill an NPC that yields XP   refAch==1198
								|| ( (achievementCriteria->ID == 4948) && (crType==BEAST)      )   // Beasts                       refAch== 107
								|| ( (achievementCriteria->ID == 4958) && (crType==CRITTER)    )   // Critters killed              refAch== 107
								|| ( (achievementCriteria->ID == 4949) && (crType==DEMON)      )   // Demons                       refAch== 107
								|| ( (achievementCriteria->ID == 4950) && (crType==DRAGONSKIN) )   // Dragonkin                    refAch== 107
								|| ( (achievementCriteria->ID == 4951) && (crType==ELEMENTAL)  )   // Elemental                    refAch== 107
								|| ( (achievementCriteria->ID == 4952) && (crType==GIANT)      )   // Giant                        refAch== 107
								|| ( (achievementCriteria->ID == 4953) && (crType==HUMANOID)   )   // Humanoid                     refAch== 107
								|| ( (achievementCriteria->ID == 4954) && (crType==MECHANICAL) )   // Mechanical                   refAch== 107
								|| ( (achievementCriteria->ID == 4955) && (crType==UNDEAD)     )   // Undead                       refAch== 107
								|| ( (achievementCriteria->ID == 4956) && (crType==NOUNITTYPE) )   // Unspecified                  refAch== 107
								|| ( (achievementCriteria->ID == 4957) && (crTotem)            ) ) // Totems                       refAch== 107
							{
								UpdateCriteriaProgress(achievementCriteria, 1);
							}
						}
					}
				}
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

		if((achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE && GetPlayer()->GetTeam() != 1) ||
		   (achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && GetPlayer()->GetTeam() != 0) )
			continue;

		switch(type)
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
				UpdateCriteriaProgress(achievementCriteria, 1);
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
			SetCriteriaProgress(achievementCriteria, (int32)GetPlayer()->m_finishedQuests.size());
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
			{
				uint32 qcinzone = 0;
				set<uint32>::iterator qc = GetPlayer()->m_finishedQuests.begin();
				for(; qc!=GetPlayer()->m_finishedQuests.end(); ++qc)
				{
					Quest* qst = QuestStorage.LookupEntry(*qc);
					if(qst && qst->zone_id == achievementCriteria->complete_quests_in_zone.zoneID)
					{
						++qcinzone;
					}
				}
				SetCriteriaProgress(achievementCriteria, qcinzone);
			}
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
			{
				uint32 completed = 0;
				set<uint32>::iterator it = GetPlayer()->m_finishedQuests.find(achievementCriteria->complete_quest.questID);
				if( it!=GetPlayer()->m_finishedQuests.end() )
				{
					++completed;
				}
				SetCriteriaProgress(achievementCriteria, completed);
			}
			break;
/* Disabling this in case it is eating up CPU time
- not even sure if it's blizz-like to count already-completed quest gold at time when achievements were added?
		case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
			{
				uint32 qrmoney = 0;
				set<uint32>::iterator fq = GetPlayer()->m_finishedQuests.begin();
				for(; fq!=GetPlayer()->m_finishedQuests.end(); ++fq)
				{
					Quest* qst = QuestStorage.LookupEntry(*fq);
					if(qst)
					{
						qrmoney += qst->reward_money;
					}
				}
				SetCriteriaProgress(achievementCriteria, qrmoney);
			}
			break;
*/
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
			{
				int32 rep = GetPlayer()->GetStanding(achievementCriteria->gain_reputation.factionID);
				SetCriteriaProgress(achievementCriteria, rep);
			}
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
			SetCriteriaProgress(achievementCriteria,GetPlayer()->GetExaltedCount());
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
			if(GetPlayer()->HasSpell(achievementCriteria->learn_spell.spellID))
				SetCriteriaProgress(achievementCriteria, 1);
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
			{
				// achievementCriteria field4 = 777 for mounts, 778 for companion pets
				SpellSet::iterator sl = GetPlayer()->mSpells.begin();
				uint32 nm = 0;
				while(sl != GetPlayer()->mSpells.end())
				{
					SpellEntry* sp = dbcSpell.LookupEntry(*sl);
					if(achievementCriteria->number_of_mounts.unknown==777 && sp && sp->MechanicsType==MECHANIC_MOUNTED) // mount spell
					{
						++nm;
					}
					else if(achievementCriteria->number_of_mounts.unknown==778 && sp && (sp->Effect[0]==SPELL_EFFECT_SUMMON) && (sp->School==0)) // Companion pet?
					{
						++nm;
					}
					++sl;
				}
				SetCriteriaProgress(achievementCriteria, nm);
			}

			break;
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
			SetCriteriaProgress(achievementCriteria, GetPlayer()->_GetSkillLineCurrent(achievementCriteria->reach_skill_level.skillID, true));
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
			SetCriteriaProgress(achievementCriteria, GetPlayer()->_GetSkillLineMax(achievementCriteria->learn_skill_level.skillID)/75);
			break;
		//End of Achievement List
		default:
			break;
		}
		if(IsCompletedCriteria(achievementCriteria))
		{
			CompletedCriteria(achievementCriteria);
		}
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
		{
			return false;
		}
	}

	CriteriaProgressMap::iterator itr = m_criteriaProgress.find(achievementCriteria->ID);
	if(itr == m_criteriaProgress.end())
	{
		return false;
	}

	CriteriaProgress *progress = itr->second;

	if(progress->counter < 1) // 0 or negative
	{
		return false;
	}

	uint32 progresscounter = (uint32)progress->counter;
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
			return progresscounter >= (int32)achievementCriteria->reach_level.level;
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
		case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
			return progresscounter >= achievementCriteria->loot_item.itemCount;
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
			return progresscounter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
			return progresscounter >= achievementCriteria->loot_money.goldInCopper;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
			return progresscounter >= achievementCriteria->complete_quest_count.totalQuestCount;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
			return progresscounter >= achievementCriteria->complete_quests_in_zone.questCount;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
			return progresscounter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
			return progresscounter >= achievementCriteria->quest_reward_money.goldInCopper;
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
			return progresscounter >= achievementCriteria->gain_reputation.reputationAmount;
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
			return progresscounter >= achievementCriteria->gain_exalted_reputation.numberOfExaltedFactions;
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
			return progresscounter>0;
		case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
			return progresscounter >= achievementCriteria->number_of_mounts.mountCount;
		case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
			return progresscounter >= achievementCriteria->kill_creature.creatureCount;
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
			return progresscounter >= achievementCriteria->reach_skill_level.skillLevel;
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
			return progresscounter >= achievementCriteria->learn_skill_level.skillLevel;
		case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
			return progresscounter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
			return progresscounter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
			return progresscounter >= 1;
		case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
			return progresscounter >= achievementCriteria->use_item.itemCount;
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

	uint32 completedCount = 0;
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
		else
		{
			++completedCount;
		}
	}
	if(!foundOutstanding)
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
	}

	if( (entry->count > 1) && (completedCount >= entry->count) )
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
	}
	return ACHIEVEMENT_COMPLETED_NONE;
}

void AchievementMgr::SetCriteriaProgress(AchievementCriteriaEntry const* entry, int32 newValue, bool relative)
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
	if(progress->counter > 0)
	{
		SendCriteriaUpdate( progress );
	}
}

void AchievementMgr::UpdateCriteriaProgress(AchievementCriteriaEntry const* entry, int32 updateByValue)
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
	if(progress->counter > 0)
	{
		SendCriteriaUpdate( progress );
	}
}

void AchievementMgr::CompletedAchievement(AchievementEntry const* achievement)
{
	if( achievement->flags & ACHIEVEMENT_FLAG_COUNTER || m_completedAchievements.find(achievement->ID)!=m_completedAchievements.end() )
	{
		return;
	}

	SendAchievementEarned( achievement );
	m_completedAchievements[achievement->ID] = time(NULL);

	if(!(achievement->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
	{
		objmgr.allCompletedAchievements.insert( achievement->ID );
		UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT );
	}

	// check for reward
	GiveAchievementReward(achievement);
}

void AchievementMgr::SendAllAchievementData(Player* player)
{
	WorldPacket data( SMSG_ALL_ACHIEVEMENT_DATA,4*3+m_completedAchievements.size()*4*2+GetCriteriaProgressCount()*7*4 );
	BuildAllDataPacket(&data);
	player->GetSession()->SendPacket(&data);
}

void AchievementMgr::SendRespondInspectAchievements(Player* player)
{
	WorldPacket data( SMSG_ALL_ACHIEVEMENT_DATA,4+4*3+m_completedAchievements.size()*4*2+GetCriteriaProgressCount()*7*4 );
	data.append(GetPlayer()->GetGUID());
	BuildAllDataPacket(&data, false);
	player->GetSession()->SendPacket(&data);
}

void AchievementMgr::BuildAllDataPacket(WorldPacket *data, bool self)
{
	for( CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); ++iter )
	{
		*data << uint32(iter->first);
		*data << uint32(secsToTimeBitFields(iter->second));
	}
	*data << int32(-1);
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
	{
		AchievementEntry const *achievement = dbcAchievementStore.LookupEntry(iter->second->id);
		if(self) // achievement progress to send to self
		{
			if(SendAchievementProgress(iter->second))
			{
				*data << uint32(iter->second->id);
				data->appendPackGUID(iter->second->counter);
				*data << GetPlayer()->GetNewGUID(); 
				*data << uint32(0);
				*data << uint32(secsToTimeBitFields(iter->second->date));
				*data << uint32(0);
				*data << uint32(0);
			}
		}
		else // achievement progress to send to other players (inspect)
		{
			if(IsStatisticAchievement(achievement)) // only send statistics, no other unfinished achievement progress, since client only displays them as completed or not completed
			{
				*data << uint32(iter->second->id);
				data->appendPackGUID(iter->second->counter);
				*data << GetPlayer()->GetNewGUID(); 
				*data << uint32(0);
				*data << uint32(secsToTimeBitFields(iter->second->date));
				*data << uint32(0);
				*data << uint32(0);
			}
		}
	}
	*data << int32(-1);
}

uint32 AchievementMgr::GetCriteriaProgressCount(void)
{
	uint32 criteriapc = 0;
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
	{
		AchievementEntry const *achievement = dbcAchievementStore.LookupEntry(iter->second->id);
		if(SendAchievementProgress(iter->second))
		{
			++criteriapc;
		}
	}
	return criteriapc;
}

void AchievementMgr::GiveAchievementReward(AchievementEntry const* entry)
{
	AchievementReward r;
	r.type = ACHIEVEMENT_REWARDTYPE_NONE;
	r.itemId = 0;
	r.rankId = 0;

	if(strlen(entry->rewardName)>0)
	{
		switch(entry->unknown2)
		{
			case 0x0000000b: // Title Reward: The Flawless Victor
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_FLAWLESS_VICTOR;
				break;
			case 0x000000b6: // Title Reward: Champion of the Frozen Wastes
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CHAMPION_OF_THE_FROZEN_WASTES;
				break;
			case 0x00000229: // Title Reward: Guardian of Cenarius
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_GUARDIAN_OF_CENARIUS;
				break;
			case 0x00000244: // Title Reward: Salty
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_SALTY;
				break;
			case 0x000005f2: // Reward: Reeking Pet Carrier
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 40653;
				break;
			case 0x0000066c: // Reward: Title & Loremaster's Colors
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM | ACHIEVEMENT_REWARDTYPE_TITLE;
				r.itemId = 43300;
				r.rankId = PVPTITLE_LOREMASTER;
				break;
			case 0x000006a4: // Title Reward: The Magic Seeker
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_MAGIC_SEEKER;
				break;
			case 0x000006a7: // Reward: Black War Bear [Horde]
				// spellId 60018 or 60019 ?
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 44224;
				break;
			case 0x000006a8: // Reward: Black War Bear [Alliance]
				// spellId 60018 or 60019 ?
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 44223;
				break;
			case 0x00000749: // Reward: The Schools of Arcane Magic - Mastery
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 59983;
				break;
			case 0x0000076a: // Title Reward: Conqueror of Naxxramas
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CONQUEROR_OF_NAXXRAMAS;
				break;
			case 0x000007fc: // Reward: Black Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 59976;
				break;
			case 0x00000858: // Title Reward: Elder
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ELDER;
				break;
			case 0x0000085b: // Title Reward: The Argent Champion
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_ARGENT_CHAMPION;
				break;
			case 0x0000085f: // Title Reward: The Immortal
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_IMMORTAL;
				break;
			case 0x000008f4: // Title Reward: The Undying
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_UNDYING;
				break;
			case 0x00000975: // Title: Bloodsail Admiral
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BLOODSAIL_ADMIRAL;
				break;
			case 0x000009d3: // Title Reward: Brewmaster
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BREWMASTER;
				break;
			case 0x000009db: // Title Reward: Matron/Patron
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = (GetPlayer()->getGender()==0) ? /* Matron */ PVPTITLE_MATRON : /* Patron */ PVPTITLE_PATRON;
				break;
			case 0x00000a03: // Title Reward: Conqueror
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CONQUEROR;
				break;
			case 0x00000ab1: // Title Reward: The Diplomat
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_DIPLOMAT;
				break;
			case 0x00000ac7: // Title Reward: The Explorer
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_EXPLORER;
				break;
			case 0x00000b6c: // Title Reward: Justicar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_JUSTICAR;
				break;
			case 0x00000b9e: // Title Reward: Flame Warden
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_FLAME_WARDEN;
				break;
			case 0x00000b9f: // Title Reward: Flame Keeper
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_FLAME_KEEPER;
				break;
			case 0x00000bb1: // Reward: Titanium Seal of Dalaran
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60650;
				break;
			case 0x00000be0: // Reward: Tabard of the Achiever
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 40643;
				break;
			case 0x00000c14: // Title Reward: Merrymaker
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_MERRYMAKER;
				break;
			case 0x00000c78: // Title Reward: The Love Fool
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_LOVE_FOOL;
				break;
			case 0x00000cb4: // Title Reward: Of the Nightfall [Normal] Title Reward: Twilight Vanquisher [Heroic]
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = (entry->ID == 2051) ? /* Normal */ PVPTITLE_OF_THE_NIGHTFALL : /* Heroic ID==2054 */ PVPTITLE_TWILIGHT_VANQUISHER;
				break;
			case 0x00000cec: // Title Reward: Obsidian Slayer
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OBSIDIAN_SLAYER;
				break;
			case 0x00000d2c: // Title Reward: Battlemaster
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BATTLEMASTER;
				break;
			case 0x00000d2d: // Title Reward: Battlemaster
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BATTLEMASTER;
				break;
			case 0x00000d2e: // Title Reward: Ambassador
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_AMBASSADOR;
				break;
			case 0x00000d2f: // Title Reward: Ambassador
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_AMBASSADOR;
				break;
			case 0x00000d56: // Title Reward: The Seeker
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_SEEKER;
				break;
			case 0x00000d74: // Reward: Albino Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60025;
				break;
			case 0x00000d7d: // Title Reward: Of the Horde or Of the Alliance
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = (GetPlayer()->GetTeam()==1) ? /* Horde */ PVPTITLE_OF_THE_HORDE : /* Alliance */ PVPTITLE_OF_THE_ALLIANCE;
				break;
			case 0x00000da5: // Reward: Tabard of the Explorer
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 43348;
				break;
			case 0x00000da6: // Reward: Red Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 59961;
				break;
			case 0x00000da7: // Title: Jenkins
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_JENKINS;
				break;
			case 0x00000dab: // Reward: Plagued Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60021;
				break;
			case 0x00000dac: // Reward: Violet Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60024;
				break;
			case 0x00000dba: // Title Reward: The Hallowed
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_HALLOWED;
				break;
			case 0x00000e0b: // Reward: Tabard of Brute Force
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 43349; // Hmm, Tabard of Brute Force not found in my item DB ...
			case 0x00000e10: // Title Reward: Arena Master
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ARENA_MASTER;
				break;
			case 0x00000e12: // Title Reward: The Exalted
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_EXALTED;
				break;
			case 0x00000e5e: // Title Reward: Chef
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CHEF;
				break;
			default:
				break;
		}
		if(r.type & ACHIEVEMENT_REWARDTYPE_TITLE)
		{
			GetPlayer()->SetKnownTitle(static_cast< RankTitles >(r.rankId), true);
			GetPlayer()->SetUInt32Value( PLAYER_CHOSEN_TITLE, 0 );
		}
		if(r.type & ACHIEVEMENT_REWARDTYPE_ITEM)
		{
			// How does this work? Add item directly to inventory, or send through mail?
			ItemPrototype* it = ItemPrototypeStorage.LookupEntry(r.itemId);
			if(it)
			{
				Item *item;
				item = objmgr.CreateItem(r.itemId, GetPlayer());
				if(item == NULL) // this is bad - item not found in db or unable to be created for some reason
				{
					GetPlayer()->GetSession()->SendNotification("Unable to create item with id %lu!", r.itemId);
					return;
				}
				item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
				if(it->Bonding==ITEM_BIND_ON_PICKUP)
				{
					if(it->Flags & ITEM_FLAG_ACCOUNTBOUND) // any "accountbound" items for achievement rewards?
					{
						item->AccountBind();
					}
					else
					{
						item->SoulBind();
					}
				}

				if(!GetPlayer()->GetItemInterface()->AddItemToFreeSlot(item)) // this is bad. inventory full. maybe we should mail it instead?
				{
					GetPlayer()->GetSession()->SendNotification("No free slots were found in your inventory!");
					item->DeleteMe();
					return;
				}
			}
		}
		if(r.type & ACHIEVEMENT_REWARDTYPE_SPELL)
		{
			GetPlayer()->addSpell(r.spellId);
		}
	}
}

uint32 AchievementMgr::GetCompletedAchievementsCount() const
{
	return (uint32)m_completedAchievements.size();
}

