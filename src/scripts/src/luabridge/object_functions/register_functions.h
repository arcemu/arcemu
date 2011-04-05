/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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
#pragma once
#include "../interpreter/LUAEngine.h"

static bool registerServerHook(uint32 hook, lua_function);
static bool registerUnitEvent(uint32 , uint32 , lua_function);
static bool registerQuestEvent(uint32,  uint32, lua_function);
static bool registerGameObjectEvent(uint32, uint32, lua_function);
static bool registerUnitGossipEvent(uint32, uint32, lua_function);
static bool registerItemGossipEvent(uint32, uint32, lua_function);
static bool registerGOGossipEvent(uint32, uint32, lua_function);
static int suspendluathread(lua_thread, int wait_time, variadic_parameter *);
static void resumeluathread(lua_thread);
//static bool registerTimedEvent(variadic_parameter *);
static void removeTimedEvents();
static bool registerDummySpell(uint32, lua_function, variadic_parameter*);
static bool registerInstanceEvent(uint32, uint32, lua_function);

namespace lua_engine
{
	void bindRegisterMethods(luabridge::module & m)
	{
		m	.function("RegisterServerHook",&registerServerHook)
			.function("RegisterUnitEvent", &registerUnitEvent)
			.function("RegisterQuestEvent", &registerQuestEvent)
			.function("RegisterGameObjectEvent", &registerGameObjectEvent)
			.function("RegisterUnitGossipEvent", &registerUnitGossipEvent)
			.function("RegisterItemGossipEvent", &registerItemGossipEvent)
			.function("RegisterGOGossipEvent", &registerGOGossipEvent)
			//.function("RegisterTimedEvent", &registerTimedEvent)
			.function("RegisterDummySpell", &registerDummySpell)
			.function("RegisterInstanceEvent", &registerInstanceEvent)
			.function("RemoveTimedEvents", &removeTimedEvents)
			.function("SuspendLuaThread", &suspendluathread);
	}
}
using namespace lua_engine;
bool registerServerHook(uint32 hook, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(hook < NUM_SERVER_HOOKS && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::HookFRefMap::iterator itr = li_->m_hooks.find(hook);
		for(; itr != li_->m_hooks.upper_bound(hook); ++itr)
		{
			if(itr->second == ref)
			{
				found = true;
				break;
			}
		}
		if(!found)
			li_->m_hooks.insert(make_pair(hook,ref) );
	}
	return !found;
}
bool registerUnitEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < CREATURE_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_unitBinding.find(entry);
		if(itr != li_->m_unitBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref(li_->lu, (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_unitBinding.insert(make_pair(entry,bind) );
		}
	}
	return found;
}
bool registerQuestEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < QUEST_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_questBinding.find(entry);
		if(itr != li_->m_questBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref( li_->lu, (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_questBinding.insert(make_pair(entry,bind) );
		}
	}
	return found;
}
bool registerGameObjectEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < GAMEOBJECT_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_goBinding.find(entry);
		if(itr != li_->m_goBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref( li_->lu , (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_goBinding.insert(make_pair(entry,bind) );
		}
	}
	return found;
}
bool registerUnitGossipEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < GOSSIP_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_unitGossipBinding.find(entry);
		if(itr != li_->m_unitGossipBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref( li_->lu, (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_unitGossipBinding.insert(make_pair(entry,bind) );
		}
	}
	return found;
}
bool registerItemGossipEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < GOSSIP_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_itemGossipBinding.find(entry);
		if(itr != li_->m_itemGossipBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref( li_->lu, (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_itemGossipBinding.insert( make_pair(entry, bind) );
		}
	}
	return found;
}
bool registerGOGossipEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < GOSSIP_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_goGossipBinding.find(entry);
		if(itr != li_->m_goGossipBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref( li_->lu , (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_goGossipBinding.insert(make_pair(entry, bind) );
		}
	}
	return found;
}
bool registerInstanceEvent(uint32 entry, uint32 evt, lua_function ref)
{
	bool found = false;
	PLUA_INSTANCE li_ = lua_instance.get();
	if(evt < INSTANCE_EVENT_COUNT && (ptrdiff_t)ref != LUA_REFNIL)
	{
		li::ObjectBindingMap::iterator itr = li_->m_instanceBinding.find(entry);
		if(itr != li_->m_instanceBinding.end() )
		{
			found = true;
			if(itr->second->refs[evt] != NULL)
				lua_unref( li_->lu, (ptrdiff_t)itr->second->refs[evt]);
			itr->second->refs[evt] = ref;
		}
		else
		{
			PObjectBinding bind = new ObjectBinding;
			memset(bind, 0, sizeof(ObjectBinding) );
			bind->refs[evt] = ref;
			li_->m_instanceBinding.insert(make_pair(entry,bind) );
		}

	}
	return found;
}
bool registerDummySpell(uint32 entry, lua_function ref, variadic_parameter * params)
{
	bool found = true;
	PLUA_INSTANCE li_ = lua_instance.get();
	if( (ptrdiff_t)ref != LUA_REFNIL )
	{
		li::SpellFRefMap::iterator itr = li_->m_dummySpells.find(entry);
		if(itr == li_->m_dummySpells.end() )
		{
			found = false;
			PSpellMapEntry pentry = new SpellMapEntry;
			pentry->ref = (ptrdiff_t)ref;
			pentry->params = params;
			li_->m_dummySpells.insert( make_pair(entry, pentry) );
		}
	}
	return !found;
}
void removeTimedEvents()
{
	// TO-DO : Reimplement
	//lua_instance->eventMgr.RemoveEvents();
}

ptrdiff_t extractfRefFromCString(lua_State * L,const char * functionName)
{
	ptrdiff_t functionRef = LUA_REFNIL;
	//lua_State * L = lua_state;
	ptrdiff_t top = lua_gettop(L);
	if(functionName != NULL)
	{
		char copy[256];
		strcpy(copy, functionName);
		char * token = strtok(copy, ".:");
		if(strpbrk(functionName,".:") == NULL)
		{
			lua_getglobal(L,functionName);
			if (lua_isfunction(L,-1) && !lua_iscfunction(L,-1))
				functionRef = lua_ref(L,true);
			else
				luaL_error(L,"Reference creation failed! (%s) is not a valid Lua function. \n",functionName);
		}
		else
		{
			lua_getglobal(L,"_G");
			while (token != NULL)
			{
				lua_getfield(L,-1,token);
				if(lua_isfunction(L,-1) && !lua_iscfunction(L,-1) )
				{
					functionRef = lua_ref(L,true);
					break;
				}
				else if (lua_istable(L,-1) )
				{
					token = strtok(NULL,".:");
					continue;
				}
				else
				{
					luaL_error(L,"Reference creation failed! (%s) is not a valid Lua function. \n",functionName);
					break;
				}
			}
		}
		lua_settop(L,top);
	}
	return functionRef;
}

int suspendluathread(lua_thread thread, int wait_time, variadic_parameter * params)
{
	PLUA_INSTANCE li_ = lua_instance.get();
	if(li_->map != NULL && thread != NULL && wait_time > 0)
	{
		TimedEvent * evt = TimedEvent::Allocate(NULL,new CallBackFunctionP1<lua_thread>(resumeluathread,thread),0,wait_time,1);
		//add the event to the current mapmgr.
		li_->map->event_AddEvent(evt);
		//All that remains now are the extra arguments passed to this function.
		if(params != NULL)
			luabridge::tdstack<variadic_parameter*>::push(thread, params);
		ptrdiff_t pcnt = params->count;
		//clear up references held by variadic since they are now on the thread.
		cleanup_varparam(params, li_->lu);
		li_->coroutines_.insert( thread);
		return lua_yield(thread, pcnt);
	}
	return 0;
}
void resumeluathread(lua_thread thread)
{
	//Make sure we still have the thread
	PLUA_INSTANCE li_ = lua_instance.get();
	LUA_INSTANCE::Coroutines::iterator itr = li_->coroutines_.find(thread);
	if( itr != li_->coroutines_.end() )
	{
		lua_resume(thread, lua_gettop(thread) );
		//Stop keeping track of it
		li_->coroutines_.erase( itr);
	}
}
