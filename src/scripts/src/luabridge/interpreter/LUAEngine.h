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

extern "C"
{
	// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "../../lualib/lua.h"
#include "../../lualib/lauxlib.h"
#include "../../lualib/lualib.h"
};

#include "StdAfx.h"
#include "lua_defs.h"
#include "class_decls.h"
#include "lua_instance.h"

#include <sys/stat.h>
#include <sys/types.h>

#include "../luabridge_src/luabridge.hpp"

/*	A thread local storage lua_instance that will only get initialized for maps that attempt to run lua scripts. */
/*	A lua_State that acts like a compiler for lua scripts, it loads scripts during engine startup to detect and report any errors */

#define lua_state (lua_instance.get() )->lu



//Used for checking headers in scripts.
#define ARCLUA_PREFIX "--?!"	//begins with a comment notation so that Lua will ignore our header.
#define ARCLUA_PREFIX_SIZE 4
#define ARCLUA_SUFFIX '\x0A'	//line feed char
#define ARCLUA_SUFFIX_SIZE 1
#define GET_LOCK
#define RELEASE_LOCK
#define NULL_BINDING_CHECK GET_LOCK; if(m_binding == NULL) return;
//don't execute lua code from invalid lua states, might prevent some server hooks from firing lua code.
#define CHECKVALIDSTATE { if(lua_instance.get() == NULL) return; }

namespace lua_engine
{
	extern CreatureAIScript* createluacreature(Creature*);
	extern GameObjectAIScript* createluagameobject(GameObject*);
	extern QuestScript* createluaquest(uint32);
	extern Arcemu::Gossip::Script* createunitgossipInterface(uint32 id);
	extern Arcemu::Gossip::Script* createitemgossipInterface(uint32 id);
	extern Arcemu::Gossip::Script* creategogossipInterface(uint32 id);
	extern InstanceScript* createluainstance(MapMgr*);

	typedef HM_NAMESPACE::hash_map<std::string, PLUA_SCRIPT> LuaScriptData;
	typedef std::set<uint32> hooked_dummySpells;
	typedef std::set<MapMgr*> ActiveStates;

	//static Object binding maps.
	//static InstanceInterfaceMap m_instanceInterfaceMap;
	//All scripts are read and stored here, then when new states are created, they load the script data contained here.
	extern LuaScriptData compiled_scripts;
	extern hooked_dummySpells _hooked_dummies;
	//Store thread ids and whether they need to restart.
	extern ActiveStates activeStates;
	extern FastMutex activestates_lock;
	//Locked when we are accessing cached script data
	extern FastMutex scriptLock;


	static void startupEngine();
	static void loadScripts(PLUA_INSTANCE);
	extern bool loadScript(const char*);
	static void loadScripts();
	static void scriptload_searchdir(char* Dirname, deque<string>&);
	extern void restartEngine();
	//static void shutdownEngine();
	extern void restartThread(MapMgr*);
	extern void shutdownThread(MapMgr*);

	static void unload_resources(PLUA_INSTANCE);
	extern void report(lua_State*);
	//static void loadCompiler(PLUA_INSTANCE);
	static void loadState(PLUA_INSTANCE);
	//a lua reader that simply passes lua script data.
	static const char* readScript(lua_State*, void*, size_t*);
	//a lua writer that dumps scripts in binary form
	//static int dumpScript(lua_State *, const void *, size_t, void*);

	//static void dumpScripts2HDD();
	//special header parsing
	static void parseHeader(PLUA_SCRIPT);
	//c to lua and lua to c methods
	extern void BeginLuaFunctionCall(lua_function ref);
	extern bool ExecuteLuaFunction(int = 0, int = 0, variadic_parameter** = NULL, bool = false);
	extern void ExecuteLuaFunction(variadic_parameter*);
	extern void EndLuaFunctionCall(int results = 0);


	//Binding methods, implemented in other files to keep things neat.
	extern void bindRegisterMethods(luabridge::module &);
	extern void bindObjectMethods(luabridge::module &);
	extern void bindUnitMethods(luabridge::module &);
	extern void bindCreatureMethods(luabridge::module &);
	extern void bindPlayerMethods(luabridge::module &);
	extern void bindQuestMethods(luabridge::module &);
	extern void bindGameobjectMethods(luabridge::module &);
	extern void bindSQLMethods(luabridge::module &);
	extern void bindSpellMethods(luabridge::module &);
	extern void bindPacketMethods(luabridge::module &);
	extern void bindAuraMethods(luabridge::module &);
	extern void bindTaxiMethods(luabridge::module &);
	extern void bindItemMethods(luabridge::module &);
	extern void bindGlobalMethods(luabridge::module &);
	extern void bindMapMethods(luabridge::module &);


//push macros
#define push_obj(type, obj) luabridge::tdstack<type*>::push(lua_state, obj )
#define push_function(ref)  luabridge::tdstack<lua_function>::push(lua_state, (ref))
#define push_coroutine(co) luabridge::tdstack<lua_thread>::push(lua_state, co);
#define push_unit(obj) push_obj(Unit, TO_UNIT(obj))
#define push_wobj(obj) push_obj(Object, TO_OBJECT(obj) )
#define push_go(obj) push_obj(GameObject, TO_GAMEOBJECT(obj))
#define push_item(obj) push_obj(Item, TO_ITEM(obj))
#define push_player(obj) push_obj(Player, TO_PLAYER(obj) )
#define push_creature(obj) push_obj(Creature, TO_CREATURE(obj) )
#define push_packet(obj) push_obj(luapacket, (obj))
#define push_sqlresult(obj) push_obj(LuaResult, static_cast<LuaResult>(obj) )
#define push_sqlfield(obj) push_obj(Field, (obj))
#define push_taxi(obj) push_obj(TaxiPath, (obj))
#define push_taxinode(obj) push_obj(luataxinode, static_cast<luataxinode>(obj) )
#define push_spell(obj) push_obj(Spell, (obj))
#define push_aura(obj) push_obj(Aura, (obj) )
#define push_bool(bewl) luabridge::tdstack<bool>::push(lua_state, (bewl) )
#define push_int(integer) luabridge::tdstack<int>::push(lua_state, (integer) )
#define push_nil() luabridge::tdstack<void>::push(lua_state)
#define push_str(cstr) luabridge::tdstack<const char*>::push(lua_state, (cstr) )
#define push_float(flewt) luabridge::tdstack<float>::push(lua_state, (flewt) )
#define push_double(doublee) luabridge::tdstack<double>::push(lua_state, (doublee))
#define push_varargs(varargs) luabridge::tdstack<variadic_parameter*>::push(lua_state, (varargs) )

}

#define le lua_engine
#define li LUA_INSTANCE

//	We can no longer insert double pointers so we are safe here.
#define RegisterHook(evt, _func) { if(!m_scriptMgr->has_hook(evt, _func) ) m_scriptMgr->register_hook( (ServerHookEvents)(evt), (_func) ); }

extern void DestroyAllLuaEvents(PLUA_INSTANCE instance);
extern bool LuaHookOnNewCharacter(uint32 Race, uint32 Class, WorldSession*, const char* Name);
extern void LuaHookOnKillPlayer(Player* pPlayer, Player* pVictim);
extern void LuaHookOnFirstEnterWorld(Player* pPlayer);
extern void LuaHookOnEnterWorld(Player* pPlayer);
extern void LuaHookOnGuildJoin(Player* pPlayer, Guild* pGuild);
extern void LuaHookOnDeath(Player* pPlayer);
extern bool LuaHookOnRepop(Player* pPlayer);
extern void LuaHookOnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit);
extern void LuaHookOnEnterCombat(Player* pPlayer, Unit* pTarget);
extern bool LuaHookOnCastSpell(Player* pPlayer, SpellEntry* pSpell, Spell* spell);
extern void LuaHookOnTick();
extern bool LuaHookOnLogoutRequest(Player* pPlayer);
extern void LuaHookOnLogout(Player* pPlayer);
extern void LuaHookOnQuestAccept(Player* pPlayer, Quest* pQuest, Object* pQuestGiver);
extern void LuaHookOnZone(Player* pPlayer, uint32 Zone, uint32 oldZone);
extern bool LuaHookOnChat(Player* pPlayer, uint32 Type, uint32 Lang, const char* Message, const char* Misc);
extern void LuaHookOnLoot(Player* pPlayer, Unit* pTarget, uint32 Money, uint32 ItemId);
extern void LuaHookOnGuildCreate(Player* pLeader, Guild* pGuild);
extern void LuaHookOnFullLogin(Player* pPlayer);
extern void LuaHookOnCharacterCreate(Player* pPlayer);
extern void LuaHookOnQuestCancelled(Player* pPlayer, Quest* pQuest);
extern void LuaHookOnQuestFinished(Player* pPlayer, Quest* pQuest, Object* pQuestGiver);
extern void LuaHookOnHonorableKill(Player* pPlayer, Player* pKilled);
extern void LuaHookOnArenaFinish(Player* pPlayer, ArenaTeam* pTeam, bool victory, bool rated);
extern void LuaHookOnObjectLoot(Player* pPlayer, Object* pTarget, uint32 Money, uint32 ItemId);
extern void LuaHookOnAreaTrigger(Player* pPlayer, uint32 areaTrigger);
extern void LuaHookOnPostLevelUp(Player* pPlayer);
extern bool LuaHookOnPreUnitDie(Unit* Killer, Unit* Victim);
extern void LuaHookOnAdvanceSkillLine(Player* pPlayer, uint32 SkillLine, uint32 Current);
extern void LuaHookOnDuelFinished(Player* pWinner, Player* pLoser);
extern void LuaHookOnAuraRemove(Aura* aura);
extern bool LuaHookOnResurrect(Player* pPlayer);
extern bool LuaOnDummySpell(uint32 effectIndex, Spell* pSpell);



