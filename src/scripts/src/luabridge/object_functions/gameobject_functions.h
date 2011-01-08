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

#include "StdAfx.h"
#include "LUAEngine.h"
//include our custom go methods
#include "lua_gameobject.h"


class LuaGameObject : public GameObjectAIScript
{
public:
	LuaGameObject(GameObject * go) : GameObjectAIScript(go), m_binding(NULL) {}
	~LuaGameObject() {}
	ARCEMU_FORCEINLINE GameObject * getGO() { return _gameobject; }
	void OnCreate()
	{
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_CREATE]);
		push_go(_gameobject);
		lua_engine::ExecuteLuaFunction(1);

		RELEASE_LOCK
	}
	void OnSpawn()
	{

		NULL_BINDING_CHECK

		lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_SPAWN]);
		push_go(_gameobject);
		lua_engine::ExecuteLuaFunction(1);

		RELEASE_LOCK
	}
	void OnDespawn()
	{
		NULL_BINDING_CHECK

		lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_DESPAWN]);
		push_go(_gameobject);
		lua_engine::ExecuteLuaFunction(1);
		RELEASE_LOCK
	}
	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{

		NULL_BINDING_CHECK

		lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_LOOT_TAKEN]);
		push_go(_gameobject);
		push_int(GAMEOBJECT_EVENT_ON_LOOT_TAKEN);
		push_unit(pLooter);
		push_int(pItemInfo->ItemId);
		lua_engine::ExecuteLuaFunction(4);
		RELEASE_LOCK
	}
	void OnActivate(Player * pPlayer)
	{
		NULL_BINDING_CHECK

		lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_ON_USE]);
		push_go(_gameobject);
		push_int(GAMEOBJECT_EVENT_ON_USE);
		push_unit(pPlayer);
		lua_engine::ExecuteLuaFunction(3);
		RELEASE_LOCK
	}

	void AIUpdate()
	{
		NULL_BINDING_CHECK
		lua_engine::BeginLuaFunctionCall(m_binding->refs[GAMEOBJECT_EVENT_AIUPDATE]);
		push_go(_gameobject);
		lua_engine::ExecuteLuaFunction(1);
		RELEASE_LOCK
	}
	void Destroy ()
	{
		{
			li::GOInterfaceMap::iterator it;
			li::GOInterfaceMap::iterator itr = lua_instance->m_goInterfaceMap.find(_gameobject->GetEntry() );
			li::GOInterfaceMap::iterator itend = lua_instance->m_goInterfaceMap.upper_bound(_gameobject->GetEntry() );
			while(itr != itend)
			{
				it = itr++;
				if(it->second != NULL && it->second == this)
					lua_instance->m_goInterfaceMap.erase(it);
			}
		}
		//clean up any refs being used by this go.
		{
			li::ObjectFRefMap::iterator itr = lua_instance->m_goFRefs.find(_gameobject->GetLowGUID() );
			li::ObjectFRefMap::iterator itend = lua_instance->m_goFRefs.upper_bound(_gameobject->GetLowGUID() );
			li::ObjectFRefMap::iterator it;
			while(itr != itend)
			{
				it = itr++;
				cleanup_varparam(it->second,lua_state);
				lua_instance->m_goFRefs.erase(it);
			}
		}
		delete this;
	}
	PObjectBinding m_binding;
};
namespace lua_engine
{
	GameObjectAIScript * createluagameobject(GameObject * src)
	{
		LuaGameObject * script = NULL;
		if(src != NULL) 
		{
			uint32 id = src->GetInfo()->ID;
			uint32 low_guid = src->GetLowGUID();
			li::ObjectBindingMap::iterator itr = lua_instance->m_goBinding.find(id);
			PObjectBinding pBinding = (itr != lua_instance->m_goBinding.end() ) ? itr->second : NULL;
			if( pBinding != NULL) 
			{
				li::GOInterfaceMap::iterator itr = lua_instance->m_goInterfaceMap.find(id), itend = lua_instance->m_goInterfaceMap.upper_bound(id);
				for(;itr != itend; ++itr)
				{
					if(itr->second == NULL)
					{
						script = itr->second = new LuaGameObject(src);
						break;
					}
					if(itr->second != NULL && itr->second->getGO() != NULL && itr->second->getGO()->GetLowGUID() == low_guid)
					{
						script = itr->second;
						break;
					}
				}
				if(script == NULL)
				{
					script = new LuaGameObject(src);
					lua_instance->m_goInterfaceMap.insert(make_pair(id,script));
				}
				script->m_binding = pBinding;
			}
		}
		return script;
	}

	void bindGameobjectMethods(luabridge::module & m )
	{
		m	.class_decl<GameObject>("GameObject");
		m	.subclass<lua_go, Object>("GameObject")
			.method("HasQuests", &GameObject::HasQuests)
			.method("NumQuests", &GameObject::NumOfQuests)
			.method("Close", &GameObject::EventCloseDoor)
			.method("Despawn", &GameObject::Despawn)	
			.method("ExpireAndDelete", &GameObject::ExpireAndDelete)
			.method("IsQuestGiver", &GameObject::isQuestGiver)
			//.property_ro("summoner", &lua_go::m_summoner)
			.method("RemoveFromWorld", &GameObject::RemoveFromWorld)
			.method("GetScript", &GameObject::GetScript)
			.method("RegisterEvent", &lua_go::RegisterScriptEngineFunction)
			.method("RemoveEvents", &lua_go::RemoveScriptEngineEvents)
			.method("GetInfo", &GameObject::GetInfo);

		//Gameobject ai script, used for registering ai update.
		m	.class_<GameObjectAIScript>("GameObjectAIScript")
			.method("RegisterAIUpdate", &GameObjectAIScript::RegisterAIUpdateEvent)
			.method("ModifyAIUpdate", &GameObjectAIScript::ModifyAIUpdateEvent)
			.method("RemoveAIUpdate", &GameObjectAIScript::RemoveAIUpdateEvent);

		m	.class_<GameObjectInfo>("GameObjectInfo")
			.property_ro("ID", &GameObjectInfo::ID)
			.property_ro("Type", &GameObjectInfo::Type)
			.property_ro("DisplayID", &GameObjectInfo::DisplayID)
			.property_ro("name", &GameObjectInfo::Name)
			.property_ro("SpellFocus", &GameObjectInfo::SpellFocus);
	}
}
