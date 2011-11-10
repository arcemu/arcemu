/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
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
#ifdef DEBUG
#define LUA_USE_APICHECK
#endif

extern "C"
{
	// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "../lualib/lua.h"
#include "../lualib/lauxlib.h"
#include "../lualib/lualib.h"
};

#include "StdAfx.h"

#include <sys/stat.h>
#include <sys/types.h>

class LuaEngine;
class LuaCreature;
class LuaGameObjectScript;
class LuaQuest;
class LuaInstance;
class LuaGossip;
class ArcLuna;

#ifdef WIN32
HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#define dropNotice sLog.outDebug
#define dropWarning sLog.outDetail
#define dropError sLog.outString
#define dropFatal sLog.outError

extern LuaEngine g_luaMgr;
#define sLuaMgr g_luaMgr
#define sLuaEventMgr g_luaMgr.LuaEventMgr

Arcemu::Gossip::Menu *Menu;

#define GET_LOCK sLuaMgr.getLock().Acquire();
#define RELEASE_LOCK sLuaMgr.getLock().Release();
#define CHECK_BINDING_ACQUIRELOCK GET_LOCK if(m_binding == NULL) { RELEASE_LOCK return; }

#define RegisterHook(evt, _func) { \
	if(EventAsToFuncName[(evt)].size() > 0 && !sLuaMgr.HookInfo.hooks[(evt)]) { \
		sLuaMgr.HookInfo.hooks[(evt)] = true; \
		m_scriptMgr->register_hook( (ServerHookEvents)(evt), (_func) ); } }

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
    CREATURE_EVENT_ON_DAMAGE_TAKEN = 23,
	CREATURE_EVENT_ON_ENTER_VEHICLE = 24,
	CREATURE_EVENT_ON_EXIT_VEHICLE = 25,
	CREATURE_EVENT_ON_FIRST_PASSENGER_ENTERED = 26,
	CREATURE_EVENT_ON_VEHICLE_FULL  = 27,
	CREATURE_EVENT_ON_LAST_PASSENGER_LEFT  = 28,
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
	GAMEOBJECT_EVENT_ON_DAMAGED			= 7,
	GAMEOBJECT_EVENT_ON_DESTROYED		= 8,
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

enum InstanceHooks
{
    INSTANCE_EVENT_ON_PLAYER_DEATH = 1,
    INSTANCE_EVENT_ON_PLAYER_ENTER = 2,
    INSTANCE_EVENT_ON_AREA_TRIGGER = 3,
    INSTANCE_EVENT_ON_ZONE_CHANGE = 4,
    INSTANCE_EVENT_ON_CREATURE_DEATH = 5,
    INSTANCE_EVENT_ON_CREATURE_PUSH = 6,
    INSTANCE_EVENT_ON_GO_ACTIVATE = 7,
    INSTANCE_EVENT_ON_GO_PUSH = 8,
    INSTANCE_EVENT_ONLOAD = 9,
    INSTANCE_EVENT_DESTROY = 10,
    INSTANCE_EVENT_COUNT,
};

//reg type defines
#define REGTYPE_UNIT (1 << 0)
#define REGTYPE_GO (1 << 1)
#define REGTYPE_QUEST (1 << 2)
#define REGTYPE_SERVHOOK (1 << 3)
#define REGTYPE_ITEM (1 << 4)
#define REGTYPE_GOSSIP (1 << 5)
#define REGTYPE_DUMMYSPELL (1 << 6)
#define REGTYPE_INSTANCE (1 << 7)
#define REGTYPE_UNIT_GOSSIP (REGTYPE_UNIT | REGTYPE_GOSSIP)
#define REGTYPE_GO_GOSSIP (REGTYPE_GO | REGTYPE_GOSSIP)
#define REGTYPE_ITEM_GOSSIP (REGTYPE_ITEM | REGTYPE_GOSSIP)
//Nice thing about this is that we can ignore any new core events(as long as they are added in order), it will automatically update.
enum CustomLuaEvenTypes
{
    LUA_EVENT_START = NUM_EVENT_TYPES, // Used as a placeholder
    EVENT_LUA_TIMED,
    EVENT_LUA_CREATURE_EVENTS,
    EVENT_LUA_GAMEOBJ_EVENTS,
    LUA_EVENTS_END
};

struct LUALoadScripts
{
	set<string> luaFiles;
};

struct EventInfoHolder
{
	const char* funcName;
	TimedEvent* te;
};

std::vector<uint32> OnLoadInfo;

struct LuaObjectBinding
{
	uint16 m_functionReferences[CREATURE_EVENT_COUNT];
};
std::vector<uint16> EventAsToFuncName[NUM_SERVER_HOOKS];
std::map<uint32, uint16> m_luaDummySpells;

template<typename T>
struct RegType
{
	const char* name;
	int(*mfunc)(lua_State*, T*);
};
template<typename T> RegType<T>* GetMethodTable();
template<typename T> const char* GetTClassName();
void report(lua_State*);

class LuaEngine
{
	private:
		lua_State* lu;  // main state.
		Mutex call_lock;
		Mutex co_lock;

		typedef HM_NAMESPACE::hash_map<uint32, LuaObjectBinding> LuaObjectBindingMap;

		std::set<int> m_pendingThreads;
		std::set<int> m_functionRefs;
		std::map< uint64, std::set<int> > m_objectFunctionRefs;

		//maps to creature, & go script interfaces
		std::multimap<uint32, LuaCreature*> m_cAIScripts;
		std::multimap<uint32, LuaGameObjectScript*> m_gAIScripts;
		HM_NAMESPACE::hash_map<uint32, LuaQuest*> m_qAIScripts;
		HM_NAMESPACE::hash_map<uint32, LuaInstance*> m_iAIScripts;

		HM_NAMESPACE::hash_map<uint32, LuaGossip*> m_unitgAIScripts;
		HM_NAMESPACE::hash_map<uint32, LuaGossip*> m_itemgAIScripts;
		HM_NAMESPACE::hash_map<uint32, LuaGossip*> m_gogAIScripts;

		LuaObjectBindingMap m_unitBinding;
		LuaObjectBindingMap m_questBinding;
		LuaObjectBindingMap m_gameobjectBinding;
		LuaObjectBindingMap m_instanceBinding;
		LuaObjectBindingMap m_unit_gossipBinding;
		LuaObjectBindingMap m_item_gossipBinding;
		LuaObjectBindingMap m_go_gossipBinding;

	public:
		~LuaEngine()
		{
		}
		void Startup();
		void LoadScripts();
		void Restart();

		void RegisterEvent(uint8, uint32, uint32 , uint16);
		void ResumeLuaThread(int);
		void BeginCall(uint16);
		void HyperCallFunction(const char*, int);
		void CallFunctionByReference(int);
		void DestroyAllLuaEvents();
		ARCEMU_INLINE bool ExecuteCall(uint8 params = 0, uint8 res = 0);
		ARCEMU_INLINE void EndCall(uint8 res = 0);
		//Wrappers
		ARCEMU_INLINE Unit* CheckUnit(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<Unit>::check(lu, narg);
			else return ArcLuna<Unit>::check(L, narg);
		}
		ARCEMU_INLINE GameObject* CheckGo(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<GameObject>::check(lu, narg);
			else return ArcLuna<GameObject>::check(L, narg);
		}
		ARCEMU_INLINE Item* CheckItem(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<Item>::check(lu, narg);
			else return ArcLuna<Item>::check(L, narg);
		}
		ARCEMU_INLINE WorldPacket* CheckPacket(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<WorldPacket>::check(lu, narg);
			else return ArcLuna<WorldPacket>::check(L, narg);
		}
		ARCEMU_INLINE uint64 CheckGuid(lua_State* L, int narg)
		{
			if(L == NULL) return GUID_MGR::check(lu, narg);
			else return GUID_MGR::check(L, narg);
		}
		ARCEMU_INLINE Object* CheckObject(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<Object>::check(lu, narg);
			else return ArcLuna<Object>::check(L, narg);
		}
		ARCEMU_INLINE TaxiPath* CheckTaxiPath(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<TaxiPath>::check(lu, narg);
			else return ArcLuna<TaxiPath>::check(L, narg);
		}
		ARCEMU_INLINE Spell* CheckSpell(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<Spell>::check(lu, narg);
			else return ArcLuna<Spell>::check(L, narg);
		}
		ARCEMU_INLINE Aura* CheckAura(lua_State* L, int narg)
		{
			if(L == NULL) return ArcLuna<Aura>::check(lu, narg);
			else return ArcLuna<Aura>::check(L, narg);
		}

		bool CheckBool(lua_State* L, int narg)
		{
			//first try with bool type
			if(lua_isboolean(L, narg))
				return lua_toboolean(L, narg) > 0;
			//then try with integer type
			else if(lua_isnumber(L, narg))
				return lua_tonumber(L, narg) > 0;
			//then return true by default
			else return true;
		}

		void PushUnit(Object* unit, lua_State* L = NULL);
		void PushGo(Object* go, lua_State* L = NULL);
		void PushItem(Object* item, lua_State* L = NULL);
		void PushGuid(uint64 guid, lua_State* L = NULL);
		void PushPacket(WorldPacket* packet, lua_State* L = NULL);
		void PushTaxiPath(TaxiPath* tp, lua_State* L = NULL);
		void PushSpell(Spell* sp, lua_State* L = NULL);
		void PushSqlField(Field* field, lua_State* L = NULL);
		void PushSqlResult(QueryResult* res, lua_State* L = NULL);
		void PushAura(Aura* aura, lua_State* L = NULL);

		ARCEMU_INLINE void PUSH_BOOL(bool bewl)
		{
			if(bewl)
				lua_pushboolean(lu, 1);
			else
				lua_pushboolean(lu, 0);
		}
		ARCEMU_INLINE void PUSH_NIL(lua_State* L = NULL)
		{
			if(L == NULL)
				lua_pushnil(lu);
			else
				lua_pushnil(L);
		}
		ARCEMU_INLINE void PUSH_INT(int32 value)
		{
			lua_pushinteger(lu, value);
		}
		ARCEMU_INLINE void PUSH_UINT(uint32 value)
		{
			lua_pushnumber(lu, value);
		}
		ARCEMU_INLINE void PUSH_FLOAT(float value)
		{
			lua_pushnumber(lu, value);
		}
		ARCEMU_INLINE void PUSH_STRING(const char* str)
		{
			lua_pushstring(lu, str);
		}
		void RegisterCoreFunctions();

		ARCEMU_INLINE Mutex & getLock() { return call_lock; }
		ARCEMU_INLINE Mutex & getcoLock() { return co_lock; }
		ARCEMU_INLINE lua_State* getluState() { return lu; }

		LuaObjectBinding* getUnitBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_unitBinding.find(Id);
			return (itr == m_unitBinding.end()) ? NULL : &itr->second;
		}

		LuaObjectBinding* getQuestBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_questBinding.find(Id);
			return (itr == m_questBinding.end()) ? NULL : &itr->second;
		}

		LuaObjectBinding* getGameObjectBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_gameobjectBinding.find(Id);
			return (itr == m_gameobjectBinding.end()) ? NULL : &itr->second;
		}

		LuaObjectBinding* getInstanceBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_instanceBinding.find(Id);
			return (itr == m_instanceBinding.end()) ? NULL : &itr->second;
		}

		LuaObjectBinding* getLuaUnitGossipBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_unit_gossipBinding.find(Id);
			return (itr == m_unit_gossipBinding.end()) ? NULL : &itr->second;
		}

		LuaObjectBinding* getLuaItemGossipBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_item_gossipBinding.find(Id);
			return (itr == m_item_gossipBinding.end()) ? NULL : &itr->second;
		}

		LuaObjectBinding* getLuaGOGossipBinding(uint32 Id)
		{
			LuaObjectBindingMap::iterator itr = m_go_gossipBinding.find(Id);
			return (itr == m_go_gossipBinding.end()) ? NULL : &itr->second;
		}
		LuaQuest* getLuaQuest(uint32 id)
		{
			HM_NAMESPACE::hash_map<uint32, LuaQuest*>::iterator itr = m_qAIScripts.find(id);
			return (itr == m_qAIScripts.end()) ? NULL : itr->second;
		}
		/*int getPendingThread(lua_State * threadtosearch) {
			set<lua_State*>::iterator itr = m_pendingThreads.find(threadtosearch);
			return (itr == m_pendingThreads.end() )? NULL : (*itr);
		}*/
		LuaGossip* getUnitGossipInterface(uint32 id)
		{
			HM_NAMESPACE::hash_map<uint32, LuaGossip*>::iterator itr = m_unitgAIScripts.find(id);
			return (itr == m_unitgAIScripts.end()) ? NULL : itr->second;
		}
		LuaGossip* getItemGossipInterface(uint32 id)
		{
			HM_NAMESPACE::hash_map<uint32, LuaGossip*>::iterator itr = m_itemgAIScripts.find(id);
			return (itr == m_itemgAIScripts.end()) ? NULL : itr->second;
		}
		LuaGossip* getGameObjectGossipInterface(uint32 id)
		{
			HM_NAMESPACE::hash_map<uint32, LuaGossip*>::iterator itr = m_gogAIScripts.find(id);
			return (itr == m_gogAIScripts.end()) ? NULL : itr->second;
		}
		ARCEMU_INLINE std::multimap<uint32, LuaCreature*> & getLuCreatureMap() { return m_cAIScripts; }
		ARCEMU_INLINE std::multimap<uint32, LuaGameObjectScript*> & getLuGameObjectMap() { return m_gAIScripts; }
		ARCEMU_INLINE HM_NAMESPACE::hash_map<uint32, LuaQuest*> & getLuQuestMap() { return m_qAIScripts; }
		ARCEMU_INLINE HM_NAMESPACE::hash_map<uint32, LuaInstance*> & getLuInstanceMap() { return m_iAIScripts; }
		ARCEMU_INLINE HM_NAMESPACE::hash_map<uint32, LuaGossip*> & getUnitGossipInterfaceMap() { return m_unitgAIScripts; }
		ARCEMU_INLINE HM_NAMESPACE::hash_map<uint32, LuaGossip*> & getItemGossipInterfaceMap() { return m_itemgAIScripts; }
		ARCEMU_INLINE HM_NAMESPACE::hash_map<uint32, LuaGossip*> & getGameObjectGossipInterfaceMap() { return m_gogAIScripts; }
		ARCEMU_INLINE std::set<int> & getThreadRefs() { return m_pendingThreads; }
		ARCEMU_INLINE std::set<int> & getFunctionRefs() { return m_functionRefs; }
		ARCEMU_INLINE std::map< uint64, std::set<int> > & getObjectFunctionRefs() { return m_objectFunctionRefs; }

		HM_NAMESPACE::hash_map<int, EventInfoHolder*> m_registeredTimedEvents;

		struct _ENGINEHOOKINFO
		{
			bool hooks[NUM_SERVER_HOOKS];
			std::vector<uint32> dummyHooks;
			_ENGINEHOOKINFO()
			{
				for(int i = 0; i < NUM_SERVER_HOOKS; ++i)
					hooks[i] = false;
			}
		} HookInfo;

		class luEventMgr : public EventableObject
		{
			public:
				bool HasEvent(int ref)
				{
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.find(ref);
					return (itr != sLuaMgr.m_registeredTimedEvents.end());
				}
				bool HasEventInTable(const char* table)
				{
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.begin();
					for(; itr != sLuaMgr.m_registeredTimedEvents.end(); ++itr)
					{
						if(strncmp(itr->second->funcName, table, strlen(table)) == 0)
						{
							return true;
						}
					}
					return false;
				}
				bool HasEventWithName(const char* name)
				{
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.begin();
					for(; itr != sLuaMgr.m_registeredTimedEvents.end(); ++itr)
					{
						if(strcmp(itr->second->funcName, name) == 0)
						{
							return true;
						}
					}
					return false;
				}
				void RemoveEventsInTable(const char* table)
				{
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.begin(), itr2;
					for(; itr != sLuaMgr.m_registeredTimedEvents.end();)
					{
						itr2 = itr++;
						if(strncmp(itr2->second->funcName, table, strlen(table)) == 0)
						{
							event_RemoveByPointer(itr2->second->te);
							free((void*)itr2->second->funcName);
							luaL_unref(sLuaMgr.getluState(), LUA_REGISTRYINDEX, itr2->first);
							sLuaMgr.m_registeredTimedEvents.erase(itr2);
						}
					}
				}
				void RemoveEventsByName(const char* name)
				{
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.begin(), itr2;
					for(; itr != sLuaMgr.m_registeredTimedEvents.end();)
					{
						itr2 = itr++;
						if(strcmp(itr2->second->funcName, name) == 0)
						{
							event_RemoveByPointer(itr2->second->te);
							free((void*)itr2->second->funcName);
							luaL_unref(sLuaMgr.getluState(), LUA_REGISTRYINDEX, itr2->first);
							sLuaMgr.m_registeredTimedEvents.erase(itr2);
						}
					}
				}
				void RemoveEventByRef(int ref)
				{
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.find(ref);
					if(itr != sLuaMgr.m_registeredTimedEvents.end())
					{
						event_RemoveByPointer(itr->second->te);
						free((void*)itr->second->funcName);
						luaL_unref(sLuaMgr.getluState(), LUA_REGISTRYINDEX, itr->first);
						sLuaMgr.m_registeredTimedEvents.erase(itr);
					}
				}
				void RemoveEvents()
				{
					event_RemoveEvents(EVENT_LUA_TIMED);
					HM_NAMESPACE::hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.begin();
					for(; itr != sLuaMgr.m_registeredTimedEvents.end(); ++itr)
					{
						free((void*)itr->second->funcName);
						luaL_unref(sLuaMgr.getluState(), LUA_REGISTRYINDEX, itr->first);
					}
					sLuaMgr.m_registeredTimedEvents.clear();
				}
		} LuaEventMgr;

	protected:
		//Hidden methods
		void Unload();
		void ScriptLoadDir(char* Dirname, LUALoadScripts* pak);

		template <typename T>
		class ArcLuna
		{
			public:
				typedef int (*mfp)(lua_State* L, T* ptr);
				typedef struct { const char* name; mfp mfunc; } RegType;

				static void Register(lua_State* L)
				{
					lua_newtable(L);
					int methods = lua_gettop(L);

					luaL_newmetatable(L, GetTClassName<T>());
					int metatable = lua_gettop(L);

					luaL_newmetatable(L, "DO NOT TRASH");
					lua_pop(L, 1);

					// store method table in globals so that
					// scripts can add functions written in Lua.
					lua_pushvalue(L, methods);
					lua_setglobal(L, GetTClassName<T>());

					// hide metatable from Lua getmetatable()
					lua_pushvalue(L, methods);
					lua_setfield(L, metatable, "__metatable");

					lua_pushcfunction(L, index);
					lua_setfield(L, metatable, "__index");

					lua_pushcfunction(L, tostring_T);
					lua_setfield(L, metatable, "__tostring");

					lua_pushcfunction(L, gc_T);
					lua_setfield(L, metatable, "__gc");

					lua_newtable(L);                // mt for method table
					lua_setmetatable(L, methods);

					// fill method table with methods from class T
					for(RegType* l = ((RegType*)GetMethodTable<T>()); l->name; l++)
					{
						lua_pushstring(L, l->name);
						lua_pushlightuserdata(L, (void*)l);
						lua_pushcclosure(L, thunk, 1);
						lua_settable(L, methods);
					}
					lua_pop(L, 2);  // drop metatable and method table
				}

				// push onto the Lua stack a userdata containing a pointer to T object
				static int push(lua_State* L, T* obj, bool gc = false)
				{
					if(!obj) { lua_pushnil(L); return lua_gettop(L); }
					luaL_getmetatable(L, GetTClassName<T>());  // lookup metatable in Lua registry
					if(lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", GetTClassName<T>());
					int mt = lua_gettop(L);
					T** ptrHold = (T**)lua_newuserdata(L, sizeof(T**));
					int ud = lua_gettop(L);
					if(ptrHold != NULL)
					{
						*ptrHold = obj;
						lua_pushvalue(L, mt);
						lua_setmetatable(L, -2);
						char name[32];
						tostring(name, obj);
						lua_getfield(L, LUA_REGISTRYINDEX, "DO NOT TRASH");
						if(lua_isnil(L, -1))
						{
							luaL_newmetatable(L, "DO NOT TRASH");
							lua_pop(L, 1);
						}
						lua_getfield(L, LUA_REGISTRYINDEX, "DO NOT TRASH");
						if(gc == false)
						{
							lua_pushboolean(L, 1);
							lua_setfield(L, -2, name);
						}
						lua_pop(L, 1);
					}
					lua_settop(L, ud);
					lua_replace(L, mt);
					lua_settop(L, mt);
					return mt;  // index of userdata containing pointer to T object
				}

				// get userdata from Lua stack and return pointer to T object
				static T* check(lua_State* L, int narg)
				{
					T** ptrHold = static_cast<T**>(lua_touserdata(L, narg));
					if(ptrHold == NULL)
						return NULL;
					return *ptrHold;
				}

			private:
				ArcLuna();  // hide default constructor

				// member function dispatcher
				static int thunk(lua_State* L)
				{
					// stack has userdata, followed by method args
					T* obj = check(L, 1);  // get 'self', or if you prefer, 'this'
					lua_remove(L, 1);  // remove self so member function args start at index 1
					// get member function from upvalue
					RegType* l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
					//return (obj->*(l->mfunc))(L);  // call member function
					return l->mfunc(L, obj);
				}

				// garbage collection metamethod
				static int gc_T(lua_State* L)
				{
					T* obj = check(L, 1);
					if(obj == NULL)
						return 0;
					lua_getfield(L, LUA_REGISTRYINDEX, "DO NOT TRASH");
					if(lua_istable(L, -1))
					{
						char name[32];
						tostring(name, obj);
						lua_getfield(L, -1, string(name).c_str());
						if(lua_isnil(L, -1))
						{
							delete obj;
							obj = NULL;
						}
					}
					lua_pop(L, 3);
					return 0;
				}
				static int tostring_T(lua_State* L)
				{
					char buff[32];
					T** ptrHold = (T**)lua_touserdata(L, 1);
					T* obj = *ptrHold;
					sprintf(buff, "%p", obj);
					lua_pushfstring(L, "%s (%s)", GetTClassName<T>(), buff);
					return 1;
				}
				ARCEMU_INLINE static void tostring(char* buff, void* obj)
				{
					sprintf(buff, "%p", obj);
				}
				static int index(lua_State* L)
				{
					/*Paroxysm : the table obj and the missing key are currently on the stack(index 1 & 2) */
					lua_getglobal(L, GetTClassName<T>());
					// string form of the key.
					const char* key = lua_tostring(L, 2);
					if(lua_istable(L, -1))
					{
						lua_pushvalue(L, 2);
						lua_rawget(L, -2);
						//If the key were looking for is not in the table, retrieve its' metatables' index value.
						if(lua_isnil(L, -1))
						{
							lua_getmetatable(L, -2);
							if(lua_istable(L, -1))
							{
								lua_getfield(L, -1, "__index");
								if(lua_isfunction(L, -1))
								{
									lua_pushvalue(L, 1);
									lua_pushvalue(L, 2);
									lua_pcall(L, 2, 1, 0);
								}
								else if(lua_istable(L, -1))
									lua_getfield(L, -1, key);
								else
									lua_pushnil(L);
							}
							else
								lua_pushnil(L);
						}
						else if(lua_istable(L, -1))
						{
							lua_pushvalue(L, 2);
							lua_rawget(L, -2);
						}
					}
					else
						lua_pushnil(L);

					lua_insert(L, 1);
					lua_settop(L, 1);
					return 1;
				}
		};
		class GUID_MGR
		{
				static const char* GetName() { return "WoWGUID"; }
			public:
				static void Register(lua_State* L)
				{
					luaL_newmetatable(L, GetName());
					int mt = lua_gettop(L);
					//Hide metatable.
					lua_pushnil(L);
					lua_setfield(L, mt, "__metatable");
					//nil gc method
					lua_pushnil(L);
					lua_setfield(L, mt, "__gc");
					//set our tostring method
					lua_pushcfunction(L, _tostring);
					lua_setfield(L, mt, "__tostring");
					//nil __index field
					lua_pushnil(L);
					lua_setfield(L, mt, "__index");
					//set __newindex method
					lua_pushcfunction(L, _newindex);
					lua_setfield(L, mt, "__newindex");
					//no call method
					lua_pushnil(L);
					lua_setfield(L, mt, "__call");
					//pop metatable
					lua_pop(L, 1);
				}
				static uint64 check(lua_State* L, int narg)
				{
					uint64 GUID = 0;
					uint64* ptrHold = (uint64*)lua_touserdata(L, narg);
					if(ptrHold != NULL)
						GUID = *ptrHold;
					return GUID;
				}
				static int push(lua_State* L, uint64 guid)
				{
					int index = 0;
					if(guid == 0)
					{
						lua_pushnil(L);
						index = lua_gettop(L);
					}
					else
					{
						luaL_getmetatable(L, GetName());
						if(lua_isnoneornil(L, -1))
							luaL_error(L, "%s metatable not found!. \n", GetName());
						else
						{
							int mt = lua_gettop(L);
							uint64* guidHold = (uint64*)lua_newuserdata(L, sizeof(uint64));
							int ud = lua_gettop(L);
							if(guidHold == NULL)
								luaL_error(L, "Lua tried to allocate size %d of memory and failed! \n", sizeof(uint64*));
							else
							{
								(*guidHold) = guid;
								lua_pushvalue(L, mt);
								lua_setmetatable(L, ud);
								lua_replace(L, mt);
								lua_settop(L, mt);
								index = mt;
							}
						}
					}
					return index;
				}
			private:
				GUID_MGR() {}
				//This method prints formats the GUID in hexform and pushes to the stack.
				static int _tostring(lua_State* L)
				{
					uint64 GUID = GUID_MGR::check(L, 1);
					if(GUID == 0)
						lua_pushnil(L);
					else
					{
						char buff[32];
						sprintf(buff, "%X", GUID);
						lua_pushfstring(L, "%s", buff);
					}
					return 1;
				}
				static int _newindex(lua_State* L)
				{
					//Remove table, key, and value
					lua_remove(L, 1);
					lua_remove(L, 1);
					lua_remove(L, 1);
					luaL_error(L, "OPERATION PROHIBITED!\n");
					return 0;
				}
		};
};
#endif

