/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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
#ifdef ENABLE_ACHIEVEMENTS
/**
	Takes achievementlink c-string and returns the ID value from it.
*/
uint32 GetAchievementIDFromLink(const char* achievementlink)
{
	if( achievementlink == NULL )
		return 0;

	const char* ptr = strstr(achievementlink, "|Hachievement:");
	if( ptr == NULL )
		return 0;

	// achievement id is just past "|Hachievement:" (14 bytes)
	return atol(ptr + 14);
}

/**
	True if CriteriaProgress should be sent to Player; False if CriteriaProgress should not be sent.
	If the CriteriaProgress specified should not be sent to the Player, it returns false, otherwise it returns true.
	Examples of CriteriaProgress that should not be sent to the Player are:
		1. When counter is zero or negative, which would indicate the achievement hasn't been started yet.
		2. Reputation type achievements, where the progress is not shown in the client.
		3. Reach-Level type achievements, where the progress is not shown in the client.
*/
bool SendAchievementProgress(const CriteriaProgress* c)
{
	if( c == NULL || c->counter <= 0 )
		// achievement not started yet, don't send progress
		return false;

	AchievementCriteriaEntry const* acEntry = dbcAchievementCriteriaStore.LookupEntryForced(c->id);
	if( !acEntry )
		return false;

	if( acEntry->requiredType == ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION )
		// Exalted with X faction (don't send 12323/42000 progress, it's not shown anyway)
		return false;

	if( acEntry->requiredType == ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL )
		// Reach level (don't send 7/80 progress, it's not shown anyway)
		return false;

	return true;
}

/**
	True if CriteriaProgress should be saved to database.  False if CriteriaProgress should not be saved to database.
	Not all achievement progresses get saved to progress database, since some are saved in the character database,
		or are easily computable when the player logs in.
*/
bool SaveAchievementProgressToDB(const CriteriaProgress* c)
{
	if( c->counter <= 0 )
	{
		// don't save it if it's not started yet
		return false;
	}
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
		case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
			return false;
		default:
			break;
	}
	return true;
}

/**
	true if the achievement should be shown; false otherwise
*/
bool ShowCompletedAchievement(uint32 achievementID, const Player* plr)
{
	switch(achievementID)
	{
		case  457: // Realm First! Level 80
		case  467: // Realm First! Level 80 Shaman
		case  466: // Realm First! Level 80 Druid
		case  465: // Realm First! Level 80 Paladin
		case  464: // Realm First! Level 80 Priest
		case  463: // Realm First! Level 80 Warlock
		case  462: // Realm First! Level 80 Hunter
		case  461: // Realm First! Level 80 Death Knight
		case  460: // Realm First! Level 80 Mage
		case  459: // Realm First! Level 80 Warrior
		case  458: // Realm First! Level 80 Rogue
		case 1404: // Realm First! Level 80 Gnome
		case 1405: // Realm First! Level 80 Blood Elf
		case 1406: // Realm First! Level 80 Draenei
		case 1407: // Realm First! Level 80 Dwarf
		case 1408: // Realm First! Level 80 Human
		case 1409: // Realm First! Level 80 Night Elf
		case 1410: // Realm First! Level 80 Orc
		case 1411: // Realm First! Level 80 Tauren
		case 1412: // Realm First! Level 80 Troll
		case 1413: // Realm First! Level 80 Forsaken
		case 1415: // Realm First! Grand Master Alchemist
		case 1414: // Realm First! Grand Master Blacksmith
		case 1416: // Realm First! Cooking Grand Master
		case 1417: // Realm First! Grand Master Enchanter
		case 1418: // Realm First! Grand Master Engineer
		case 1419: // Realm First! First Aid Grand Master
		case 1420: // Realm First! Grand Master Angler
		case 1421: // Realm First! Grand Master Herbalist
		case 1422: // Realm First! Grand Master Scribe
		case 1423: // Realm First! Grand Master Jewelcrafter
		case 1424: // Realm First! Grand Master Leatherworker
		case 1425: // Realm First! Grand Master Miner
		case 1426: // Realm First! Grand Master Skinner
		case 1427: // Realm First! Grand Master Tailor
		case 1463: // Realm First! Northrend Vanguard: First player on the realm to gain exalted reputation with the Argent Crusade, Wyrmrest Accord, Kirin Tor and Knights of the Ebon Blade.
			{
				QueryResult* achievementResult = CharacterDatabase.Query("SELECT guid FROM character_achievement WHERE achievement=%u ORDER BY date LIMIT 1", achievementID);
				if( achievementResult != NULL )
				{
					Field* field = achievementResult->Fetch();
					if( field != NULL )
					{
						// somebody has this Realm First achievement... is it this player?
						uint64 firstguid = field->GetUInt32();
						if( firstguid != (uint32)plr->GetGUID() )
						{
							// nope, somebody else was first.
							delete achievementResult;
							return false;
						}
					}
					delete achievementResult;
				}
			}
			break;
// All raid members should receive these last 3 Realm First achievements when they first occur.
// (not implemented yet)
//		case 1400: // Realm First! Magic Seeker: Participated in the realm first defeat of Malygos on Heroic Difficulty.
//		case  456: // Realm First! Obsidian Slayer: Participated in the realm first defeat of Sartharion the Onyx Guardian on Heroic Difficulty.
//		case 1402: // Realm First! Conqueror of Naxxramas: Participated in the realm first defeat of Kel'Thuzad on Heroic Difficulty in Naxxramas.
		default:
			break;
	}
	return true;
}

/**
	AchievementMgr constructor
*/
AchievementMgr::AchievementMgr(Player *player)
:
m_player(player),
isCharacterLoading(true)
{

}

/**
	AchievementMgr destructor
*/
AchievementMgr::~AchievementMgr()
{
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
		delete iter->second;
	m_criteriaProgress.clear();
	m_completedAchievements.clear();
}

/**
	Save Achievement data to database
	Saves all completed achievements to database.  Saves all achievement progresses that have been started, and that aren't calculated on login, to database.
*/
void AchievementMgr::SaveToDB()
{
	if( !m_completedAchievements.empty() )
	{
		std::ostringstream ss;

        ss << "DELETE FROM character_achievement WHERE guid = ";
        ss << m_player->GetLowGUID();
        ss << ";";

        CharacterDatabase.ExecuteNA( ss.str().c_str() );

        ss.rdbuf()->str("");

		ss << "INSERT INTO character_achievement VALUES ";
		bool first = true;
		for(CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter!=m_completedAchievements.end(); iter++)
		{
			if( ss.str().length() >= 16000 )
			{
				// SQL query length is limited to 16384 characters
				CharacterDatabase.ExecuteNA( ss.str().c_str() );
				ss.str("");
				ss << "INSERT INTO character_achievement VALUES ";
				first = true;
			}

			if( !first )
			{
				ss << ", ";
			}
			else
			{
				first = false;
			}
            ss << "(" << m_player->GetLowGUID() << ", " << iter->first << ", " << iter->second << ")";
		}
		CharacterDatabase.ExecuteNA( ss.str().c_str() );
	}

	if( !m_criteriaProgress.empty() )
	{
		std::ostringstream ss;

        ss << "DELETE FROM character_achievement_progress WHERE guid = ";
        ss << m_player->GetLowGUID();
        ss << ";";

        CharacterDatabase.ExecuteNA( ss.str().c_str() );

        ss.rdbuf()->str("");

		ss << "INSERT INTO character_achievement_progress VALUES ";
		bool first = true;
		for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
		{
			if( SaveAchievementProgressToDB(iter->second) )
			{
				// only save some progresses, others will be updated when character logs in
				if( ss.str().length() >= 16000 )
				{
					// SQL query length is limited to 16384 characters
					CharacterDatabase.ExecuteNA( ss.str().c_str() );
					ss.str("");
					ss << "INSERT INTO character_achievement_progress VALUES ";
					first = true;
				}
				if( !first )
				{
					ss << ", ";
				}
				else
				{
					first = false;
				}
                ss << "(" << m_player->GetLowGUID() << ", " << iter->first << ", " << iter->second->counter << ", " << iter->second->date << ")";
			}
		}
		if( !first )
		{
			// don't execute query if there's no entries to save
			CharacterDatabase.ExecuteNA( ss.str().c_str() );
		}
	}
}

/**
	Load achievements from database.
	Loads completed achievements and achievement progresses from the database.
*/
void AchievementMgr::LoadFromDB(QueryResult *achievementResult, QueryResult *criteriaResult)
{
	if( achievementResult )
	{
		do
		{
			Field *fields = achievementResult->Fetch();
			uint32 id = fields[0].GetUInt32();
			if( m_completedAchievements[id] == 0 )
				m_completedAchievements[id] = fields[1].GetUInt32();
			else 
				sLog.outError("Duplicate completed achievement %u for player %u, skipping", id, (uint32)m_player->GetGUID() );
		} while(achievementResult->NextRow());
		delete achievementResult;
	}

	if( criteriaResult )
	{
		do
		{
			Field *fields = criteriaResult->Fetch();
			uint32 progress_id = fields[0].GetUInt32();
			if( m_criteriaProgress[ progress_id ] == NULL )
			{
				CriteriaProgress *progress = new CriteriaProgress( progress_id, fields[1].GetUInt32(), fields[2].GetUInt64() );
				m_criteriaProgress[ progress_id ] = progress;
			}
			else
				sLog.outError( "Duplicate criteria progress %u for player %u, skipping", progress_id, (uint32) m_player->GetGUID() );

		}while( criteriaResult->NextRow() );
		delete criteriaResult;
	}
}

/**
	Sends message to player(s) that the achievement has been completed.
	Realm first! achievements get sent to all players currently online.
	All other achievements get sent to all of the achieving player's guild members, group members, and other in-range players.
*/
void AchievementMgr::SendAchievementEarned(AchievementEntry const* achievement)
{
	if( achievement == NULL || isCharacterLoading )
	{
		return;
	}

	const char *msg = "|Hplayer:$N|h[$N]|h has earned the achievement $a!";
	uint32* guidList = NULL;
	uint32 guidCount = 0;
	uint32 guidIndex = 0;

	// Send Achievement message to everyone currently on the server
	if( achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_KILL | ACHIEVEMENT_FLAG_REALM_FIRST_REACH) )
	{
		WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, 200);
		data << GetPlayer()->GetName();
		data << uint64(GetPlayer()->GetGUID());
		data << uint32(achievement->ID);
		data << uint32(0);
		sWorld.SendGlobalMessage(&data);
	}
	else
	{
		// allocate enough space
		guidList = new uint32[sWorld.GetSessionCount() + 256];
		// Send Achievement message to every guild member currently on the server
		if( GetPlayer()->IsInGuild() )
		{
			Guild* guild = GetPlayer()->getPlayerInfo()->guild;
			WorldPacket data(SMSG_MESSAGECHAT, 200);
			data << uint8(CHAT_MSG_GUILD_ACHIEVEMENT);
			data << uint32(LANG_UNIVERSAL);
			data << uint64(GetPlayer()->GetGUID());
			data << uint32(5);
			data << uint64(GetPlayer()->GetGUID());
			data << uint32(strlen(msg)+1);
			data << msg;
			data << uint8(0);
			data << uint32(achievement->ID);
//			guild->SendPacket(&data);
			GuildMemberMap::iterator guildItr = guild->GetGuildMembersBegin();
			GuildMemberMap::iterator guildItrLast = guild->GetGuildMembersEnd();
			while(guildItr != guildItrLast)
			{
				if( guildItr->first->m_loggedInPlayer && guildItr->first->m_loggedInPlayer->GetSession() )
				{
					guildItr->first->m_loggedInPlayer->GetSession()->SendPacket(&data);
					// store GUID, so we don't send message to the same player again (anti-spam)
					guidList[guidCount++] = guildItr->first->guid;
				}
				++guildItr;
			}
		}
		// Build generic packet for group members and nearby players
		WorldPacket cdata(SMSG_MESSAGECHAT, 200);
		cdata << uint8(CHAT_MSG_ACHIEVEMENT);
		cdata << uint32(LANG_UNIVERSAL);
		cdata << uint64(GetPlayer()->GetGUID());
		cdata << uint32(5);
		cdata << uint64(GetPlayer()->GetGUID());
		cdata << uint32(strlen(msg)+1);
		cdata << msg;
		cdata << uint8(0);
		cdata << uint32(achievement->ID);
		bool alreadySent;
		// Send Achievement message to group members
		Group* grp = GetPlayer()->GetGroup();
		
		if( grp )
		{
//			grp->SendPacketToAll(&cdata);
			uint8 i = 0;
			GroupMembersSet::iterator groupItr;
			GroupMembersSet::iterator groupItrLast;
			grp->Lock();
			for(; i < grp->GetSubGroupCount(); ++i)
			{
				SubGroup* sg = grp->GetSubGroup(i);
				if( sg == NULL )
				{
					continue;
				}

				groupItr = sg->GetGroupMembersBegin();
				groupItrLast = sg->GetGroupMembersEnd();
				for(; groupItr != groupItrLast; ++groupItr)
				{
					if( (*groupItr)->m_loggedInPlayer != NULL && (*groupItr)->m_loggedInPlayer->GetSession() )
					{
						// check if achievement message has already been sent to this player (if they received a guild achievement message already)
						alreadySent = false;
						for(guidIndex = 0; guidIndex < guidCount; ++guidIndex)
						{
							if( guidList[guidIndex] == (*groupItr)->guid )
							{
								alreadySent = true;
								guidIndex = guidCount;
							}
						}
						if( !alreadySent )
						{
							(*groupItr)->m_loggedInPlayer->GetSession()->SendPacket(&cdata);
							guidList[guidCount++] = (*groupItr)->guid;
						}
					}
				}
			}
			grp->Unlock();
		}
		// Send Achievement message to nearby players
		std::set<Object*>::iterator inRangeItr = GetPlayer()->GetInRangePlayerSetBegin();
		std::set<Object*>::iterator inRangeItrLast = GetPlayer()->GetInRangePlayerSetEnd();
		for(; inRangeItr != inRangeItrLast; ++inRangeItr)
		{

            Player *p = static_cast< Player* >( (*inRangeItr) );

			if( p && p->GetSession() &&  !p->Social_IsIgnoring( GetPlayer()->GetLowGUID() ) )
			{
				// check if achievement message has already been sent to this player (in guild or group)
				alreadySent = false;
				for(guidIndex = 0; guidIndex < guidCount; ++guidIndex)
				{
					if( guidList[guidIndex] == p->GetLowGUID() )
					{
						alreadySent = true;
						guidIndex = guidCount;
					}
				}
				if( !alreadySent )
				{
					p->GetSession()->SendPacket(&cdata);
					guidList[guidCount++] = p->GetLowGUID();
				}
			}
		}
		// Have we sent the message to the achieving player yet?
		alreadySent = false;
		for(guidIndex = 0; guidIndex < guidCount; ++guidIndex)
		{
			if( guidList[guidIndex] == GetPlayer()->GetLowGUID() )
			{
				alreadySent = true;
				guidIndex = guidCount;
			}
			if( !alreadySent )
			{
				GetPlayer()->GetSession()->SendPacket(&cdata);
			}
		}
	}
//	GetPlayer()->SendMessageToSet(&cdata, true);

	WorldPacket data( SMSG_ACHIEVEMENT_EARNED, 30);
	data << GetPlayer()->GetNewGUID();
	data << uint32( achievement->ID );
	data << uint32( secsToTimeBitFields(UNIXTIME) );
	data << uint32(0);
	GetPlayer()->GetSession()->SendPacket(&data);
	if( guidList )
	{
		delete [] guidList;
	}
}

/**
	Sends update to achievement criteria to the player.
*/
void AchievementMgr::SendCriteriaUpdate(CriteriaProgress* progress)
{
	if( progress == NULL || isCharacterLoading )
	{
		return;
	}

	WorldPacket data(SMSG_CRITERIA_UPDATE, 32);
	data << uint32(progress->id);

	data.appendPackGUID(progress->counter);

	data << GetPlayer()->GetNewGUID();
	data << uint32(0);
	data << uint32(secsToTimeBitFields(progress->date));
	data << uint32(0);  // timer 1
	data << uint32(0);  // timer 2
	if( !GetPlayer()->IsInWorld() ) //VLack: maybe we should NOT send these delayed, for 3.1.1, but seems logical
		GetPlayer()->CopyAndSendDelayedPacket(&data);
	else
		GetPlayer()->GetSession()->SendPacket(&data);
}

/**
	Updates ALL achievement criteria
	This is called during player login to update some criteria which aren't saved in achievement progress DB,
		since they are saved in the character DB or can easily be computed.
*/
void AchievementMgr::CheckAllAchievementCriteria()
{
	for(uint32 i= 0; i<ACHIEVEMENT_CRITERIA_TYPE_TOTAL; i++)
		UpdateAchievementCriteria(AchievementCriteriaTypes(i));
}

/**
	Updates achievement criteria of the specified type
	This is what should be called from other places in the code (upon killing a monster, or looting an object, or completing a quest, etc.).
	miscvalue1, miscvalue2 depend on the achievement type.
	Generally, miscvalue1 is an ID of some type (quest ID, item ID, faction ID, etc.), and miscvalue2 is the amount to increase the progress.
*/
void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type, int32 miscvalue1, int32 miscvalue2, uint32 time)
{
	uint64 selectedGUID;
	if( type == ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE )
	{
		selectedGUID = GetPlayer()->GetSelection();
	}
	AchievementCriteriaEntryList const& achievementCriteriaList = objmgr.GetAchievementCriteriaByType(type);
	for(AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i!=achievementCriteriaList.end(); ++i)
	{
		AchievementCriteriaEntry const *achievementCriteria = (*i);

		if( IsCompletedCriteria(achievementCriteria) )
		{
			// don't bother updating it, if it has already been completed
			continue;
		}

		if( (achievementCriteria->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP) && GetPlayer()->GetGroup() )
		{
			// criteria requires that the player not be in a group, but they are in a group, so don't update it
			continue;
		}

		AchievementEntry const *achievement = dbcAchievementStore.LookupEntryForced(achievementCriteria->referredAchievement);
		if( !achievement )
		{
			// referred achievement not found (shouldn't normally happen)
			continue;
		}

		if( (achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE && !GetPlayer()->IsTeamHorde() ) ||
			(achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && !GetPlayer()->IsTeamAlliance() ) )
		{
			// achievement requires a faction of which the player is not a member
			continue;
		}

		switch (type)
		{
			//Start of Achievement List
			case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
				SetCriteriaProgress(achievementCriteria, GetPlayer()->getLevel());
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
			case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
				if( achievementCriteria->loot_item.itemID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, miscvalue2);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
				if( GetPlayer()->HasOverlayUncovered(achievementCriteria->explore_area.areaReference) )
				{
					SetCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
				if( achievementCriteria->complete_quests_in_zone.zoneID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
				if( achievementCriteria->complete_quest.questID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
				if( achievementCriteria->gain_reputation.factionID == static_cast<uint32>( miscvalue1 ))
				{
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
				if( achievementCriteria->learn_spell.spellID == static_cast<uint32>( miscvalue1 ))
				{
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
				// Vanity pets owned - miscvalue1==778
				// Number of mounts  - miscvalue1==777
				if( achievementCriteria->number_of_mounts.unknown == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
			case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
				if( achievementCriteria->be_spell_target.spellID == static_cast<uint32>( miscvalue1))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
				if( achievementCriteria->kill_creature.creatureID == static_cast<uint32>( miscvalue1 ))
				{
					switch(achievement->ID)
					{
						case 2556: // Pest Control
							if(   (miscvalue1== 3300 && achievementCriteria->index== 1)   // Adder
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
// Kill creature X in Heroic dungeon
						case 489: // Heroic: Utgarde Keep
						case 490: // Heroic: The Nexus
						case 491: // Heroic: Azjol-Nerub
						case 492: // Heroic: Ahn'kahet: The Old Kingdom
						case 493: // Heroic: Drak'Tharon Keep
						case 494: // Heroic: The Violet Hold
						case 495: // Heroic: Gundrak
						case 496: // Heroic: Halls of Stone
						case 497: // Heroic: Halls of Lightning
						case 498: // Heroic: The Oculus
						case 499: // Heroic: Utgarde Pinnacle
						case 500: // Heroic: The Culling of Stratholme
						case 563: // Heroic: The Arachnid Quarter
						case 565: // Heroic: The Construct Quarter
						case 567: // Heroic: The Plague Quarter
						case 569: // Heroic: The Military Quarter
						case 573: // Heroic: Sapphiron's Demise
						case 575: // Heroic: Kel'Thuzad's Defeat
						case 577: // Heroic: The Fall of Naxxramas
						case 623: // Heroic: The Spellweaver's Downfall
						case 625: // Heroic: Besting the Black Dragonflight
						case 667: // Heroic: Hellfire Ramparts
						case 668: // Heroic: The Blood Furnace
						case 669: // Heroic: The Slave Pens
						case 670: // Heroic: Underbog
						case 671: // Heroic: Mana-Tombs
						case 672: // Heroic: Auchenai Crypts
						case 673: // Heroic: The Escape From Durnholde
						case 674: // Heroic: Sethekk Halls
						case 675: // Heroic: Shadow Labyrinth
						case 676: // Heroic: Opening of the Dark Portal
						case 677: // Heroic: The Steamvault
						case 678: // Heroic: The Shattered Halls
						case 679: // Heroic: The Mechanar
						case 680: // Heroic: The Botanica
						case 681: // Heroic: The Arcatraz
						case 682: // Heroic: Magister's Terrace
						case 1312: // Utgarde Keep bosses on Heroic Difficulty.
						case 1504: // Ingvar the Plunderer kills (Heroic Utgarde Keep)
						case 1505: // Keristrasza kills (Heroic Nexus)
						case 1506: // Anub'arak kills (Heroic Azjol-Nerub)
						case 1507: // Herald Volazj kills (Heroic Ahn'kahet)
						case 1508: // The Prophet Tharon'ja kills (Heroic Drak'Tharon Keep)
						case 1509: // Cyanigosa kills (Heroic Violet Hold)
						case 1510: // Gal'darah kills (Heroic Gundrak)
						case 1511: // Sjonnir the Ironshaper kills (Heroic Halls of Stone)
						case 1512: // Loken kills (Heroic Halls of Lightning)
						case 1513: // Ley-Guardian Eregos kills (Heroic Oculus)
						case 1514: // King Ymiron kills (Heroic Utgarde Pinnacle)
						case 1515: // Mal'Ganis defeated (Heroic CoT: Stratholme)
						case 1721: // Heroic: Archavon the Stone Watcher
						case 1817: // The Culling of Time
						case 1865: // Lockdown!
							if( GetPlayer()->iInstanceType >= MODE_HEROIC )
							{
								UpdateCriteriaProgress(achievementCriteria, 1);
							}
							break;
// TODO: More complicated achievements: time limits, group size limits, other criteria...
						case 1870: // Heroic: A Poke In The Eye
							// Defeat Malygos on Heroic Difficulty with fewer than 21.
						case 2056: // Volunteer Work
							// Defeat Jedoga Shadowseeker in Ahn'kahet on Heroic Difficulty without killing any Twilight Volunteers.
						case 1875: // Heroic: You Don't Have An Eternity
							// Defeat Malygos in 6 minutes or less on Heroic Difficulty.
						case 2185: // Heroic: Just Can't Get Enough
							// Defeat Kel'Thuzad on Heroic Difficulty in Naxxramas while killing at least 18 abominations in his chamber.
						case 1862: // Volazj's Quick Demise
							// Defeat Herald Volazj in Ahn'kahet on Heroic Difficulty in 2 minutes or less.
						case 2186: // The Immortal
							// Within one raid lockout period, defeat every boss in Naxxramas on Heroic Difficulty without allowing any raid member to die during any of the boss encounters.
						case 2038: // Respect Your Elders
							// Defeat Elder Nadox in Ahn'kahet on Heroic Difficulty without killing any Ahn'kahar Guardians.
						case 2183: // Heroic: Spore Loser
							// Defeat Loatheb in Naxxramas on Heroic Difficulty without killing any spores.
						case 1297: // Hadronox Denied
							// Defeat Hadronox in Azjol-Nerub on Heroic Difficulty before he webs the top doors and prevents more creatures from spawning.
						case 2177: // Heroic: And They Would All Go Down Together
							// Defeat the 4 Horsemen in Naxxramas on Heroic Difficulty, ensuring that they all die within 15 seconds of each other.
						case 1860: // Gotta Go!
							// Defeat Anub'arak in Azjol-Nerub on Heroic Difficulty in 2 minutes or less.
						case 2147: // Heroic: The Hundred Club
							// Defeat Sapphiron on Heroic Difficulty in Naxxramas without any member of the raid having a frost resist value over 100.
						case 1861: // The Party's Over
							// Defeat Prince Taldaram in Ahn'kahet on Heroic Difficulty with less than 5 people.
						case 2181: // Heroic: Subtraction
							// Defeat Thaddius in Naxxramas on Heroic Difficulty with less than 21 people.
						case 579: // Heroic: The Dedicated Few
							// Defeat the bosses of Naxxramas with less than 21 people in the zone on Heroic Difficulty.
						case 1296: // Watch Him Die
							// Defeat Krik'thir the Gatewatcher in Azjol-Nerub on Heroic Difficulty while Watcher Gashra, Watcher Narjil and Watcher Silthik are still alive.
						case 1589: // Heroic: Arachnophobia
							// Kill Maexxna in Naxxramas within 20 minutes of Anub'Rekhan's death on Heroic Difficulty.
						case 1857: // Heroic: Make Quick Werk Of Him
							// Kill Patchwerk in Naxxramas in 3 minutes or less on Heroic Difficulty.
						case 1877: // Heroic: Less Is More
							// Defeat Sartharion the Onyx Guardian and the Twilight Drakes on Heroic Difficulty with fewer than 21.
						case 1919: // On The Rocks
							// Defeat Prince Keleseth in Utgarde Keep on Heroic Difficulty without shattering any Frost Tombs.
						case 2036: // Intense Cold
							// Defeat Keristrasza in The Nexus on Heroic Difficulty without allowing Intense Cold to reach more than two stacks.
						case 2139: // Heroic: The Safety Dance
							// Defeat Heigan the Unclean in Naxxramas on Heroic Difficulty without anyone in the raid dying.
						case 2140: // Heroic: Momma Said Knock You Out
							// Defeat Grand Widow Faerlina in Naxxramas on Heroic Difficulty without dispelling frenzy.
						case 2150: // Split Personality
							// Defeat Grand Magus Telestra in The Nexus on Heroic Difficulty after having killed her images within 5 seconds of each other during both splits.
						case 2151: // Consumption Junction
							// Defeat Trollgore in Drak'Tharon Keep on Heroic Difficulty before Consume reaches ten stacks.
						case 2179: // Heroic: Shocking!
							// Defeat Thaddius in Naxxramas on Heroic Difficulty without anyone in the raid crossing the negative and positive charges.
						case 2037: // Chaos Theory
							// Defeat Anomalus in The Nexus on Heroic Difficulty without destroying any Chaotic Rifts.
						case 2039: // Better Off Dred
							// Engage King Dred in Drak'Tharon Keep on Heroic Difficulty and slay 6 Drakkari Gutrippers or Drakkari Scytheclaw during his defeat.
						case 2048: // Heroic: Gonna Go When the Volcano Blows
							// Defeat Sartharion the Onyx Guardian on Heroic Difficulty without getting hit by Lava Strike.
						case 2057: // Oh Novos!
							// Defeat Novos the Summoner in Drak'Tharon Keep on Heroic Difficulty without allowing any undead minions to reach the floor.
						case 1816: // Defenseless
							// Defeat Cyanigosa in The Violet Hold without using Defense Control Crystals and with Prison Seal Integrity at 100% while in Heroic Difficulty.
						case 2052: // Heroic: Twilight Assist
							// With at least one Twilight Drake still alive, engage and defeat Sartharion the Onyx Guardian on Heroic Difficulty.
						case 2053: // Heroic: Twilight Duo
							// With at least two Twilight Drakes still alive, engage and defeat Sartharion the Onyx Guardian on Heroic Difficulty.
						case 2041: // Dehydration
							// Defeat Ichoron in the Violet Hold on Heroic Difficulty without allowing any Ichor Globules to merge.
						case 2054: // Heroic: The Twilight Zone
							// With all three Twilight Drakes still alive, engage and defeat Sartharion the Onyx Guardian on Heroic Difficulty.
						case 1864: // What the Eck?
							// Defeat Gal'darah in Gundrak on Heroic Difficulty while under the effects of Eck Residue.
						case 2152: // Share The Love
							// Defeat Gal'darah in Gundrak on Heroic Difficulty and have 5 unique party members get impaled throughout the fight.
						case 2040: // Less-rabi
							// Defeat Moorabi in Gundrak on Heroic Difficulty while preventing him from transforming into a mammoth at any point during the encounter.
						case 2058: // Snakes. Why'd It Have To Be Snakes?
							// Defeat Slad'ran in Gundrak on Heroic Difficulty without getting snake wrapped.
						case 1866: // Good Grief
							// Defeat the Maiden of Grief in the Halls of Stone on Heroic Difficulty in 1 minute or less.
						case 2155: // Abuse the Ooze
							// Defeat Sjonnir the Ironshaper in the Halls of Stone on Heroic Difficulty and kill 5 Iron Sludges during the encounter.
						case 2154: // Brann Spankin' New
							// Defeat the Tribunal of Ages encounter in the Halls of Stone on Heroic Difficulty without allowing Brann Bronzebeard to take any damage.
						case 1867: // Timely Death
							// Defeat Loken in the Halls of Lightning on Heroic Difficulty in 2 minutes or less.
						case 1834: //Lightning Struck
							// Defeat General Bjarngrim in the Halls of Lightning on Heroic Difficulty while he has a Temporary Electrical Charge.
						case 2042: // Shatter Resistant
							// Defeat Volkhan in the Halls of Lightning on Heroic Difficulty without allowing him to shatter more than 4 Brittle Golems.
						case 1872: // Zombiefest!
							// Kill 100 Risen Zombies in 1 minute in The Culling of Stratholme on Heroic Difficulty.
						case 2043: // The Incredible Hulk
							// Force Svala Sorrowgrave to kill a Scourge Hulk on Heroic Difficulty in Utgarde Pinnacle.
						case 1873: // Lodi Dodi We Loves the Skadi
							// Defeat Skadi the Ruthless in Utgarde Pinnacle on Heroic Difficulty within 3 minutes of starting the gauntlet event.
						case 2156: // My Girl Loves to Skadi All the Time
							// Defeat Skadi the Ruthless in Utgarde Pinnacle on Heroic Difficulty after having killed Grauf from 100% to dead in a single pass.
						case 2157: // King's Bane
							// Defeat King Ymiron in Utgarde Pinnacle on Heroic Difficulty without anyone in the party triggering Bane.
						case 1871: // Experienced Drake Rider
							// On three different visits to The Oculus, get credit for defeating Ley-Guardian Eregos while riding an Amber, Emerald, and Ruby drake on Heroic Difficulty.
						case 1868: // Make It Count
							// Defeat Ley-Guardian Eregos in The Oculus on Heroic Difficulty within 20 minutes of Drakos the Interrogator's death.
						case 2044: // Ruby Void
							// Defeat Ley-Guardian Eregos in The Oculus on Heroic Difficulty without anyone in your party using a Ruby Drake.
						case 2045: // Emerald Void
							// Defeat Ley-Guardian Eregos in The Oculus on Heroic Difficulty without anyone in your party using an Emerald Drake.
						case 2046: // Amber Void
							// Defeat Ley-Guardian Eregos in The Oculus on Heroic Difficulty without anyone in your party using an Amber Drake.
							break;
						default:
							if( !IS_INSTANCE(GetPlayer()->GetMapId()) || (GetPlayer()->iInstanceType == MODE_NORMAL) )
							{
								// already tested heroic achievements above, the rest should be normal or non-dungeon
								UpdateCriteriaProgress(achievementCriteria, 1);
							}
							break;
					}
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
				if( achievementCriteria->reach_skill_level.skillID == static_cast<uint32>( miscvalue1 ))
				{
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
				if( achievementCriteria->learn_skill_level.skillID == static_cast<uint32>( miscvalue1 ))
				{
					SetCriteriaProgress(achievementCriteria, miscvalue2);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
				if( achievementCriteria->equip_item.itemID == static_cast<uint32>( miscvalue1 ))
				{
					SetCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
				// Achievement ID:556 description Equip an epic item in every slot with a minimum item level of 213.
				// "213" value not found in achievement or criteria entries (dbc), have to hard-code it here? :(
				// Achievement ID:557 description Equip a superior item in every slot with a minimum item level of 187.
				// "187" value not found in achievement or criteria entries (dbc), have to hard-code it here? :(
				// AchievementType for both Achievement ID:556 (Equip epic items) and ID:557 (Equip superior items)
				//    is the same (47) ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM
				// Going to send item slot in miscvalue1 and item quality in miscvalue2 when calling UpdateAchievementCriteria.
				if( achievementCriteria->equip_epic_item.itemSlot == static_cast<uint32>( miscvalue1 ))
				{
					if( (achievementCriteria->referredAchievement == 556) && (miscvalue2 == ITEM_QUALITY_EPIC_PURPLE) )
					{
						SetCriteriaProgress(achievementCriteria, 1);
					}
					else if( (achievementCriteria->referredAchievement == 557) && (miscvalue2 == ITEM_QUALITY_RARE_BLUE) )
					{
						SetCriteriaProgress(achievementCriteria, 1);
					}
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
				if( achievementCriteria->do_emote.emoteID == static_cast<uint32>( miscvalue1 ))
				{
					// emote matches, check the achievement target ... (if required)
					Unit* pUnit = GetPlayer()->GetMapMgr()->GetUnit(selectedGUID);
					switch(achievement->ID)
					{
						case 1206: // To All The Squirrels I've Loved Before
							// requires a target
							if( pUnit )
							{
								uint32 ent = pUnit->GetEntry();
								if(   (ent== 1412 && achievementCriteria->index== 1)   // Squirrel
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
							// requires a target
							if( pUnit )
							{
								uint32 ent = pUnit->GetEntry();
								if(   (ent==29328 && achievementCriteria->index== 1)   // Arctic Hare
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
								if( pTarget && pTarget->IsPlayer() && pTarget->IsDead() && isHostile(pTarget, GetPlayer()) )
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

						// Statistics for emotes
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
						if( miscvalue1 == 30 )
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1496: // Arathi Basin Killing Blows
						if( miscvalue1 == 529 )
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1497: // Warsong Gulch Killing Blows
						if( miscvalue1 == 489 )
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1498: // Eye of the Storm Killing Blows
						if( miscvalue1 == 566 )
						{
							UpdateCriteriaProgress(achievementCriteria, 1);
						}
						break;
					case 1499: // Strand of the Ancients Killing Blows
						if( miscvalue1 == 607 )
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
			case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM: // itemID in miscvalue1
				if( achievementCriteria->use_item.itemID == static_cast<uint32>( miscvalue1 ))
				{
					switch(achievementCriteria->referredAchievement)
					{
						case 1281: // Shoot off 10 Red Rocket Clusters in 25 seconds or less
						case 1552: // Shoot off 10 Festival Firecrackers in 30 seconds or less
						case 1696: // Shoot off 10 Love Rockets in 20 seconds or less
						case 1781: // Get 10 critters in 3 minutes
						case 1791: // Hearthstone with kid out
							break;
						default:
							UpdateCriteriaProgress(achievementCriteria, 1);
							break;
					}
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
				// Total NPC kills, Kill an NPC that yields XP, Beasts, Critters killed, Demons, Dragonkin ...
				// miscvalue1 = killed creature high GUID
				// miscvalue2 = killed creature low GUID
				{
					uint64 crGUID = miscvalue1;
					crGUID <<= 32; // shift to high 32-bits
					crGUID += miscvalue2;
					Unit* pUnit = GetPlayer()->GetMapMgr()->GetUnit(crGUID);
					if( pUnit )
					{
						uint32 crType = UNIT_TYPE_NONE;
						bool crTotem = false;
						bool yieldXP = CalculateXpToGive( pUnit, GetPlayer() )  > 0;
						if( pUnit->IsCreature() )
						{
							crTotem = TO_CREATURE(pUnit)->IsTotem();
							crType = TO_CREATURE(pUnit)->GetCreatureInfo()->Type;
							if(     (achievementCriteria->ID == 4944)                             // Total NPC kills              refAch==1197
								|| ( (achievementCriteria->ID == 4946) && (yieldXP)            )   // Kill an NPC that yields XP   refAch==1198
								|| ( (achievementCriteria->ID == 4948) && (crType==UNIT_TYPE_BEAST)      )   // Beasts                       refAch== 107
								|| ( (achievementCriteria->ID == 4958) && (crType==UNIT_TYPE_CRITTER)    )   // Critters killed              refAch== 107
								|| ( (achievementCriteria->ID == 4949) && (crType==UNIT_TYPE_DEMON)      )   // Demons                       refAch== 107
								|| ( (achievementCriteria->ID == 4950) && (crType==UNIT_TYPE_DRAGONKIN) )   // Dragonkin                    refAch== 107
								|| ( (achievementCriteria->ID == 4951) && (crType==UNIT_TYPE_ELEMENTAL)  )   // Elemental                    refAch== 107
								|| ( (achievementCriteria->ID == 4952) && (crType==UNIT_TYPE_GIANT)      )   // Giant                        refAch== 107
								|| ( (achievementCriteria->ID == 4953) && (crType==UNIT_TYPE_HUMANOID)   )   // Humanoid                     refAch== 107
								|| ( (achievementCriteria->ID == 4954) && (crType==UNIT_TYPE_MECHANICAL) )   // Mechanical                   refAch== 107
								|| ( (achievementCriteria->ID == 4955) && (crType==UNIT_TYPE_UNDEAD)     )   // Undead                       refAch== 107
								|| ( (achievementCriteria->ID == 4956) && (crType==UNIT_TYPE_NONE)		 )   // Unspecified                  refAch== 107
								|| ( (achievementCriteria->ID == 4957) && (crTotem)            ) ) // Totems                       refAch== 107
							{
								UpdateCriteriaProgress(achievementCriteria, 1);
							}
						}
					}
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
				if( achievementCriteria->use_gameobject.goEntry == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
				// fall distance (>=65) has been checked before UpdateAchievementCriteria() call, but it is sent in miscvalue1 just in case "they" add more...
				if( achievement->ID == 1260 )
				{
					// Fall 65 yards without dying while completely smashed during the Brewfest Holiday.
					if( miscvalue2 == DRUNKEN_SMASHED )
					{
						// drunken state, "completely smashed"
						// TODO: Check if it is during the Brewfest Holiday ...
						UpdateCriteriaProgress(achievementCriteria, 1);
					}
				}
				else
				{
					// achievement->ID==964 // Fall 65 yards without dying.
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
				if( achievementCriteria->honorable_kill_at_area.areaID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
				if( achievementCriteria->hk_class.classID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
				if( achievementCriteria->hk_race.raceID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;
			case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
				if( achievementCriteria->death_at_map.mapID == static_cast<uint32>( miscvalue1 ))
				{
					UpdateCriteriaProgress(achievementCriteria, 1);
				}
				break;

			// these achievement criteria types simply update the progress by the value passed in miscvalue1
			case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
			case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
			case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
			case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
			case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
			case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
			case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
			case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
			case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
			case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
			case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
				UpdateCriteriaProgress(achievementCriteria, miscvalue1);
				break;
			//End of Achievement List
			default:
				return;
		}
		CompletedCriteria(achievementCriteria);
	}
}

/**
	Updates all achievement criteria of the specified type.
	This is only called from CheckAllAchievementCriteria(), during player login
*/
void AchievementMgr::UpdateAchievementCriteria(AchievementCriteriaTypes type)
{
	AchievementCriteriaEntryList const& achievementCriteriaList = objmgr.GetAchievementCriteriaByType(type);
	for(AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i!=achievementCriteriaList.end(); ++i)
	{
		AchievementCriteriaEntry const *achievementCriteria = (*i);

		AchievementEntry const *achievement = dbcAchievementStore.LookupEntryForced(achievementCriteria->referredAchievement);
		if( !achievement //|| IsCompletedCriteria(achievementCriteria)
			|| (achievement->flags & ACHIEVEMENT_FLAG_COUNTER)
			|| (achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE && !m_player->IsTeamHorde() )
			|| (achievement->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && !m_player->IsTeamAlliance() ) )
		{
			continue;
		}

		switch(type)
		{
		//Start of Achievement List
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
			SetCriteriaProgress(achievementCriteria, GetPlayer()->getLevel());
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
			if( GetPlayer()->HasOverlayUncovered(achievementCriteria->explore_area.areaReference) )
			{
				SetCriteriaProgress(achievementCriteria, 1);
			}
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
			if( m_completedAchievements.find(achievementCriteria->complete_achievement.linkedAchievement) != m_completedAchievements.end() )
			{
				SetCriteriaProgress(achievementCriteria, 1, true);
			}
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
					if( qst && qst->zone_id == achievementCriteria->complete_quests_in_zone.zoneID )
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
			if( GetPlayer()->HasSpell(achievementCriteria->learn_spell.spellID) )
			{
				SetCriteriaProgress(achievementCriteria, 1);
			}
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
			{
				// achievementCriteria field4 = 777 for mounts, 778 for companion pets
				SpellSet::iterator sl = GetPlayer()->mSpells.begin();
				uint32 nm = 0;
				while(sl != GetPlayer()->mSpells.end())
				{
					SpellEntry* sp = dbcSpell.LookupEntryForced(*sl);
					if( achievementCriteria->number_of_mounts.unknown == 777 && sp && sp->MechanicsType == MECHANIC_MOUNTED )
					{
						// mount spell
						++nm;
					}
					else if( achievementCriteria->number_of_mounts.unknown == 778 && sp && (sp->Effect[0]==SPELL_EFFECT_SUMMON) )
					{
						// Companion pet?
						// make sure it's a companion pet, not some other summon-type spell
						if( strncmp(sp->Description,"Right Cl", 8) == 0 )
						{
							// "Right Click to summon and dismiss " ...
							++nm;
						}
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
		case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
			SetCriteriaProgress(achievementCriteria, (uint8)(GetPlayer()->GetUInt32Value(PLAYER_BYTES_2) >> 16));
			break;
		//End of Achievement List
		default:
			break;
		}
		CompletedCriteria(achievementCriteria);
	}
}

/**
	True if the criteria has been completed; false if error; false if criteria has not been completed.
*/
bool AchievementMgr::IsCompletedCriteria(AchievementCriteriaEntry const* achievementCriteria)
{
	if( !achievementCriteria )
	{
		return false;
	}
	AchievementEntry const* achievement = dbcAchievementStore.LookupEntryForced(achievementCriteria->referredAchievement);
	if( achievement == NULL )
	{
		return false;
	}


	if( achievement->flags & ACHIEVEMENT_FLAG_COUNTER )
	{
		return false;
	}

	if( achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL) )
	{
		if( objmgr.allCompletedAchievements.find(achievement->ID)!=objmgr.allCompletedAchievements.end() )
		{
			return false;
		}
	}

	CriteriaProgressMap::iterator itr = m_criteriaProgress.find(achievementCriteria->ID);
	if( itr == m_criteriaProgress.end() )
	{
		return false;
	}

	CriteriaProgress *progress = itr->second;

	// 0 or negative, not completed.
	if( progress->counter < 1 )
	{
		return false;
	}

	uint32 progresscounter = (uint32)progress->counter;
	switch(achievementCriteria->requiredType)
	{
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
			if( ( achievement->ID == 467 && GetPlayer()->getClass() != SHAMAN ) ||
				( achievement->ID == 466 && GetPlayer()->getClass() != DRUID ) ||
				( achievement->ID == 465 && GetPlayer()->getClass() != PALADIN ) ||
				( achievement->ID == 464 && GetPlayer()->getClass() != PRIEST ) ||
				( achievement->ID == 463 && GetPlayer()->getClass() != WARLOCK ) ||
				( achievement->ID == 462 && GetPlayer()->getClass() != HUNTER ) ||
				( achievement->ID == 461 && GetPlayer()->getClass() != DEATHKNIGHT ) ||
				( achievement->ID == 460 && GetPlayer()->getClass() != MAGE ) ||
				( achievement->ID == 459 && GetPlayer()->getClass() != WARRIOR ) ||
				( achievement->ID == 458 && GetPlayer()->getClass() != ROGUE ) ||
				( achievement->ID == 1404 && GetPlayer()->getRace() != RACE_GNOME ) ||
				( achievement->ID == 1405 && GetPlayer()->getRace() != RACE_BLOODELF ) ||
				( achievement->ID == 1406 && GetPlayer()->getRace() != RACE_DRAENEI ) ||
				( achievement->ID == 1407 && GetPlayer()->getRace() != RACE_DWARF ) ||
				( achievement->ID == 1408 && GetPlayer()->getRace() != RACE_HUMAN ) ||
				( achievement->ID == 1409 && GetPlayer()->getRace() != RACE_NIGHTELF ) ||
				( achievement->ID == 1410 && GetPlayer()->getRace() != RACE_ORC ) ||
				( achievement->ID == 1411 && GetPlayer()->getRace() != RACE_TAUREN ) ||
				( achievement->ID == 1412 && GetPlayer()->getRace() != RACE_TROLL ) ||
				( achievement->ID == 1413 && GetPlayer()->getRace() != RACE_UNDEAD ) )
			{
				return false;
			}
			return progresscounter >= achievementCriteria->reach_level.level;
			break;
		case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
		case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
			return progresscounter >= achievementCriteria->loot_item.itemCount;
		case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
			return progresscounter >= achievementCriteria->loot_money.goldInCopper;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
			return progresscounter >= achievementCriteria->complete_quest_count.totalQuestCount;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
			return progresscounter >= achievementCriteria->complete_quests_in_zone.questCount;
		case ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD:
			return progresscounter >= achievementCriteria->quest_reward_money.goldInCopper;
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
			return progresscounter >= achievementCriteria->gain_reputation.reputationAmount;
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
			return progresscounter >= achievementCriteria->gain_exalted_reputation.numberOfExaltedFactions;
		case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
			return progresscounter >= achievementCriteria->number_of_mounts.mountCount;
		case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
		case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
			return progresscounter >= achievementCriteria->be_spell_target.spellCount;
		case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
			return progresscounter >= achievementCriteria->kill_creature.creatureCount;
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
			return progresscounter >= achievementCriteria->reach_skill_level.skillLevel;
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
			return progresscounter >= achievementCriteria->learn_skill_level.skillLevel;
		case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
			return progresscounter >= achievementCriteria->use_item.itemCount;
		case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
			return progresscounter >= achievementCriteria->use_gameobject.useCount;
		case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
			return progresscounter >= achievementCriteria->buy_bank_slot.numberOfSlots;
		case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
			return progresscounter >= achievementCriteria->honorable_kill.killCount;
		case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
			return progresscounter >= achievementCriteria->honorable_kill_at_area.killCount;
		case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
			return progresscounter >= achievementCriteria->hk_class.count;
		case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
			return progresscounter >= achievementCriteria->hk_race.count;
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
			return m_completedAchievements.find(achievementCriteria->complete_achievement.linkedAchievement) != m_completedAchievements.end();

		// These achievements only require counter to be 1 (or higher)
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
		case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
		case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
		case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
		case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
		case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
			return progresscounter >= 1;

		// unknown or need to be finished:
		default:
			if( achievementCriteria->raw.field4 > 0 )
			{
				return progresscounter >= achievementCriteria->raw.field4;
			}
			break;
	}
	return false;
}

/**
	If achievement criteria has been completed, checks whether to complete the achievement too.
*/
void AchievementMgr::CompletedCriteria(AchievementCriteriaEntry const* criteria)
{
	if( !IsCompletedCriteria( criteria ) )
	{
		return;
	}
	AchievementEntry const* achievement = dbcAchievementStore.LookupEntry(criteria->referredAchievement);

	if( criteria->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL || GetAchievementCompletionState(achievement) == ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED )
	{
		CompletedAchievement(achievement);
	}
}

/**
	Returns the completion state of the achievement.
	ACHIEVEMENT_COMPLETED_COMPLETED_STORED: Achievement has been completed and stored already.
	ACHIVEMENT_COMPLETED_COMPLETED_NOT_STORED: Achievement has been completed but not stored yet.
	ACHIEVEMENT_COMPLETED_NONE: Achievement has not been completed yet.
*/
AchievementCompletionState AchievementMgr::GetAchievementCompletionState(AchievementEntry const* entry)
{
	if( m_completedAchievements.find(entry->ID)!=m_completedAchievements.end() )
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_STORED;
	}

	uint32 completedCount = 0;
	bool foundOutstanding = false;
	for ( uint32 rowId = 0; rowId<dbcAchievementCriteriaStore.GetNumRows(); ++rowId )
	{
		AchievementCriteriaEntry const* criteria = dbcAchievementCriteriaStore.LookupRowForced(rowId);
		if( criteria == NULL || criteria->referredAchievement!= entry->ID )
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
	if( !foundOutstanding )
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
	}

	if( (entry->count > 1) && (completedCount >= entry->count) )
	{
		return ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED;
	}
	return ACHIEVEMENT_COMPLETED_NONE;
}

/**
	Sets progress of the achievement criteria.
	If relative argument is true, this behaves the same as UpdateCriteriaProgress.
*/
void AchievementMgr::SetCriteriaProgress(AchievementCriteriaEntry const* entry, int32 newValue, bool relative)
{
	CriteriaProgress *progress = NULL;

	if( m_criteriaProgress.find(entry->ID) == m_criteriaProgress.end() )
	{
		if( newValue < 1 )
		{
			return;
		}
		progress = new CriteriaProgress(entry->ID, newValue);
		m_criteriaProgress[entry->ID]=progress;
	}
	else
	{
		progress = m_criteriaProgress[entry->ID];
		if( progress->counter == newValue )
		{
			return;
		}
		progress->counter = newValue;
	}
	if( progress->counter > 0 )
	{
		// Send update only if criteria is started (counter > 0)
		SendCriteriaUpdate( progress );
	}
}

/**
	Updates progress of the achievement criteria.
	updateByValue is added to the current progress counter.
*/
void AchievementMgr::UpdateCriteriaProgress(AchievementCriteriaEntry const* entry, int32 updateByValue)
{
	CriteriaProgress *progress = NULL;

	if( m_criteriaProgress.find(entry->ID) == m_criteriaProgress.end() )
	{
		if( updateByValue < 1 )
		{
			return;
		}
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

/**
	Completes the achievement for the player.
*/
void AchievementMgr::CompletedAchievement(AchievementEntry const* achievement)
{
	if( achievement->flags & ACHIEVEMENT_FLAG_COUNTER || m_completedAchievements.find(achievement->ID)!=m_completedAchievements.end() )
	{
		return;
	}

	if( ShowCompletedAchievement(achievement->ID, GetPlayer()) )
	{
		SendAchievementEarned( achievement );
	}
	m_completedAchievements[achievement->ID] = time(NULL);

	objmgr.allCompletedAchievements.insert( achievement->ID );
	UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT );

	// check for reward
	GiveAchievementReward(achievement);
}

/**
	Sends all achievement data to the player. Also used for achievement inspection.
*/
void AchievementMgr::SendAllAchievementData(Player* player)
{
	// maximum size for the SMSG_ALL_ACHIEVEMENT_DATA packet without causing client problems seems to be 0x7fff
	uint32 packetSize = 18 + ((uint32)m_completedAchievements.size() * 8) + (GetCriteriaProgressCount() * 36);
	bool doneCompleted = false;
	bool doneProgress = false;
	AchievementCriteriaEntry const* acEntry;
	AchievementEntry const* achievement;

	WorldPacket data;
	if( packetSize < 0x8000 )
	{
		data.resize( packetSize );
	}
	else
	{
		data.resize( 0x7fff );
	}

	CompletedAchievementMap::iterator completeIter = m_completedAchievements.begin();
	CriteriaProgressMap::iterator progressIter = m_criteriaProgress.begin();
	bool packetFull;

	while( !doneCompleted || !doneProgress )
	{
		data.clear();
		if( player == m_player )
		{
			data.SetOpcode ( SMSG_ALL_ACHIEVEMENT_DATA );
		}
		else
		{
			data.SetOpcode( SMSG_RESPOND_INSPECT_ACHIEVEMENTS );
			FastGUIDPack(data, m_player->GetGUID());
		}
		packetFull = false;

		// add the completed achievements
		if( !doneCompleted )
		{
			for( ; completeIter != m_completedAchievements.end() && !packetFull; ++completeIter )
			{
				if( ShowCompletedAchievement(completeIter->first, m_player) )
				{
					data << uint32(completeIter->first);
					data << uint32(secsToTimeBitFields(completeIter->second));
				}
				packetFull = data.size() > 0x7f00;
			}
			if( completeIter == m_completedAchievements.end() )
			{
				doneCompleted = true;
			}
		}
		
		// 0xffffffff separates between completed achievements and ones in progress
		data << int32(-1);
		for(; progressIter != m_criteriaProgress.end() && !packetFull; ++progressIter)
		{
			acEntry = dbcAchievementCriteriaStore.LookupEntryForced(progressIter->first);
			if( !acEntry )
			{
				continue;
			}
			achievement = dbcAchievementStore.LookupEntryForced(acEntry->referredAchievement);
			if( !achievement )
			{
				continue;
			}
			// achievement progress to send to self
			if( player == m_player )
			{
				if( SendAchievementProgress(progressIter->second) )
				{
					data << uint32(progressIter->first);
					data.appendPackGUID(progressIter->second->counter);
					data << GetPlayer()->GetNewGUID();
					data << uint32(0);
					data << uint32(secsToTimeBitFields(progressIter->second->date));
					data << uint32(0);
					data << uint32(0);
				}
			}
			// achievement progress to send to other players (inspect)
			else
			{
				// only send statistics, no other unfinished achievement progress, since client only displays them as completed or not completed
				if( (progressIter->second->counter > 0) && (achievement->flags & ACHIEVEMENT_FLAG_COUNTER) )
				{
					data << uint32(progressIter->first);
					data.appendPackGUID(progressIter->second->counter);
					data << GetPlayer()->GetNewGUID();
					data << uint32(0);
					data << uint32(secsToTimeBitFields(progressIter->second->date));
					data << uint32(0);
					data << uint32(0);
				}
			}
			packetFull = data.size() > 0x7f00;
		}
		if( progressIter == m_criteriaProgress.end() )
		{
			doneProgress = true;
		}

		// another 0xffffffff denotes end of the packet
		data << int32(-1);
		player->GetSession()->SendPacket(&data);
	}
	if( isCharacterLoading && player == m_player )
	{
		// a SMSG_ALL_ACHIEVEMENT_DATA packet has been sent to the player, so the achievement manager can send SMSG_CRITERIA_UPDATE and SMSG_ACHIEVEMENT_EARNED when it gets them
		isCharacterLoading = false;
	}
}

/**
	Returns the number of achievement progresses that get sent to the player.
*/
uint32 AchievementMgr::GetCriteriaProgressCount()
{
	uint32 criteriapc = 0;
	for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
	{
		//AchievementEntry const *achievement = dbcAchievementStore.LookupEntry(iter->second->id);
		if( SendAchievementProgress(iter->second) )
		{
			++criteriapc;
		}
	}
	return criteriapc;
}

/**
	Gives reward to player for completing the achievement.
*/
void AchievementMgr::GiveAchievementReward(AchievementEntry const* entry)
{
	if( entry == NULL || isCharacterLoading )
	{
		return;
	}
	AchievementReward r;
	r.type = ACHIEVEMENT_REWARDTYPE_NONE;
	r.itemId = 0;
	r.rankId = 0;
	r.spellId = 0;

	if( strlen(entry->rewardName) > 0 )
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
				r.rankId = (GetPlayer()->getGender()==1) ? /* Matron */ PVPTITLE_MATRON : /* Patron */ PVPTITLE_PATRON;
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
				r.rankId = GetPlayer()->IsTeamHorde() ? /* Horde */ PVPTITLE_OF_THE_HORDE : /* Alliance */ PVPTITLE_OF_THE_ALLIANCE;
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
				r.itemId = 43349;
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
		if( r.type & ACHIEVEMENT_REWARDTYPE_TITLE )
		{
			GetPlayer()->SetKnownTitle(static_cast< RankTitles >(r.rankId), true);
			GetPlayer()->SetChosenTitle(0 );
		}
		if( r.type & ACHIEVEMENT_REWARDTYPE_ITEM )
		{
			// How does this work? Add item directly to inventory, or send through mail?
			ItemPrototype* it = ItemPrototypeStorage.LookupEntry(r.itemId);
			if( it )
			{
				Item *item;
				item = objmgr.CreateItem(r.itemId, GetPlayer());
				if( item == NULL )
				{
					// this is bad - item not found in db or unable to be created for some reason
					GetPlayer()->GetSession()->SendNotification("Unable to create item with id %lu!", r.itemId);
					return;
				}
				item->SetStackCount(  1);
				if( it->Bonding==ITEM_BIND_ON_PICKUP )
				{
					if( it->Flags & ITEM_FLAG_ACCOUNTBOUND )
					{
						// any "accountbound" items for achievement rewards?  maybe later...
						item->AccountBind();
					}
					else
					{
						item->SoulBind();
					}
				}

				if( !GetPlayer()->GetItemInterface()->AddItemToFreeSlot(item) )
				{
					// this is bad. inventory full. maybe we should mail it instead?
					GetPlayer()->GetSession()->SendNotification("No free slots were found in your inventory!");
					item->DeleteMe();
					return;
				}
			}
		}
		if( r.type & ACHIEVEMENT_REWARDTYPE_SPELL )
		{
			GetPlayer()->addSpell(r.spellId);
		}
	}
}

/**
	Returns the number of completed achievements.
*/
uint32 AchievementMgr::GetCompletedAchievementsCount() const
{
	return (uint32)m_completedAchievements.size();
}

/**
	GM has used a command to make the specified achievement to be completed.
	If achievementID is -1, all achievements available for the player's faction get marked as completed
	Returns true if able to complete specified achievement successfully.
	Returns false if there is any error (already completed, not found, ...)
*/
bool AchievementMgr::GMCompleteAchievement(WorldSession* gmSession, int32 achievementID)
{
	if( achievementID == -1 )
	{
		uint32 nr = dbcAchievementStore.GetNumRows();
		AchievementEntry const* ach;
		for(uint32 i = 0; i < nr; ++i)
		{
			ach = dbcAchievementStore.LookupRowForced(i);
			if( ach == NULL )
			{
				m_player->GetSession()->SystemMessage("Achievement %lu entry not found.", i);
			}
			else
			{
				if( !(ach->flags & ACHIEVEMENT_FLAG_COUNTER) )
				{
					if( (ach->factionFlag == ACHIEVEMENT_FACTION_FLAG_HORDE && !m_player->IsTeamHorde() ) ||
						(ach->factionFlag == ACHIEVEMENT_FACTION_FLAG_ALLIANCE && !m_player->IsTeamAlliance() ) )
					{
						continue;
					}
					CompletedAchievement(ach);
				}
			}
		}
		m_player->GetSession()->SystemMessage("All achievements completed.");
		return true;
	}
	if( m_completedAchievements.find(achievementID) != m_completedAchievements.end() )
	{
		gmSession->SystemMessage("Player has already completed that achievement.");
		return false;
	}
	AchievementEntry const* achievement = dbcAchievementStore.LookupEntryForced(achievementID);
	if( !achievement )
	{
		gmSession->SystemMessage("Achievement %lu entry not found.", achievementID);
		return false;
	}
	if( achievement->flags & ACHIEVEMENT_FLAG_COUNTER )
	{
		gmSession->SystemMessage("Achievement (%lu) |Hachievement:%lu:"I64FMT":0:0:0:-1:0:0:0:0|h[%s]|h is a counter and cannot be completed.",
			achievement->ID, achievement->ID, gmSession->GetPlayer()->GetGUID(), achievement->name);
		return false;
	}
	CompletedAchievement(achievement);
	return true;
}

/**
	GM has used a command to make the specified achievement criteria to be completed.
	If criteriaID is -1, all achievement criteria get marked as completed
	Returns true if able to complete specified achievement criteria successfully.
	Returns false if there is any error (already completed not found, ...)
*/
bool AchievementMgr::GMCompleteCriteria(WorldSession* gmSession, int32 criteriaID)
{
	if( criteriaID == -1 )
	{
		uint32 nr = dbcAchievementCriteriaStore.GetNumRows();
		AchievementCriteriaEntry const* crt;
		for(uint32 i = 0, j = 0; j < nr; ++i)
		{
			crt = dbcAchievementCriteriaStore.LookupRowForced(i);
			if( crt == NULL )
			{
				sLog.outError("Achievement Criteria %lu entry not found.", i);
				continue;
			}
			++j;
			if( crt->raw.field4 )
			{
				SetCriteriaProgress( crt, crt->raw.field4 );
				CompletedCriteria(crt);
			}
		}
		m_player->GetSession()->SystemMessage("All achievement criteria completed.");
		return true;
	}
	AchievementCriteriaEntry const* criteria = dbcAchievementCriteriaStore.LookupEntryForced(criteriaID);
	if( !criteria )
	{
		gmSession->SystemMessage("Achievement criteria %lu not found.", criteriaID);
		return false;
	}
	if( IsCompletedCriteria(criteria) )
	{
		gmSession->SystemMessage("Achievement criteria %lu already completed.", criteriaID);
		return false;
	}
	AchievementEntry const* achievement = dbcAchievementStore.LookupEntryForced(criteria->referredAchievement);
	if( !achievement )
	{
		// achievement not found
		gmSession->SystemMessage("Referred achievement (%lu) entry not found.", criteria->referredAchievement);
		return false;
	}
	if( achievement->flags & ACHIEVEMENT_FLAG_COUNTER )
	{
		// can't complete this type of achivement (counter)
		gmSession->SystemMessage("Referred achievement (%lu) |Hachievement:%lu:"I64FMT":0:0:0:-1:0:0:0:0|h[%s]|h is a counter and cannot be completed.",
			achievement->ID, achievement->ID, gmSession->GetPlayer()->GetGUID(), achievement->name);
		return false;
	}

	CriteriaProgressMap::iterator itr = m_criteriaProgress.find(criteriaID);
	CriteriaProgress *progress;
	if( itr == m_criteriaProgress.end() )
	{
		// not in progress map
		progress = new CriteriaProgress(criteriaID, 0);
		m_criteriaProgress[criteriaID]=progress;
	}
	else
	{
		progress = itr->second;
	}

	progress->counter = criteria->raw.field4;
	SendCriteriaUpdate( progress );
	CompletedCriteria(criteria);
	return true;
}

/**
	GM has used a command to reset achievement(s) for this player.
	If achievementID is -1, all achievements get reset, otherwise only the one specified gets reset.
*/
void AchievementMgr::GMResetAchievement(int32 achievementID)
{
	std::ostringstream ss;
	if( achievementID == -1 )
	{
		// reset all achievements
		CompletedAchievementMap::iterator itr = m_completedAchievements.begin();
		for(; itr != m_completedAchievements.end(); ++itr)
		{
			WorldPacket resetData(SMSG_ACHIEVEMENT_DELETED, 4);
			resetData << uint32(itr->first);
			GetPlayer()->GetSession()->SendPacket(&resetData);
		}
		m_completedAchievements.clear();
		ss << "DELETE FROM character_achievement WHERE guid = " << m_player->GetLowGUID();
		CharacterDatabase.Execute( ss.str().c_str() );
	}
	else // reset a single achievement
	{
		WorldPacket resetData(SMSG_ACHIEVEMENT_DELETED, 4);
		resetData << uint32(achievementID);
		GetPlayer()->GetSession()->SendPacket(&resetData);
		m_completedAchievements.erase(achievementID);
		ss << "DELETE FROM character_achievement WHERE guid = " << m_player->GetLowGUID() << " AND achievement = " << achievementID;
		CharacterDatabase.Execute( ss.str().c_str() );
	}
}

/**
	GM has used a command to reset achievement criteria for this player.
	If criteriaID is -1, all achievement criteria get reset, otherwise only the one specified gets reset.
*/
void AchievementMgr::GMResetCriteria(int32 criteriaID)
{
	std::ostringstream ss;
	if( criteriaID == -1 )
	{
		// reset all achievement criteria
		for(CriteriaProgressMap::iterator iter = m_criteriaProgress.begin(); iter!=m_criteriaProgress.end(); ++iter)
		{
			WorldPacket resetData(SMSG_CRITERIA_DELETED, 4);
			resetData << uint32(iter->first);
			GetPlayer()->GetSession()->SendPacket(&resetData);
			delete iter->second;
		}
		m_criteriaProgress.clear();
		ss << "DELETE FROM character_achievement_progress WHERE guid = " << m_player->GetLowGUID();
		CharacterDatabase.Execute( ss.str().c_str() );
	}
	else // reset a single achievement criteria
	{
		WorldPacket resetData(SMSG_CRITERIA_DELETED, 4);
		resetData << uint32(criteriaID);
		GetPlayer()->GetSession()->SendPacket(&resetData);
		m_criteriaProgress.erase(criteriaID);
		ss << "DELETE FROM character_achievement_progress WHERE guid = " << m_player->GetLowGUID() << " AND criteria = " << criteriaID;
		CharacterDatabase.Execute( ss.str().c_str() );
	}
	CheckAllAchievementCriteria();
}

/**
	Date/Time (time_t) the achievement was completed, or 0 if achievement not completed yet
*/
time_t AchievementMgr::GetCompletedTime(AchievementEntry const* achievement)
{
	CompletedAchievementMap::iterator iter = m_completedAchievements.find(achievement->ID);
	if( iter != m_completedAchievements.end() )
	{
		// achievement is completed, return the date/time it was completed
		return iter->second;
	}
	return 0; // achievement not completed
}

/**
	true if achievementID has been completed by the player, false otherwise.
*/
bool AchievementMgr::HasCompleted(uint32 achievementID)
{
	return (m_completedAchievements.find(achievementID) != m_completedAchievements.end());
}

#endif
