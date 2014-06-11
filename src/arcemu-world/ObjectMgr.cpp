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

initialiseSingleton(ObjectMgr);

const char* NormalTalkMessage = "DMSG";

ObjectMgr::ObjectMgr() :
	m_hiItemGuid(0),
	m_hiGroupId(0),
	m_mailid(0),
	m_reportID(0),
	m_ticketid(0),
	m_setGUID(0),
	m_hiCorpseGuid(0),
	m_hiGuildId(0),
	m_hiPetGuid(0),
	m_hiArenaTeamId(0),
	m_hiPlayerGuid(1)
{
	memset(m_InstanceBossInfoMap , 0, sizeof(InstanceBossInfoMap*) * NUM_MAPS);
}


ObjectMgr::~ObjectMgr()
{
	Log.Notice("ObjectMgr", "Deleting Corpses...");
	CorpseCollectorUnload();

	Log.Notice("ObjectMgr", "Deleting Itemsets...");
	for(ItemSetContentMap::iterator i = mItemSets.begin(); i != mItemSets.end(); ++i)
		delete i->second;

	mItemSets.clear();

	Log.Notice("ObjectMgr", "Deleting PlayerCreateInfo...");
	for(PlayerCreateInfoMap::iterator i = mPlayerCreateInfo.begin(); i != mPlayerCreateInfo.end(); ++ i)
		delete i->second;

	mPlayerCreateInfo.clear();

	Log.Notice("ObjectMgr", "Deleting Guilds...");
	for(GuildMap::iterator i = mGuild.begin(); i != mGuild.end(); ++i)
		delete i->second;

	Log.Notice("ObjectMgr", "Deleting Vendors...");
	for(VendorMap::iterator i = mVendors.begin(); i != mVendors.end(); ++ i)
		delete i->second;

	Log.Notice("ObjectMgr", "Deleting Spell Override...");
	for(OverrideIdMap::iterator i = mOverrideIdMap.begin(); i != mOverrideIdMap.end(); ++i)
		delete i->second;

	Log.Notice("ObjectMgr", "Deleting Trainers...");
	for(TrainerMap::iterator i = mTrainers.begin(); i != mTrainers.end(); ++ i)
	{
		Trainer* t = i->second;
		if(t->UIMessage && t->UIMessage != (char*)NormalTalkMessage)
			delete [] t->UIMessage;
		delete t;
	}

	Log.Notice("ObjectMgr", "Deleting Level Information...");
	for(LevelInfoMap::iterator i = mLevelInfo.begin(); i != mLevelInfo.end(); ++i)
	{
		if (LevelMap* l = i->second)
		{
			for(LevelMap::iterator i2 = l->begin(); i2 != l->end(); ++i2)
				delete i2->second;

			l->clear();
			delete l;
		}
	}

	Log.Notice("ObjectMgr", "Deleting Waypoint Cache...");
	for(HM_NAMESPACE::hash_map<uint32, WayPointMap*>::iterator i = m_waypoints.begin(); i != m_waypoints.end(); ++i)
	{
		for(WayPointMap::iterator i2 = i->second->begin(); i2 != i->second->end(); ++i2)
			if((*i2))
				delete(*i2);

		delete i->second;
	}

	Log.Notice("ObjectMgr", "Deleting timed emote Cache...");
	for(HM_NAMESPACE::hash_map<uint32, TimedEmoteList*>::iterator i = m_timedemotes.begin(); i != m_timedemotes.end(); ++i)
	{
		for(TimedEmoteList::iterator i2 = i->second->begin(); i2 != i->second->end(); ++i2)
			if((*i2))
			{
				delete [](*i2)->msg;
				delete(*i2);
			}

		delete i->second;
	}

	Log.Notice("ObjectMgr", "Deleting NPC Say Texts...");
	for(uint32 i = 0 ; i < NUM_MONSTER_SAY_EVENTS ; ++i)
	{
		for(MonsterSayMap::iterator itr = mMonsterSays[i].begin(); itr != mMonsterSays[i].end(); ++itr)
		{
			if (NpcMonsterSay* p = itr->second)
			{
				for(uint32 j = 0; j < p->TextCount; ++j)
					free((char*)p->Texts[j]);
				delete [] p->Texts;
				free((char*)p->MonsterName);
				delete p;
			}
		}

		mMonsterSays[i].clear();
	}

	Log.Notice("ObjectMgr", "Deleting Charters...");
	for(uint8 i = 0; i < NUM_CHARTER_TYPES; ++i)
	{
		for(HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr =  m_charters[i].begin(); itr != m_charters[i].end(); ++itr)
			delete itr->second;
	}

	Log.Notice("ObjectMgr", "Deleting Reputation Tables...");
	for(ReputationModMap::iterator itr = this->m_reputation_creature.begin(); itr != m_reputation_creature.end(); ++itr)
	{
		ReputationModifier* mod = itr->second;
		mod->mods.clear();
		delete mod;
	}

	for(ReputationModMap::iterator itr = this->m_reputation_faction.begin(); itr != m_reputation_faction.end(); ++itr)
	{
		ReputationModifier* mod = itr->second;
		mod->mods.clear();
		delete mod;
	}

	for(HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*>::iterator itr = this->m_reputation_instance.begin(); itr != this->m_reputation_instance.end(); ++itr)
	{
		InstanceReputationModifier* mod = itr->second;
		mod->mods.clear();
		delete mod;
	}

	Log.Notice("ObjectMgr", "Deleting Groups...");
	for(GroupMap::iterator itr = m_groups.begin(); itr != m_groups.end(); ++itr)
	{
		if(Group* pGroup = itr->second)
		{
			for(uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i)
				if(SubGroup* pSubGroup = pGroup->GetSubGroup(i))
					pSubGroup->Disband();
			delete pGroup;
		}
	}

	Log.Notice("ObjectMgr", "Deleting Player Information...");
	for(HM_NAMESPACE::hash_map<uint32, PlayerInfo*>::iterator itr = m_playersinfo.begin(); itr != m_playersinfo.end(); ++itr)
	{
		itr->second->m_Group = NULL;
		free(itr->second->name);
		delete itr->second;
	}

	Log.Notice("ObjectMgr", "Deleting GM Tickets...");
	for(GmTicketList::iterator itr = GM_TicketList.begin(); itr != GM_TicketList.end(); ++itr)
		delete(*itr);

	Log.Notice("ObjectMgr", "Deleting Boss Information...");
	for(uint32 i = 0; i < NUM_MAPS; i++)
	{
		if(this->m_InstanceBossInfoMap[i])
		{
			for(InstanceBossInfoMap::iterator itr = this->m_InstanceBossInfoMap[i]->begin(); itr != m_InstanceBossInfoMap[i]->end(); ++itr)
				delete(*itr).second;
			delete this->m_InstanceBossInfoMap[i];
			this->m_InstanceBossInfoMap[i] = NULL;
		}
	}

	Log.Notice("ObjectMgr", "Deleting Arena Teams...");
	for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeams.begin(); itr != m_arenaTeams.end(); ++itr)
		delete(*itr).second;

	Log.Notice("ObjectMgr", "Deleting Profession Discoveries...");
	for(std::set<ProfessionDiscovery*>::iterator itr = ProfessionDiscoveryTable.begin(); itr != ProfessionDiscoveryTable.end(); itr++)
		delete(*itr);

	Log.Notice("ObjectMgr", "Deleting BroadCastStorages...");
	m_BCEntryStorage.clear();

	Log.Notice("ObjectMgr", "Deleting spell target constraints...");
	for(SpellTargetConstraintMap::iterator itr = m_spelltargetconstraints.begin(); itr != m_spelltargetconstraints.end(); ++itr)
		delete itr->second;

	m_spelltargetconstraints.clear();

	Log.Notice("ObjectMgr", "Cleaning up vehicle accessories...");
	for( std::map< uint32, std::vector< VehicleAccessoryEntry* >* >::iterator itr = vehicle_accessories.begin(); itr != vehicle_accessories.end(); ++itr ){
		std::vector< VehicleAccessoryEntry* > *v = itr->second;

		for( std::vector< VehicleAccessoryEntry* >::iterator itr2 = v->begin(); itr2 != v->end(); ++itr2 )
			delete *itr2;
		v->clear();

		delete v;
	}
	
	vehicle_accessories.clear();


	Log.Notice( "ObjectMgr", "Cleaning up worldstate templates" );
	for( std::map< uint32, std::multimap< uint32, WorldState >* >::iterator itr = worldstate_templates.begin(); itr != worldstate_templates.end(); ++itr )
	{
		itr->second->clear();
		delete itr->second;
	}

	worldstate_templates.clear();
}

//
// Groups
//

Group* ObjectMgr::GetGroupByLeader(Player* pPlayer)
{
	Group* ret = NULL;
	m_groupLock.AcquireReadLock();
	for(GroupMap::iterator itr = m_groups.begin(); itr != m_groups.end(); ++itr)
	{
		if(itr->second->GetLeader() == pPlayer->getPlayerInfo())
		{
			ret = itr->second;
			break;
		}
	}

	m_groupLock.ReleaseReadLock();
	return ret;
}

Group* ObjectMgr::GetGroupById(uint32 id)
{
	GroupMap::iterator itr = m_groups.find(id);
	Group* ret = NULL;
	m_groupLock.AcquireReadLock();
	if(itr != m_groups.end())
		ret = itr->second;

	m_groupLock.ReleaseReadLock();
	return ret;
}

//
// Player names
//
void ObjectMgr::DeletePlayerInfo(uint32 guid)
{
	HM_NAMESPACE::hash_map<uint32, PlayerInfo*>::iterator i = m_playersinfo.find(guid);
	playernamelock.AcquireWriteLock();
	if(i == m_playersinfo.end())
	{
		playernamelock.ReleaseWriteLock();
		return;
	}

	PlayerInfo* pl = i->second;
	if (!pl)
	{
		Log.Error("ObjectMgr:DeletePlayerInfo", "Cannot find player information for guid %u", guid);
		return;
	}
	
	if(pl->m_Group)
		pl->m_Group->RemovePlayer(pl);

	if(pl->guild)
	{
		if(pl->guild->GetGuildLeader() == pl->guid)
			pl->guild->Disband();
		else
			pl->guild->RemoveGuildMember(pl, NULL);
	}

	string pnam = string(pl->name);
	arcemu_TOLOWER(pnam);
	PlayerNameStringIndexMap::iterator i2 = m_playersInfoByName.find(pnam);
	if(i2 != m_playersInfoByName.end() && i2->second == pl)
		m_playersInfoByName.erase(i2);

	free(pl->name);
	delete i->second;
	m_playersinfo.erase(i);

	playernamelock.ReleaseWriteLock();
}

PlayerInfo* ObjectMgr::GetPlayerInfo(uint32 guid)
{
	PlayerInfo* rv = NULL;
	playernamelock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, PlayerInfo*>::iterator i = m_playersinfo.find(guid);
	if(i != m_playersinfo.end())
		rv = i->second;

	playernamelock.ReleaseReadLock();
	return rv;
}

void ObjectMgr::AddPlayerInfo(PlayerInfo* pn)
{
	playernamelock.AcquireWriteLock();
	m_playersinfo[pn->guid] =  pn ;
	string pnam = string(pn->name);
	arcemu_TOLOWER(pnam);
	m_playersInfoByName[pnam] = pn;
	playernamelock.ReleaseWriteLock();
}

void ObjectMgr::RenamePlayerInfo(PlayerInfo* pn, const char* oldname, const char* newname)
{
	playernamelock.AcquireWriteLock();
	string oldn = string(oldname);
	arcemu_TOLOWER(oldn);

	PlayerNameStringIndexMap::iterator itr = m_playersInfoByName.find(oldn);
	if(itr != m_playersInfoByName.end() && itr->second == pn)
	{
		string newn = string(newname);
		arcemu_TOLOWER(newn);
		m_playersInfoByName.erase(itr);
		m_playersInfoByName[newn] = pn;
	}

	playernamelock.ReleaseWriteLock();
}

void ObjectMgr::LoadSpellSkills()
{
//	int total = sSkillStore.GetNumRows();

	for(uint32 i = 0; i < dbcSkillLineSpell.GetNumRows(); i++)
	{
		if(skilllinespell* sp = dbcSkillLineSpell.LookupRowForced(i))
			mSpellSkills[sp->spell] = sp;
	}
	Log.Success("ObjectMgr", "Loaded %u spell skills.", mSpellSkills.size());
}

skilllinespell* ObjectMgr::GetSpellSkill(uint32 id)
{
	return mSpellSkills[id];
}

SpellEntry* ObjectMgr::GetNextSpellRank(SpellEntry* sp, uint32 level)
{
	// Looks for next spell rank
	if(!sp)
		return NULL;

	skilllinespell* skill = GetSpellSkill(sp->Id);
	if(skill && skill->next)
	{
		SpellEntry* sp1 = dbcSpell.LookupEntry(skill->next);
		if(sp1 && sp1->baseLevel <= level)   // check level
			return GetNextSpellRank(sp1, level);   // recursive for higher ranks
	}
	return sp;
}

void ObjectMgr::LoadPlayersInfo()
{
	Log.Notice("ObjectMgr", "Loading players...");
	if(QueryResult* result = CharacterDatabase.Query("SELECT guid,name,race,class,level,gender,zoneid,timestamp,acct FROM characters"))
	{
		do
		{
			Field* fields = result->Fetch();
			PlayerInfo* pn = new PlayerInfo;
			pn->guid = fields[0].GetUInt32();
			pn->name = strdup(fields[1].GetString());
			pn->race = fields[2].GetUInt8();
			pn->cl = fields[3].GetUInt8();
			pn->lastLevel = fields[4].GetUInt32();
			pn->gender = fields[5].GetUInt8();
			pn->lastZone = fields[6].GetUInt32();
			pn->lastOnline = fields[7].GetUInt32();
			pn->acct = fields[8].GetUInt32();
			pn->m_Group = 0;
			pn->subGroup = 0;
			pn->m_loggedInPlayer = NULL;
			pn->guild = NULL;
			pn->guildRank = NULL;
			pn->guildMember = NULL;

			// Raid & heroic Instance IDs
			// Must be done before entering world...
			if(QueryResult* result2 = CharacterDatabase.Query("SELECT instanceid, mode, mapid FROM instanceids WHERE playerguid = %u", pn->guid))
			{
				do
				{
					uint32 instanceId = result2->Fetch()[0].GetUInt32();
					uint32 mode = result2->Fetch()[1].GetUInt32();
					uint32 mapId = result2->Fetch()[2].GetUInt32();
					if(mode >= NUM_INSTANCE_MODES || mapId >= NUM_MAPS)
						continue;

					pn->savedInstanceIdsLock.Acquire();
					PlayerInstanceMap::iterator itr = pn->savedInstanceIds[mode].find(mapId);
					if(itr == pn->savedInstanceIds[mode].end())
						pn->savedInstanceIds[mode].insert(PlayerInstanceMap::value_type(mapId, instanceId));
					else
						(*itr).second = instanceId;

					//TODO: Instances not loaded yet ~.~
					//if(!sInstanceMgr.InstanceExists(mapId, pn->m_savedInstanceIds[mapId][mode]))
					//{
					//	pn->m_savedInstanceIds[mapId][mode] = 0;
					//	CharacterDatabase.Execute("DELETE FROM instanceids WHERE mapId = %u AND instanceId = %u AND mode = %u", mapId, instanceId, mode);
					//}

					pn->savedInstanceIdsLock.Release();
				}
				while(result2->NextRow());
				delete result2;
			}

			if(pn->race == RACE_HUMAN || pn->race == RACE_DWARF || pn->race == RACE_GNOME || pn->race == RACE_NIGHTELF || pn->race == RACE_DRAENEI)
				pn->team = TEAM_ALLIANCE;
			else
				pn->team = TEAM_HORDE;

			if(GetPlayerInfoByName(pn->name))
			{
				// gotta rename him
				char temp[300];
				snprintf(temp, 300, "%s__%X__", pn->name, pn->guid);
				Log.Notice("ObjectMgr", "Renaming duplicate player %s to %s. (%u)", pn->name, temp, pn->guid);
				CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s', forced_rename_pending = 1 WHERE guid = %u",
				                              CharacterDatabase.EscapeString(string(temp)).c_str(), pn->guid);

				free(pn->name);
				pn->name = strdup(temp);
			}

			string lpn = string(pn->name);
			arcemu_TOLOWER(lpn);
			m_playersInfoByName[lpn] = pn;

			//this is startup -> no need in lock -> don't use addplayerinfo
			m_playersinfo[(uint32)pn->guid] = pn;
		}
		while(result->NextRow());

		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u players.", m_playersinfo.size());
	LoadGuilds();
}

PlayerInfo* ObjectMgr::GetPlayerInfoByName(const char* name)
{
	string lpn = string(name);
	arcemu_TOLOWER(lpn);
	PlayerNameStringIndexMap::iterator i;
	PlayerInfo* rv = NULL;
	playernamelock.AcquireReadLock();

	i = m_playersInfoByName.find(lpn);
	if(i != m_playersInfoByName.end())
		rv = i->second;

	playernamelock.ReleaseReadLock();
	return rv;
}

#ifdef ENABLE_ACHIEVEMENTS
void ObjectMgr::LoadCompletedAchievements()
{
	if (QueryResult* result = CharacterDatabase.Query("SELECT achievement FROM character_achievement GROUP BY achievement"))
    {
        do
        {
            Field* fields = result->Fetch();
            allCompletedAchievements.insert(fields[0].GetUInt32());
        }
        while(result->NextRow());
        delete result;
    }
    Log.Success("ObjectMgr", "Loaded %u completed achievements", allCompletedAchievements.size());
}
#endif

void ObjectMgr::LoadPlayerCreateInfo()
{
	std::string query = "SELECT index, race, factionTemplate, class, mapid, zoneid, position_x, position_y, position_z, displayId,";
	query.append("BaseStrength, BaseAgility, BaseStamina, BaseIntellect, BaseSpirit, BaseHealth, BaseMana, BaseRage, BaseFocus,");
	query.append("BaseEnergy, attackpower, mindmg, maxdmg, introid, taximask FROM playercreateinfo");

	if (QueryResult* result = WorldDatabase.Query(query.c_str()))
	{
		do
		{
			Field* fields = result->Fetch();
			PlayerCreateInfo* pPlayerCreateInfo = new PlayerCreateInfo;

			pPlayerCreateInfo->index = fields[0].GetUInt8();
			pPlayerCreateInfo->race = fields[1].GetUInt8();
			if (pPlayerCreateInfo->race >= MAX_RACE || pPlayerCreateInfo->race == 10)
			{
				Log.Error("ObjectMgr", "Invalid race %u for playercreateinfo index %u.", pPlayerCreateInfo->race, pPlayerCreateInfo->index);
				continue;
			}

			pPlayerCreateInfo->factiontemplate = fields[2].GetUInt32();
			if (!dbcFaction.LookupEntryForced(pPlayerCreateInfo->factiontemplate))
			{
				Log.Error("ObjectMgr", "Invalid faction %u for playercreateinfo index %u.", pPlayerCreateInfo->factiontemplate, pPlayerCreateInfo->index);
				continue;
			}

			pPlayerCreateInfo->class_ = fields[3].GetUInt8();
			if (pPlayerCreateInfo->class_ >= MAX_PLAYER_CLASSES || pPlayerCreateInfo->class_ == 10)
			{
				Log.Error("ObjectMgr", "Invalid faction %u for playercreateinfo index %u.", pPlayerCreateInfo->class_, pPlayerCreateInfo->index);
				continue;
			}

			pPlayerCreateInfo->mapId = fields[4].GetUInt32();
			if (!dbcMap.LookupEntryForced(pPlayerCreateInfo->mapId))
			{
				Log.Error("ObjectMgr", "Invalid map entry %u for playercreateinfo index %u.", pPlayerCreateInfo->mapId, pPlayerCreateInfo->index);
				continue;
			}

			pPlayerCreateInfo->zoneId = fields[5].GetUInt32();
			if (!dbcArea.LookupEntryForced(pPlayerCreateInfo->zoneId))
			{
				Log.Error("ObjectMgr", "Invalid zone entry %u for playercreateinfo index %u.", pPlayerCreateInfo->zoneId, pPlayerCreateInfo->index);
				continue;
			}

			pPlayerCreateInfo->positionX = fields[6].GetFloat();
			pPlayerCreateInfo->positionY = fields[7].GetFloat();
			pPlayerCreateInfo->positionZ = fields[8].GetFloat();
			pPlayerCreateInfo->displayId = fields[9].GetUInt16();
			pPlayerCreateInfo->strength = fields[10].GetUInt8();
			pPlayerCreateInfo->ability = fields[11].GetUInt8();
			pPlayerCreateInfo->stamina = fields[12].GetUInt8();
			pPlayerCreateInfo->intellect = fields[13].GetUInt8();
			pPlayerCreateInfo->spirit = fields[14].GetUInt8();
			pPlayerCreateInfo->health = fields[15].GetUInt32();
			if (pPlayerCreateInfo->health == 0)
			{
				Log.Error("ObjectMgr", "Invalid health value for playercreateinfo index %u.", pPlayerCreateInfo->index);
				continue;
			}
			pPlayerCreateInfo->mana = fields[16].GetUInt32();
			pPlayerCreateInfo->rage = fields[17].GetUInt32();
			pPlayerCreateInfo->focus = fields[18].GetUInt32();
			pPlayerCreateInfo->energy = fields[19].GetUInt32();
			pPlayerCreateInfo->attackpower = fields[20].GetUInt32();
			pPlayerCreateInfo->mindmg = fields[21].GetFloat();
			pPlayerCreateInfo->maxdmg = fields[22].GetFloat();
			pPlayerCreateInfo->introid = fields[23].GetUInt32();

			string taxiMaskStr = fields[24].GetString();
			vector<string> tokens = StrSplit(taxiMaskStr, " ");

			memset(pPlayerCreateInfo->taximask, 0, sizeof(pPlayerCreateInfo->taximask));
			int index = 0;
			vector<string>::iterator iter = tokens.begin();
			for (; (index < MAX_PLAYER_CLASSES) && (iter != tokens.end()); ++iter, ++index)
				pPlayerCreateInfo->taximask[index] = atol((*iter).c_str());

			if (QueryResult* sk_sql = WorldDatabase.Query("SELECT skillid, level, maxlevel FROM playercreateinfo_skills WHERE indexid=%u", pPlayerCreateInfo->index))
			{
				do
				{
					Field* fields2 = sk_sql->Fetch();
					CreateInfo_SkillStruct tsk;
					tsk.skillid = fields2[0].GetUInt32();
					tsk.currentval = fields2[1].GetUInt32();
					tsk.maxval = fields2[2].GetUInt32();
					pPlayerCreateInfo->skills.push_back(tsk);
				} while (sk_sql->NextRow());
				delete sk_sql;
			}

			if (QueryResult* sp_sql = WorldDatabase.Query("SELECT spellid FROM playercreateinfo_spells WHERE indexid=%u", pPlayerCreateInfo->index))
			{
				do
				{
					pPlayerCreateInfo->spell_list.insert(sp_sql->Fetch()[0].GetUInt32());
				}while (sp_sql->NextRow());
				delete sp_sql;
			}

			if (QueryResult* items_sql = WorldDatabase.Query("SELECT protoid, slotid, amount FROM playercreateinfo_items WHERE indexid=%u", pPlayerCreateInfo->index))
			{
				do
				{
					Field* fields2 = items_sql->Fetch();
					CreateInfo_ItemStruct itm;
					itm.protoid = fields2[0].GetUInt32();
					itm.slot = fields2[1].GetUInt8();
					itm.amount = fields2[2].GetUInt32();
					pPlayerCreateInfo->items.push_back(itm);
				} while (items_sql->NextRow());
				delete items_sql;
			}

			if (QueryResult* bars_sql = WorldDatabase.Query("SELECT button, action, type, misc FROM playercreateinfo_bars WHERE class=%u and race=%u", pPlayerCreateInfo->class_, pPlayerCreateInfo->race))
			{
				do
				{
					Field* fields2 = bars_sql->Fetch();
					CreateInfo_ActionBarStruct bar;
					bar.button = fields2[0].GetUInt32();
					bar.action = fields2[1].GetUInt32();
					bar.type = fields2[2].GetUInt32();
					bar.misc = fields2[3].GetUInt32();
					pPlayerCreateInfo->actionbars.push_back(bar);
				} while (bars_sql->NextRow());
				delete bars_sql;
			}

			mPlayerCreateInfo[pPlayerCreateInfo->index] = pPlayerCreateInfo;
		} while (result->NextRow());
		delete result;
	}

	Log.Success("ObjectMgr", "Loaded %u player create information.", mPlayerCreateInfo.size());
	GenerateLevelUpInfo();
}

void ObjectMgr::LoadGuilds()
{
	Log.Notice("ObjectMgr", "Loading guilds...");

	std::string query = "SELECT guildId, guildName, leaderGuid, emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor, guildInfo,";
	query.append("motd, createdate, bankBalance FROM guilds");
	if(QueryResult* result = CharacterDatabase.Query(query.c_str()))
	{
		do
		{
			Guild* pGuild = Guild::Create();
			if(!pGuild->LoadFromDB(result->Fetch()))
				delete pGuild;
			else
				mGuild.insert(make_pair(pGuild->GetGuildId(), pGuild));
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u guilds.", mGuild.size());
}

Corpse* ObjectMgr::LoadCorpse(uint32 guid)
{
    Corpse* pCorpse = NULL;
	if(QueryResult* result = CharacterDatabase.Query("SELECT guid, positionX, positionY, positionZ, orientation, zoneid, mapid, instanceId, data FROM Corpses WHERE guid =%u ", guid))
    {
        do
        {
            Field* fields = result->Fetch();
            pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE, fields[0].GetUInt32());
            pCorpse->SetPosition(fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat());
            pCorpse->SetZoneId(fields[5].GetUInt32());
            pCorpse->SetMapId(fields[6].GetUInt32());
            pCorpse->LoadValues(fields[7].GetString());
            if(pCorpse->GetDisplayId() == 0)
            {
                delete pCorpse;
                pCorpse = NULL;
                continue;
            }

            pCorpse->SetLoadedFromDB(true);
            pCorpse->SetInstanceID(fields[8].GetUInt32());
            pCorpse->AddToWorld();
        }
        while(result->NextRow());
        delete result;
    }
	return pCorpse;
}


//------------------------------------------------------
// Live corpse retreival.
// comments: I use the same tricky method to start from the last corpse instead of the first
//------------------------------------------------------
Corpse* ObjectMgr::GetCorpseByOwner(uint32 ownerguid)
{
	Corpse* rv = NULL;

	_corpseslock.Acquire();
	for(CorpseMap::const_iterator itr = m_corpses.begin(); itr != m_corpses.end(); ++itr)
	{
		if(GET_LOWGUID_PART(itr->second->GetOwner()) == ownerguid)
		{
			rv = itr->second;
			break;
		}
	}
	_corpseslock.Release();


	return rv;
}

void ObjectMgr::DelinkPlayerCorpses(Player* pOwner)
{
	Corpse* c = this->GetCorpseByOwner(pOwner->GetLowGUID());
	if(!c)
        return;
	sEventMgr.AddEvent(c, &Corpse::Delink, EVENT_CORPSE_SPAWN_BONES, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	CorpseAddEventDespawn(c);
}

void ObjectMgr::LoadGMTickets()
{
	Log.Notice("ObjectMgr", "Loading GM tickets...");
    uint32 count = 0;
	if(QueryResult* result = CharacterDatabase.Query("SELECT ticketid, playerguid, name, level, map, posx, posy, posz, message, timestamp, deleted, assignedto, comment FROM gm_tickets WHERE deleted = 0"))
    {
        uint32 deleted = 0;
        do
        {
            Field* fields = result->Fetch();

            GM_Ticket* ticket = new GM_Ticket;
            ticket->guid = fields[0].GetUInt64();
            ticket->playerGuid = fields[1].GetUInt64();
            ticket->name = fields[2].GetString();
            ticket->level = fields[3].GetUInt32();
            ticket->map = fields[4].GetUInt32();
            ticket->posX = fields[5].GetFloat();
            ticket->posY = fields[6].GetFloat();
            ticket->posZ = fields[7].GetFloat();
            ticket->message = fields[8].GetString();
            ticket->timestamp = fields[9].GetUInt32();

            deleted = fields[10].GetUInt32();

            if(deleted == 1)
                ticket->deleted = true;
            else
                ticket->deleted = false;

            ticket->assignedToPlayer = fields[11].GetUInt64();
            ticket->comment = fields[12].GetString();

            AddGMTicket(ticket, true);
            ++count;
        }
        while(result->NextRow());
        delete result;
    }

	Log.Success("ObjectMgr", "Loaded %u active GM Tickets.", count);
}

void ObjectMgr::LoadInstanceBossInfos()
{
	Log.Notice("ObjectMgr", "Loading instance boss info data...");
    uint32 cnt = 0;
	if (QueryResult* result = WorldDatabase.Query("SELECT mapid, creatureid, trash, trash_respawn_override FROM instance_bosses"))
    {
        do
        {
            InstanceBossInfo* bossInfo = new InstanceBossInfo();
            bossInfo->mapid = (uint32)result->Fetch()[0].GetUInt32();

            MapInfo* mapInfo = WorldMapInfoStorage.LookupEntry(bossInfo->mapid);
            if(!mapInfo || mapInfo->type == INSTANCE_NULL)
            {
                LOG_DETAIL("Not loading boss information for map %u! (continent or unknown map)", bossInfo->mapid);
                delete bossInfo;
                continue;
            }

            if(bossInfo->mapid >= NUM_MAPS)
            {
                LOG_DETAIL("Not loading boss information for map %u! (map id out of range)", bossInfo->mapid);
                delete bossInfo;
                continue;
            }

            bossInfo->creatureid = (uint32)result->Fetch()[1].GetUInt32();
            char* trash = strdup(result->Fetch()[2].GetString());
            char* q = trash;
            char* p = strchr(q, ' ');
            while(p)
            {
                *p = 0;
                uint32 val = atoi(q);
                if(val)
                    bossInfo->trash.insert(val);
                q = p + 1;
                p = strchr(q, ' ');
            }
            free(trash);
            bossInfo->trashRespawnOverride = (uint32)result->Fetch()[3].GetUInt32();


            if(this->m_InstanceBossInfoMap[bossInfo->mapid] == NULL)
                this->m_InstanceBossInfoMap[bossInfo->mapid] = new InstanceBossInfoMap;
            this->m_InstanceBossInfoMap[bossInfo->mapid]->insert(InstanceBossInfoMap::value_type(bossInfo->creatureid, bossInfo));
            cnt++;
        }
        while(result->NextRow());
        delete result;
    }

	Log.Success("ObjectMgr", "%u boss information loaded.", cnt);
}

void ObjectMgr::SaveGMTicket(GM_Ticket* ticket, QueryBuffer* buf)
{
	std::stringstream ss;

	ss << "DELETE FROM gm_tickets WHERE ticketid = ";
	ss << ticket->guid;
	ss << ";";

	if(!buf)
		CharacterDatabase.ExecuteNA(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());

	ss.rdbuf()->str("");

	ss << "INSERT INTO gm_tickets (ticketid, playerguid, name, level, map, posx, posy, posz, message, timestamp, deleted, assignedto, comment) VALUES(";
	ss << ticket->guid << ", ";
	ss << ticket->playerGuid << ", '";
	ss << CharacterDatabase.EscapeString(ticket->name) << "', ";
	ss << ticket->level << ", ";
	ss << ticket->map << ", ";
	ss << ticket->posX << ", ";
	ss << ticket->posY << ", ";
	ss << ticket->posZ << ", '";
	ss << CharacterDatabase.EscapeString(ticket->message) << "', ";
	ss << ticket->timestamp << ", ";

	if(ticket->deleted)
		ss << uint8(1);
	else
		ss << uint8(0);
	ss << ",";

	ss << ticket->assignedToPlayer << ", '";
	ss << CharacterDatabase.EscapeString(ticket->comment) << "');";

	if(!buf)
		CharacterDatabase.ExecuteNA(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

void ObjectMgr::SetHighestGuids()
{
	Log.Notice("ObjectMgr", "Setting up highest guids...");
	QueryResult* result;

	result = CharacterDatabase.Query("SELECT MAX(guid) FROM characters");
	if(result)
	{
		m_hiPlayerGuid.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(guid) FROM playeritems");
	if(result)
	{
		m_hiItemGuid.SetVal((uint32)result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(guid) FROM corpses");
	if(result)
	{
		m_hiCorpseGuid.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = WorldDatabase.Query("SELECT MAX(id) FROM creature_spawns");
	if(result)
	{
		m_hiCreatureSpawnId.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = WorldDatabase.Query("SELECT MAX(id) FROM gameobject_spawns");
	if(result)
	{
		m_hiGameObjectSpawnId.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(group_id) FROM groups");
	if(result)
	{
		m_hiGroupId.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(charterid) FROM charters");
	if(result)
	{
		m_hiCharterId.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(guildid) FROM guilds");
	if(result)
	{
		m_hiGuildId.SetVal(result->Fetch()[0].GetUInt32());
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX( UID ) FROM playerbugreports");
	if(result != NULL)
	{
		m_reportID.SetVal(uint32(result->Fetch()[0].GetUInt64() + 1));
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX(ticketid) FROM gm_tickets");
	if(result)
	{
		m_ticketid.SetVal(uint32(result->Fetch()[0].GetUInt64() + 1));
		delete result;
	}


	result = CharacterDatabase.Query("SELECT MAX( message_id ) FROM mailbox");
	if(result)
	{
		m_mailid.SetVal(uint32(result->Fetch()[0].GetUInt64() + 1));
		delete result;
	}

	result = CharacterDatabase.Query("SELECT MAX( setGUID ) FROM equipmentsets");
	if(result != NULL)
	{
		m_setGUID.SetVal(uint32(result->Fetch()[0].GetUInt32() + 1));
		delete result;
	}

	Log.Notice("ObjectMgr", "HighGuid(CORPSE) = %u",		m_hiCorpseGuid.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(PLAYER) = %u",		m_hiPlayerGuid.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(GAMEOBJ) = %u",		m_hiGameObjectSpawnId.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(UNIT) = %u",			m_hiCreatureSpawnId.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(ITEM) = %u",			m_hiItemGuid.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(CONTAINER) = %u",		m_hiItemGuid.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(GROUP) = %u",			m_hiGroupId.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(CHARTER) = %u",		m_hiCharterId.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(GUILD) = %u",			m_hiGuildId.GetVal());
	Log.Notice("ObjectMgr", "HighGuid(BUGREPORT) = %u",		uint32(m_reportID.GetVal() - 1));
	Log.Notice("ObjectMgr", "HighGuid(TICKET) = %u",		uint32(m_ticketid.GetVal() - 1));
	Log.Notice("ObjectMgr", "HighGuid(MAIL) = %u",			uint32(m_mailid.GetVal()));
	Log.Notice("ObjectMgr", "HighGuid(EQUIPMENTSET) = %u",	uint32(m_setGUID.GetVal() - 1));
}

uint32 ObjectMgr::GenerateReportID()
{
	return ++m_reportID;
}

uint32 ObjectMgr::GenerateTicketID()
{
	return ++m_ticketid;
}


uint32 ObjectMgr::GenerateEquipmentSetID()
{
	return ++m_setGUID;
}

uint32 ObjectMgr::GenerateMailID()
{
	return ++m_mailid;
}

uint32 ObjectMgr::GenerateLowGuid(uint32 guidhigh)
{
	ARCEMU_ASSERT(guidhigh == HIGHGUID_TYPE_ITEM || guidhigh == HIGHGUID_TYPE_CONTAINER || guidhigh == HIGHGUID_TYPE_PLAYER);

	uint32 ret = 0;
	if(guidhigh == HIGHGUID_TYPE_ITEM)
		ret = ++m_hiItemGuid;
	else if(guidhigh == HIGHGUID_TYPE_PLAYER)
		ret = ++m_hiPlayerGuid;
	else
		ret = ++m_hiItemGuid;

	return ret;
}

void ObjectMgr::ProcessGameobjectQuests()
{
	Log.Notice("ObjectMgr", "Loading gameobject quest item binding data...");
	uint32 count = 0;
	if(QueryResult* result = WorldDatabase.Query("SELECT entry, quest, item, item_count FROM gameobject_quest_item_binding"))
	{
		do
		{
			Field* fields = result->Fetch();
			GameObjectInfo* gon = GameObjectNameStorage.LookupEntry(fields[0].GetUInt32());
			if (!gon)
			{
				Log.Error("ObjectMgr", "Tryed to load gameobject quest item binding data for non existing entry %u.", fields[0].GetUInt32());
				continue;
			}

			Quest* qst = QuestStorage.LookupEntry(fields[1].GetUInt32());
			if (!qst)
			{
				Log.Error("ObjectMgr", "Tryed to load quest item binding data for %u gameobject with non existing quest %u.", fields[0].GetUInt32(), fields[1].GetUInt32());
				continue;
			}
			gon->itemMap[qst].insert(make_pair(fields[2].GetUInt32(), fields[3].GetUInt32()));
			++count;
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u gameobject quest item bindings.", count);

	Log.Notice("ObjectMgr", "Loading gameobject quest pickup binding data...");
	count = 0;
	if(QueryResult* result = WorldDatabase.Query("SELECT entry, quest, required_count FROM gameobject_quest_pickup_binding"))
	{
		do
		{
			Field* fields = result->Fetch();
			GameObjectInfo* gon = GameObjectNameStorage.LookupEntry(fields[0].GetUInt32());
			if (!gon)
			{
				Log.Error("ObjectMgr", "Tryed to load gameobject quest pickup binding data for non existing entry %u.", fields[0].GetUInt32());
				continue;
			}

			Quest* qst = QuestStorage.LookupEntry(fields[1].GetUInt32());
			if (!qst)
			{
				Log.Error("ObjectMgr", "Tryed to load gameobject quest pickup binding data for %u gameobject with non existing quest %u.", fields[0].GetUInt32(), fields[1].GetUInt32());
				continue;
			}

			gon->goMap.insert(make_pair(qst, fields[2].GetUInt32()));
			++count;
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u gameobject quest pickup bindings", count);

	Log.Notice("ObjectMgr", "Loading npc gossip textid data...");
	if(QueryResult* result = WorldDatabase.Query("SELECT * FROM npc_gossip_textid"))
	{
		do
		{
			uint32 entry = result->Fetch()[0].GetUInt32();
			if (!CreatureProtoStorage.LookupEntry(entry))
			{
				Log.Error("ObjectMgr", "Tryed to load npc gossip text id for non existing %u npc entry", entry);
				continue;
			}

			uint32 text  = result->Fetch()[1].GetUInt32();
			if (!NpcTextStorage.LookupEntry(text))
			{
				Log.Error("ObjectMgr", "Npc gossip text id %u for %u npc entry doesn't exists.", text, entry);
				continue;
			}
			mNpcToGossipText[entry] = text;
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u npc gossip textIds.", mNpcToGossipText.size());
}

Player* ObjectMgr::GetPlayer(const char* name, bool caseSensitive)
{
	Player* rv = NULL;
	_playerslock.AcquireReadLock();

	if(!caseSensitive)
	{
		std::string strName = name;
		arcemu_TOLOWER(strName);
		for (PlayerStorageMap::const_iterator itr = _players.begin(); itr != _players.end(); ++itr)
		{
			if(!stricmp(itr->second->GetNameString()->c_str(), strName.c_str()))
			{
				rv = itr->second;
				break;
			}
		}
	}
	else
	{
		for (PlayerStorageMap::const_iterator itr = _players.begin(); itr != _players.end(); ++itr)
		{
			if(!strcmp(itr->second->GetName(), name))
			{
				rv = itr->second;
				break;
			}
		}
	}

	_playerslock.ReleaseReadLock();
	return rv;
}

Player* ObjectMgr::GetPlayer(uint32 guid)
{
	_playerslock.AcquireReadLock();
	PlayerStorageMap::const_iterator itr = _players.find(guid);
	if(itr != _players.end())
		return itr->second;

	_playerslock.ReleaseReadLock();
	return NULL;
}

PlayerCreateInfo* ObjectMgr::GetPlayerCreateInfo(uint8 race, uint8 class_) const
{
	for (PlayerCreateInfoMap::const_iterator itr = mPlayerCreateInfo.begin(); itr != mPlayerCreateInfo.end(); ++itr)
		if((itr->second->race == race) && (itr->second->class_ == class_))
			return itr->second;

	return NULL;
}

void ObjectMgr::AddGuild(Guild* pGuild)
{
	ARCEMU_ASSERT(pGuild != NULL);
	mGuild[pGuild->GetGuildId()] = pGuild;
}

uint32 ObjectMgr::GetTotalGuildCount()
{
	return (uint32)mGuild.size();
}

bool ObjectMgr::RemoveGuild(uint32 guildId)
{
	GuildMap::iterator i = mGuild.find(guildId);
	if(i == mGuild.end())
		return false;

	mGuild.erase(i);
	return true;
}

Guild* ObjectMgr::GetGuild(uint32 guildId)
{
	GuildMap::const_iterator itr = mGuild.find(guildId);
	if(itr == mGuild.end())
		return NULL;

	return itr->second;
}

Guild* ObjectMgr::GetGuildByLeaderGuid(uint64 leaderGuid)
{
	for (GuildMap::const_iterator itr = mGuild.begin(); itr != mGuild.end(); ++itr)
		if(itr->second->GetGuildLeader() == leaderGuid)
			return itr->second;

	return NULL;
}

Guild* ObjectMgr::GetGuildByGuildName(std::string guildName)
{
	for (GuildMap::const_iterator itr = mGuild.begin(); itr != mGuild.end(); ++itr)
		if(itr->second->GetGuildName() == guildName)
			return itr->second;

	return NULL;
}


void ObjectMgr::AddGMTicket(GM_Ticket* ticket, bool startup)
{
	ARCEMU_ASSERT(ticket  != NULL);
	GM_TicketList.push_back(ticket);

	// save
	if(!startup)
		SaveGMTicket(ticket, NULL);
}

void ObjectMgr::UpdateGMTicket(GM_Ticket* ticket)
{
	SaveGMTicket(ticket, NULL);
}

void ObjectMgr::DeleteGMTicketPermanently(uint64 ticketGuid)
{
	for (GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); ++i)
	{
		if ((*i)->guid == ticketGuid)
		{
			i = GM_TicketList.erase(i);
			break;
		}
	}

	// kill from db
	CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE guid=%u", ticketGuid);
}

void ObjectMgr::DeleteAllRemovedGMTickets()
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); i++)
		if((*i)->deleted)
			i = GM_TicketList.erase(i);

	CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE deleted=1");
}

void ObjectMgr::RemoveGMTicketByPlayer(uint64 playerGuid)
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); ++i)
	{
		if((*i)->playerGuid == playerGuid && !(*i)->deleted)
		{
			(*i)->deleted = true;
			SaveGMTicket((*i), NULL);
		}
	}
}

void ObjectMgr::RemoveGMTicket(uint64 ticketGuid)
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); ++i)
	{
		if((*i)->guid == ticketGuid && !(*i)->deleted)
		{
			(*i)->deleted = true;
			SaveGMTicket((*i), NULL);
		}
	}
}

GM_Ticket* ObjectMgr::GetGMTicketByPlayer(uint64 playerGuid)
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); ++i)
		if((*i)->playerGuid == playerGuid && !(*i)->deleted)
			return (*i);

	return NULL;
}

GM_Ticket* ObjectMgr::GetGMTicket(uint64 ticketGuid)
{
	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end(); ++i)
		if((*i)->guid == ticketGuid)
			return (*i);

	return NULL;
}

//std::list<GM_Ticket*>* ObjectMgr::GetGMTicketsByPlayer(uint64 playerGuid)
//{
//	std::list<GM_Ticket*>* list = new std::list<GM_Ticket*>();
//	for(GmTicketList::iterator i = GM_TicketList.begin(); i != GM_TicketList.end();)
//	{
//		if((*i)->playerGuid == playerGuid)
//		{
//			list->push_back((*i));
//		}
//		++i;
//	}
//	return list;
//}

void ObjectMgr::LoadVendors()
{
	Log.Notice("ObjectMgr", "Loading vendors data...");
	if (QueryResult* result = WorldDatabase.Query("SELECT entry, item, amount, max_amount, inctime, extended_cost FROM vendors"))
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 entry = fields[0].GetUInt32();
			if (!CreatureNameStorage.LookupEntry(entry))
			{
				Log.Error("ObjectMgr", "Tryed to load vendor data for non existing creature entry %u.", entry);
				continue;
			}

			if (!(CreatureProtoStorage.LookupEntry(entry)->NPCFLags & UNIT_NPC_FLAG_VENDOR))
			{
				Log.Error("ObjectMgr", "Tryed to load vendor data for non existing creature entry %u but creature has no vendor flag.", entry);
				continue;
			}

			HM_NAMESPACE::hash_map<uint32, std::vector<CreatureItem>*>::const_iterator itr = mVendors.find(entry);
			std::vector<CreatureItem> *items;

			if(itr == mVendors.end())
			{
				items = new std::vector<CreatureItem>;
				mVendors[fields[0].GetUInt32()] = items;
			}
			else
				items = itr->second;

			if (!ItemPrototypeStorage.LookupEntry(fields[1].GetUInt32()))
			{
				Log.Error("ObjectMgr", "Vendor item  %u for npc %u does not exists.", fields[1].GetUInt32(), fields[0].GetUInt32());
				continue;
			}

			CreatureItem itm;
			itm.itemid = fields[1].GetUInt32();
			itm.amount = fields[2].GetUInt32();
			itm.available_amount = fields[3].GetUInt32();
			itm.max_amount = fields[3].GetUInt32();
			itm.incrtime = fields[4].GetUInt32();

			ItemExtendedCostEntry* ec = NULL;
			if(fields[5].GetUInt32())
			{
				ec = dbcItemExtendedCost.LookupEntryForced(fields[5].GetUInt32());
				if (!ec)
				{
					Log.Error("ObjectMgr", "Extendedcost for npc vendor %u and item %u does not exists.", fields[0].GetUInt32(), fields[1].GetUInt32());
					continue;
				}
			}

			itm.extended_cost = ec;
			items->push_back(itm);
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u vendors.", mVendors.size());
}

void ObjectMgr::ReloadVendors()
{
	mVendors.clear();
	LoadVendors();
}

std::vector<CreatureItem>* ObjectMgr::GetVendorList(uint32 entry)
{
	return mVendors[entry];
}

void ObjectMgr::LoadAIThreatToSpellId()
{
	Log.Notice("ObjectMgr", "Loading ai threattospellid data...");
	uint32 count = 0;
	if (QueryResult* result = WorldDatabase.Query("SELECT spell, mod, modcoef FROM ai_threattospellid"))
	{
		do
		{
			Field* fields = result->Fetch();
			if (SpellEntry* sp = dbcSpell.LookupEntryForced(fields[0].GetUInt32()))
			{
				sp->ThreatForSpell = fields[1].GetUInt32();
				sp->ThreatForSpellCoef = fields[2].GetFloat();
				++count;
			}
			else
				Log.Error("AIThreatSpell", "Cannot apply to spell %u; spell is nonexistent.", fields[0].GetUInt32());

		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u ai_threattospellid entries.", count);
}

void ObjectMgr::LoadSpellProcs()
{
	Log.Notice("ObjectMgr", "Loading spell proc data...");
	uint32 count = 0;
	std::string query = "SELECT spellId, ProcOnNameHash, ProcFlag, TargetSelf, ProcChance, ProcCharges, ProcInterval, EffectTriggerSpell,";
	query.append("EffectTriggerSpell, EffectTriggerSpell FROM spell_proc");

	if (QueryResult* result = WorldDatabase.Query(query.c_str()))
	{
		do
		{
			Field* f = result->Fetch();
			uint32 spe_spellId = f[0].GetUInt32();
			uint32 spe_NameHash = f[1].GetUInt32();

			if (SpellEntry* sp = dbcSpell.LookupEntryForced(spe_spellId))
			{
				uint8 x = 0;
				for (; x < 3; ++x)
				{
					if (sp->ProcOnNameHash[x] == 0)
						break;
				}

				if (x != 3)
					sp->ProcOnNameHash[x] = spe_NameHash;
				else
					Log.Error("ObjectMgr", "Wrong ProcOnNameHash for Spell: %u!", spe_spellId);

				sp->procFlags = f[2].GetUInt32();

				if (f[3].GetUInt32() == 1)
					sp->procFlags |= PROC_TARGET_SELF;

				if (f[4].GetInt32() >= 0)
					sp->procChance = f[4].GetUInt32();

				if (f[5].GetInt32() >= 0)
					sp->procCharges = f[5].GetInt32();

				sp->proc_interval = f[6].GetUInt32();

				if (f[7].GetInt32() >= 0)
					sp->EffectTriggerSpell[0] = f[7].GetUInt32();

				if (f[8].GetInt32() >= 0)
					sp->EffectTriggerSpell[1] = f[8].GetUInt32();

				if (f[9].GetInt32() >= 0)
					sp->EffectTriggerSpell[2] = f[9].GetUInt32();

				++count;
			}
			else
				Log.Error("ObjectMgr", "Tryed to load non existing spell proc data for non existing spell %u.", spe_spellId);
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u spell proc entries.", count);
}

void ObjectMgr::LoadSpellEffectsOverride()
{
	Log.Notice("ObjectMgr", "Loading spell effect override data...");
	uint32 count = 0;
	std::string query = "SELECT spellId, EffectID, Disable, Effect, BasePoints, ApplyAuraName, SpellGroupRelation, MiscValue, TriggerSpell, ImplicitTargetA,";
	query.append("ImplicitTargetB, EffectCustomFlag FROM spell_effects_override");

	if (QueryResult* result = WorldDatabase.Query(query.c_str()))
	{
		do
		{
			Field* f = result->Fetch();
			uint32 seo_SpellId = f[0].GetUInt32();
			uint32 seo_EffectId = f[1].GetUInt32();
			uint32 seo_Disable = f[2].GetUInt32();
			uint32 seo_Effect = f[3].GetUInt32();
			uint32 seo_BasePoints = f[4].GetUInt32();
			uint32 seo_ApplyAuraName = f[5].GetUInt32();
			//uint32 seo_SpellGroupRelation = f[6].GetUInt32();
			uint32 seo_MiscValue = f[7].GetUInt32();
			uint32 seo_TriggerSpell = f[8].GetUInt32();
			uint32 seo_ImplicitTargetA = f[9].GetUInt32();
			uint32 seo_ImplicitTargetB = f[10].GetUInt32();
			uint32 seo_EffectCustomFlag = f[11].GetUInt32();

			if(seo_SpellId)
			{
				if (SpellEntry* sp = dbcSpell.LookupEntryForced(seo_SpellId))
				{
					if(seo_Disable)
						sp->Effect[seo_EffectId] = SPELL_EFFECT_NULL;

					if(seo_Effect)
						sp->Effect[seo_EffectId] = seo_Effect;

					if(seo_BasePoints)
						sp->EffectBasePoints[seo_EffectId] = seo_BasePoints;

					if(seo_ApplyAuraName)
						sp->EffectApplyAuraName[seo_EffectId] = seo_ApplyAuraName;

//					if( seo_SpellGroupRelation )
//						sp->EffectSpellGroupRelation[seo_EffectId] = seo_SpellGroupRelation;

					if(seo_MiscValue)
						sp->EffectMiscValue[seo_EffectId] = seo_MiscValue;

					if(seo_TriggerSpell)
						sp->EffectTriggerSpell[seo_EffectId] = seo_TriggerSpell;

					if(seo_ImplicitTargetA)
						sp->EffectImplicitTargetA[seo_EffectId] = seo_ImplicitTargetA;

					if(seo_ImplicitTargetB)
						sp->EffectImplicitTargetB[seo_EffectId] = seo_ImplicitTargetB;

					if(seo_EffectCustomFlag)
						sp->EffectCustomFlag[ seo_Effect ] = seo_EffectCustomFlag;

					++count;
				}
				else
					Log.Error("ObjectMgr", "Tried to load a spell effect override for a nonexistant spell %u.", seo_SpellId);
			}
			else Log.Error("ObjectMgr", "spell effect override can't contain data for entry 0");

		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u spell effect override entries.", count);
}

Item* ObjectMgr::CreateItem(uint32 entry, Player* owner)
{
	ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(entry);
	if(!proto) 
	{
		Log.Debug("ObjectMgr:CreateItem", "Item entry %u doesn't exists");
		return NULL;
	}

	if(proto->InventoryType == INVTYPE_BAG)
	{
		Container* pContainer = new Container(HIGHGUID_TYPE_CONTAINER, GenerateLowGuid(HIGHGUID_TYPE_CONTAINER));
		pContainer->Create(entry, owner);
		pContainer->SetStackCount(1);
		return pContainer;
	}
	else
	{
		Item* pItem = new Item;
		pItem->Init(HIGHGUID_TYPE_ITEM, GenerateLowGuid(HIGHGUID_TYPE_ITEM));
		pItem->Create(entry, owner);
		pItem->SetStackCount(1);

		if(owner != NULL)
		{
			uint32* played = owner->GetPlayedtime();
			pItem->SetCreationTime(played[1]);
		}

		return pItem;
	}
}

Item* ObjectMgr::LoadItem(uint32 lowguid)
{
	Item* pReturn = NULL;
	std::string query = "SELECT ownerguid, guid, entry, wrapped_item_id, wrapped_creator, creator, count, charges, flags, randomprop,";
	query.append("randomsuffix, itemtext, durability, containerslot, slot, enchantments, duration_expireson, refund_purchasedon,");
	query.append("refund_costid, text FROM playeritems WHERE guid = %u");

	if (QueryResult* result = CharacterDatabase.Query(query.c_str(), lowguid))
	{
		ItemPrototype* pProto = ItemPrototypeStorage.LookupEntry(result->Fetch()[2].GetUInt32());
		if(!pProto)
			return NULL;

		if(pProto->InventoryType == INVTYPE_BAG)
		{
			Container* pContainer = new Container(HIGHGUID_TYPE_CONTAINER, lowguid);
			pContainer->LoadFromDB(result->Fetch());
			pReturn = pContainer;
		}
		else
		{
			Item* pItem = new Item;
			pItem->Init(HIGHGUID_TYPE_ITEM, lowguid);
			pItem->LoadFromDB(result->Fetch(), NULL, false);
			pReturn = pItem;
		}
		delete result;
	}

	return pReturn;
}

void ObjectMgr::LoadCorpses(MapMgr* mgr)
{
	if (QueryResult* result = CharacterDatabase.Query("SELECT guid, positionX, positionY, positionZ, orientation, zoneId, mapId, instanceId, data FROM corpses WHERE instanceid = %u", mgr->GetInstanceID()))
	{
		do
		{
			Field* fields = result->Fetch();
			Corpse* pCorpse = new Corpse(HIGHGUID_TYPE_CORPSE, fields[0].GetUInt32());
			pCorpse->SetPosition(fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat());
			pCorpse->SetZoneId(fields[5].GetUInt32());
			pCorpse->SetMapId(fields[6].GetUInt32());
			pCorpse->SetInstanceID(fields[7].GetUInt32());
			pCorpse->LoadValues(fields[8].GetString());
			if(pCorpse->GetDisplayId() == 0)
			{
				delete pCorpse;
				continue;
			}

			pCorpse->PushToWorld(mgr);
		}
		while(result->NextRow());
		delete result;
	}
}

#ifdef ENABLE_ACHIEVEMENTS
AchievementCriteriaEntryList const & ObjectMgr::GetAchievementCriteriaByType(AchievementCriteriaTypes type)
{
	return m_AchievementCriteriasByType[type];
}

void ObjectMgr::LoadAchievementCriteriaList()
{
	for(uint32 rowId = 0; rowId < dbcAchievementCriteriaStore.GetNumRows(); ++rowId)
	{
		AchievementCriteriaEntry const* criteria = dbcAchievementCriteriaStore.LookupRowForced(rowId);
		if(!criteria)
			continue;

		m_AchievementCriteriasByType[criteria->requiredType].push_back(criteria);
	}
}
#endif

std::list<ItemPrototype*>* ObjectMgr::GetListForItemSet(uint32 setid)
{
	return mItemSets[setid];
}

void ObjectMgr::CorpseAddEventDespawn(Corpse* pCorpse)
{
	if(!pCorpse->IsInWorld())
		delete pCorpse;
	else
		sEventMgr.AddEvent(pCorpse->GetMapMgr(), &MapMgr::EventCorpseDespawn, pCorpse->GetGUID(), EVENT_CORPSE_DESPAWN, 600000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void ObjectMgr::CorpseCollectorUnload()
{
	_corpseslock.Acquire();
	for (CorpseMap::const_iterator itr = m_corpses.begin(); itr != m_corpses.end(); ++itr)
	{
		Corpse* c = itr->second;
		if(c && c->IsInWorld())
			c->RemoveFromWorld(false);
		delete c;
	}
	m_corpses.clear();
	_corpseslock.Release();
}

GossipMenu::GossipMenu(uint64 Creature_Guid, uint32 Text_Id) : TextId(Text_Id), CreatureGuid(Creature_Guid)
{

}

void GossipMenu::AddItem(uint8 Icon, const char* Text, int32 Id /* = -1 */, int8 Extra /* = 0 */)
{
	GossipMenuItem Item;

	Item.Icon = Icon;
	Item.Extra = Extra;
	Item.Text = Text;
	Item.m_gBoxMessage = "";
	Item.m_gBoxMoney = 0;
	Item.Id = uint32(Menu.size());

	if(Id > 0)
		Item.IntId = Id;
	else
		Item.IntId = Item.Id;

	Menu.push_back(Item);
}

void GossipMenu::AddMenuItem(uint8 Icon, const char* Message, uint32 dtSender, uint32 dtAction, const char* BoxMessage, uint32 BoxMoney, bool Coded)
{
	GossipMenuItem Item;

	Item.Icon = Icon;
	Item.Extra = Coded;
	Item.Text = Message;
	Item.m_gBoxMessage = BoxMessage;
	Item.m_gBoxMoney = BoxMoney;
	Item.Id = uint32(Menu.size());
	Item.IntId = dtAction;

	Menu.push_back(Item);
}

void GossipMenu::AddItem(GossipMenuItem* GossipItem)
{
	Menu.push_back(*GossipItem);
}

void GossipMenu::BuildPacket(WorldPacket & Packet)
{
	Packet << CreatureGuid;
	Packet << uint32(0);			// some new menu type in 2.4?
	Packet << TextId;
	Packet << uint32(Menu.size());

	for(std::vector<GossipMenuItem>::iterator iter = Menu.begin();
	        iter != Menu.end(); ++iter)
	{
		Packet << iter->Id;
		Packet << iter->Icon;
		Packet << iter->Extra;
		Packet << uint32(iter->m_gBoxMoney);    // money required to open menu, 2.0.3
		Packet << iter->Text;
		Packet << iter->m_gBoxMessage;          // accept text (related to money) pop up box, 2.0.3
	}
}

void GossipMenu::SendTo(Player* Plr)
{
	WorldPacket data(SMSG_GOSSIP_MESSAGE, Menu.size() * 50 + 24);
	BuildPacket(data);
	data << uint32(0);  // 0 quests obviously
	Plr->GetSession()->SendPacket(&data);
}

void ObjectMgr::CreateGossipMenuForPlayer(GossipMenu** Location, uint64 Guid, uint32 TextID, Player* Plr)
{
	if(!TextID)
	{
		//TextID = 0 will not show the gossip to the player. Using "2" since it's the default value in GossipScript::GossipHello()
		Log.Debug("ObjectMgr::CreateGossipMenuForPlayer", "Object with GUID %llu is trying to create a GossipMenu with non existing TextID", Guid);
		TextID = 1;
	}

	GossipMenu* Menu = new GossipMenu(Guid, TextID);
	ARCEMU_ASSERT(Menu != NULL);

	if(Plr->CurrentGossipMenu)
		delete Plr->CurrentGossipMenu;

	Plr->CurrentGossipMenu = Menu;
	*Location = Menu;
}

GossipMenuItem GossipMenu::GetItem(uint32 Id)
{
	if(Id >= Menu.size())
	{
		GossipMenuItem k;
		k.IntId = 1;
		k.Extra = 0;

		return k;
	}
	else
		return Menu[Id];
}

uint32 ObjectMgr::GetGossipTextForNpc(uint32 ID)
{
	return mNpcToGossipText[ID];
}

void ObjectMgr::LoadTrainers()
{
	LoadDisabledSpells();
	Log.Notice("ObjectMgr", "Loading trainers data...");
	std::string query = "SELECT entry, required_skill, required_skillvalue, req_class, RequiredRace, RequiredReputation, RequiredReputationValue, trainer_type,";
	query.append("trainer_ui_window_message, can_train_gossip_textid, cannot_train_gossip_textid FROM trainer_defs");

	if (QueryResult* result = WorldDatabase.Query(query.c_str()))
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 entry = fields[0].GetUInt32();
			if (!CreatureNameStorage.LookupEntry(entry))
			{
				Log.Error("ObjectMgr", "Tryed to load trainer data for non existing npc entry %u.", entry);
				continue;
			}

			if (!(CreatureProtoStorage.LookupEntry(entry)->NPCFLags & UNIT_NPC_FLAG_TRAINER))
			{
				Log.Error("ObjectMgr", "Tryed to load trainer data for npc %u but npc is not a trainer!", entry);
				continue;
			}

			Trainer* tr = new Trainer;
			tr->RequiredSkill = fields[1].GetUInt32();
			tr->RequiredSkillLine = fields[2].GetUInt32();
			tr->RequiredClass = fields[3].GetUInt32();
			tr->RequiredRace = fields[4].GetUInt32();
			tr->RequiredRepFaction = fields[5].GetUInt32();
			tr->RequiredRepValue = fields[6].GetUInt32();
			tr->TrainerType = fields[7].GetUInt32();
			tr->Can_Train_Gossip_TextId = fields[9].GetUInt32();
			tr->Cannot_Train_GossipTextId = fields[10].GetUInt32();

			if (tr->TrainerType != TRAINER_TYPE_TRADESKILLS && !NpcTextStorage.LookupEntry(tr->Can_Train_Gossip_TextId))
			{
				Log.Error("ObjectMgr", "Trainer %u has wrong \"can train gossip textid (%u)\". Will use text id 1 instead.", entry, tr->Can_Train_Gossip_TextId);
				tr->Can_Train_Gossip_TextId = 1;
			}

			if (tr->TrainerType != TRAINER_TYPE_TRADESKILLS && !NpcTextStorage.LookupEntry(tr->Cannot_Train_GossipTextId))
			{
				Log.Error("ObjectMgr", "Trainer %u has wrong \"cannot train gossip textid (%u)\". Will use text id 1 instead.", entry, tr->Cannot_Train_GossipTextId);
				tr->Cannot_Train_GossipTextId = 1;
			}

			const char* temp = fields[8].GetString();
			size_t len = strlen(temp);
			if (len)
			{
				tr->UIMessage = new char[len + 1];
				strcpy(tr->UIMessage, temp);
				tr->UIMessage[len] = 0;
			}
			else
			{
				tr->UIMessage = new char[strlen(NormalTalkMessage) + 1];
				strcpy(tr->UIMessage, NormalTalkMessage);
				tr->UIMessage[strlen(NormalTalkMessage)] = 0;
			}

			//now load the spells
			if (QueryResult* result2 = WorldDatabase.Query("SELECT entry, cast_spell, learn_spell, spellcost, reqspell, reqskill, reqskillvalue, reqlevel, deletespell FROM trainer_spells where entry='%u'", entry))
			{
				do
				{
					Field* fields2 = result2->Fetch();
					TrainerSpell ts;
					bool abrt = false;
					uint32 CastSpellID = fields2[1].GetUInt32();
					uint32 LearnSpellID = fields2[2].GetUInt32();

					ts.pCastSpell = NULL;
					ts.pLearnSpell = NULL;
					ts.pCastRealSpell = NULL;

					if (CastSpellID)
					{
						ts.pCastSpell = dbcSpell.LookupEntryForced(CastSpellID);
						if (ts.pCastSpell)
						{
							for (uint8 k = 0; k < 3; ++k)
							{
								if (ts.pCastSpell->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
								{
									ts.pCastRealSpell = dbcSpell.LookupEntryForced(ts.pCastSpell->EffectTriggerSpell[k]);
									if (!ts.pCastRealSpell)
									{
										Log.Error("ObjectMgr", "Trainer %u contains cast spell %u that is non-teaching", entry, CastSpellID);
										abrt = true;
									}
									break;
								}
							}
						}

						if (abrt)
							continue;
					}

					if (LearnSpellID)
						ts.pLearnSpell = dbcSpell.LookupEntryForced(LearnSpellID);

					if (ts.pCastSpell == NULL && ts.pLearnSpell == NULL)
					{
						Log.Error("ObjectMgr", "Trainer %u without valid spells (%u/%u).", entry, CastSpellID, LearnSpellID);
						continue;
					}

					if (ts.pCastSpell && !ts.pCastRealSpell)
						continue;

					ts.Cost = fields2[3].GetUInt32();
					ts.RequiredSpell = fields2[4].GetUInt32();
					ts.RequiredSkillLine = fields2[5].GetUInt32();
					ts.RequiredSkillLineValue = fields2[6].GetUInt32();
					ts.RequiredLevel = fields2[7].GetUInt32();
					ts.DeleteSpell = fields2[8].GetUInt32();
					//IsProfession is true if the TrainerSpell will teach a primary profession
					if (ts.RequiredSkillLine == 0 && ts.pCastRealSpell != NULL && ts.pCastRealSpell->Effect[1] == SPELL_EFFECT_SKILL)
					{
						uint32 skill = ts.pCastRealSpell->EffectMiscValue[1];
						skilllineentry* sk = dbcSkillLine.LookupEntryForced(skill);
						ARCEMU_ASSERT(sk != NULL);
						if (sk->type == SKILL_TYPE_PROFESSION)
							ts.IsProfession = true;
						else
							ts.IsProfession = false;
					}
					else
						ts.IsProfession = false;

					tr->Spells.push_back(ts);
				} while (result2->NextRow());
				delete result2;

				tr->SpellCount = (uint32)tr->Spells.size();

				//and now we insert it to our lookup table
				if (!tr->SpellCount)
				{
					if (tr->UIMessage != NormalTalkMessage)
						delete[] tr->UIMessage;
					delete tr;
					continue;
				}

				mTrainers.insert(TrainerMap::value_type(entry, tr));
			}

		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u trainers.", mTrainers.size());
}

Trainer* ObjectMgr::GetTrainer(uint32 Entry)
{
	TrainerMap::iterator iter = mTrainers.find(Entry);
	if(iter == mTrainers.end())
		return NULL;

	return iter->second;
}

void ObjectMgr::GenerateLevelUpInfo()
{
	Log.Notice("ObjectMgr", "Generating levelup information");
	// Generate levelup information for each class.
	for(uint32 Class = WARRIOR; Class <= DRUID; ++Class)
	{
		// These are empty.
		if (Class == 10)
			continue;

		// Search for a playercreateinfo.
		for(uint32 Race = RACE_HUMAN; Race < MAX_RACE; ++Race)
		{
			PlayerCreateInfo* PCI = GetPlayerCreateInfo(static_cast<uint8>(Race), static_cast<uint8>(Class));

			if(!PCI)
				continue;   // Class not valid for this race.

			// Generate each level's information
			uint8 MaxLevel = sWorld.m_genLevelCap + 1;
			LevelInfo* lvl = new LevelInfo, lastlvl;
			lastlvl.HP = PCI->health;
			lastlvl.Mana = PCI->mana;
			lastlvl.Stat[0] = PCI->strength;
			lastlvl.Stat[1] = PCI->ability;
			lastlvl.Stat[2] = PCI->stamina;
			lastlvl.Stat[3] = PCI->intellect;
			lastlvl.Stat[4] = PCI->spirit;
			lastlvl.XPToNextLevel = 400;
			LevelMap* lMap = new LevelMap;

			// Create first level.
			*lvl = lastlvl;

			// Insert into map
			lMap->insert(LevelMap::value_type(1, lvl));

			for(uint8 Level = 2; Level < MaxLevel; ++Level)
			{
				lvl = new LevelInfo;

				// Calculate Stats
				for (uint8 s = 0; s < MAX_STATS; ++s)
				{
					uint32 val = GainStat(static_cast<uint16>(Level), static_cast<uint8>(Class), static_cast<uint8>(s));
					lvl->Stat[s] = lastlvl.Stat[s] + val;
				}

				// Calculate HP/Mana
				uint32 TotalHealthGain = 0;
				uint32 TotalManaGain = 0;

				switch(Class)
				{
					case WARRIOR:
						if(Level < 13)
							TotalHealthGain += 19;
						else if(Level < 36) 
							TotalHealthGain += Level + 6;
						else if(Level > 60) 
							TotalHealthGain += Level + 206;
						else 
							TotalHealthGain += 2 * Level - 30;
						break;
					case HUNTER:
						if(Level < 13)
							TotalHealthGain += 17;
						else if(Level > 60) 
							TotalHealthGain += Level + 161;
						else 
							TotalHealthGain += Level + 4;

						if(Level < 11)
							TotalManaGain += 29;
						else if(Level < 27)
							TotalManaGain += Level + 18;
						else if(Level > 60)
							TotalManaGain += Level + 150;
						else 
							TotalManaGain += 45;
						break;
					case ROGUE:
						if(Level < 15)
							TotalHealthGain += 17;
						else if(Level > 60) 
							TotalHealthGain += Level + 191;
						else 
							TotalHealthGain += Level + 2;
						break;
					case DRUID:
						if(Level < 17)
							TotalHealthGain += 17;
//					else if(Level >60) TotalHealthGain+=Level+55;
						else if(Level > 60) 
							TotalHealthGain += Level + 176;
						else 
							TotalHealthGain += Level;

						if(Level < 26)
							TotalManaGain += Level + 20;
						else if(Level > 60)
							TotalManaGain += Level + 150;
						else 
							TotalManaGain += 45;
						break;
					case MAGE:
						if(Level < 23)
							TotalHealthGain += 15;
						else if(Level > 60) 
							TotalHealthGain += Level + 190;
						else 
							TotalHealthGain += Level - 8;

						if(Level < 28)
							TotalManaGain += Level + 23;
						else if(Level > 60)
							TotalManaGain += Level + 115;
						else 
							TotalManaGain += 51;
						break;
					case SHAMAN:
						if(Level < 16)
							TotalHealthGain += 17;
						else if(Level > 60) 
							TotalHealthGain += Level + 157;
						else 
							TotalHealthGain += Level + 1;

						if(Level < 22)
							TotalManaGain += Level + 19;
						else if(Level > 60)
							TotalManaGain += Level + 175;
						else 
							TotalManaGain += 49;
						break;
					case WARLOCK:
						if(Level < 17)
							TotalHealthGain += 17;
						else if(Level > 60) 
							TotalHealthGain += Level + 192;
						else 
							TotalHealthGain += Level - 2;

						if(Level < 30)
							TotalManaGain += Level + 21;
						else if(Level > 60)
							TotalManaGain += Level + 121;
						else 
							TotalManaGain += 51;
						break;
					case PALADIN:
						if(Level < 14)
							TotalHealthGain += 18;
						else if(Level > 60)
							TotalHealthGain += Level + 167;
						else 
							TotalHealthGain += Level + 4;

						if(Level < 30)
							TotalManaGain += Level + 17;
						else if(Level > 60)
							TotalManaGain += Level + 131;
						else 
							TotalManaGain += 42;
						break;
					case PRIEST:
						if(Level < 21)
							TotalHealthGain += 15;
						else if(Level > 60) 
							TotalHealthGain += Level + 157;
						else 
							TotalHealthGain += Level - 6;

						if(Level < 22)
							TotalManaGain += Level + 22;
						else if(Level < 32)
							TotalManaGain += Level + 37;
						else if(Level > 60)
							TotalManaGain += Level + 207;
						else 
							TotalManaGain += 54;
						break;
					case DEATHKNIGHT: // Based on 55-56 more testing will be done.
						if(Level < 60)
							TotalHealthGain += 92;
						else 
							TotalHealthGain += 92;
						break;
				}

				// Apply HP/Mana
				lvl->HP = lastlvl.HP + TotalHealthGain;
				lvl->Mana = lastlvl.Mana + TotalManaGain;

				// Calculate next level XP
				uint32 nextLvlXP = 0;
				/*				if( Level > 0 && Level <= 30 )
								{
									nextLvlXP = ((int)((((double)(8 * Level * ((Level * 5) + 45)))/100)+0.5))*100;
								}
								else if( Level == 31 )
								{
									nextLvlXP = ((int)((((double)(((8 * Level) + 3) * ((Level * 5) + 45)))/100)+0.5))*100;
								}
								else if( Level == 32 )
								{
									nextLvlXP = ((int)((((double)(((8 * Level) + 6) * ((Level * 5) + 45)))/100)+0.5))*100;
								}
								else
								{
									nextLvlXP = ((int)((((double)(((8 * Level) + ((Level - 30) * 5)) * ((Level * 5) + 45)))/100)+0.5))*100;
								}*/

				//this is a fixed table taken from 2.3.0 wow. This can;t get more blizzlike with the "if" cases ;)
				if((Level - 1) < MAX_PREDEFINED_NEXTLEVELXP)
				{
					nextLvlXP = NextLevelXp[(Level - 1)];
				}
				else
				{
					// 2.2
					//double MXP = 45 + ( 5 * level );
					// 2.3
					double MXP = 235 + (5 * Level);
					double DIFF = Level < 29 ? 0.0 : Level < 30 ? 1.0 : Level < 31 ? 3.0 : Level < 32 ? 6.0 : 5.0 * (double(Level) - 30.0);
					double XP = ((8.0 * double(Level)) + DIFF) * MXP;
					nextLvlXP = (int)((XP / 100.0) + 0.5) * 100;
				}

				lvl->XPToNextLevel = nextLvlXP;
				lastlvl = *lvl;

				// Apply to map.
				lMap->insert(LevelMap::value_type(Level, lvl));
			}

			// Now that our level map is full, let's create the class/race pair
			pair<uint32, uint32> p;
			p.first = Race;
			p.second = Class;

			// Insert back into the main map.
			mLevelInfo.insert(LevelInfoMap::value_type(p, lMap));
		}
	}
	Log.Notice("ObjectMgr", "Generated %u level up information.", mLevelInfo.size());
}

LevelInfo* ObjectMgr::GetLevelInfo(uint32 Race, uint32 Class, uint32 Level)
{
	// Iterate levelinfo map until we find the right class+race.
	for (LevelInfoMap::iterator itr = mLevelInfo.begin(); itr != mLevelInfo.end(); ++itr)
	{
		if(itr->first.first == Race && itr->first.second == Class)
		{
			// We got a match.
			// Let's check that our level is valid first.
			if(Level > sWorld.m_genLevelCap)   // too far.
				Level = sWorld.m_genLevelCap;

			// Pull the level information from the second map.
			LevelMap::iterator it2 = itr->second->find(Level);
			ARCEMU_ASSERT(it2 != itr->second->end());

			return it2->second;
		}
	}

	return NULL;
}

void ObjectMgr::LoadDefaultPetSpells()
{
	Log.Notice("ObjectMgr", "Loading default pet spells data...");
	if (QueryResult* result = WorldDatabase.Query("SELECT entry, spell FROM petdefaultspells"))
	{
		do
		{
			Field* f = result->Fetch();
			uint32 Entry = f[0].GetUInt32();
			uint32 spell = f[1].GetUInt32();
			SpellEntry* sp = dbcSpell.LookupEntryForced(spell);
			if (!sp)
			{
				Log.Error("ObjectMgr", "Pet default spells contains non existing spell entry %u for pet entry %u.", spell, Entry);
				continue;
			}

			PetDefaultSpellMap::iterator itr = mDefaultPetSpells.find(Entry);
			if(itr != mDefaultPetSpells.end())
				itr->second.insert(sp);
			else
			{
				set<SpellEntry*> s;
				s.insert(sp);
				mDefaultPetSpells[Entry] = s;
			}
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u pet default spells.", mDefaultPetSpells.size());
}

set<SpellEntry*>* ObjectMgr::GetDefaultPetSpells(uint32 Entry)
{
	PetDefaultSpellMap::iterator itr = mDefaultPetSpells.find(Entry);
	if(itr == mDefaultPetSpells.end())
		return 0;

	return &(itr->second);
}

void ObjectMgr::LoadPetSpellCooldowns()
{
	for(DBCStorage< CreatureSpellDataEntry >::iterator itr = dbcCreatureSpellData.begin(); itr != dbcCreatureSpellData.end(); ++itr)
	{
		CreatureSpellDataEntry* csde = *itr;

		for(uint8 j = 0; j < 3; ++j)
		{
			uint32 SpellId = csde->Spells[ j ];
			uint32 Cooldown = csde->Cooldowns[ j ] * 10;

			if(SpellId)
			{
				PetSpellCooldownMap::iterator itr = mPetSpellCooldowns.find(SpellId);
				if(itr == mPetSpellCooldowns.end())
				{
					if(Cooldown)
						mPetSpellCooldowns.insert(make_pair(SpellId, Cooldown));
				}
				else
				{
					uint32 SP2 = mPetSpellCooldowns[SpellId];
					ARCEMU_ASSERT(Cooldown == SP2);
				}
			}
		}
	}
}

uint32 ObjectMgr::GetPetSpellCooldown(uint32 SpellId)
{
	PetSpellCooldownMap::iterator itr = mPetSpellCooldowns.find(SpellId);
	if(itr != mPetSpellCooldowns.end())
		return itr->second;

	SpellEntry* sp = dbcSpell.LookupEntry(SpellId);
	if( sp->RecoveryTime > sp->CategoryRecoveryTime )
		return sp->RecoveryTime;
	else
		return sp->CategoryRecoveryTime;
}

void ObjectMgr::LoadSpellOverride()
{
	Log.Notice("ObjectMgr", "Loading spell override data...");
	if (QueryResult* result = WorldDatabase.Query("SELECT DISTINCT overrideId FROM spelloverride"))
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 entry = fields[0].GetUInt32();
			if (!dbcSpell.LookupEntryForced(entry))
			{
				Log.Error("ObjectMgr", "Spell override entry %u does not exists!", entry);
				continue;
			}
			std::stringstream query;
			query.rdbuf()->str("");
			query << "SELECT spellId FROM spelloverride WHERE overrideId = ";
			query << fields[0].GetUInt32();
			QueryResult* resultIn = WorldDatabase.Query(query.str().c_str());
			std::list<SpellEntry*>* list = new std::list<SpellEntry*>;
			if (resultIn)
			{
				do
				{
					Field* fieldsIn = resultIn->Fetch();
					uint32 spellid = fieldsIn[0].GetUInt32();
					SpellEntry* sp = dbcSpell.LookupEntryForced(spellid);
					if (!sp)
					{
						Log.Error("ObjectMgr", "Spell override entry %u contains non existing spellid %u.", entry, spellid);
						continue;
					}
					list->push_back(sp);
				} while (resultIn->NextRow());
				delete resultIn;
			}
			if (list->size() == 0)
				delete list;
			else
				mOverrideIdMap.insert(OverrideIdMap::value_type(fields[0].GetUInt32(), list));
		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u spell overrides.", mOverrideIdMap.size());
}

void ObjectMgr::SetVendorList(uint32 Entry, std::vector<CreatureItem>* list_)
{
	mVendors[Entry] = list_;
}

void ObjectMgr::LoadCreatureTimedEmotes()
{
	Log.Notice("ObjectMgr", "Loading creature timed emotes data...");
	uint32 count = 0;
	if (QueryResult* result = WorldDatabase.Query("SELECT spawnid, rowid, type, value, msg, msg_type, msg_lang, expire_after FROM creature_timed_emotes order by rowid asc"))
	{
		do
		{
			Field* fields = result->Fetch();
			spawn_timed_emotes* te = new spawn_timed_emotes;
			te->type = fields[2].GetUInt8();
			te->value = fields[3].GetUInt32();
			char* str = (char*)fields[4].GetString();
			if (str)
			{
				uint32 len = (int)strlen(str);
				te->msg = new char[len + 1];
				memcpy(te->msg, str, len + 1);
			}
			else te->msg = NULL;
			te->msg_type = static_cast<uint8>(fields[5].GetUInt32());
			te->msg_lang = static_cast<uint8>(fields[6].GetUInt32());
			te->expire_after = fields[7].GetUInt32();

			HM_NAMESPACE::hash_map<uint32, TimedEmoteList*>::const_iterator i;
			uint32 spawnid = fields[0].GetUInt32();
			i = m_timedemotes.find(spawnid);
			if (i == m_timedemotes.end())
			{
				TimedEmoteList* m = new TimedEmoteList;
				m->push_back(te);
				m_timedemotes[spawnid] = m;
			}
			else
			{
				i->second->push_back(te);
			}
			++count;
		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u creature timed emotes.", count);
}

TimedEmoteList* ObjectMgr::GetTimedEmoteList(uint32 spawnid)
{
	HM_NAMESPACE::hash_map<uint32, TimedEmoteList*>::const_iterator i = m_timedemotes.find(spawnid);
	if(i != m_timedemotes.end())
	{
		TimedEmoteList* m = i->second;
		return m;
	}

	return NULL;
}

void ObjectMgr::LoadCreatureWaypoints()
{
	Log.Notice("ObjectMgr", "Loading creature waypoints...");
	uint64 count = 0;
	std::string query = "SELECT spawnid, waypointid, position_x, position_y, position_z, waittime, flags, forwardemoteoneshot, forwardemoteid,";
	query.append("backwardemoteoneshot, backwardemoteid, forwardskinid, backwardskinid FROM creature_waypoints");
	if (QueryResult* result = WorldDatabase.Query(query.c_str()))
	{
		do
		{
			Field* fields = result->Fetch();
			WayPoint* wp = new WayPoint;
			wp->id = fields[1].GetUInt32();
			wp->x = fields[2].GetFloat();
			wp->y = fields[3].GetFloat();
			wp->z = fields[4].GetFloat();
			wp->waittime = fields[5].GetUInt32();
			wp->flags = fields[6].GetUInt32();
			wp->forwardemoteoneshot = fields[7].GetBool();
			wp->forwardemoteid = fields[8].GetUInt32();
			wp->backwardemoteoneshot = fields[9].GetBool();
			wp->backwardemoteid = fields[10].GetUInt32();
			wp->forwardskinid = fields[11].GetUInt32();
			wp->backwardskinid = fields[12].GetUInt32();

			uint32 spawnid = fields[0].GetUInt32();
			HM_NAMESPACE::hash_map<uint32, WayPointMap*>::const_iterator i = m_waypoints.find(spawnid);
			if (i == m_waypoints.end())
			{
				WayPointMap* m = new WayPointMap;
				if (m->size() <= wp->id)
					m->resize(wp->id + 1);
				(*m)[wp->id] = wp;
				m_waypoints[spawnid] = m;
			}
			else
			{
				if (i->second->size() <= wp->id)
					i->second->resize(wp->id + 1);

				(*(i->second))[wp->id] = wp;
			}
			++count;
		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %lu creature waypoints", count);
}

WayPointMap* ObjectMgr::GetWayPointMap(uint32 spawnid)
{
	HM_NAMESPACE::hash_map<uint32, WayPointMap*>::const_iterator i = m_waypoints.find(spawnid);
	if(i != m_waypoints.end())
	{
		WayPointMap* m = i->second;
		// we don't wanna erase from the map, because some are used more
		// than once (for instances)

		//m_waypoints.erase(i);
		return m;
	}

	return NULL;
}

Pet* ObjectMgr::CreatePet(uint32 entry)
{
	uint32 guid = ++m_hiPetGuid;
	return new Pet(Arcemu::Util::MAKE_PET_GUID(entry, guid));
}

Player* ObjectMgr::CreatePlayer(uint8 _class)
{
	uint32 guid = ++m_hiPlayerGuid;
	Player* result = NULL;

	switch(_class)
	{
		case WARRIOR:
			result = new Warrior(guid);
			break;
		case PALADIN:
			result = new Paladin(guid);
			break;
		case HUNTER:
			result = new Hunter(guid);
			break;
		case ROGUE:
			result = new Rogue(guid);
			break;
		case PRIEST:
			result = new Priest(guid);
			break;
		case DEATHKNIGHT:
			result = new DeathKnight(guid);
			break;
		case SHAMAN:
			result = new Shaman(guid);
			break;
		case MAGE:
			result = new Mage(guid);
			break;
		case WARLOCK:
			result = new Warlock(guid);
			break;
		case DRUID:
			result = new Druid(guid);
			break;
	}

	return result;
}

void ObjectMgr::AddPlayer(Player* p) //add it to global storage
{
	_playerslock.AcquireWriteLock();
	_players[p->GetLowGUID()] = p;
	_playerslock.ReleaseWriteLock();
}

void ObjectMgr::RemovePlayer(Player* p)
{
	_playerslock.AcquireWriteLock();
	_players.erase(p->GetLowGUID());
	_playerslock.ReleaseWriteLock();

}

Corpse* ObjectMgr::CreateCorpse()
{
	uint32 guid = ++m_hiCorpseGuid;
	return new Corpse(HIGHGUID_TYPE_CORPSE, guid);
}

void ObjectMgr::AddCorpse(Corpse* p) //add it to global storage
{
	_corpseslock.Acquire();
	m_corpses[p->GetLowGUID()] = p;
	_corpseslock.Release();
}

void ObjectMgr::RemoveCorpse(Corpse* p)
{
	_corpseslock.Acquire();
	m_corpses.erase(p->GetLowGUID());
	_corpseslock.Release();
}

Corpse* ObjectMgr::GetCorpse(uint32 corpseguid)
{
	_corpseslock.Acquire();
	CorpseMap::const_iterator itr = m_corpses.find(corpseguid);
	Corpse* rv = (itr != m_corpses.end()) ? itr->second : 0;
	_corpseslock.Release();
	return rv;
}

Transporter* ObjectMgr::GetTransporter(uint32 guid)
{
	_TransportLock.Acquire();
	HM_NAMESPACE::hash_map<uint32, Transporter*>::const_iterator itr = mTransports.find(guid);
	Transporter* rv = (itr != mTransports.end()) ? itr->second : 0;
	_TransportLock.Release();
	return rv;
}

void ObjectMgr::AddTransport(Transporter* pTransporter)
{
	_TransportLock.Acquire();
	mTransports[pTransporter->GetUIdFromGUID()] = pTransporter;
	_TransportLock.Release();
}

Transporter* ObjectMgr::GetTransporterByEntry(uint32 entry)
{
	Transporter* rv = NULL;
	_TransportLock.Acquire();
	for (HM_NAMESPACE::hash_map<uint32, Transporter*>::iterator itr = mTransports.begin(); itr != mTransports.end(); ++itr)
	{
		if(itr->second->GetEntry() == entry)
		{
			rv = itr->second;
			break;
		}
	}
	_TransportLock.Release();
	return rv;
}

void ObjectMgr::LoadGuildCharters()
{
	Log.Notice("ObjectMgr", "Loading guild charters...");
	m_hiCharterId.SetVal(0);
	std::string query = "SELECT charterId, charterType, leaderGuid, guildName, itemGuid, signer1, signer2, signer3, signer4, signer5, signer6";
	query.append("signer7, signer8, signer9 FROM charters");

	if (QueryResult* result = CharacterDatabase.Query(query.c_str()))
	{
		do
		{
			Charter* c = new Charter(result->Fetch());
			m_charters[c->CharterType].insert(make_pair(c->GetID(), c));
			if (c->GetID() > int64(m_hiCharterId.GetVal()))
				m_hiCharterId.SetVal(c->GetID());
		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u guild charters.", m_charters[0].size());
}

Charter* ObjectMgr::GetCharter(uint32 CharterId, CharterTypes Type)
{
	m_charterLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[Type].find(CharterId);
	Charter* rv = (itr == m_charters[Type].end()) ? 0 : itr->second;
	m_charterLock.ReleaseReadLock();
	return rv;
}

Charter* ObjectMgr::CreateCharter(uint32 LeaderGuid, CharterTypes Type)
{

	uint32 charterid = ++m_hiCharterId;
	if (Charter* c = new Charter(charterid, LeaderGuid, Type))
	{
		m_charters[c->CharterType].insert(make_pair(c->GetID(), c));
		return c;
	}
	return NULL;
}

Charter::Charter(Field* fields)
{
	uint32 f = 0;
	CharterId = fields[f++].GetUInt32();
	CharterType = fields[f++].GetUInt32();
	LeaderGuid = fields[f++].GetUInt32();
	GuildName = fields[f++].GetString();
	ItemGuid = fields[f++].GetUInt64();
	SignatureCount = 0;
	Slots = GetNumberOfSlotsByType();
	Signatures = new uint32[Slots];

	for(uint32 i = 0; i < Slots; ++i)
	{
		Signatures[i] = fields[f++].GetUInt32();
		if(Signatures[i])
			++SignatureCount;
	}
}

void Charter::AddSignature(uint32 PlayerGuid)
{
	if(SignatureCount >= Slots)
		return;

	SignatureCount++;
	for(uint32 i = 0; i < Slots; ++i)
	{
		if(Signatures[i] == 0)
		{
			Signatures[i] = PlayerGuid;
			break;
		}
	}
}

void Charter::RemoveSignature(uint32 PlayerGuid)
{
	for(uint32 i = 0; i < Slots; ++i)
	{
		if(Signatures[i] == PlayerGuid)
		{
			Signatures[i] = 0;
			SignatureCount--;
			SaveToDB();
			break;
		}
	}
}


void Charter::Destroy()
{
	if(Slots == 0)			// ugly hack because of f*cked memory
		return;

	//meh remove from objmgr
	objmgr.RemoveCharter(this);
	// Kill the players with this (in db/offline)
	CharacterDatabase.Execute("DELETE FROM charters WHERE charterId = %u", CharterId);
	Player* p = NULL;
#ifdef WIN32
	__try
	{
#endif
		for(uint32 i = 0; i < Slots; ++i)
		{
			if(!Signatures[i])
				continue;
			p =  objmgr.GetPlayer(Signatures[i]);
			if(p != NULL)
				p->m_charters[CharterType] = 0;
		}
#ifdef WIN32
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
#endif

	// click, click, boom!
	delete this;
}

void Charter::SaveToDB()
{
	std::stringstream ss;

	ss << "DELETE FROM charters WHERE charterId = ";
	ss << CharterId;
	ss << ";";

	CharacterDatabase.Execute(ss.str().c_str());

	ss.rdbuf()->str("");

	ss << "INSERT INTO charters VALUES(" << CharterId << "," << CharterType << "," << LeaderGuid << ",'" << GuildName << "'," << ItemGuid;

	uint32 i = 0;
	for(; i < Slots; ++i)
		ss << "," << Signatures[i];

	for(; i < 9; ++i)
		ss << ",0";

	ss << ")";
	CharacterDatabase.Execute(ss.str().c_str());
}

Charter* ObjectMgr::GetCharterByItemGuid(uint64 guid)
{
	m_charterLock.AcquireReadLock();
	for(uint8 i = 0; i < NUM_CHARTER_TYPES; ++i)
	{
		for (HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[i].begin(); itr != m_charters[i].end(); ++itr)
		{
			if(itr->second->ItemGuid == guid)
			{
				m_charterLock.ReleaseReadLock();
				return itr->second;
			}
		}
	}
	m_charterLock.ReleaseReadLock();
	return NULL;
}

Charter* ObjectMgr::GetCharterByGuid(uint64 playerguid, CharterTypes type)
{
	m_charterLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[type].begin();
	for(; itr != m_charters[type].end(); ++itr)
	{
		if(playerguid == itr->second->LeaderGuid)
		{
			m_charterLock.ReleaseReadLock();
			return itr->second;
		}

		for(uint32 j = 0; j < itr->second->SignatureCount; ++j)
		{
			if(itr->second->Signatures[j] == playerguid)
			{
				m_charterLock.ReleaseReadLock();
				return itr->second;
			}
		}
	}
	m_charterLock.ReleaseReadLock();
	return NULL;
}

Charter* ObjectMgr::GetCharterByName(string & charter_name, CharterTypes Type)
{
	Charter* rv = NULL;
	m_charterLock.AcquireReadLock();
	for (HM_NAMESPACE::hash_map<uint32, Charter*>::iterator itr = m_charters[Type].begin(); itr != m_charters[Type].end(); ++itr)
	{
		if(itr->second->GuildName == charter_name)
		{
			rv = itr->second;
			break;
		}
	}

	m_charterLock.ReleaseReadLock();
	return rv;
}

void ObjectMgr::RemoveCharter(Charter* c)
{
	if(c == NULL)
		return;

	if(c->CharterType >= NUM_CHARTER_TYPES)
	{
		Log.Notice("ObjectMgr", "Charter %u cannot be destroyed as type %u is not a sane type value.", c->CharterId, c->CharterType);
		return;
	}
	m_charterLock.AcquireWriteLock();
	m_charters[c->CharterType].erase(c->CharterId);
	m_charterLock.ReleaseWriteLock();
}

//TODO: correct repuatation_faction_kill - that function doesn't loads that table values correctly.
//TODO: split up loads
//TODO: add full select query
void ObjectMgr::LoadReputationModifierTable(const char* tablename, ReputationModMap* dmap)
{
	if (QueryResult* result = WorldDatabase.Query("SELECT * FROM %s", tablename))
	{
		do
		{
			ReputationMod mod;
			mod.faction[TEAM_ALLIANCE] = result->Fetch()[1].GetUInt32();
			if (mod.faction[TEAM_ALLIANCE] && !dbcFaction.LookupEntryForced(mod.faction[0]))
			{
				Log.Error("ObjectMgr", "Non existing faction id %u in %s table for object entry %u.", mod.faction[TEAM_ALLIANCE], tablename, result->Fetch()[0].GetUInt32());
				continue;
			}

			mod.faction[TEAM_HORDE] = result->Fetch()[2].GetUInt32();
			if (mod.faction[TEAM_HORDE] && !dbcFaction.LookupEntryForced(mod.faction[0]))
			{
				Log.Error("ObjectMgr", "Non existing faction id %u in %s table for object entry %u.", mod.faction[TEAM_HORDE], tablename, result->Fetch()[0].GetUInt32());
				continue;
			}
			mod.value = result->Fetch()[3].GetInt32();
			mod.replimit = result->Fetch()[4].GetUInt32();

			ReputationModMap::iterator itr = dmap->find(result->Fetch()[0].GetUInt32());
			if(itr == dmap->end())
			{
				ReputationModifier* modifier = new ReputationModifier;

				modifier->entry = result->Fetch()[0].GetUInt32();
				modifier->mods.push_back(mod);
				dmap->insert(ReputationModMap::value_type(result->Fetch()[0].GetUInt32(), modifier));
			}
			else
				itr->second->mods.push_back(mod);
		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u %s reputation modifiers.", dmap->size(), tablename);
}

void ObjectMgr::LoadReputationModifiers()
{
	Log.Notice("ObjectMgr", "Loading repuation creature onkill data...");
	LoadReputationModifierTable("reputation_creature_onkill", &m_reputation_creature);

	Log.Notice("ObjectMgr", "Loading faction onkill data...");
	LoadReputationModifierTable("reputation_faction_onkill", &m_reputation_faction);

	Log.Notice("ObjectMgr", "Loading instance reputation_instance_onkill data...");
	LoadInstanceReputationModifiers();
}

ReputationModifier* ObjectMgr::GetReputationModifier(uint32 entry_id, uint32 faction_id)
{
	// first, try fetching from the creature table (by faction is a fallback)
	ReputationModMap::iterator itr = m_reputation_creature.find(entry_id);
	if(itr != m_reputation_creature.end())
		return itr->second;

	// fetch from the faction table
	itr = m_reputation_faction.find(faction_id);
	if(itr != m_reputation_faction.end())
		return itr->second;

	// no data. fallback to default -5 value.
	return 0;
}

void ObjectMgr::LoadMonsterSay()
{
	Log.Notice("ObjectMgr", "Loading npc_monstersay data...");
	uint32 count = 0;
	if (QueryResult* result = WorldDatabase.Query("SELECT entry, event, chance, language, type, monstername, text0, text1, text2, text3, text4 FROM npc_monstersay"))
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 Entry = fields[0].GetUInt32();
			if (!CreatureNameStorage.LookupEntry(Entry))
			{
				Log.Error("ObjectMgr", "Tryed to load npc_monstersay data for non existing creature %u.", Entry);
				continue;
			}

			uint32 Event = fields[1].GetUInt32();

			if (Event >= NUM_MONSTER_SAY_EVENTS)
			{
				Log.Error("ObjectMgr", "Invalid monstersay event type %u for npc entry %u.", Event, Entry);
				continue;
			}

			if (mMonsterSays[Event].find(Entry) != mMonsterSays[Event].end())
			{
				Log.Error("ObjectMgr", "Duplicate monstersay event %u for entry %u, skipping", Event, Entry);
				continue;
			}

			NpcMonsterSay* ms = new NpcMonsterSay;
			ms->Chance = fields[2].GetFloat();
			ms->Language = fields[3].GetUInt32();
			ms->Type = fields[4].GetUInt32();
			ms->MonsterName = fields[5].GetString() ? strdup(fields[5].GetString()) : strdup("None");

			char* texts[5];
			char* text;
			uint8 textcount = 0;

			for (uint8 i = 0; i < 5; ++i)
			{
				text = (char*)fields[6 + i].GetString();
				if (!text) continue;
				if (strlen(fields[6 + i].GetString()) < 5)
					continue;

				texts[textcount] = strdup(fields[6 + i].GetString());

				// check for ;
				if (texts[textcount][strlen(texts[textcount]) - 1] == ';')
					texts[textcount][strlen(texts[textcount]) - 1] = 0;

				++textcount;
			}

			if (!textcount)
			{
				free(((char*)ms->MonsterName));
				delete ms;
				continue;
			}

			ms->Texts = new const char*[textcount];
			memcpy(ms->Texts, texts, sizeof(char*)* textcount);
			ms->TextCount = textcount;

			mMonsterSays[Event].insert(make_pair(Entry, ms));
			++count;
		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u monster say events.", count);
}

NpcMonsterSay* ObjectMgr::HasMonsterSay(uint32 Entry, MONSTER_SAY_EVENTS Event)
{
	MonsterSayMap::iterator itr = mMonsterSays[Event].find(Entry);
	if(itr == mMonsterSays[Event].end())
		return NULL;

	return itr->second;
}

void ObjectMgr::LoadInstanceReputationModifiers()
{
	if (QueryResult* result = WorldDatabase.Query("SELECT mapid, mob_rep_reward, mob_rep_limit, boss_rep_reward, boss_rep_limit, faction_change_alliance, faction_change_horde FROM reputation_instance_onkill"))
	{
		do
		{
			Field* fields = result->Fetch();
			InstanceReputationMod mod;
			mod.mapid = fields[0].GetUInt32();
			if (!dbcMap.LookupEntryForced(mod.mapid))
			{
				Log.Error("ObjectMgr", "Tryed to load repuation instance onkill data for non existing map entry %u.", mod.mapid);
				continue;
			}
			mod.mob_rep_reward = fields[1].GetInt32();
			mod.mob_rep_limit = fields[2].GetUInt32();
			mod.boss_rep_reward = fields[3].GetInt32();
			mod.boss_rep_limit = fields[4].GetUInt32();
			mod.faction[0] = fields[5].GetUInt32();
			if (!dbcFaction.LookupEntryForced(mod.faction[0]))
			{
				Log.Error("ObjectMgr", "Repuation instance onkill contains non existing alliance faction id %u for map %u.", mod.faction[0], mod.mapid);
				continue;
			}

			mod.faction[1] = fields[6].GetUInt32();
			if (!dbcFaction.LookupEntryForced(mod.faction[0]))
			{
				Log.Error("ObjectMgr", "Repuation instance onkill contains non existing alliance faction id %u for map %u.", mod.faction[1], mod.mapid);
				continue;
			}

			HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*>::iterator itr = m_reputation_instance.find(mod.mapid);
			if (itr == m_reputation_instance.end())
			{
				InstanceReputationModifier* m = new InstanceReputationModifier;
				m->mapid = mod.mapid;
				m->mods.push_back(mod);
				m_reputation_instance.insert(make_pair(m->mapid, m));
			}
			else
				itr->second->mods.push_back(mod);

		} while (result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u instance reputation onkill entries.", m_reputation_instance.size());
}

bool ObjectMgr::HandleInstanceReputationModifiers(Player* pPlayer, Unit* pVictim)
{
	uint32 team = pPlayer->GetTeam();
	bool is_boss = false;
	if(!pVictim->IsCreature())
		return false;

	HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*>::iterator itr = m_reputation_instance.find(pVictim->GetMapId());
	if(itr == m_reputation_instance.end())
		return false;

	if(TO< Creature* >(pVictim)->GetProto()->boss)
		is_boss = true;

	// Apply the bonuses as normal.
	int32 replimit = 0;
	int32 value = 0;

	for(vector<InstanceReputationMod>::iterator i = itr->second->mods.begin(); i !=  itr->second->mods.end(); ++i)
	{
		if(!(*i).faction[team])
			continue;

		if(is_boss)
		{
			value = i->boss_rep_reward;
			replimit = i->boss_rep_limit;
		}
		else
		{
			value = i->mob_rep_reward;
			replimit = i->mob_rep_limit;
		}

		if(!value || (replimit && pPlayer->GetStanding(i->faction[team]) >= replimit))
			continue;

		//value *= sWorld.getRate(RATE_KILLREPUTATION);
		value = float2int32(value * sWorld.getRate(RATE_KILLREPUTATION));
		pPlayer->ModStanding(i->faction[team], value);
	}

	return true;
}

void ObjectMgr::LoadDisabledSpells()
{
	Log.Notice("ObjectMgr", "Loading disabled spells...");
	if (QueryResult* result = WorldDatabase.Query("SELECT spellid, replacement_spellid FROM spell_disable"))
	{
		do
		{
			uint32 entry = result->Fetch()[0].GetUInt32();
			if (!dbcSpell.LookupEntryForced(entry))
			{
				Log.Error("ObjectMgr", "Tryed to load disabled spell entry %u but spell does not exists!", entry);
				continue;
			}
			m_disabled_spells.insert(result->Fetch()[0].GetUInt32());
		}
		while(result->NextRow());
		delete result;
	}

	Log.Success("ObjectMgr", "Loaded %u disabled spells.", m_disabled_spells.size());
}

void ObjectMgr::ReloadDisabledSpells()
{
	m_disabled_spells.clear();
	LoadDisabledSpells();
}

void ObjectMgr::LoadGroups()
{
	Log.Notice("ObjectMgr", "Loading groups...");
	std::string query = "SELECT group_id, group_type, subgroup_count, loot_method, loot_threshold, difficulty, raiddifficulty, assistant_leader, main_tank,";
	query.append("main_assist, group1member1, group1member2, group1member3, group1member4, group1member5, group2member1, group2member2, group2member3,");
	query.append("group2member4, group2member5, group3member1, group3member2, group3member3, group3member4, group3member5, group4member1, group4member2,");
	query.append("group4member3, group4member4, group4member5, group5member1, group5member2, group5member3, group5member4, group5member5, group6member1,");
	query.append("group6member2, group6member3, group6member4, group6member5, group7member1, group7member2`, group7member3, group7member4, group7member5,");
	query.append("group8member1, group8member2, group8member3, group8member4, group8member5, timestamp, instanceids FROM groups");
	if (QueryResult* result = CharacterDatabase.Query("SELECT * FROM groups"))
	{
		do
		{
			Group* g = new Group(false);
			g->LoadFromDB(result->Fetch());
		}
		while(result->NextRow());
		delete result;
	}

	Log.Success("ObjectMgr", "Loaded %u groups.", m_groups.size());
}

void ObjectMgr::LoadArenaTeams()
{
	Log.Notice("ObjectMgr", "Loading arena teams...");
	std::string query = "SELECT id, type, leader, name, emblemstyle, emblemcolour, borderstyle, bordercolour, backgroundcolour, rating, data, ranking,";
	query.append("player_data1, player_data2, player_data3, player_data4, player_data5, player_data6, player_data7, player_data8, player_data9,");
	query.append("player_data10 FROM arenateams");

	if (QueryResult* result = CharacterDatabase.Query(query.c_str()))
	{
		do
		{
			ArenaTeam* team = new ArenaTeam(result->Fetch());
			AddArenaTeam(team);
			if(team->m_id > uint32(m_hiArenaTeamId.GetVal()))
				m_hiArenaTeamId.SetVal(uint32(team->m_id));

		}
		while(result->NextRow());
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u arena teams.", m_arenaTeams.size());

	/* update the ranking */
	UpdateArenaTeamRankings();
}

ArenaTeam* ObjectMgr::GetArenaTeamByGuid(uint32 guid, uint32 Type)
{
	m_arenaTeamLock.Acquire();
	for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeamMap[Type].begin(); itr != m_arenaTeamMap[Type].end(); ++itr)
	{
		if(itr->second->HasMember(guid))
		{
			m_arenaTeamLock.Release();
			return itr->second;
		}
	}
	m_arenaTeamLock.Release();
	return NULL;
}

ArenaTeam* ObjectMgr::GetArenaTeamById(uint32 id)
{
	HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr;
	m_arenaTeamLock.Acquire();
	itr = m_arenaTeams.find(id);
	m_arenaTeamLock.Release();
	return (itr == m_arenaTeams.end()) ? NULL : itr->second;
}

ArenaTeam* ObjectMgr::GetArenaTeamByName(string & name, uint32 Type)
{
	m_arenaTeamLock.Acquire();
	for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeams.begin(); itr != m_arenaTeams.end(); ++itr)
	{
		if(!strnicmp(itr->second->m_name.c_str(), name.c_str(), name.size()))
		{
			m_arenaTeamLock.Release();
			return itr->second;
		}
	}
	m_arenaTeamLock.Release();
	return NULL;
}

void ObjectMgr::RemoveArenaTeam(ArenaTeam* team)
{
	m_arenaTeamLock.Acquire();
	m_arenaTeams.erase(team->m_id);
	m_arenaTeamMap[team->m_type].erase(team->m_id);
	m_arenaTeamLock.Release();
}

void ObjectMgr::AddArenaTeam(ArenaTeam* team)
{
	m_arenaTeamLock.Acquire();
	m_arenaTeams[team->m_id] = team;
	m_arenaTeamMap[team->m_type].insert(make_pair(team->m_id, team));
	m_arenaTeamLock.Release();
}

class ArenaSorter
{
	public:
		bool operator()(ArenaTeam* const & a, ArenaTeam* const & b)
		{
			return (a->m_stat_rating > b->m_stat_rating);
		}
		bool operator()(ArenaTeam*& a, ArenaTeam*& b)
		{
			return (a->m_stat_rating > b->m_stat_rating);
		}
};

void ObjectMgr::UpdateArenaTeamRankings()
{
	m_arenaTeamLock.Acquire();
	for(uint32 i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
	{
		vector<ArenaTeam*> ranking;

		for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeamMap[i].begin(); itr != m_arenaTeamMap[i].end(); ++itr)
			ranking.push_back(itr->second);

		std::sort(ranking.begin(), ranking.end(), ArenaSorter());
		uint32 rank = 1;
		for(vector<ArenaTeam*>::iterator itr = ranking.begin(); itr != ranking.end(); ++itr)
		{
			if((*itr)->m_stat_ranking != rank)
			{
				(*itr)->m_stat_ranking = rank;
				(*itr)->SaveToDB();
			}
			++rank;
		}
	}
	m_arenaTeamLock.Release();
}

void ObjectMgr::ResetArenaTeamRatings()
{
	m_arenaTeamLock.Acquire();
	for(uint32 i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
	{
		for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeamMap[i].begin(); itr != m_arenaTeamMap[i].end(); ++itr)
		{
			if (ArenaTeam* team = itr->second)
			{
				team->m_stat_gamesplayedseason = 0;
				team->m_stat_gamesplayedweek = 0;
				team->m_stat_gameswonseason = 0;
				team->m_stat_gameswonweek = 0;
				team->m_stat_rating = 1500;
				for(uint32 j = 0; j < team->m_memberCount; ++j)
				{
					team->m_members[j].Played_ThisSeason = 0;
					team->m_members[j].Played_ThisWeek = 0;
					team->m_members[j].Won_ThisSeason = 0;
					team->m_members[j].Won_ThisWeek = 0;
					team->m_members[j].PersonalRating = 1500;
				}
				team->SaveToDB();
			}
		}
	}
	m_arenaTeamLock.Release();

	UpdateArenaTeamRankings();
}

void ObjectMgr::UpdateArenaTeamWeekly()
{
	// reset weekly matches count for all teams and all members
	m_arenaTeamLock.Acquire();
	for(uint8 i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
	{
		for(HM_NAMESPACE::hash_map<uint32, ArenaTeam*>::iterator itr = m_arenaTeamMap[i].begin(); itr != m_arenaTeamMap[i].end(); ++itr)
		{
			ArenaTeam* team = itr->second;
			if(team)
			{
				team->m_stat_gamesplayedweek = 0;
				team->m_stat_gameswonweek = 0;
				for(uint32 j = 0; j < team->m_memberCount; ++j)
				{
					team->m_members[j].Played_ThisWeek = 0;
					team->m_members[j].Won_ThisWeek = 0;
				}
				team->SaveToDB();
			}
		}
	}
	m_arenaTeamLock.Release();
}

void ObjectMgr::ResetDailies()
{
	_playerslock.AcquireReadLock();
	PlayerStorageMap::iterator itr = _players.begin();
	for(; itr != _players.end(); itr++)
	{
		Player* pPlayer = itr->second;
		pPlayer->DailyMutex.Acquire();
		pPlayer->m_finishedDailies.clear();
		pPlayer->DailyMutex.Release();
	}
	_playerslock.ReleaseReadLock();
}

void ObjectMgr::LoadSpellTargetConstraints()
{
	enum { CREATURE_TYPE, GAMEOBJECT_TYPE };

	Log.Notice("ObjectMgr", "Loading spell target constraints...");
	if (QueryResult* result = WorldDatabase.Query("SELECT SpellID, TargetType, TargetID FROM spelltargetconstraints WHERE SpellID > 0 ORDER BY SpellID"))
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 spellid = fields[0].GetUInt32();
			if (!dbcSpell.LookupEntryForced(spellid))
			{
				Log.Error("ObjectMgr", "Tryed to load spell target constraints data for non existing spell %u.", spellid);
				continue;
			}

			uint32 type = fields[1].GetUInt32();
			if (type > GAMEOBJECT_TYPE)
			{
				Log.Error("ObjectMgr", "Tryed to load spell target constraints for spell %u with invalid type %u.", spellid, type);
				continue;
			}

			uint32 value = fields[2].GetUInt32();

			SpellTargetConstraint* stc = new SpellTargetConstraint;
			if (type == CREATURE_TYPE)
			{
				if (!CreatureNameStorage.LookupEntry(value))
				{
					Log.Error("ObjectMgr", "Tryed to add spell %u target info to non existing npc %u.", spellid, value);
					continue;
				}
				stc->AddCreature(value);
			}
			else
			{
				if (!GameObjectNameStorage.LookupEntry(value))
				{
					Log.Error("ObjectMgr", "Tryed to add spell %u target info to non existing object %u", spellid, value);
					continue;
				}
				stc->AddGameobject(value);
			}
			m_spelltargetconstraints.insert(std::pair< uint32, SpellTargetConstraint* >(spellid, stc));
		}
		while(result->NextRow());
		delete result;
	}

	Log.Success("ObjectMgr", "Loaded %u spell target constraints entries.", m_spelltargetconstraints.size());
}

SpellTargetConstraint* ObjectMgr::GetSpellTargetConstraintForSpell(uint32 spellid)
{
	SpellTargetConstraintMap::const_iterator itr = m_spelltargetconstraints.find(spellid);

	if(itr != m_spelltargetconstraints.end())
		return itr->second;

	return NULL;
}

uint32 ObjectMgr::GenerateArenaTeamId()
{
	return ++m_hiArenaTeamId;
}

uint32 ObjectMgr::GenerateGroupId()
{
	return ++m_hiGroupId;
}

uint32 ObjectMgr::GenerateGuildId()
{
	return ++m_hiGuildId;
}

uint32 ObjectMgr::GenerateCreatureSpawnID()
{
	return ++m_hiCreatureSpawnId;
}

uint32 ObjectMgr::GenerateGameObjectSpawnID()
{
	return ++m_hiGameObjectSpawnId;
}

void ObjectMgr::AddPlayerCache(uint32 guid, PlayerCache* cache)
{
	m_playerCacheLock.Acquire();
	cache->AddRef();
	PlayerCacheMap::iterator itr = m_playerCache.find(guid);
	if(itr != m_playerCache.end())
	{
		itr->second->DecRef();
		itr->second = cache;
	}
	else
		m_playerCache.insert(std::make_pair(guid, cache));
	m_playerCacheLock.Release();
}

void ObjectMgr::RemovePlayerCache(uint32 guid)
{
	m_playerCacheLock.Acquire();
	PlayerCacheMap::iterator itr = m_playerCache.find(guid);
	if(itr != m_playerCache.end())
	{
		itr->second->DecRef();
		m_playerCache.erase(itr);
	}
	m_playerCacheLock.Release();
}

PlayerCache* ObjectMgr::GetPlayerCache(uint32 guid)
{
	m_playerCacheLock.Acquire();
	PlayerCacheMap::iterator itr = m_playerCache.find(guid);
	if(itr != m_playerCache.end())
	{
		if (PlayerCache* ret = itr->second)
		{
			ret->AddRef();
			m_playerCacheLock.Release();
			return ret;
		}
	}
	m_playerCacheLock.Release();
	return NULL;
}

PlayerCache* ObjectMgr::GetPlayerCache(const char* name, bool caseSensitive /*= true*/)
{
	PlayerCache* ret = NULL;
	m_playerCacheLock.Acquire();
	PlayerCacheMap::iterator itr;

	if(!caseSensitive)
	{
		std::string strName = name;
		arcemu_TOLOWER(strName);
		for(itr = m_playerCache.begin(); itr != m_playerCache.end(); ++itr)
		{
			std::string cachename;
			itr->second->GetStringValue(CACHE_PLAYER_NAME, cachename);
			if(!stricmp(cachename.c_str(), strName.c_str()))
			{
				if (ret = itr->second)
				{
					ret->AddRef();
					break;
				}
			}
		}
	}
	else
	{
		for(itr = m_playerCache.begin(); itr != m_playerCache.end(); ++itr)
		{
			std::string cachename;
			itr->second->GetStringValue(CACHE_PLAYER_NAME, cachename);
			if(!strcmp(cachename.c_str(), name))
			{
				if (ret = itr->second)
				{
					itr->second->AddRef();
					break;
				}
			}
		}
	}

	m_playerCacheLock.Release();
	return ret;
}

void ObjectMgr::LoadVehicleAccessories()
{
	Log.Notice("ObjectMgr", "Loading vehicle accessories...");
	if (QueryResult *result = WorldDatabase.Query("SELECT creature_entry, accessory_entry, seat FROM vehicle_accessories;"))
	{
		do
		{
			Field *row = result->Fetch();
			VehicleAccessoryEntry *entry = new VehicleAccessoryEntry();
			
			uint32 creature_entry = row[0].GetUInt32();
			if (!CreatureNameStorage.LookupEntry(creature_entry))
			{
				Log.Error("ObjectMgr", "Invalid creature entry %u in vehicle_accessories", creature_entry);
				continue;
			}

			entry->accessory_entry = row[1].GetUInt32();
			//TODO: accessory entry should be checked by dbc values...

			entry->seat = row[2].GetUInt32();
			if (dbcVehicleSeat.LookupEntryForced(entry->seat))
			{
				Log.Error("ObjectMgr", "Invalid seat entry %u for npc entry %u and accessory entry %u.", entry->seat, entry->accessory_entry, creature_entry);
				continue;
			}

			std::map< uint32, std::vector< VehicleAccessoryEntry* >* >::iterator itr = vehicle_accessories.find( creature_entry );

			if (itr != vehicle_accessories.end())
			{
				itr->second->push_back(entry);
			}
			else
			{
				std::vector< VehicleAccessoryEntry* >* v = new std::vector< VehicleAccessoryEntry* >();
				v->push_back( entry );
				vehicle_accessories.insert( std::make_pair( creature_entry, v ) );
			}
		}
		while( result->NextRow() );
		delete result;
	}
	Log.Success("ObjectMgr", "Loaded %u vehicle accessories.");
}

std::vector< VehicleAccessoryEntry* >* ObjectMgr::GetVehicleAccessories( uint32 creature_entry )
{
	std::map< uint32, std::vector< VehicleAccessoryEntry* >* >::iterator itr = vehicle_accessories.find( creature_entry );
	if(itr != vehicle_accessories.end())
		return itr->second;

	return NULL;
}

void ObjectMgr::LoadWorldStateTemplates()
{
	Log.Notice("ObjectMgr", "Loading worldstate template...");
	uint32 count = 0;
	if (QueryResult *result = WorldDatabase.Query("SELECT DISTINCT map FROM worldstate_templates ORDER BY map;"))
	{
		std::list<uint32> invalidMapEntries;
		do{
			Field *row = result->Fetch();
			uint32 mapid = row[0].GetUInt32();
			if (!dbcMap.LookupEntryForced(mapid))
			{
				Log.Error("ObjectMgr", "Tryed to load worldstate template for invalid map entry %u.", mapid);
				invalidMapEntries.push_back(mapid);
				continue;
			}
			worldstate_templates.insert(std::make_pair(mapid, new std::multimap< uint32, WorldState >()));

		} while (result->NextRow());
		delete result;

		if (QueryResult* result2 = WorldDatabase.Query("SELECT map, zone, field, value FROM worldstate_templates;"))
		{
			do{
				Field *row = result2->Fetch();

				uint32 mapid = row[0].GetUInt32();
				bool mSkip = false;
				for (std::list<uint32>::iterator itr = invalidMapEntries.begin(); itr != invalidMapEntries.end(); itr++)
				{
					if (*itr == mapid)
					{
						mSkip = true;
						break;
					}
				}
					
				if (mSkip)
					continue;

				uint32 zone = row[1].GetUInt32();
				if (!dbcArea.LookupEntryForced(zone))
				{
					Log.Error("ObjectMgr", "Worldstate template data for map %u contains invalid zone id %u.", mapid, zone);
					continue;
				}

				WorldState ws;
				ws.field = row[2].GetUInt32();
				ws.value = row[3].GetUInt32();

				std::map< uint32, std::multimap< uint32, WorldState >* >::iterator itr = worldstate_templates.find(mapid);

				if (itr == worldstate_templates.end())
					continue;

				itr->second->insert(std::make_pair(zone, ws));
				++count;
			} while (result2->NextRow());
			delete result2;
		}
	}
	Log.Success("ObjectMgr", "Loaded %u worldstate templates.", count);
}

std::multimap< uint32, WorldState >* ObjectMgr::GetWorldStatesForMap( uint32 map ) const{
	std::map< uint32, std::multimap< uint32, WorldState >* >::const_iterator itr =
		worldstate_templates.find( map );

	if( itr == worldstate_templates.end() )
		return NULL;
	else
		return itr->second;
}