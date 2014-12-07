
#include "LUAEngine.h"

LuaInstance::LuaInstance(MapMgr* pMapMgr) : InstanceScript(pMapMgr), m_instanceId(pMapMgr->GetInstanceID()) , mgr_(pMapMgr) {}
LuaInstance::~LuaInstance()
{
	PLUA_INSTANCE ref = lua_instance.get();
	//assert(ref != NULL && ref->lu != NULL);
	if(ref != NULL)
		le::shutdownThread(GetInstance());
}

void LuaInstance::OnPlayerDeath(Player* pVictim, Unit* pKiller)
{
	NULL_BINDING_CHECK
	le::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_PLAYER_DEATH]);
	push_int(m_instanceId);
	push_unit(pVictim);
	push_unit(pKiller);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaInstance::OnPlayerEnter(Player* pPlayer)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_PLAYER_ENTER]);
	push_int(m_instanceId);
	push_unit(pPlayer);
	lua_engine::ExecuteLuaFunction(2);
	RELEASE_LOCK
}

void LuaInstance::OnAreaTrigger(Player* pPlayer, uint32 uAreaId)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_AREA_TRIGGER]);
	push_int(m_instanceId);
	push_unit(pPlayer);
	push_int(uAreaId);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaInstance::OnZoneChange(Player* pPlayer, uint32 uNewZone, uint32 uOldZone)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_ZONE_CHANGE]);
	push_int(m_instanceId);
	push_unit(pPlayer);
	push_int(uNewZone);
	push_int(uOldZone);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaInstance::OnCreatureDeath(Creature* pVictim, Unit* pKiller)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_CREATURE_DEATH]);
	push_int(m_instanceId);
	push_unit(pVictim);
	push_unit(pKiller);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaInstance::OnCreaturePushToWorld(Creature* pCreature)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_CREATURE_PUSH]);
	push_int(m_instanceId);
	push_unit(pCreature);
	lua_engine::ExecuteLuaFunction(2);
	RELEASE_LOCK
}

void LuaInstance::OnGameObjectActivate(GameObject* pGameObject, Player* pPlayer)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_GO_ACTIVATE]);
	push_int(m_instanceId);
	push_go(pGameObject);
	push_unit(pPlayer);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaInstance::OnGameObjectPushToWorld(GameObject* pGameObject)
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ON_GO_PUSH]);
	push_int(m_instanceId);
	push_go(pGameObject);
	lua_engine::ExecuteLuaFunction(2);
	RELEASE_LOCK
}

void LuaInstance::OnLoad()
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[INSTANCE_EVENT_ONLOAD]);
	push_int(m_instanceId);
	lua_engine::ExecuteLuaFunction(1);
	RELEASE_LOCK
}

void LuaInstance::Destroy()
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


namespace lua_engine
{
	InstanceScript* createluainstance(MapMgr* pMapMgr)
	{
		LuaInstance* pLua = NULL;


		PLUA_INSTANCE pstackInstance = lua_instance.get();
		if(pstackInstance == NULL)
		{
			pstackInstance = new LUA_INSTANCE;
			pstackInstance->lu = NULL;
			pstackInstance->map = pMapMgr;

			lua_instance = pstackInstance;
			//have it load the scripts
			le::restartThread(pMapMgr);
			//store it so we can keep track of it.
			le::activestates_lock.Acquire();
			le::activeStates.insert(pMapMgr);
			le::activestates_lock.Release();
		}

		//locate this instance's binding.
		li::ObjectBindingMap::iterator itr = pstackInstance->m_instanceBinding.find(pMapMgr->GetMapId());
		PObjectBinding bind = NULL;
		if(itr != pstackInstance->m_instanceBinding.end())
			bind = itr->second;
		pLua = new LuaInstance(pMapMgr);
		pLua->m_binding = bind;
		//Our scripts are loaded and bindings are set.
		return pLua;
	}

	void bindMapMethods(luabridge::module & m)
	{
		m	.class_<MapMgr>("MapMgr")
		.method(&MapMgr::GetMapId, "GetMapId", "getMapId", "getmapid", "getMapID", "GetMapID", NULL)  //wiki listed
		.method(&MapMgr::_GetObject, "GetObject", "getObject", "getobject", NULL)  //wiki listed
		.method(&MapMgr::CreateGameObject, "creategameobject", "CreateGameObject", "createGameObject", NULL)
		.method(&MapMgr::CreateAndSpawnGameObject, "createandspawngameobject", "CreateAndSpawnGameObject", "createAndSpawnGameObject", NULL)
		.method(&MapMgr::GetGameObject, "getGameObject", "GetGameObject", "getgameobject", NULL)  //wiki listed
		.method(&MapMgr::GetCreature, "getCreature", "getcreature", "GetCreature", NULL) //wiki listed
		.method(&MapMgr::GetMapInfo, "GetMapInfo", "getmapinfo", "getMapInfo", "getinfo", "GetInfo", "getInfo", NULL)  //wiki listed
		.method(&MapMgr::CreateCreature, "createCreature", "CreateCreature", "createcreature", NULL)
		.method(&MapMgr::CreateDynamicObject, "CreateDynamicObject", "createDynamicObject", "createdynamicobject", NULL)
		.method(&MapMgr::GetDynamicObject, "getdynamicobject", "GetDynamicObject", "getDynamicObject", NULL)
		.method(&MapMgr::GetPlayer, "getplayer", "getPlayer", "GetPlayer", NULL)  //wiki listed
		.method(&MapMgr::GetInstanceID, "getinstanceid", "getInstanceID", "GetInstanceID", NULL)  //wiki listed
		.method(&MapMgr::GetInterface, "GetInterface", "getInterface", "getinterface", NULL)
		.method(&MapMgr::HasPlayers, "hasPlayers", "HasPlayers", "hasplayers", NULL)
		.method(&MapMgr::GetPlayerCount, "getplayercount", "getPlayerCount", "GetPlayerCount", NULL)
		.method(&MapMgr::GetSqlIdCreature, "GetSqlIdCreature", "getsqlidcreature", "getSqlIdCreature", NULL)
		.method(&MapMgr::GetSqlIdGameObject, "GetSqlIdGameObject", "getsqlidgameobject", "getSqlIdGameObject", NULL)
		.method(&MapMgr::GetAreaID, "GetAreaID", "getAreaID", "getareaid", "GetAreaId", NULL)
		.method(&MapMgr::GetADTLandHeight, "GetADTLandHeight", "getADTLandHeight", "getadtlandheight", NULL)
		.method(&MapMgr::GetWorldStatesHandler, "GetWorldStatesHandler", "getWorldStatesHandler", "getworldstateshandler", NULL );

		m   .class_<WorldStatesHandler>("WorldStatesHandler")
		.method(&WorldStatesHandler::SetWorldStateForZone, "SetWorldStateForZone", "setWorldStateForZone", "setworldstateforzone", NULL );

		m	.class_<MapScriptInterface>("MapScriptInterface")
		.method(&MapScriptInterface::GetGameObjectNearestCoords, "GetGameObjectNearestCoords", "getGameObjectNearestCoords", "getgameobjectnearestcoords", NULL)
		.method(&MapScriptInterface::GetCreatureNearestCoords, "GetCreatureNearestCoords", "getCreatureNearestCoords", "getcreaturenearestcoords", NULL)
		.method(&MapScriptInterface::GetPlayerNearestCoords, "getPlayerNearestCoords", "GetPlayerNearestCoords", "getplayernearestcoords", NULL)
		.method((Creature * (MapScriptInterface::*)(uint32, float, float, float, float, bool, bool, uint32, uint32, uint32))&MapScriptInterface::SpawnCreature, "FullSpawnCreature", "fullSpawnCreature", "fullspawncreature", NULL)
		.method((GameObject * (MapScriptInterface::*)(uint32, float, float, float, float, bool, uint32, uint32, uint32))&MapScriptInterface::SpawnGameObject, "FullSpawnGameObject", "fullSpawnGameObject", "fullspawngameobject", NULL);

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
		prop(required_quest_1)
		prop(required_quest_2)
		prop(required_item)
		prop(heroic_key_1)
		prop(heroic_key_2)
		.method("HasFlag", &MapInfo::HasFlag);
#undef prop

		luabridge::tdstack<MapMgr*>::push(m.L, lua_instance.get()->map);
		//set _G[MapMgr] = lua_instance->map, so we can auto grab a mapmgr pointer as long as we execute lua code from a valid mapmgr thread.
		lua_setglobal(m.L, "MapMgr");
	}
}
