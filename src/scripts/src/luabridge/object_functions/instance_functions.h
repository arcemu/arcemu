#pragma once
#include "LUAEngine.h"

class LuaInstance : public InstanceScript
{
public:
	LuaInstance( MapMgr* pMapMgr ) : InstanceScript( pMapMgr ), m_instanceId( pMapMgr->GetInstanceID() ) 
	{
	}
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
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_DESTROY]);
		push_int(m_instanceId);
		lua_engine::ExecuteLuaFunction(1);
		RELEASE_LOCK
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
		uint32 id = pMapMgr->GetMapId();
		//Log.Notice("LuaEngine", "Spawning new lua state for map (%u) instance id (%u).",pMapMgr->GetMapId(), pMapMgr->GetInstanceID() );
		lua_instance = new LUA_INSTANCE;
		lua_state = lua_open();
		lua_instance->map = pMapMgr;
		le::loadState(lua_instance);
		LuaGuard guard(le::scriptLock);
		le::loadScripts(lua_state);
		LuaGuard guard_(le::activestates_lock);
		le::activeStates.insert(pMapMgr);

		li::ObjectBindingMap::iterator itr = lua_instance->m_instanceBinding.find(id);
		PObjectBinding bind = (itr != lua_instance->m_instanceBinding.end(id) ) ? itr->second : NULL;
		pLua = new LuaInstance(pMapMgr);
		pLua->m_binding = bind;
		return pLua;
	}

	void bindMapMethods(luabridge::module & m)
	{

	}
}