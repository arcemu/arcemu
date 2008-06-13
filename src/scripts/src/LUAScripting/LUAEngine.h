/*
 * Ascent MMORPG Server
 * LUA Interface
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
	QUEST_EVENT_ON_COMPLETE		= 1,
	QUEST_EVENT_ON_ACCEPT		= 2,
	//QUEST_EVENT_CAN_ACCEPT		= 3, 
	QUEST_EVENT_COUNT,
};

/** Creature Events
 */
enum CreatureEvents
{
	CREATURE_EVENT_ON_ENTER_COMBAT		= 1,
	CREATURE_EVENT_ON_LEAVE_COMBAT		= 2,
	CREATURE_EVENT_ON_KILLED_TARGET		= 3,
	CREATURE_EVENT_ON_DIED				= 4,
	CREATURE_EVENT_AI_TICK				= 5,
	CREATURE_EVENT_ON_SPAWN				= 6,
	CREATURE_EVENT_ON_GOSSIP_TALK		= 7,
	CREATURE_EVENT_ON_REACH_WP			= 8,
	CREATURE_EVENT_ON_LEAVE_LIMBO		= 9,
	CREATURE_EVENT_PLAYER_ENTERS_RANGE	= 10,
	CREATURE_EVENT_COUNT,
};

/** GameObject Events
 */
enum GameObjectEvents
{
	GAMEOBJECT_EVENT_ON_SPAWN			= 1,
	GAMEOBJECT_EVENT_ON_USE				= 2,
	GAMEOBJECT_EVENT_COUNT,
};

/** Gossip Events
*/
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
	RANDOM_NOT_MAINTANK  = 7
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
	ASCENT_INLINE Mutex& GetLock() { return m_Lock; }

	void OnUnitEvent(Unit * pUnit, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc);
	void OnQuestEvent(Player * QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Object * QuestStarter);
	void OnGameObjectEvent(GameObject * pGameObject, const char * FunctionName, uint32 EventType, Unit * pMiscUnit);
	void OnGossipEvent(Object * pObject, const char * FunctionName, uint32 EventType, Player * mPlayer, uint32 Id, uint32 IntId, const char *Code);
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
    // Gossip Stuff
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



