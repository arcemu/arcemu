#include "LUAEngine.h"

//all of these run similarly, they execute OnServerHook for all the functions in their respective event's list.
bool LuaHookOnNewCharacter(uint32 Race, uint32 Class, WorldSession*, const char* Name)
{
	//Disabled hook.
	PLUA_INSTANCE ref = lua_instance.get();
	if(ref == NULL) return true;
	GET_LOCK;
	bool result = true;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_NEW_CHARACTER);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_NEW_CHARACTER);
		push_str(Name);
		push_int(Race);
		push_int(Class);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(4, 1, &params, true))
		{
			//find a boolean return argument that is explicitly false.
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}
	return result;
}

void LuaHookOnKillPlayer(Player* pPlayer, Player* pVictim)
{
	CHECKVALIDSTATE
	PLUA_INSTANCE ref = lua_instance.get();
	GET_LOCK;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_KILL_PLAYER);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_KILL_PLAYER);
		push_player(pPlayer);
		push_unit(pVictim);
		lua_engine::ExecuteLuaFunction(3);
	}

}

void LuaHookOnFirstEnterWorld(Player* pPlayer)
{
	CHECKVALIDSTATE
	PLUA_INSTANCE ref = lua_instance.get();
	GET_LOCK;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}

}

void LuaHookOnEnterWorld(Player* pPlayer)
{
	CHECKVALIDSTATE
	PLUA_INSTANCE ref = lua_instance.get();
	GET_LOCK;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_ENTER_WORLD);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_ENTER_WORLD);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}

}

void LuaHookOnGuildJoin(Player* pPlayer, Guild* pGuild)
{
	CHECKVALIDSTATE
	PLUA_INSTANCE ref = lua_instance.get();
	GET_LOCK;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_GUILD_JOIN);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_GUILD_JOIN);
		push_player(pPlayer);
		push_str(pGuild->GetGuildName());
		lua_engine::ExecuteLuaFunction(3);
	}

}

void LuaHookOnDeath(Player* pPlayer)
{
	CHECKVALIDSTATE
	PLUA_INSTANCE ref = lua_instance.get();
	GET_LOCK;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_DEATH);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_DEATH);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}

}

bool LuaHookOnRepop(Player* pPlayer)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	bool result = true;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_REPOP);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_REPOP);
		push_player(pPlayer);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(2, 1, &params, true))
		{
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}

	return result;
}

void LuaHookOnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_EMOTE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_EMOTE);
		push_player(pPlayer);
		push_unit(pUnit);
		push_int(Emote);
		lua_engine::ExecuteLuaFunction(4);
	}

}

void LuaHookOnEnterCombat(Player* pPlayer, Unit* pTarget)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_ENTER_COMBAT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_ENTER_COMBAT);
		push_player(pPlayer);
		push_unit(pTarget);
		lua_engine::ExecuteLuaFunction(3);
	}

}

bool LuaHookOnCastSpell(Player* pPlayer, SpellEntry* pSpell, Spell* spell)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	bool result = true;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_CAST_SPELL);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_CAST_SPELL);
		push_player(pPlayer);
		push_int(pSpell->Id);
		push_spell(spell);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(4, 1, &params, true))
		{
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}

	return result;
}

void LuaHookOnTick()
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_TICK);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		lua_engine::ExecuteLuaFunction();
	}
}

bool LuaHookOnLogoutRequest(Player* pPlayer)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	bool result = true;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST);
		push_player(pPlayer);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(2, 1, &params, true))
		{
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}

	return result;
}

void LuaHookOnLogout(Player* pPlayer)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_LOGOUT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_LOGOUT);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}

}

void LuaHookOnQuestAccept(Player* pPlayer, Quest* pQuest, Object* pQuestGiver)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT);
		push_player(pPlayer);
		push_int(pQuest->id);
		if(!pQuestGiver)
			push_nil();
		else if(pQuestGiver->IsUnit())
			push_unit(pQuestGiver);
		else if(pQuestGiver->IsGameObject())
			push_go(pQuestGiver);
		else if(pQuestGiver->GetTypeId() == TYPEID_ITEM)
			push_item(pQuestGiver);
		else
			push_nil();
		lua_engine::ExecuteLuaFunction(4);
	}

}

void LuaHookOnZone(Player* pPlayer, uint32 Zone, uint32 oldZone)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_ZONE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_ZONE);
		push_player(pPlayer);
		push_int(Zone);
		push_int(oldZone);
		lua_engine::ExecuteLuaFunction(4);
	}

}

bool LuaHookOnChat(Player* pPlayer, uint32 Type, uint32 Lang, const char* Message, const char* Misc)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	bool result = true;
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_CHAT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_CHAT);
		push_player(pPlayer);
		push_str(Message);
		push_int(Type);
		push_int(Lang);
		push_str(Misc);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(6, 1, &params, true))
		{
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}

	return result;
}

void LuaHookOnLoot(Player* pPlayer, Unit* pTarget, uint32 Money, uint32 ItemId)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_LOOT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_LOOT);
		push_player(pPlayer);
		push_unit(pTarget);
		push_int(Money);
		push_int(ItemId);
		lua_engine::ExecuteLuaFunction(5);
	}

}

void LuaHookOnGuildCreate(Player* pLeader, Guild* pGuild)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_GUILD_CREATE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_GUILD_CREATE);
		push_player(pLeader);
		push_str(pGuild->GetGuildName());
		lua_engine::ExecuteLuaFunction(3);
	}

}

void LuaHookOnFullLogin(Player* pPlayer)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair<li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_FULL_LOGIN);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_FULL_LOGIN);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}
}

void LuaHookOnCharacterCreate(Player* pPlayer)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_CHARACTER_CREATE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_CHARACTER_CREATE);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}

}

void LuaHookOnQuestCancelled(Player* pPlayer, Quest* pQuest)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_QUEST_CANCELLED);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_QUEST_CANCELLED);
		push_player(pPlayer);
		push_int(pQuest->id);
		lua_engine::ExecuteLuaFunction(3);
	}

}

void LuaHookOnQuestFinished(Player* pPlayer, Quest* pQuest, Object* pQuestGiver)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_QUEST_FINISHED);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_QUEST_FINISHED);
		push_player(pPlayer);
		push_int(pQuest->id);
		if(!pQuestGiver)
			push_nil();
		else if(pQuestGiver->IsUnit())
			push_unit(pQuestGiver);
		else if(pQuestGiver->IsGameObject())
			push_go(pQuestGiver);
		else if(pQuestGiver->GetTypeId() == TYPEID_ITEM)
			push_item(pQuestGiver);
		else
			push_nil();
		lua_engine::ExecuteLuaFunction(4);
	}

}

void LuaHookOnHonorableKill(Player* pPlayer, Player* pKilled)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_HONORABLE_KILL);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_HONORABLE_KILL);
		push_player(pPlayer);
		push_player(pKilled);
		lua_engine::ExecuteLuaFunction(3);
	}
}

void LuaHookOnArenaFinish(Player* pPlayer, ArenaTeam* pTeam, bool victory, bool rated)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_HONORABLE_KILL);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_ARENA_FINISH);
		push_player(pPlayer);
		push_str(pTeam->m_name.c_str());
		push_bool(victory);
		push_bool(rated);
		lua_engine::ExecuteLuaFunction(5);
	}
}

void LuaHookOnObjectLoot(Player* pPlayer, Object* pTarget, uint32 Money, uint32 ItemId)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_OBJECTLOOT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_OBJECTLOOT);
		push_player(pPlayer);
		push_wobj(pTarget);
		push_int(Money);
		push_int(ItemId);
		lua_engine::ExecuteLuaFunction(5);
	}

}

void LuaHookOnAreaTrigger(Player* pPlayer, uint32 areaTrigger)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_AREATRIGGER);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_AREATRIGGER);
		push_player(pPlayer);
		push_int(areaTrigger);
		lua_engine::ExecuteLuaFunction(3);
	}

}

void LuaHookOnPostLevelUp(Player* pPlayer)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_POST_LEVELUP);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_POST_LEVELUP);
		push_player(pPlayer);
		lua_engine::ExecuteLuaFunction(2);
	}

}

bool LuaHookOnPreUnitDie(Unit* Killer, Unit* Victim)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	bool result = true;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_PRE_DIE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_PRE_DIE);
		push_unit(Killer);
		push_unit(Victim);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(3, 1, &params, true))
		{
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}

	return result;
}

void LuaHookOnAdvanceSkillLine(Player* pPlayer, uint32 SkillLine, uint32 Current)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE);
		push_player(pPlayer);
		push_int(SkillLine);
		push_int(Current);
		lua_engine::ExecuteLuaFunction(4);
	}

}

void LuaHookOnDuelFinished(Player* pWinner, Player* pLoser)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_DUEL_FINISHED);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_DUEL_FINISHED);
		push_player(pWinner);
		push_player(pLoser);
		lua_engine::ExecuteLuaFunction(3);
	}

}

void LuaHookOnAuraRemove(Aura* aura)
{
	CHECKVALIDSTATE
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_AURA_REMOVE);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_AURA_REMOVE);
		push_aura(aura);
		lua_engine::ExecuteLuaFunction(2);
	}

}

bool LuaHookOnResurrect(Player* pPlayer)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	bool result = true;
	PLUA_INSTANCE ref = lua_instance.get();
	std::pair< li::HookFRefMap::iterator, li::HookFRefMap::iterator> hooks = ref->m_hooks.equal_range(SERVER_HOOK_EVENT_ON_RESURRECT);
	for(; hooks.first != hooks.second; ++hooks.first)
	{
		lua_engine::BeginLuaFunctionCall(hooks.first->second);
		push_int(SERVER_HOOK_EVENT_ON_RESURRECT);
		push_player(pPlayer);
		variadic_parameter* params = NULL;
		if(lua_engine::ExecuteLuaFunction(2, 1, &params, true))
		{
			if(params != NULL && params->head_node->type == LUA_TBOOLEAN && params->head_node->val.bewl == 0)
				result = false;
			cleanup_varparam(params, ref->lu);
			lua_engine::EndLuaFunctionCall(1);
		}
	}

	return result;
}

bool LuaOnDummySpell(uint32 effectIndex, Spell* pSpell)
{
	if(lua_instance == NULL) return true;
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	li::SpellFRefMap::iterator itr = ref->m_dummySpells.find(pSpell->GetProto()->Id);
	if(itr != ref->m_dummySpells.end())
	{
		lua_engine::BeginLuaFunctionCall((lua_function)itr->second->ref);
		push_int(effectIndex);
		push_spell(pSpell);
		push_varargs(itr->second->params);
		lua_engine::ExecuteLuaFunction(2);
	}
	return true;
}
