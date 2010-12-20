#pragma once
#include "LUAEngine.h"

class LuaGossip : public GossipScript
{
	uint32 id;
public:
	LuaGossip(uint32 _id) : GossipScript(), m_go_gossip_binding(NULL),m_item_gossip_binding(NULL),m_unit_gossip_binding(NULL), id(_id) {}
	~LuaGossip() 
	{
		if(this->m_go_gossip_binding != NULL)
		{
			li::GossipInterfaceMap::iterator itr = lua_instance->m_go_gossipInterfaceMap.find(id);
			if(itr != lua_instance->m_go_gossipInterfaceMap.end() )
				lua_instance->m_go_gossipInterfaceMap.erase(itr);
		}
		else if(this->m_unit_gossip_binding != NULL)
		{
			li::GossipInterfaceMap::iterator itr = lua_instance->m_unit_gossipInterfaceMap.find(id);
			if(itr != lua_instance->m_unit_gossipInterfaceMap.end() )
				lua_instance->m_unit_gossipInterfaceMap.erase(itr);
		}
		else if(this->m_item_gossip_binding != NULL)
		{
			li::GossipInterfaceMap::iterator itr = lua_instance->m_item_gossipInterfaceMap.find(id);
			if(itr != lua_instance->m_item_gossipInterfaceMap.end( ) )
				lua_instance->m_item_gossipInterfaceMap.erase(itr);
		}
	}

	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GET_LOCK;
		if(pObject->GetTypeId() == TYPEID_UNIT)
		{
			if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }

			lua_engine::BeginLuaFunctionCall(m_unit_gossip_binding->refs[GOSSIP_EVENT_ON_TALK]);
			push_unit(TO_UNIT(pObject) );
			push_int(GOSSIP_EVENT_ON_TALK);
			push_unit(Plr);
			push_bool(AutoSend);
			lua_engine::ExecuteLuaFunction(4);
		}
		else if(pObject->GetTypeId() == TYPEID_ITEM)
		{
			if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }

			lua_engine::BeginLuaFunctionCall(m_item_gossip_binding->refs[GOSSIP_EVENT_ON_TALK]);
			push_item( TO_ITEM(pObject) );
			push_int(GOSSIP_EVENT_ON_TALK);
			push_unit(Plr);
			push_bool(AutoSend);
			lua_engine::ExecuteLuaFunction(4);
		}
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }

			lua_engine::BeginLuaFunctionCall(m_go_gossip_binding->refs[GOSSIP_EVENT_ON_TALK]);
			push_go(TO_GAMEOBJECT(pObject));
			push_int(GOSSIP_EVENT_ON_TALK);
			push_unit(Plr);
			push_bool(AutoSend);
			lua_engine::ExecuteLuaFunction(4);
		}
		RELEASE_LOCK
	}

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		GET_LOCK;
		if(pObject->GetTypeId() == TYPEID_UNIT)
		{
			if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }

			lua_engine::BeginLuaFunctionCall(m_unit_gossip_binding->refs[GOSSIP_EVENT_ON_SELECT_OPTION]);
			push_unit(TO_UNIT(pObject));
			push_int(GOSSIP_EVENT_ON_SELECT_OPTION);
			push_unit(Plr);
			push_int(Id);
			push_int(IntId);
			push_str(EnteredCode);
			lua_engine::ExecuteLuaFunction(6);
		}
		else if(pObject->GetTypeId() == TYPEID_ITEM)
		{
			if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }
			lua_engine::BeginLuaFunctionCall(m_item_gossip_binding->refs[GOSSIP_EVENT_ON_SELECT_OPTION]);
			push_item(TO_ITEM(pObject));
			push_int(GOSSIP_EVENT_ON_SELECT_OPTION);
			push_unit(Plr);
			push_int(Id);
			push_int(IntId);
			push_str(EnteredCode);
			lua_engine::ExecuteLuaFunction(6);
		}
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }
			lua_engine::BeginLuaFunctionCall(m_go_gossip_binding->refs[GOSSIP_EVENT_ON_SELECT_OPTION]);
			push_go(TO_GAMEOBJECT(pObject));
			push_int(GOSSIP_EVENT_ON_SELECT_OPTION);
			push_unit(Plr);
			push_int(Id);
			push_int(IntId);
			push_str(EnteredCode);
			lua_engine::ExecuteLuaFunction(6);
		}
		RELEASE_LOCK
	}

	void GossipEnd(Object* pObject, Player* Plr)
	{
		GET_LOCK;
		if(pObject->GetTypeId() == TYPEID_UNIT)
		{
			if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }
			lua_engine::BeginLuaFunctionCall(m_unit_gossip_binding->refs[GOSSIP_EVENT_ON_END]);
			push_unit(TO_UNIT(pObject));
			push_int(GOSSIP_EVENT_ON_END);
			push_unit(Plr);
			lua_engine::ExecuteLuaFunction(3);
		}
		else if(pObject->GetTypeId() == TYPEID_ITEM)
		{
			if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }
			lua_engine::BeginLuaFunctionCall(m_item_gossip_binding->refs[GOSSIP_EVENT_ON_END]);
			push_item(TO_ITEM(pObject));
			push_int(GOSSIP_EVENT_ON_END);
			push_unit(Plr);
			lua_engine::ExecuteLuaFunction(3);
		}
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }
			lua_engine::BeginLuaFunctionCall(m_go_gossip_binding->refs[GOSSIP_EVENT_ON_END]);
			push_go(TO_GAMEOBJECT(pObject));
			push_int(GOSSIP_EVENT_ON_END);
			push_unit(Plr);
			lua_engine::ExecuteLuaFunction(3);
		}
		RELEASE_LOCK
	}

	PObjectBinding m_unit_gossip_binding;
	PObjectBinding m_item_gossip_binding;
	PObjectBinding m_go_gossip_binding;
};

namespace lua_engine
{
	GossipScript * createunitgossipInterface(uint32 id)
	{
		LuaGossip * pLua = NULL;
		li::ObjectBindingMap::iterator itr = lua_instance->m_unitGossipBinding.find(id);
		PObjectBinding pBinding = (itr != lua_instance->m_unitGossipBinding.end() ) ? itr->second : NULL;
		if( pBinding != NULL )
		{
			li::GossipInterfaceMap::iterator itr = lua_instance->m_unit_gossipInterfaceMap.find(id);
			if(itr != lua_instance->m_unit_gossipInterfaceMap.end() )
			{
				if(itr->second == NULL)
					pLua = itr->second = new LuaGossip(id);
				else
					pLua = itr->second;
			}
			else
			{
				pLua = new LuaGossip(id);
				lua_instance->m_unit_gossipInterfaceMap.insert(make_pair(id,pLua));
			}
			pLua->m_unit_gossip_binding = pBinding;
		}
		return pLua;
	}
	GossipScript * createitemgossipInterface(uint32 id)
	{
		LuaGossip * pLua = NULL;
		li::ObjectBindingMap::iterator itr = lua_instance->m_itemGossipBinding.find(id);
		PObjectBinding pBinding = (itr != lua_instance->m_itemGossipBinding.end() ) ? itr->second : NULL;
		if( pBinding != NULL )
		{
			li::GossipInterfaceMap::iterator itr = lua_instance->m_item_gossipInterfaceMap.find(id);
			if(itr != lua_instance->m_item_gossipInterfaceMap.end() )
			{
				if(itr->second == NULL)
					pLua = itr->second = new LuaGossip(id);
				else
					pLua = itr->second;
			}
			else
			{
				pLua = new LuaGossip(id);
				lua_instance->m_item_gossipInterfaceMap.insert(make_pair(id,pLua));
			}
			pLua->m_item_gossip_binding = pBinding;
		}
		return pLua;
	}
	GossipScript * creategogossipInterface(uint32 id)
	{
		LuaGossip * pLua = NULL;
		li::ObjectBindingMap::iterator itr = lua_instance->m_goGossipBinding.find(id);
		PObjectBinding pBinding = (itr != lua_instance->m_goGossipBinding.end() ) ? itr->second : NULL;
		if( pBinding != NULL )
		{
			li::GossipInterfaceMap::iterator itr = lua_instance->m_go_gossipInterfaceMap.find(id);
			if(itr != lua_instance->m_go_gossipInterfaceMap.end() )
			{
				if(itr->second == NULL)
					pLua = itr->second = new LuaGossip(id);
				else
					pLua = itr->second;
			}
			else
			{
				pLua = new LuaGossip(id);
				lua_instance->m_go_gossipInterfaceMap.insert(make_pair(id,pLua));
			}
			pLua->m_go_gossip_binding = pBinding;
		}
		return pLua;
	}
}

int CreateLuaEvent(lua_function fref, int delay, int repeats, variadic_parameter* params)
{
	int ref = LUA_REFNIL;
	if(delay > 0 && (int)fref != LUA_REFNIL && params != NULL)
	{
		//embed the function ref and repeats as part of our parameters.
		variadic_node * func_node = new variadic_node;
		variadic_node * repeats_node = new variadic_node;
		repeats_node->type = CUSTOM_TYPE_REPEATS_ARG;
		repeats_node->val.bewl = repeats;
		func_node->type = LUA_TFUNCTION;
		func_node->val.obj_ref = (int)fref;
		//make the function the new head node.
		func_node->next = repeats_node;
		repeats_node->next = params->head_node;
		params->head_node = func_node;
		//update args count
		params->count +=2;
		TimedEvent * ev = TimedEvent::Allocate(World::getSingletonPtr(),new CallBackFunctionP1<variadic_parameter*>(&lua_engine::ExecuteLuaFunction,params),0,delay,repeats);
		ev->eventType  = LUA_EVENTS_END+(int)fref; //Create custom reference by adding the ref number to the max lua event type to get a unique reference for every function.
		sWorld.event_AddEvent(ev);
		lua_instance->m_globalFRefs.insert(params);
	}
	return ref;
}
void lua_engine::ExecuteLuaFunction(variadic_parameter * params)
{
	if(params != NULL )
	{
		lua_State * lu = lua_state;
		//place the function on the stack.
		lua_getref(lu,params->head_node->val.obj_ref);
		int arg_cnt = params->count - 2;
		if(arg_cnt > 0)
		{
			//retrieve the repeats.
			variadic_node * function_node = params->head_node;
			uint16 repeats = params->head_node->next->val.bewl;
			/*	Prepare to push arguments, 1st assign the head node to the actual arguments registered to this function */
			params->head_node = function_node->next->next;
			//subtract the function n repeat node from arg count.
			params->count-= 2;
			//Now we push all args.
			luabridge::tdstack<variadic_parameter*>::push(lu, params);
			//call the function
			if(lua_pcall(lu, arg_cnt, 0,0) )
				report(lu);
			//if it's not an infinite/one time call event.
			if(repeats > 1)
				//decrement repeats and put it back in the params.
				function_node->next->val.bewl = (int)--repeats;
			else if(repeats == 1)
			{
				//reset our function node as the starting node.
					
				variadic_node * repeats_node = function_node->next;
				function_node->next = params->head_node;
				params->head_node = function_node;
				//de-allocate repeats node
				delete repeats_node;
				//remove this function from storage.
				lua_instance->m_globalFRefs.erase(params);
				//since we've put the function node back.
				params->count++;
				//clean up the rest of the args
				cleanup_varparam(params, lu);
			}
		}	
	}
}
void ModifyLuaEventInterval(lua_function ref, int newInterval)
{
	//Easy interval modification.
	sEventMgr.ModifyEventTime(World::getSingletonPtr(),(int)ref+LUA_EVENTS_END, newInterval);
}
static void DestroyLuaEvent(lua_function ref)
{
	//Simply remove the reference, CallFunctionByReference will find the reference has been freed and skip any processing.
	lua_unref(lua_state,(int)ref);
	for(li::References::iterator itr = lua_instance->m_globalFRefs.begin(); itr != lua_instance->m_globalFRefs.end(); ++itr)
	{
		if( (*itr) != NULL && (*itr)->head_node->type == LUA_TFUNCTION && (*itr)->head_node->val.obj_ref == (int)ref)
		{
			lua_instance->m_globalFRefs.erase(itr);
			break;
		}
	}
	sEventMgr.RemoveEvents(World::getSingletonPtr(),(int)ref+LUA_EVENTS_END);
}

//Used to clean up any pending events when restarting.
void DestroyAllLuaEvents(PLUA_INSTANCE instance)
{
	//Clean up for all events.
	li::References::iterator itr = instance->m_globalFRefs.begin();
	int ref = LUA_REFNIL;
	GET_LOCK;
	for(; itr != instance->m_globalFRefs.end(); ++itr)
	{
		if( (*itr) != NULL && (*itr)->head_node != NULL && (*itr)->head_node->type == LUA_TFUNCTION)
		{
			ref = (*itr)->head_node->val.obj_ref;
			sEventMgr.RemoveEvents(World::getSingletonPtr(),ref+LUA_EVENTS_END);
			cleanup_varparam( (*itr), lua_state);
		}
	}
	lua_instance->m_globalFRefs.clear();
}

static int GetMapID()
{
	if(lua_instance != NULL && lua_instance->map != NULL)
		return (int)lua_instance->map->GetMapId();
	return -1;
}
static int GetInstanceID()
{
	if(lua_instance != NULL && lua_instance->map != NULL)
		return (int)lua_instance->map->GetInstanceID();
	return -1;
}
static void GetRegistryTable(const char * name, lua_stack stack)
{
	lua_getfield( (lua_thread)stack, LUA_REGISTRYINDEX, name);
	if(lua_type( (lua_thread)stack, -1) != LUA_TTABLE)
	{
		lua_pop( (lua_thread)stack, 1);
		lua_pushnil( (lua_thread) stack);
	}
}
#include "GlobalFunctions.h"

namespace lua_engine
{
	void bindGlobalMethods(luabridge::module & m)
	{
		m	.function("CreateLuaEvent", &CreateLuaEvent)
			.function("ModifyLuaEventInterval", &ModifyLuaEventInterval)
			.function("ModifyLuaEventTimer", &ModifyLuaEventInterval)
			.function("DestroyLuaEvent", &DestroyLuaEvent)
			.function("GetMapID", &GetMapID)
			.function("GetInstanceID", &GetInstanceID)
			//Used to retrieve object method tables.
			.function("getregistry", &GetRegistryTable)
#define bind(name) .function(#name, &##name)
			bind(GetGameTime)
			bind(GetPlayer)
			.function("GetEngineName", &enginename)
			.function("GetEngineVersion", &engineversion)
			bind(logcolor)
			bind(WorldDBQuery)
			bind(CharDBQuery)
			bind(SendWorldMessage)
			bind(ReloadTable)
			bind(ReloadLuaEngine)
			bind(GetPlayersInWorld)
			bind(Rehash)
			bind(GetArcemuRevision)
			bind(GetPlayersInMap)
			bind(GetPlayersInZone)
			.function("bit_and", &luabit_and)
			.function("bit_or", &luabit_or)
			.function("bit_xor", &luabit_xor)
			.function("bit_not", &luabit_not)
			bind(bit_shiftleft)
			bind(bit_shiftright)
			bind(GetPlatform)
			bind(SendPacketToZone)
			bind(SendPacketToInstance)
			bind(SendPacketToWorld)
			bind(SendPacketToChannel)
			bind(GetInstanceCreature)
			bind(GetInstancePlayerCount)
			bind(GetPlayersInInstance);
#undef bind

		m	.class_<DBCStorage<SpellEntry> >("dbcSpell")
			.method("LookupEntry", &DBCStorage<SpellEntry>::LookupEntry)
			.method("GetNumRows", &DBCStorage<SpellEntry>::GetNumRows)
			.method("LookupEntryForced", &DBCStorage<SpellEntry>::LookupEntryForced);
		//now we overwrite our dbcSpell static table with dbcSpell storage object.
		luabridge::tdstack<DBCStorage<SpellEntry>*>::push(m.L, &dbcSpell);
		lua_setglobal(m.L, "dbcSpell");

	}
}