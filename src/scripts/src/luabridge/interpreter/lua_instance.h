#pragma once
#include "StdAfx.h"
#include "lua_defs.h"
extern "C" 
{		// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include "../lua/lualib.h"
};

typedef struct _LUA_INSTANCE
{
	//the lua state that lua code uses.
	lua_State * lu;
	//FastMutex restart_Lock;
	//stores the mapmgr object whos thread we execute in.
	MapMgr * map;
#ifdef WIN32
#define InterfaceMap std::hash_multimap
#else
#define InterfaceMap std::multimap
#endif
	typedef InterfaceMap<uint32, variadic_parameter*> ObjectFRefMap;
	typedef InterfaceMap<uint32, LuaCreature*> CreatureInterfaceMap;
	typedef InterfaceMap<uint32, LuaGameObject*> GOInterfaceMap;
	typedef InterfaceMap<uint32, LuaQuest*> QuestInterfaceMap;
	typedef InterfaceMap<uint32, lua_function> HookFRefMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGossip*> GossipInterfaceMap;
	//typedef HM_NAMESPACE::hash_map<int, EventInfoHolder*> RegisteredTimedEvents;
	typedef HM_NAMESPACE::hash_map<uint32, PSpellMapEntry> SpellFRefMap;
	typedef std::set<variadic_parameter*> References;
	typedef HM_NAMESPACE::hash_map<uint32, PObjectBinding> ObjectBindingMap;

	//Reference storing maps;
	ObjectFRefMap m_creatureFRefs;
	ObjectFRefMap m_goFRefs;
	ObjectFRefMap m_questFRefs;
	ObjectFRefMap m_instanceFRefs;
	ObjectFRefMap m_itemGossipFRefs;
	ObjectFRefMap m_unitGossipFRefs;
	ObjectFRefMap m_goGossipFRefs;
	SpellFRefMap m_dummySpells;
	HookFRefMap m_hooks;
	References pendingThreads;	//Stores coroutines that are still waiting to be executed(still yielded for a certain ms).
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

} LUA_INSTANCE, *PLUA_INSTANCE;

class LuaGuard
{
public:
	FastMutex & lock;
	LuaGuard(FastMutex & other) : lock(other)
	{
		other.Acquire();
	}
	void operator =(LuaGuard &);
	~LuaGuard() {
		lock.Release();
	}
};