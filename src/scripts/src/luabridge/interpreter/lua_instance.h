#pragma once

#include "StdAfx.h"

class ScriptManager
{
	public:
		static const char* mt;
		lua_State* state;
		ScriptManager() : state(NULL) {}
		~ScriptManager() {}

		void initialize()
		{
			lua_newtable(state);
			lua_setfield(state, LUA_REGISTRYINDEX, mt); //perform registry["LOADED_SCRIPTS"] = {}
		}
		void unload()
		{
			lua_pushnil(state);
			lua_setfield(state, LUA_REGISTRYINDEX, mt); // registry["LOADED_SCRIPTS"] = nil
		}

		void add(const std::string file)
		{
			std::string filename;
			extractFileName(file, filename);
			lua_State* L = state;
			luaL_getmetatable(L, mt);
			lua_pushstring(L, filename.c_str());
			lua_pushvalue(L, -1);
			lua_gettable(L, -2);
			if(lua_isnil(L, -1))
			{
				lua_pop(L, 1);
				lua_pushboolean(L, 1);
				lua_settable(L, -3); // LOADED_SCRIPTS[file] = true
			}
			lua_pop(L, 1); // pop our table
		}

		void remove(const std::string file)
		{
			std::string filename;
			extractFileName(file, filename);
			lua_pushnil(state);
			lua_setfield(state, LUA_REGISTRYINDEX, filename.c_str());
		}

		bool isLoaded(const std::string file) const
		{
			std::string filename;
			extractFileName(file, filename);
			lua_State* L = state;
			luaL_getmetatable(L, mt);
			lua_getfield(L, -1, filename.c_str());
			bool loaded = (lua_isnil(L, -1) != true);
			lua_pop(L, 2);
			return loaded;
		}

		/*void getloadedby(const std::string file, std::string & out) const
		{
			std::string filename;
			extractFileName(file, filename);
			luaL_getmetatable(stack, mt);
			lua_getfield(state, -2, filename.c_str() );
			if(lua_type(state, -1) == LUA_TSTRING)
				out = lua_tostring(state, -1);
			lua_pop(state, 2); //pop table and result.

		}*/

		static void extractFileName(const std::string & in, std::string & out)
		{
			out.clear();
			size_t start = in.rfind("\\");
			if(start == std::string::npos)
				start = in.rfind("/");
			if(start != std::string::npos)
				out = in.substr(++start, (in.length() - start));
			else
				out = in;
		}
};

typedef struct _LUA_INSTANCE
{
	public:
		//the lua state that lua code uses.
		lua_State* lu;
		//FastMutex restart_Lock;
		//stores the mapmgr object whos thread we execute in.
		MapMgr* map;
		typedef HM_NAMESPACE::hash_multimap<uint32, variadic_parameter*> ObjectFRefMap;
		typedef HM_NAMESPACE::hash_multimap<uint32, LuaCreature*> CreatureInterfaceMap;
		typedef HM_NAMESPACE::hash_multimap<uint32, LuaGameObject*> GOInterfaceMap;
		typedef HM_NAMESPACE::hash_multimap<uint32, LuaQuest*> QuestInterfaceMap;
		typedef HM_NAMESPACE::hash_multimap<uint32, lua_function> HookFRefMap;
		typedef HM_NAMESPACE::hash_map<uint32, LuaGossip*> GossipInterfaceMap;
		typedef HM_NAMESPACE::hash_map<uint32, PSpellMapEntry> SpellFRefMap;
		typedef HM_NAMESPACE::hash_set<variadic_parameter*> References;
		typedef HM_NAMESPACE::hash_set<lua_thread> Coroutines;
		typedef HM_NAMESPACE::hash_map<uint32, PObjectBinding> ObjectBindingMap;

		/*	Reference storing maps that store references to functions and their parameters that object allocate.
			They are required in order to validate them when they are executed at a later time. */
		ObjectFRefMap m_creatureFRefs;
		ObjectFRefMap m_goFRefs;
		ObjectFRefMap m_questFRefs;
		ObjectFRefMap m_instanceFRefs;
		ObjectFRefMap m_itemGossipFRefs;
		ObjectFRefMap m_unitGossipFRefs;
		ObjectFRefMap m_goGossipFRefs;
		SpellFRefMap m_dummySpells;
		HookFRefMap m_hooks;
		//HM_NAMESPACE::hash_set<ptrdiff_t> currentReferences;
		/*	Stores coroutines that are still waiting to be resumed.
			Since the engine can be restarted during the waiting time, it will invalidate all previously created coroutines.
			The callbacks will then try to resume an invalid coroutine pointer and crash, so we keep track of these coroutines
			and clear them when the lua instance restarts. That way, when resuming a certain coroutine, we make sure it's contained
			in this set before trying to execute it. */
		Coroutines coroutines_;
		References m_globalFRefs;	//All globally registered functions get stored here as references.

		/*	We store all previously created interfaces here because of the reload feature, if we change the functions that an interface uses,
			then the binding changes, so in order to have the new changes take effect, we have to grab a ref to the interface and update the binding. */
		CreatureInterfaceMap m_creatureInterfaceMap;
		GOInterfaceMap m_goInterfaceMap;
		QuestInterfaceMap m_questInterfaceMap;
		GossipInterfaceMap m_unit_gossipInterfaceMap;
		GossipInterfaceMap m_item_gossipInterfaceMap;
		GossipInterfaceMap m_go_gossipInterfaceMap;

		ObjectBindingMap m_unitBinding;
		ObjectBindingMap m_questBinding;
		ObjectBindingMap m_goBinding;
		ObjectBindingMap m_instanceBinding;
		ObjectBindingMap m_unitGossipBinding;
		ObjectBindingMap m_itemGossipBinding;
		ObjectBindingMap m_goGossipBinding;
		ScriptManager scripts_;

} LUA_INSTANCE, *PLUA_INSTANCE;

extern Arcemu::Utility::TLSObject<PLUA_INSTANCE> lua_instance;
extern PLUA_INSTANCE LUA_COMPILER;

class LuaGuard
{
	public:
		FastMutex & lock;
		LuaGuard(FastMutex & other) : lock(other)
		{
			other.Acquire();
		}
		void operator =(LuaGuard &);
		~LuaGuard()
		{
			lock.Release();
		}
};