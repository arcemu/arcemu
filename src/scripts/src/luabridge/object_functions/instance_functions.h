#pragma once
#include "LUAEngine.h"

class LuaInstance : public InstanceScript
{
public:
	LuaInstance( MapMgr* pMapMgr ) : InstanceScript( pMapMgr ), m_instanceId( pMapMgr->GetInstanceID() ) 
	{}
	~LuaInstance() 
	{
		if(lua_instance != NULL)
			le::shutdownThread(GetInstance() );
	}

	// Player
	void OnPlayerDeath( Player* pVictim, Unit* pKiller ) 
	{
		NULL_BINDING_CHECK
		le::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_PLAYER_DEATH]);
		push_int(m_instanceId);
		push_unit(pVictim);
		push_unit(pKiller);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	};

	// Area and AreaTrigger
	void OnPlayerEnter( Player* pPlayer ) 
	{
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_PLAYER_ENTER]);
		push_int(m_instanceId);
		push_unit(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
		RELEASE_LOCK
	};
	void OnAreaTrigger( Player* pPlayer, uint32 uAreaId ) 
	{
		NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_AREA_TRIGGER]);
		push_int(m_instanceId);
		push_unit(pPlayer);
		push_int(uAreaId);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	};
	void OnZoneChange( Player* pPlayer, uint32 uNewZone, uint32 uOldZone ) 
	{
		NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_ZONE_CHANGE]);
		push_int(m_instanceId);
		push_unit(pPlayer);
		push_int(uNewZone);
		push_int(uOldZone);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	};

	// Creature / GameObject - part of it is simple reimplementation for easier use Creature / GO < --- > Script
	void OnCreatureDeath( Creature* pVictim, Unit* pKiller ) 
	{
		NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_CREATURE_DEATH]);
		push_int(m_instanceId);
		push_unit(pVictim);
		push_unit(pKiller);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	};

	void OnCreaturePushToWorld( Creature* pCreature ) 
	{
		NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_CREATURE_PUSH]);
		push_int(m_instanceId);
		push_unit(pCreature);
		lua_engine::ExecuteLuaFunction(2);
		RELEASE_LOCK
	};

	void OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer ) 
	{
		NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_GO_ACTIVATE]);
		push_int(m_instanceId);
		push_go(pGameObject);
		push_unit(pPlayer);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	};

	void OnGameObjectPushToWorld( GameObject* pGameObject ) 
	{
		NULL_BINDING_CHECK
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_GO_PUSH]);
		push_int(m_instanceId);
		push_go(pGameObject);
		lua_engine::ExecuteLuaFunction(2);
		RELEASE_LOCK
	};

	// Standard virtual methods
	void OnLoad()
	{
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ONLOAD]);
		push_int(m_instanceId);
		lua_engine::ExecuteLuaFunction(1);
		RELEASE_LOCK
	};

	void Destroy() 
	{
		if(m_binding != NULL)
		{
			GET_LOCK;
			lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_DESTROY]);
			push_int(m_instanceId);
			lua_engine::ExecuteLuaFunction(1);
			RELEASE_LOCK
		}
		delete this;
	};

	uint32 m_instanceId;
	PObjectBinding m_binding;
};


namespace lua_engine
{
	InstanceScript * createluainstance(MapMgr* pMapMgr)
	{
		LuaInstance * pLua = NULL;
		//always allocate a new one.
		lua_instance = new LUA_INSTANCE;
		lua_state = NULL;
		lua_instance->map = pMapMgr;
		//have it load the scripts
		le::restartThread(pMapMgr);
		//store it so we can keep track of it.
		le::activestates_lock.Acquire();
		le::activeStates.insert(pMapMgr);
		le::activestates_lock.Release();

		//locate this instance's binding.
		li::ObjectBindingMap::iterator itr = lua_instance->m_instanceBinding.find( pMapMgr->GetMapId() );
		PObjectBinding bind = NULL;
		if(itr != lua_instance->m_instanceBinding.end() )
			bind = itr->second;
		pLua = new LuaInstance(pMapMgr);
		pLua->m_binding = bind;
		//Our scripts are loaded and bindings are set.
		return pLua;
	}

	void bindMapMethods(luabridge::module & m)
	{
		m	.class_<MapMgr>("MapMgr")
			.method("GetMapID", &MapMgr::GetMapId)
#define method(name) .method(#name, &MapMgr::name)
			method(CreateGameObject)
			method(CreateAndSpawnGameObject)
			method(GetGameObject)
			method(GetCreature)
			method(GetMapInfo)
			method(CreateCreature)
			method(CreateDynamicObject)
			method(GetDynamicObject)
			method(GetPlayer)
			method(GetInstanceID)
			method(GetInterface)
			method(HasPlayers)
			method(GetPlayerCount)
			method(SetWorldState)
			method(GetSqlIdCreature)
			method(GetSqlIdGameObject);
#undef method
#define meth(name) .method(#name, &MapScriptInterface::name)
		m	.class_<MapScriptInterface>("MapScriptInterface")
			meth(GetGameObjectNearestCoords)
			meth(GetCreatureNearestCoords)
			meth(GetPlayerNearestCoords)
			.method("SpawnCreature", (Creature *(MapScriptInterface::*)(uint32,float,float,float,float,bool,bool,uint32,uint32,uint32) )&MapScriptInterface::SpawnCreature)
			.method("SpawnGameObject", (GameObject*(MapScriptInterface::*)(uint32,float,float,float,float, bool, uint32, uint32, uint32) )&MapScriptInterface::SpawnGameObject);
#undef meth
#define prop(name) .property_ro(#name, &MapInfo::name)
		m	.class_<MapInfo>("MapInfo")
			prop(mapid)
			prop(screenid)
			prop(type)
			prop(playerlimit)
			prop(minlevel)
			prop(minlevel_heroic)
			prop(repopx)
			prop(repopy)
			prop(repopz)
			prop(repopmapid)
			prop(name)
			prop(flags)
			prop(cooldown)
			prop(required_quest)
			prop(required_item)
			prop(heroic_key_1)
			prop(heroic_key_2)
			.method("HasFlag", &MapInfo::HasFlag);
#undef prop

		luabridge::tdstack<MapMgr*>::push(lua_instance->lu, lua_instance->map);
		//set _G[MapMgr] = lua_instance->map, so we can auto grab a mapmgr pointer as long as we execute lua code from a valid mapmgr thread.
		lua_setglobal(lua_instance->lu, "MapMgr");
	}
}
