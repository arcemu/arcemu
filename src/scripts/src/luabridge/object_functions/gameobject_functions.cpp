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

#include "LUAEngine.h"
//include our custom go methods
#include "lua_gameobject.h"

LuaGameObject::LuaGameObject(GameObject* go) : GameObjectAIScript(go), m_binding(NULL) {}
LuaGameObject::~LuaGameObject() {}
void LuaGameObject::OnCreate()
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_CREATE]);
	push_go(_gameobject);
	lua_engine::ExecuteLuaFunction(1);

	RELEASE_LOCK
}

void LuaGameObject::OnSpawn()
{

	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_SPAWN]);
	push_go(_gameobject);
	lua_engine::ExecuteLuaFunction(1);

	RELEASE_LOCK
}

void LuaGameObject::OnDespawn()
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_DESPAWN]);
	push_go(_gameobject);
	lua_engine::ExecuteLuaFunction(1);
	RELEASE_LOCK
}

void LuaGameObject::OnLootTaken(Player* pLooter, ItemPrototype* pItemInfo)
{

	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_LOOT_TAKEN]);
	push_go(_gameobject);
	push_int(GAMEOBJECT_EVENT_ON_LOOT_TAKEN);
	push_player(pLooter);
	push_int(pItemInfo->ItemId);
	lua_engine::ExecuteLuaFunction(4);
	RELEASE_LOCK
}

void LuaGameObject::OnActivate(Player* pPlayer)
{
	NULL_BINDING_CHECK

	lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_USE]);
	push_go(_gameobject);
	push_int(GAMEOBJECT_EVENT_ON_USE);
	push_player(pPlayer);
	lua_engine::ExecuteLuaFunction(3);
	RELEASE_LOCK
}

void LuaGameObject::AIUpdate()
{
	NULL_BINDING_CHECK
	lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_AIUPDATE]);
	push_go(_gameobject);
	lua_engine::ExecuteLuaFunction(1);
	RELEASE_LOCK
}

void LuaGameObject::Destroy()
{
	sEventMgr.RemoveEvents(_gameobject, EVENT_LUA_GAMEOBJ_EVENTS);
	PLUA_INSTANCE ref = lua_instance.get();
	{
		li::GOInterfaceMap::iterator it;
		std::pair< li::GOInterfaceMap::iterator, li::GOInterfaceMap::iterator> interfaces = ref->m_goInterfaceMap.equal_range(_gameobject->GetEntry());
		for(; interfaces.first != interfaces.second;)
		{
			it = interfaces.first++;
			if(it->second != NULL && it->second == this)
				ref->m_goInterfaceMap.erase(it);
		}
	}
	//clean up any refs being used by this go.
	{
		std::pair<li::ObjectFRefMap::iterator, li::ObjectFRefMap::iterator> frefs = ref->m_goFRefs.equal_range(_gameobject->GetLowGUID());
		for(; frefs.first != frefs.second; ++frefs.first)
			cleanup_varparam(frefs.first->second, ref->lu);
		ref->m_goFRefs.erase(_gameobject->GetLowGUID());
	}
	delete this;
}


namespace lua_engine
{
	GameObjectAIScript* createluagameobject(GameObject* src)
	{
		LuaGameObject* script = NULL;
		PLUA_INSTANCE ref = lua_instance.get();
		if(src != NULL)
		{
			uint32 id = src->GetInfo()->ID;
			uint32 low_guid = src->GetLowGUID();
			li::ObjectBindingMap::iterator itr = ref->m_goBinding.find(id);
			PObjectBinding pBinding = (itr != ref->m_goBinding.end()) ? itr->second : NULL;
			if(pBinding != NULL)
			{
				script = new LuaGameObject(src);
				ref->m_goInterfaceMap.insert(make_pair(id, script));
				script->m_binding = pBinding;
			}
		}
		return script;
	}

	void bindGameobjectMethods(luabridge::module & m)
	{
		m	.class_decl<lua_go>("GameObject");
		m	.subclass<GameObject, Object>("GameObject")
		.method(&GameObject::HasQuests, "HasQuests", "hasQuests", "hasquests", NULL)
		.method(&GameObject::NumOfQuests, "NumQuests", "numofquests", "NumOfQuests", NULL)
		//.method("Close", &GameObject::EventCloseDoor)
		.method(&GameObject::Despawn, "Despawn", "despawn", NULL)
		.method(&GameObject::ExpireAndDelete, "ExpireAndDelete", "expireanddelete", "expirendelete",  NULL)
		.method(&GameObject::isQuestGiver, "IsQuestGiver", "isQuestGiver", "isquestgiver", NULL)
		//.property_ro("summoner", &lua_go::m_summoner)
		.method(&GameObject::RemoveFromWorld, "RemoveFromWorld", "removeFromWorld", "removefromworld", NULL)
		.method(&GameObject::GetScript, "GetScript", "getScript", "getscript", NULL)
		.method(&lua_go::RegisterScriptEngineFunction, "RegisterEvent", "registerEvent", "registerevent", NULL)
		.method(&lua_go::RemoveScriptEngineEvents, "RemoveEvents", "removeEvents", "removeevents", "removevents", NULL)
		.method(&GameObject::GetInfo, "GetInfo", "getInfo", "getinfo", NULL)
		.method(&GameObject::SaveToDB, "SaveToDB", "saveToDB", "savetodb", NULL)
		.method(&lua_go::AddLoot, "AddLoot", "addLoot", "addloot", NULL)
		.method(&lua_go::GetSpawnId, "GetSpawnId", "GetSQL_id", "GetSpawnID", "getSpawnID", "getspawnid", NULL);

		//Gameobject ai script, used for registering ai update.
		m	.class_<GameObjectAIScript>("GameObjectAIScript")
		.method(&GameObjectAIScript::RegisterAIUpdateEvent, "RegisterAIUpdateEvent", "registerAIUpdate", "registeraiupdate", NULL)
		.method(&GameObjectAIScript::ModifyAIUpdateEvent, "ModifyAIUpdateEvent", "ModifyAIUpdate", "modifyAIUpdate", "modifyaiupdate", NULL)
		.method(&GameObjectAIScript::RemoveAIUpdateEvent, "RemoveAIUpdateEvent", "RemoveAIUpdate", "removeAIUpdate", "removeaiupdate", NULL);

		m	.class_<GameObjectInfo>("GameObjectInfo")
		.property_ro("ID", &GameObjectInfo::ID)
		.property_ro("Type", &GameObjectInfo::Type)
		.property_ro("DisplayID", &GameObjectInfo::DisplayID)
		.property_ro("Name", &GameObjectInfo::Name)
		.property_ro("SpellFocus", &GameObjectInfo::SpellFocus);
	}
}
