/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LUAENGINE_H
#define __LUAENGINE_H

extern "C" {		// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
};

class LuaEngine;
class LuaEngineMgr;

extern LuaEngineMgr g_luaMgr;
extern LuaEngine * g_engine;

GossipMenu * Menu;

/** Macros for calling lua-based events
 */
#define LUA_ON_UNIT_EVENT(unit,eventtype,miscunit,misc) if(unit->GetTypeId()==TYPEID_UNIT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->OnUnitEvent(unit,eventtype,miscunit,misc); }
#define LUA_ON_QUEST_EVENT(plr,quest,eventtype,miscobject) if(plr->IsPlayer() && plr->IsInWorld() && miscobject->IsInWorld() && !miscobject->IsPlayer()) { plr->GetMapMgr()->GetScriptEngine()->OnQuestEvent(plr,quest,eventtype,miscobject); } 
#define LUA_ON_GO_EVENT(unit,evtype,miscunit) if(unit->GetTypeId()==TYPEID_GAMEOBJECT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->OnGameObjectEvent(unit,evtype,miscunit); }
#define LUA_ON_GOSSIP_EVENT(object, evtype, player, id, intid, code) if(object->IsInWorld()) { object->GetMapMgr()->GetScriptEngine()->OnGossipEvent(object, evtype, player, id, intid, code); }
#define LUA_CALL_FUNC(unit,funcname) if(unit->GetTypeId()==TYPEID_UNIT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->CallFunction(unit,funcname); }

/** Quest Events
 */
enum QuestEvents
{
	QUEST_EVENT_ON_ACCEPT = 1,
	QUEST_EVENT_ON_COMPLETE = 2,
	QUEST_EVENT_ON_CANCEL = 3,
	QUEST_EVENT_GAMEOBJECT_ACTIVATE = 4,
	QUEST_EVENT_ON_CREATURE_KILL  = 5,
	QUEST_EVENT_ON_EXPLORE_AREA = 6,
	QUEST_EVENT_ON_PLAYER_ITEMPICKUP = 7,
	QUEST_EVENT_COUNT,
};

/** Creature Events
 */
enum CreatureEvents
{
	CREATURE_EVENT_ON_ENTER_COMBAT		= 1,
	CREATURE_EVENT_ON_LEAVE_COMBAT		= 2,
	CREATURE_EVENT_ON_TARGET_DIED		= 3,
	CREATURE_EVENT_ON_DIED		= 4,
	CREATURE_EVENT_ON_TARGET_PARRIED		= 5,
	CREATURE_EVENT_ON_TARGET_DODGED		= 6,
	CREATURE_EVENT_ON_TARGET_BLOCKED		= 7,
	CREATURE_EVENT_ON_TARGET_CRIT_HIT		= 8,
	CREATURE_EVENT_ON_PARRY		= 9,
	CREATURE_EVENT_ON_DODGED		= 10,
	CREATURE_EVENT_ON_BLOCKED		= 11,
	CREATURE_EVENT_ON_CRIT_HIT		= 12,
	CREATURE_EVENT_ON_HIT		= 13,
	CREATURE_EVENT_ON_ASSIST_TARGET_DIED		= 14,
	CREATURE_EVENT_ON_FEAR		= 15,
	CREATURE_EVENT_ON_FLEE		= 16,
	CREATURE_EVENT_ON_CALL_FOR_HELP		= 17,
	CREATURE_EVENT_ON_LOAD		= 18,
	CREATURE_EVENT_ON_REACH_WP		= 19,
	CREATURE_EVENT_ON_LOOT_TAKEN		= 20,
	CREATURE_EVENT_ON_AIUPDATE		= 21,
	CREATURE_EVENT_ON_EMOTE		= 22,
	CREATURE_EVENT_COUNT,
};

/** GameObject Events
 */
enum GameObjectEvents
{
	GAMEOBJECT_EVENT_ON_CREATE			= 1,
	GAMEOBJECT_EVENT_ON_SPAWN			= 2,
	GAMEOBJECT_EVENT_ON_LOOT_TAKEN		= 3,
	GAMEOBJECT_EVENT_ON_USE				= 4,
	GAMEOBJECT_EVENT_AIUPDATE			= 5,
	GAMEOBJECT_EVENT_ON_DESPAWN			= 6,
	GAMEOBJECT_EVENT_COUNT,
};

enum GossipEvents
{
	GOSSIP_EVENT_ON_TALK			= 1,
	GOSSIP_EVENT_ON_SELECT_OPTION	= 2,
	GOSSIP_EVENT_ON_END             = 3,
	GOSSIP_EVENT_COUNT,
};

enum RandomFlags
{
	RANDOM_ANY           = 0,
	RANDOM_IN_SHORTRANGE = 1,
	RANDOM_IN_MIDRANGE   = 2,
	RANDOM_IN_LONGRANGE  = 3,
	RANDOM_WITH_MANA     = 4,
	RANDOM_WITH_RAGE     = 5,
	RANDOM_WITH_ENERGY   = 6,
	RANDOM_NOT_MAINTANK  = 7,
	RANDOM_COUNT,
};

class LuaEngine
{
private:
	lua_State * L;
	Mutex m_Lock;

public:
	LuaEngine();
	~LuaEngine();

	void LoadScripts();
	void Shutdown();
	void Restart();
	void RegisterCoreFunctions();

	ARCEMU_INLINE Mutex& GetLock() { return m_Lock; }

	void OnUnitEvent(Unit * pUnit, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc, int32 Misc2, float Misc3, bool Yes, ItemPrototype * Misc4);
	void OnQuestEvent(Player * QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Object * QuestStarter,uint32 Misc, uint32 Misc2);
	void OnGameObjectEvent(GameObject * pGameObject, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc, ItemPrototype* Misc2);
    void OnGossipEvent(Object * pObject, const char * FunctionName, uint32 EventType, Player * mPlayer, uint32 Id, uint32 IntId, const char * Code);
	void CallFunction(Unit * pUnit, const char * FuncName);
};

struct LuaUnitBinding { const char * Functions[CREATURE_EVENT_COUNT]; };
struct LuaQuestBinding { const char * Functions[QUEST_EVENT_COUNT]; };
struct LuaGameObjectBinding { const char * Functions[GAMEOBJECT_EVENT_COUNT]; };
struct LuaUnitGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };
struct LuaItemGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };
struct LuaGOGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };

class LuaEngineMgr
{
private:
	typedef HM_NAMESPACE::hash_map<uint32, LuaUnitBinding> UnitBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaQuestBinding> QuestBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGameObjectBinding> GameObjectBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaUnitGossipBinding> GossipUnitScriptsBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaItemGossipBinding> GossipItemScriptsBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGOGossipBinding> GossipGOScriptsBindingMap;

	UnitBindingMap m_unitBinding;
	QuestBindingMap m_questBinding;
	GameObjectBindingMap m_gameobjectBinding;
	GossipUnitScriptsBindingMap m_unit_gossipBinding;
	GossipItemScriptsBindingMap m_item_gossipBinding;
	GossipGOScriptsBindingMap m_go_gossipBinding;

public:
	LuaEngine * m_engine;
	void Startup();
	void Unload();

	void RegisterUnitEvent(uint32 Id, uint32 Event, const char * FunctionName);
	void RegisterQuestEvent(uint32 Id, uint32 Event, const char * FunctionName);
	void RegisterGameObjectEvent(uint32 Id, uint32 Event, const char * FunctionName);
    void RegisterUnitGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
    void RegisterItemGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
    void RegisterGOGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);

	LuaUnitBinding * GetUnitBinding(uint32 Id)
	{
		UnitBindingMap::iterator itr = m_unitBinding.find(Id);
		return (itr == m_unitBinding.end()) ? NULL : &itr->second;
	}

	LuaQuestBinding * GetQuestBinding(uint32 Id)
	{
		QuestBindingMap::iterator itr = m_questBinding.find(Id);
		return (itr == m_questBinding.end()) ? NULL : &itr->second;
	}

	LuaGameObjectBinding * GetGameObjectBinding(uint32 Id)
	{
		GameObjectBindingMap::iterator itr =m_gameobjectBinding.find(Id);
		return (itr == m_gameobjectBinding.end()) ? NULL : &itr->second;
	}

	  LuaUnitGossipBinding * GetLuaUnitGossipBinding(uint32 Id)
 	{
		GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.find(Id);
		return (itr == m_unit_gossipBinding.end()) ? NULL : &itr->second;
 	}

    LuaItemGossipBinding * GetLuaItemGossipBinding(uint32 Id)
	{
		GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.find(Id);
		return (itr == m_item_gossipBinding.end()) ? NULL : &itr->second;
	}

    LuaGOGossipBinding * GetLuaGOGossipBinding(uint32 Id)
	{
		GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.find(Id);
		return (itr == m_go_gossipBinding.end()) ? NULL : &itr->second;
	}
};

#endif




