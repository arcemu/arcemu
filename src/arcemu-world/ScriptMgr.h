/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#define SCRIPT_MODULE void*
#define ADD_CREATURE_FACTORY_FUNCTION(cl) static CreatureAIScript * Create(Creature * c) { return new cl(c); }
#define ADD_INSTANCE_FACTORY_FUNCTION( ClassName ) static InstanceScript* Create( MapMgr* pMapMgr ) { return new ClassName( pMapMgr ); }; 
#define ADD_GAMEOBJECT_FACTORY_FUNCTION( ClassName ) static GameObjectAIScript* Create(GameObject* GO) { return new ClassName(GO); }; 

#ifndef WIN32
#include <dlfcn.h>
#endif

class Channel;
class Guild;
struct Quest;
enum ServerHookEvents
{
	SERVER_HOOK_EVENT_ON_NEW_CHARACTER		= 1,
	SERVER_HOOK_EVENT_ON_KILL_PLAYER		= 2,
	SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD	= 3,
	SERVER_HOOK_EVENT_ON_ENTER_WORLD		= 4,
	SERVER_HOOK_EVENT_ON_GUILD_JOIN			= 5,
	SERVER_HOOK_EVENT_ON_DEATH				= 6,
	SERVER_HOOK_EVENT_ON_REPOP				= 7,
	SERVER_HOOK_EVENT_ON_EMOTE				= 8,
	SERVER_HOOK_EVENT_ON_ENTER_COMBAT		= 9,
	SERVER_HOOK_EVENT_ON_CAST_SPELL			= 10,
	SERVER_HOOK_EVENT_ON_TICK				= 11,
	SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST		= 12,
	SERVER_HOOK_EVENT_ON_LOGOUT				= 13,
	SERVER_HOOK_EVENT_ON_QUEST_ACCEPT		= 14,
	SERVER_HOOK_EVENT_ON_ZONE				= 15,
	SERVER_HOOK_EVENT_ON_CHAT				= 16,
	SERVER_HOOK_EVENT_ON_LOOT				= 17,
	SERVER_HOOK_EVENT_ON_GUILD_CREATE		= 18,
	SERVER_HOOK_EVENT_ON_FULL_LOGIN			= 19,
	SERVER_HOOK_EVENT_ON_CHARACTER_CREATE	= 20,
	SERVER_HOOK_EVENT_ON_QUEST_CANCELLED	= 21,
	SERVER_HOOK_EVENT_ON_QUEST_FINISHED		= 22,
	SERVER_HOOK_EVENT_ON_HONORABLE_KILL		= 23,
	SERVER_HOOK_EVENT_ON_ARENA_FINISH		= 24,
	SERVER_HOOK_EVENT_ON_OBJECTLOOT			= 25,
	SERVER_HOOK_EVENT_ON_AREATRIGGER		= 26,
	SERVER_HOOK_EVENT_ON_POST_LEVELUP       = 27,
	SERVER_HOOK_EVENT_ON_PRE_DIE	        = 28,	//general unit die, not only based on players
	SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE  = 29,
    SERVER_HOOK_EVENT_ON_DUEL_FINISHED      = 30,
	SERVER_HOOK_EVENT_ON_AURA_REMOVE		= 31,
	SERVER_HOOK_EVENT_ON_RESURRECT		= 32,

	NUM_SERVER_HOOKS,
};

enum ScriptTypes
{
	SCRIPT_TYPE_MISC						= 0x01,
	SCRIPT_TYPE_INFODUMPER					= 0x02,
	SCRIPT_TYPE_SCRIPT_ENGINE				= 0x20,
	SCRIPT_TYPE_SCRIPT_ENGINE_LUA			= 0x21,
};


/* Hook typedefs */
typedef bool(*tOnNewCharacter)(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
typedef void(*tOnKillPlayer)(Player * pPlayer, Player * pVictim);
typedef void(*tOCharacterCreate)(Player * pPlayer);
typedef void(*tOnFirstEnterWorld)(Player * pPlayer);
typedef void(*tOnEnterWorld)(Player * pPlayer);
typedef void(*tOnGuildCreate)(Player * pLeader, Guild * pGuild);
typedef void(*tOnGuildJoin)(Player * pPlayer, Guild * pGuild);
typedef void(*tOnDeath)(Player * pPlayer);
typedef bool(*tOnRepop)(Player * pPlayer);
typedef void(*tOnEmote)(Player * pPlayer, uint32 Emote, Unit * pUnit);
typedef void(*tOnEnterCombat)(Player * pPlayer, Unit * pTarget);
typedef bool(*tOnCastSpell)(Player * pPlayer, SpellEntry * pSpell, Spell * spell);
typedef void(*tOnTick)();
typedef bool(*tOnLogoutRequest)(Player * pPlayer);
typedef void(*tOnLogout)(Player * pPlayer);
typedef void(*tOnQuestAccept)(Player * pPlayer, Quest * pQuest, Object * pQuestGiver);
typedef void(*tOnZone)(Player * pPlayer, uint32 Zone, uint32 oldzone);
typedef bool(*tOnChat)(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc);
typedef void(*tOnLoot)(Player * pPlayer, Unit * pTarget, uint32 Money, uint32 ItemId);
typedef bool(*ItemScript)(Item * pItem, Player * pPlayer);
typedef void(*tOnQuestCancel)(Player * pPlayer, Quest * pQuest);
typedef void(*tOnQuestFinished)(Player * pPlayer, Quest * pQuest, Object * pQuestGiver);
typedef void(*tOnHonorableKill)(Player * pPlayer, Player * pKilled);
typedef void(*tOnArenaFinish)(Player * pPlayer, ArenaTeam * pTeam, bool victory, bool rated);
typedef void(*tOnObjectLoot)(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId);
typedef void(*tOnAreaTrigger)(Player * pPlayer, uint32 areaTrigger);
typedef void(*tOnPostLevelUp)(Player * pPlayer);
typedef bool(*tOnPreUnitDie)(Unit *killer, Unit *target);
typedef void(*tOnAdvanceSkillLine)(Player * pPlayer, uint32 SkillLine, uint32 Current);
typedef void(*tOnDuelFinished)(Player * Winner, Player * Looser);
typedef void(*tOnAuraRemove)(Aura * aura);
typedef bool(*tOnResurrect)(Player * pPlayer);

class Spell;
class Aura;
class Creature;
class CreatureAIScript;
class GossipScript;
class GameObjectAIScript;
class InstanceScript;
class ScriptMgr;
struct ItemPrototype;
class QuestLogEntry;

/* Factory Imports (from script lib) */
typedef CreatureAIScript*(*exp_create_creature_ai)(Creature * pCreature);
typedef GameObjectAIScript*(*exp_create_gameobject_ai)(GameObject * pGameObject);
typedef InstanceScript* ( *exp_create_instance_ai )( MapMgr* pMapMgr );
typedef bool(*exp_handle_dummy_spell)(uint32 i, Spell * pSpell);
typedef bool(*exp_handle_script_effect)(uint32 i, Spell * pSpell);
typedef bool(*exp_handle_dummy_aura)(uint32 i, Aura * pAura, bool apply);
typedef void(*exp_script_register)(ScriptMgr * mgr);
typedef void(*exp_engine_reload)();
typedef uint32(*exp_get_script_type)();

typedef uint32(*exp_get_version)();

/* Hashmap typedefs */
typedef HM_NAMESPACE::hash_map<uint32, exp_create_creature_ai> CreatureCreateMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_create_gameobject_ai> GameObjectCreateMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_handle_dummy_aura> HandleDummyAuraMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_handle_dummy_spell> HandleDummySpellMap;
typedef HM_NAMESPACE::hash_map< uint32, exp_handle_script_effect > HandleScriptEffectMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_create_instance_ai> InstanceCreateMap;
typedef set<GossipScript*> CustomGossipScripts;
typedef set<QuestScript*> QuestScripts;
typedef list<void*> ServerHookList;
typedef list<SCRIPT_MODULE> LibraryHandleMap;

#define VISIBLE_RANGE (26.46f)
#define MAX_SCRIPTS 1000
#define MAX_INSTANCE_SCRIPTS 1000

class SERVER_DECL ScriptMgr : public Singleton<ScriptMgr>
{
public:

	ScriptMgr();
	~ScriptMgr();

	friend class HookInterface;

	void LoadScripts();
	void UnloadScripts();

	/////////////////////////////////////////////////////////////////////////
	//void DumpUnimplementedSpells()
	//  Dumps the IDs of the spells with dummy/script effects or dummy aura
	//  that are not yet implemented.
	//
	//Parameters
	//  None.
	//
	//Return Value
	//  None.
	//
	//
	/////////////////////////////////////////////////////////////////////////
	void DumpUnimplementedSpells();

	CreatureAIScript * CreateAIScriptClassForEntry(Creature* pCreature);
	GameObjectAIScript * CreateAIScriptClassForGameObject(uint32 uEntryId, GameObject* pGameObject);
	InstanceScript* CreateScriptClassForInstance( uint32 pMapId, MapMgr* pMapMgr );

	bool CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell);
	bool HandleScriptedSpellEffect( uint32 SpellId, uint32 i, Spell *s );
	bool CallScriptedDummyAura( uint32 uSpellId, uint32 i, Aura* pAura, bool apply);
	bool CallScriptedItem(Item * pItem, Player * pPlayer);

	//Single Entry Registers
	void register_creature_script(uint32 entry, exp_create_creature_ai callback);
	void register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback);
	void register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback);
	void register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback);
	void register_script_effect( uint32 entry, exp_handle_script_effect callback );
	void register_instance_script( uint32 pMapId, exp_create_instance_ai pCallback ); 
	void register_gossip_script(uint32 entry, GossipScript * gs);
	void register_go_gossip_script(uint32 entry, GossipScript * gs);
	void register_hook(ServerHookEvents event, void * function_pointer);
	void register_item_gossip_script(uint32 entry, GossipScript * gs);
	void register_quest_script(uint32 entry, QuestScript * qs);

	//Mutliple Entry Registers
	void register_creature_script(uint32* entries, exp_create_creature_ai callback);
	void register_gameobject_script(uint32* entries, exp_create_gameobject_ai callback);
	void register_dummy_aura(uint32* entries, exp_handle_dummy_aura callback);
	void register_dummy_spell(uint32* entries, exp_handle_dummy_spell callback);
	void register_script_effect(uint32* entries, exp_handle_script_effect callback);

	void ReloadScriptEngines() {
		//for all scripting engines that allow reloading, assuming there will be new scripting engines.
		exp_get_script_type version_function;
		exp_engine_reload engine_reloadfunc;
		for(LibraryHandleMap::iterator itr = _handles.begin(); itr != _handles.end(); ++itr)
		{
#ifdef WIN32
			version_function = (exp_get_script_type)GetProcAddress( (HMODULE)(*itr),"_exp_get_script_type");
			if(version_function == 0)
				continue;
			if(version_function() & SCRIPT_TYPE_SCRIPT_ENGINE)
			{
				engine_reloadfunc = (exp_engine_reload)GetProcAddress( (HMODULE)(*itr),"_export_engine_reload");
				if(engine_reloadfunc != 0)
					engine_reloadfunc();
			}
#else
			version_function = (exp_get_script_type)dlsym( (SCRIPT_MODULE)(*itr),"_exp_get_script_type");
			if(version_function == 0)
				continue;
			if(version_function() & SCRIPT_TYPE_SCRIPT_ENGINE)
			{
				engine_reloadfunc = (exp_engine_reload)dlsym( (SCRIPT_MODULE)(*itr),"_export_engine_reload");
				if(engine_reloadfunc != 0)
					engine_reloadfunc();
			}
#endif
		}
	}

	ARCEMU_INLINE GossipScript * GetDefaultGossipScript() { return DefaultGossipScript; }

protected:
	InstanceCreateMap mInstances; 
	CreatureCreateMap _creatures;
	GameObjectCreateMap _gameobjects;
	HandleDummyAuraMap _auras;
	HandleDummySpellMap _spells;
	HandleScriptEffectMap SpellScriptEffects;
	LibraryHandleMap _handles;
	ServerHookList _hooks[NUM_SERVER_HOOKS];
	GossipScript * DefaultGossipScript;
	CustomGossipScripts _customgossipscripts;
	QuestScripts _questscripts;
};

class SERVER_DECL CreatureAIScript
{
public:
	CreatureAIScript(Creature* creature);
	virtual ~CreatureAIScript();

	virtual void OnCombatStart(Unit* mTarget) {}
	virtual void OnCombatStop(Unit* mTarget) {}
	virtual void OnDamageTaken(Unit* mAttacker, uint32 fAmount) {}
	virtual void OnCastSpell(uint32 iSpellId) {}
	virtual void OnTargetParried(Unit* mTarget) {}
	virtual void OnTargetDodged(Unit* mTarget) {}
	virtual void OnTargetBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnTargetCritHit(Unit* mTarget, int32 fAmount) {}
	virtual void OnTargetDied(Unit* mTarget) {}
	virtual void OnParried(Unit* mTarget) {}
	virtual void OnDodged(Unit* mTarget) {}
	virtual void OnBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnCritHit(Unit* mTarget, int32 fAmount) {}
	virtual void OnHit(Unit* mTarget, float fAmount) {}
	virtual void OnDied(Unit *mKiller) {}
	virtual void OnAssistTargetDied(Unit* mAssistTarget) {}
	virtual void OnFear(Unit* mFeared, uint32 iSpellId) {}
	virtual void OnFlee(Unit* mFlee) {}
	virtual void OnCallForHelp() {}
	virtual void OnLoad() {}
	virtual void OnDespawn() {}
	virtual void OnReachWP(uint32 iWaypointId, bool bForwards) {}
	virtual void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype) {}
	virtual void AIUpdate() {}
	virtual void OnEmote(Player * pPlayer, EmoteType Emote) {}
	virtual void StringFunctionCall(int) {}

	void RegisterAIUpdateEvent(uint32 frequency);
	void ModifyAIUpdateEvent(uint32 newfrequency);
	void RemoveAIUpdateEvent();

	bool IsAlive();

	virtual void Destroy() { delete this; }
	Creature* GetUnit() { return _unit; }

	CreatureAIScript* GetLinkedCreature() { return linkedCreatureAI; }
	void SetLinkedCreature(CreatureAIScript* creatureAI);
	void LinkedCreatureDeleted();

protected:
	Creature* _unit;

private:
	CreatureAIScript* linkedCreatureAI;
};

class SERVER_DECL GameObjectAIScript
{
public:
	GameObjectAIScript(GameObject* goinstance);
	virtual ~GameObjectAIScript(){}

	virtual void OnCreate() {}
	virtual void OnSpawn() {}
	virtual void OnDespawn() {}
	virtual void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo) {}
	virtual void OnActivate(Player * pPlayer) {}
	virtual void AIUpdate() {}
	virtual void Destroy() { delete this; }

	void RegisterAIUpdateEvent(uint32 frequency);
	void ModifyAIUpdateEvent(uint32 newfrequency);
	void RemoveAIUpdateEvent();
protected:

	GameObject* _gameobject;
};

class SERVER_DECL GossipScript
{
public:
	GossipScript();
	virtual ~GossipScript() {} 

	virtual void GossipHello(Object* pObject, Player* Plr, bool AutoSend);
	virtual void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode);
	virtual void GossipEnd(Object* pObject, Player* Plr);
	virtual void Destroy();
};

class SERVER_DECL QuestScript
{
public:
	QuestScript() {};
	virtual ~QuestScript() {};

	virtual void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestCancel(Player* mTarget) {}
	virtual void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry *qLogEntry) {}
};

/* * Class InstanceScript 
* Instanced class created for each instance of the map, holds all  
* scriptable exports 
*/

class SERVER_DECL InstanceScript
{
public:
	InstanceScript( MapMgr* pMapMgr ); 
	virtual ~InstanceScript() {};

	// Procedures that had been here before
	virtual GameObject*	GetObjectForOpenLock( Player* pCaster, Spell* pSpell, SpellEntry* pSpellEntry ) { return NULL; };
	virtual void				SetLockOptions( uint32 pEntryId, GameObject* pGameObject ) {};
	virtual uint32				GetRespawnTimeForCreature( uint32 pEntryId, Creature* pCreature) { return 240000; };

	// Player
	virtual void				OnPlayerDeath( Player* pVictim, Unit* pKiller ) {};

	// Area and AreaTrigger
	virtual void				OnPlayerEnter( Player* pPlayer ) {};
	virtual void				OnAreaTrigger( Player* pPlayer, uint32 pAreaId ) {};
	virtual void				OnZoneChange( Player* pPlayer, uint32 pNewZone, uint32 pOldZone ) {};

	// Data get / set - idea taken from ScriptDev2
	virtual void				SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData ) {};
	virtual uint32				GetInstanceData( uint32 pType, uint32 pIndex ) { return 0; };

	// Creature / GameObject - part of it is simple reimplementation for easier use Creature / GO < --- > Script
	virtual void				OnCreatureDeath( Creature* pVictim, Unit* pKiller ) {};
	virtual void				OnCreaturePushToWorld( Creature* pCreature ) {};
	virtual void				OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer ) {};
	virtual void				OnGameObjectPushToWorld( GameObject* pGameObject ) {};

	// Standard virtual methods
	virtual void				OnLoad() {};
	virtual void				UpdateEvent() {};
	virtual void				Destroy() {};

	// UpdateEvent
	void						RegisterUpdateEvent( uint32 pFrequency );
	void						ModifyUpdateEvent( uint32 pNewFrequency );
	void						RemoveUpdateEvent();

	// Something to return Instance's MapMgr
	MapMgr*				GetInstance() { return mInstance; };

protected:

	MapMgr* mInstance;
};
class SERVER_DECL HookInterface : public Singleton<HookInterface>
{
public:
	friend class ScriptMgr;

	bool OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
	void OnKillPlayer(Player * pPlayer, Player * pVictim);
	void OnFirstEnterWorld(Player * pPlayer);
	void OnEnterWorld(Player * pPlayer);
	void OnGuildCreate(Player * pLeader, Guild * pGuild);
	void OnGuildJoin(Player * pPlayer, Guild * pGuild);
	void OnDeath(Player * pPlayer);
	bool OnRepop(Player * pPlayer);
	void OnEmote(Player * pPlayer, uint32 Emote, Unit * pUnit);
	void OnEnterCombat(Player * pPlayer, Unit * pTarget);
	bool OnCastSpell(Player * pPlayer, SpellEntry * pSpell, Spell * spell);
	bool OnLogoutRequest(Player * pPlayer);
	void OnLogout(Player * pPlayer);
	void OnQuestAccept(Player * pPlayer, Quest * pQuest, Object * pQuestGiver);
	void OnZone(Player * pPlayer, uint32 Zone, uint32 oldZone);
	bool OnChat(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc);
	void OnLoot(Player * pPlayer, Unit * pTarget, uint32 Money, uint32 ItemId);
	void OnFullLogin(Player * pPlayer);
	void OnCharacterCreate(Player * pPlayer);
	void OnQuestCancelled(Player * pPlayer, Quest * pQuest);
	void OnQuestFinished(Player * pPlayer, Quest * pQuest, Object * pQuestGiver);
	void OnHonorableKill(Player * pPlayer, Player * pKilled);
	void OnArenaFinish(Player * pPlayer, ArenaTeam* pTeam, bool victory, bool rated);
	void OnObjectLoot(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId);
	void OnAreaTrigger(Player * pPlayer, uint32 areaTrigger);
	void OnPostLevelUp(Player * pPlayer);
	bool OnPreUnitDie(Unit *Killer, Unit *Victim);
	void OnAdvanceSkillLine(Player * pPlayer, uint32 SkillLine, uint32 Current);
	void OnDuelFinished(Player * Winner, Player * Looser);
	void OnAuraRemove(Aura * aura);
	bool OnResurrect(Player * pPlayer);
};

#define sScriptMgr ScriptMgr::getSingleton()
#define sHookInterface HookInterface::getSingleton()

#endif
