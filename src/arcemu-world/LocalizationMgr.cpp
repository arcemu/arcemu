/*
 * arcemu MMORPG Server
 * String Localization Manager
 * Copyright (C) 2007 Burlex <burlex@gmail.com>
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

LocalizationMgr sLocalizationMgr;

void LocalizationMgr::Shutdown()
{
	if(m_disabled)
		return;

#define SAFE_FREE_PTR(x) if(deletedPointers.find((x)) == deletedPointers.end()) { deletedPointers.insert((x)); free((x)); }

	set<void*> deletedPointers;
	uint32 maxid=0;
	uint32 i,j;
	vector<pair<uint32,uint32> >::iterator xtr = m_languages.begin();
	for(; xtr != m_languages.end(); ++xtr)
		if(xtr->second>maxid)
			maxid=xtr->second;

	maxid++;
	Log.Notice("LocalizationMgr", "Beginning pointer cleanup...");
	uint32 t = getMSTime();

    for(i = 0; i < maxid; ++i)
	{
		for(HM_NAMESPACE::hash_map<uint32, LocalizedQuest>::iterator itr = m_Quests[i].begin(); itr != m_Quests[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Title);
			SAFE_FREE_PTR(itr->second.Details);
			SAFE_FREE_PTR(itr->second.Objectives);
			SAFE_FREE_PTR(itr->second.CompletionText);
			SAFE_FREE_PTR(itr->second.IncompleteText);
			SAFE_FREE_PTR(itr->second.EndText);
			SAFE_FREE_PTR(itr->second.ObjectiveText[0]);
			SAFE_FREE_PTR(itr->second.ObjectiveText[1]);
			SAFE_FREE_PTR(itr->second.ObjectiveText[2]);
			SAFE_FREE_PTR(itr->second.ObjectiveText[3]);
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedItem>::iterator itr = m_Items[i].begin(); itr != m_Items[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Name);
			SAFE_FREE_PTR(itr->second.Description);
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedNpcText>::iterator itr = m_NpcTexts[i].begin(); itr != m_NpcTexts[i].end(); ++itr)
		{
			for(j = 0; j < 8; ++j)
			{
				SAFE_FREE_PTR(itr->second.Texts[j][0]);
				SAFE_FREE_PTR(itr->second.Texts[j][1]);
			}
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedCreatureName>::iterator itr = m_CreatureNames[i].begin(); itr != m_CreatureNames[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Name);
			SAFE_FREE_PTR(itr->second.SubName);
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedGameObjectName>::iterator itr = m_GameObjectNames[i].begin(); itr != m_GameObjectNames[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Name);
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedItemPage>::iterator itr = m_ItemPages[i].begin(); itr != m_ItemPages[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Text);
		}

		// (p2wow) world server common message going to be localized.
		for(HM_NAMESPACE::hash_map<uint32, LocalizedWorldStringTable>::iterator itr = m_WorldStrings[i].begin(); itr != m_WorldStrings[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Text);
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedWorldBroadCast>::iterator itr = m_WorldBroadCast[i].begin(); itr != m_WorldBroadCast[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Text);
		}

		for(HM_NAMESPACE::hash_map<uint32, LocalizedWorldMapInfo>::iterator itr = m_WorldMapInfo[i].begin(); itr != m_WorldMapInfo[i].end(); ++itr)
		{
			SAFE_FREE_PTR(itr->second.Text);
		}


	}

	deletedPointers.clear();
	delete [] m_ItemPages;
	delete [] m_CreatureNames;
	delete [] m_GameObjectNames;
	delete [] m_Items;
	delete [] m_NpcTexts;
	delete [] m_Quests;
	delete [] m_WorldStrings;
	delete [] m_WorldBroadCast;
	delete [] m_WorldMapInfo;
	m_languages.clear();
	Log.Notice("LocalizationMgr", "Pointer cleanup completed in %.4f seconds.", float(float(getMSTime()-t) / 1000.0f));
#undef SAFE_FREE_PTR
}

void LocalizationMgr::Lower(string& conv)
{
	for(size_t i = 0; i < conv.length(); ++i)
		conv[i] = tolower(conv[i]);
}

void GetDistinctLanguages(set<string>& dest, const char * table)
{
	QueryResult * result = WorldDatabase.Query("SELECT DISTINCT language_code FROM %s", table);
	if(result == NULL)
		return;

	string lc;
	do 
	{
		lc = result->Fetch()[0].GetString();
		sLocalizationMgr.Lower(lc);
        if(dest.find(lc)==dest.end())
			dest.insert(lc);

	} while(result->NextRow());
	delete result;
}

uint32 LocalizationMgr::GetLanguageId(uint32 full)
{
	if(m_disabled)
		return 0;

	for(vector<pair<uint32,uint32> >::iterator itr = m_languages.begin(); itr != m_languages.end(); ++itr)
		if(itr->first == full)
			return itr->second;

	return 0;
}

void LocalizationMgr::Reload(bool first)
{
	if(first)
		return;

	QueryResult * result;
	set<string> languages;
	map<string, string> bound_languages;
	GetDistinctLanguages(languages, "creature_names_localized");
	GetDistinctLanguages(languages, "gameobject_names_localized");
	GetDistinctLanguages(languages, "items_localized");
	GetDistinctLanguages(languages, "quests_localized");
	GetDistinctLanguages(languages, "npc_text_localized");
	GetDistinctLanguages(languages, "itempages_localized");
	GetDistinctLanguages(languages, "worldstring_tables_localized");
	GetDistinctLanguages(languages, "worldbroadcast_localized");
	GetDistinctLanguages(languages, "worldmap_info_localized");

	/************************************************************************/
	/* Read Language Bindings From Config                                   */
	/************************************************************************/
	string ls = Config.MainConfig.GetStringDefault("Localization", "LocaleBindings", "");
	vector<string> tbindings = StrSplit(ls, " ");
	for(vector<string>::iterator ztr = tbindings.begin(); ztr != tbindings.end(); ++ztr)
	{
		char lb[200];
		string ll1,ll2;
		strcpy(lb,(*ztr).c_str());

		char * lbp = strchr(lb,'=');
		if(lbp==NULL)
			continue;
		*lbp=0;
		lbp++;

		ll1 = string(lb);
		ll2 = string(lbp);
		Lower(ll1);
		Lower(ll2);

		if(languages.find(ll1) == languages.end())
		{
			bound_languages[ll1] = ll2;
			languages.insert(ll1);
		}
	}

	/************************************************************************/
	/* Generate Language IDs                                                */
	/************************************************************************/

	uint32 langid=1;
	pair<uint32,uint32> dpr;
	for(set<string>::iterator sitr = languages.begin(); sitr != languages.end(); ++sitr)
	{
		if((*sitr)=="enus")		// Default
		{
			dpr.first = *(uint32*)sitr->c_str();
			dpr.second = 0;
		}
		else
		{
			dpr.first = *(uint32*)sitr->c_str();
			dpr.second = langid++;
		}

		m_languages.push_back(dpr);
	}

	if(m_languages.size() == 0)
	{
		m_disabled = true;
		return;		// No localizations
	}
	else
		m_disabled = false;

	m_CreatureNames = new HM_NAMESPACE::hash_map<uint32, LocalizedCreatureName>[langid];
	m_GameObjectNames = new HM_NAMESPACE::hash_map<uint32, LocalizedGameObjectName>[langid];
	m_Quests = new HM_NAMESPACE::hash_map<uint32, LocalizedQuest>[langid];
	m_NpcTexts = new HM_NAMESPACE::hash_map<uint32, LocalizedNpcText>[langid];
	m_Items = new HM_NAMESPACE::hash_map<uint32, LocalizedItem>[langid];
	m_ItemPages = new HM_NAMESPACE::hash_map<uint32, LocalizedItemPage>[langid];
	m_WorldStrings = new HM_NAMESPACE::hash_map<uint32, LocalizedWorldStringTable>[langid];
	m_WorldBroadCast = new HM_NAMESPACE::hash_map<uint32, LocalizedWorldBroadCast>[langid];
	m_WorldMapInfo = new HM_NAMESPACE::hash_map<uint32, LocalizedWorldMapInfo>[langid];

	/************************************************************************/
	/* Creature Names                                                       */
	/************************************************************************/
	{
		LocalizedCreatureName cn;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM creature_names_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;		// no loading enus stuff.. lawl

				cn.Name = strdup(f[2].GetString());
				cn.SubName = strdup(f[3].GetString());
				m_CreatureNames[lid].insert(make_pair(entry, cn));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* GameObject Names                                                     */
	/************************************************************************/
	{
		LocalizedGameObjectName gn;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM gameobject_names_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;		// no loading enus stuff.. lawl

				gn.Name = strdup(f[2].GetString());
				m_GameObjectNames[lid].insert(make_pair(entry, gn));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* Items                                                                */
	/************************************************************************/
	{
		LocalizedItem it;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM items_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;		// no loading enus stuff.. lawl

				if(m_Items[lid].find(entry) != m_Items[lid].end())
				{
					continue;
				}

				it.Name = strdup(f[2].GetString());
				it.Description = strdup(f[3].GetString());
				m_Items[lid].insert(make_pair(entry, it));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* Quests                                                               */
	/************************************************************************/
	{
		LocalizedQuest q;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM quests_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;		// no loading enus stuff.. lawl

				q.Title = strdup(f[2].GetString());
				q.Details = strdup(f[3].GetString());
				q.Objectives = strdup(f[4].GetString());
				q.CompletionText = strdup(f[5].GetString());
				q.IncompleteText = strdup(f[6].GetString());
				q.EndText = strdup(f[7].GetString());
				q.ObjectiveText[0] = strdup(f[8].GetString());
				q.ObjectiveText[1] = strdup(f[9].GetString());
				q.ObjectiveText[2] = strdup(f[10].GetString());
				q.ObjectiveText[3] = strdup(f[11].GetString());
				
				m_Quests[lid].insert(make_pair(entry, q));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* NPC Texts                                                            */
	/************************************************************************/
	{
		LocalizedNpcText nt;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;
		uint32 counter;

		result = WorldDatabase.Query("SELECT * FROM npc_text_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;		// no loading enus stuff.. lawl

				counter = 2;
				for(uint32 i = 0; i < 8; ++i)
				{
					nt.Texts[i][0] = strdup(f[counter++].GetString());
					nt.Texts[i][1] = strdup(f[counter++].GetString());
				}

				m_NpcTexts[lid].insert(make_pair(entry, nt));
			} while(result->NextRow());
			delete result;
		}
	}
	/************************************************************************/
	/* Item Pages                                                           */
	/************************************************************************/
	{
		LocalizedItemPage nt;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM itempages_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;		// no loading enus stuff.. lawl

				nt.Text = strdup(f[2].GetString());
				m_ItemPages[lid].insert(make_pair(entry, nt));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* World Common Message                                                 */
	/************************************************************************/
	{
		LocalizedWorldStringTable nt;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM worldstring_tables_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;

				nt.Text = strdup(f[2].GetString());
				m_WorldStrings[lid].insert(make_pair(entry, nt));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* World BroadCast Messages                                             */
	/************************************************************************/
	{
		LocalizedWorldBroadCast nt;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM worldbroadcast_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;

				nt.Text = strdup(f[2].GetString());
				m_WorldBroadCast[lid].insert(make_pair(entry, nt));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* World MapInfo Entry Name                                             */
	/************************************************************************/
	{
		LocalizedWorldMapInfo nt;
		string str;
		uint32 entry;
		Field * f;
		uint32 lid;

		result = WorldDatabase.Query("SELECT * FROM worldmap_info_localized");
		if(result)
		{
			do 
			{
				f = result->Fetch();
				str = string(f[1].GetString());
				entry = f[0].GetUInt32();

				lid = GetLanguageId(str);
				if(lid == 0)
					continue;

				nt.Text = strdup(f[2].GetString());
				m_WorldMapInfo[lid].insert(make_pair(entry, nt));
			} while(result->NextRow());
			delete result;
		}
	}

	/************************************************************************/
	/* Apply all the language bindings.                                     */
	/************************************************************************/
	for(map<string,string>::iterator itr = bound_languages.begin(); itr != bound_languages.end(); ++itr)
	{
		uint32 source_language_id = GetLanguageId(itr->second);
		uint32 dest_language_id = GetLanguageId(itr->first);
		if(source_language_id==0 || dest_language_id == 0)
		{
			Log.Error("LocalizationMgr", "Invalid locale conversion string specified: %u->%u (%s->%s)", source_language_id, dest_language_id, itr->second.c_str(), itr->first.c_str());
			continue;
		}

		/* duplicate the hashmaps (we can save the pointers here) */
		CopyHashMap<LocalizedItem>(&m_Items[source_language_id], &m_Items[dest_language_id]);
		CopyHashMap<LocalizedCreatureName>(&m_CreatureNames[source_language_id], &m_CreatureNames[dest_language_id]);
		CopyHashMap<LocalizedGameObjectName>(&m_GameObjectNames[source_language_id], &m_GameObjectNames[dest_language_id]);
		CopyHashMap<LocalizedItemPage>(&m_ItemPages[source_language_id], &m_ItemPages[dest_language_id]);
		CopyHashMap<LocalizedQuest>(&m_Quests[source_language_id], &m_Quests[dest_language_id]);
		CopyHashMap<LocalizedNpcText>(&m_NpcTexts[source_language_id], &m_NpcTexts[dest_language_id]);
		CopyHashMap<LocalizedWorldStringTable>(&m_WorldStrings[source_language_id], &m_WorldStrings[dest_language_id]);
		CopyHashMap<LocalizedWorldBroadCast>(&m_WorldBroadCast[source_language_id], &m_WorldBroadCast[dest_language_id]);
		CopyHashMap<LocalizedWorldMapInfo>(&m_WorldMapInfo[source_language_id], &m_WorldMapInfo[dest_language_id]);
	}
}

#define MAKE_LOOKUP_FUNCTION(t, hm, fn) t * LocalizationMgr::fn(uint32 id, uint32 language) { \
	if(m_disabled) { return NULL; } \
	HM_NAMESPACE::hash_map<uint32, t>::iterator itr = hm[language].find(id); \
	return (itr == hm[language].end()) ? NULL : &itr->second; }

MAKE_LOOKUP_FUNCTION(LocalizedCreatureName, m_CreatureNames, GetLocalizedCreatureName);
MAKE_LOOKUP_FUNCTION(LocalizedGameObjectName, m_GameObjectNames, GetLocalizedGameObjectName);
MAKE_LOOKUP_FUNCTION(LocalizedQuest, m_Quests, GetLocalizedQuest);
MAKE_LOOKUP_FUNCTION(LocalizedItem, m_Items, GetLocalizedItem);
MAKE_LOOKUP_FUNCTION(LocalizedNpcText, m_NpcTexts, GetLocalizedNpcText);
MAKE_LOOKUP_FUNCTION(LocalizedItemPage, m_ItemPages, GetLocalizedItemPage);
MAKE_LOOKUP_FUNCTION(LocalizedWorldStringTable, m_WorldStrings, GetLocalizedWorldStringTable);
MAKE_LOOKUP_FUNCTION(LocalizedWorldBroadCast, m_WorldBroadCast, GetLocalizedWorldBroadCast);
MAKE_LOOKUP_FUNCTION(LocalizedWorldMapInfo, m_WorldMapInfo, GetLocalizedWorldMapInfo);

