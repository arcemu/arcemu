/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

/*
English Worldstrings as of 08.16.2009

entry	text
1 	I would like to browse your goods.
2 	I seek
3 	mage
4 	shaman
5 	warrior
6 	paladin
7 	warlock
8 	hunter
9 	rogue
10 	druid
11 	priest
12 	training
13 	Train me in the ways of the beast.
14 	Give me a ride.
15 	I would like to make a bid.
16 	Make this inn your home.
17 	I would like to check my deposit box.
18 	Bring me back to life.
19 	How do I create a guild/arena team?
20 	I want to create a guild crest.
21 	I would like to go to the battleground.
22 	I would like to reset my talents.
23 	I wish to untrain my pet.
24 	I understand, continue.
25 	Yes, please do.
26 	This instance is unavailable.
27 	You must have The Burning Crusade Expansion to access this content.
28 	Heroic mode unavailable for this instance.
29 	You must be in a raid group to pass through here.
30 	You do not have the required attunement to pass through here.
31 	You must be at least level %u to pass through here.
32 	You must be in a party to pass through here.
33 	You must be level 70 to enter heroic mode.
34 	-
35 	You must have the item, `%s` to pass through here.
36 	You must have the item, UNKNOWN to pass through here.
37 	What can I teach you, $N?
38 	Alterac Valley
39 	Warsong Gulch
40 	Arathi Basin
41 	Arena 2v2
42 	Arena 3v3
43 	Arena 5v5
44 	Eye of the Storm
45 	Unknown Battleground
46 	One minute until the battle for %s begins!
47 	Thirty seconds until the battle for %s begins!
48 	Fifteen seconds until the battle for %s begins!
49 	The battle for %s has begun!
50 	Arena
51 	You have tried to join an invalid instance id.
52 	Your queue on battleground instance id %u is no longer valid. Reason: Instance Deleted.
53 	You cannot join this battleground as it has already ended.
54 	Your queue on battleground instance %u is no longer valid, the instance no longer exists.
55 	Sorry, raid groups joining battlegrounds are currently unsupported.
56 	You must be the party leader to add a group to an arena.
57 	You must be in a team to join rated arena.
58 	You have too many players in your party to join this type of arena.
59 	Sorry, some of your party members are not level 70.
60 	One or more of your party members are already queued or inside a battleground.
61 	One or more of your party members are not members of your team.
62 	Welcome to
63 	Horde
64 	Alliance
65 	[ |cff00ccffAttention|r ] Welcome! A new challenger (|cff00ff00{%d}|r - |cffff0000%s|r) has arrived and joined into |cffff0000%s|r,their force has already been increased.
66 	This instance is scheduled to reset on
67 	Auto loot passing is now %s
68 	On
69 	Off
70 	Hey there, $N. How can I help you?
71 	You are already in an arena team.
72 	That name is already in use.
73 	You already have an arena charter.
74 	A guild with that name already exists.
75 	You already have a guild charter.
76 	Item not found.
77 	Target is of the wrong faction.
78 	Target player cannot sign your charter for one or more reasons.
79 	You have already signed that charter.
80 	You don't have the required amount of signatures to turn in this petition.
81 	You must have Wrath of the Lich King Expansion to access this content.
82  Deathknight
*/

#include "StdAfx.h"

#include <git_version.h>

namespace worldstring
{

}



initialiseSingleton(ScriptMgr);
initialiseSingleton(HookInterface);

ScriptMgr::ScriptMgr()
{
}

ScriptMgr::~ScriptMgr()
{

}

struct ScriptingEngine_dl
{
	Arcemu::DynLib* dl;
	exp_script_register InitializeCall;
	uint32 Type;

	ScriptingEngine_dl()
	{
		dl = NULL;
		InitializeCall = NULL;
		Type = 0;
	}
};

void ScriptMgr::LoadScripts()
{
	if(HookInterface::getSingletonPtr() == NULL)
		new HookInterface;

	Log.Success("Server", "Loading External Script Libraries...");

	std::string Path;
	std::string FileMask;
	Path = PREFIX;
	Path += '/';
#ifdef WIN32
	/*Path = Config.MainConfig.GetStringDefault( "Script", "BinaryLocation", "script_bin" );
	Path += "\\";*/
	FileMask = ".dll";
#else
#ifndef __APPLE__
	FileMask = ".so";
#else
	FileMask = ".dylib";
#endif
#endif

	Arcemu::FindFilesResult findres;
	std::vector< ScriptingEngine_dl > Engines;

	Arcemu::FindFiles(Path.c_str(), FileMask.c_str(), findres);
	uint32 count = 0;

	while(findres.HasNext())
	{
		std::stringstream loadmessage;
		std::string fname = Path + findres.GetNext();
		Arcemu::DynLib* dl = new Arcemu::DynLib(fname.c_str());

		loadmessage << "  " << dl->GetName() << " : ";

		if(!dl->Load())
		{
			loadmessage << "ERROR: Cannot open library.";
			LOG_ERROR(loadmessage.str().c_str());
			delete dl;
			continue;

		}
		else
		{
			exp_get_version vcall     = reinterpret_cast< exp_get_version >(dl->GetAddressForSymbol("_exp_get_version"));
			exp_script_register rcall = reinterpret_cast< exp_script_register >(dl->GetAddressForSymbol("_exp_script_register"));
			exp_get_script_type scall = reinterpret_cast< exp_get_script_type >(dl->GetAddressForSymbol("_exp_get_script_type"));

			if((vcall == NULL) || (rcall == NULL) || (scall == NULL))
			{
				loadmessage << "ERROR: Cannot find version functions.";
				LOG_ERROR(loadmessage.str().c_str());
				delete dl;
				continue;
			}
			else
			{
				const char *version = vcall();
				uint32 stype = scall();

				if( strcmp( version, BUILD_HASH_STR ) != 0 )
				{
					loadmessage << "ERROR: Version mismatch.";
					LOG_ERROR(loadmessage.str().c_str());
					delete dl;
					continue;

				}
				else
				{
					loadmessage << ' ' << std::string( BUILD_HASH_STR ) << " : ";

					if((stype & SCRIPT_TYPE_SCRIPT_ENGINE) != 0)
					{
						ScriptingEngine_dl se;

						se.dl = dl;
						se.InitializeCall = rcall;
						se.Type = stype;

						Engines.push_back(se);

						loadmessage << "delayed load";

					}
					else
					{
						rcall(this);
						dynamiclibs.push_back(dl);

						loadmessage << "loaded";
					}
					LOG_BASIC(loadmessage.str().c_str());
					count++;
				}
			}
		}
	}

	if(count == 0)
	{
		LOG_ERROR("  No external scripts found! Server will continue to function with limited functionality.");
	}
	else
	{
		Log.Success("Server", "Loaded %u external libraries.", count);
		Log.Success("Server", "Loading optional scripting engine(s)...");

		for(std::vector< ScriptingEngine_dl >::iterator itr = Engines.begin(); itr != Engines.end(); ++itr)
		{
			itr->InitializeCall(this);
			dynamiclibs.push_back(itr->dl);
		}

		Log.Success("Server", "Done loading scripting engine(s)...");
	}
}

void ScriptMgr::UnloadScripts()
{
	if(HookInterface::getSingletonPtr())
		delete HookInterface::getSingletonPtr();

	for(CustomGossipScripts::iterator itr = _customgossipscripts.begin(); itr != _customgossipscripts.end(); ++itr)
		(*itr)->Destroy();
	_customgossipscripts.clear();

	for(QuestScripts::iterator itr = _questscripts.begin(); itr != _questscripts.end(); ++itr)
		delete *itr;
	_questscripts.clear();

	UnloadScriptEngines();

	for(DynamicLibraryMap::iterator itr = dynamiclibs.begin(); itr != dynamiclibs.end(); ++itr)
		delete *itr;

	dynamiclibs.clear();
}

void ScriptMgr::DumpUnimplementedSpells()
{
	std::ofstream of;

	LOG_BASIC("Dumping IDs for spells with unimplemented dummy/script effect(s)");
	uint32 count = 0;

	of.open("unimplemented1.txt");

	for(DBCStorage< SpellEntry >::iterator itr = dbcSpell.begin(); itr != dbcSpell.end(); ++itr)
	{
		SpellEntry* sp = *itr;

		if(!sp->HasEffect(SPELL_EFFECT_DUMMY) && !sp->HasEffect(SPELL_EFFECT_SCRIPT_EFFECT) && !sp->HasEffect(SPELL_EFFECT_SEND_EVENT))
			continue;

		HandleDummySpellMap::iterator sitr = _spells.find(sp->Id);
		if(sitr != _spells.end())
			continue;

		HandleScriptEffectMap::iterator seitr = SpellScriptEffects.find(sp->Id);
		if(seitr != SpellScriptEffects.end())
			continue;

		std::stringstream ss;
		ss << sp->Id;
		ss << std::endl;

		of.write(ss.str().c_str(), ss.str().length());

		count++;
	}

	of.close();

	LOG_BASIC("Dumped %u IDs.", count);

	LOG_BASIC("Dumping IDs for spells with unimplemented dummy aura effect.");

	std::ofstream of2;
	of2.open("unimplemented2.txt");

	count = 0;

	for(DBCStorage< SpellEntry >::iterator itr = dbcSpell.begin(); itr != dbcSpell.end(); ++itr)
	{
		SpellEntry* sp = *itr;

		if(!sp->AppliesAura(SPELL_AURA_DUMMY))
			continue;

		HandleDummyAuraMap::iterator ditr = _auras.find(sp->Id);
		if(ditr != _auras.end())
			continue;

		std::stringstream ss;
		ss << sp->Id;
		ss << std::endl;

		of2.write(ss.str().c_str(), ss.str().length());

		count++;
	}

	of2.close();

	LOG_BASIC("Dumped %u IDs.", count);
}

void ScriptMgr::register_creature_script(uint32 entry, exp_create_creature_ai callback)
{
	if(_creatures.find(entry) != _creatures.end())
		LOG_ERROR("ScriptMgr is trying to register a script for Creature ID: %u even if there's already one for that Creature. Remove one of those scripts.", entry);

	_creatures.insert(CreatureCreateMap::value_type(entry, callback));
}

void ScriptMgr::register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback)
{
	if(_gameobjects.find(entry) != _gameobjects.end())
		LOG_ERROR("ScriptMgr is trying to register a script for GameObject ID: %u even if there's already one for that GameObject. Remove one of those scripts.", entry);

	_gameobjects.insert(GameObjectCreateMap::value_type(entry, callback));
}

void ScriptMgr::register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback)
{
	if(_auras.find(entry) != _auras.end())
	{
		LOG_ERROR("ScriptMgr is trying to register a script for Aura ID: %u even if there's already one for that Aura. Remove one of those scripts.", entry);
	}

	SpellEntry* sp = dbcSpell.LookupEntryForced(entry);
	if(sp == NULL)
	{
		LOG_ERROR("ScriptMgr is trying to register a dummy aura handler for Spell ID: %u which is invalid.", entry);
		return;
	}

	if(!sp->AppliesAura(SPELL_AURA_DUMMY) && !sp->AppliesAura(SPELL_AURA_PERIODIC_TRIGGER_DUMMY))
		LOG_ERROR("ScriptMgr has registered a dummy aura handler for Spell ID: %u ( %s ), but spell has no dummy aura!", entry, sp->Name);

	_auras.insert(HandleDummyAuraMap::value_type(entry, callback));
}

void ScriptMgr::register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback)
{
	if(_spells.find(entry) != _spells.end())
	{
		LOG_ERROR("ScriptMgr is trying to register a script for Spell ID: %u even if there's already one for that Spell. Remove one of those scripts.", entry);
		return;
	}

	SpellEntry* sp = dbcSpell.LookupEntryForced(entry);
	if(sp == NULL)
	{
		LOG_ERROR("ScriptMgr is trying to register a dummy handler for Spell ID: %u which is invalid.", entry);
		return;
	}

	if(!sp->HasEffect(SPELL_EFFECT_DUMMY) && !sp->HasEffect(SPELL_EFFECT_SCRIPT_EFFECT) && !sp->HasEffect(SPELL_EFFECT_SEND_EVENT))
		LOG_ERROR("ScriptMgr has registered a dummy handler for Spell ID: %u ( %s ), but spell has no dummy/script/send event effect!", entry, sp->Name);

	_spells.insert(HandleDummySpellMap::value_type(entry, callback));
}

void ScriptMgr::register_gossip_script(uint32 entry, GossipScript* gs)
{
	register_creature_gossip(entry, gs);
}

void ScriptMgr::register_go_gossip_script(uint32 entry, GossipScript* gs)
{
	register_go_gossip(entry, gs);
}

void ScriptMgr::register_quest_script(uint32 entry, QuestScript* qs)
{
	Quest* q = QuestStorage.LookupEntry(entry);
	if(q != NULL)
	{
		if(q->pQuestScript != NULL)
			LOG_ERROR("ScriptMgr is trying to register a script for Quest ID: %u even if there's already one for that Quest. Remove one of those scripts.", entry);

		q->pQuestScript = qs;
	}

	_questscripts.insert(qs);
}

void ScriptMgr::register_instance_script(uint32 pMapId, exp_create_instance_ai pCallback)
{
	if(mInstances.find(pMapId) != mInstances.end())
		LOG_ERROR("ScriptMgr is trying to register a script for Instance ID: %u even if there's already one for that Instance. Remove one of those scripts.", pMapId);

	mInstances.insert(InstanceCreateMap::value_type(pMapId, pCallback));
};

void ScriptMgr::register_creature_script(uint32* entries, exp_create_creature_ai callback)
{
	for(uint32 y = 0; entries[y] != 0; y++)
	{
		register_creature_script(entries[y], callback);
	}
};

void ScriptMgr::register_gameobject_script(uint32* entries, exp_create_gameobject_ai callback)
{
	for(uint32 y = 0; entries[y] != 0; y++)
	{
		register_gameobject_script(entries[y], callback);
	}
};

void ScriptMgr::register_dummy_aura(uint32* entries, exp_handle_dummy_aura callback)
{
	for(uint32 y = 0; entries[y] != 0; y++)
	{
		register_dummy_aura(entries[y], callback);
	}
};

void ScriptMgr::register_dummy_spell(uint32* entries, exp_handle_dummy_spell callback)
{
	for(uint32 y = 0; entries[y] != 0; y++)
	{
		register_dummy_spell(entries[y], callback);
	}
};

void ScriptMgr::register_script_effect(uint32* entries, exp_handle_script_effect callback)
{
	for(uint32 y = 0; entries[y] != 0; y++)
	{
		register_script_effect(entries[y], callback);
	}
};

void ScriptMgr::register_script_effect(uint32 entry, exp_handle_script_effect callback)
{

	HandleScriptEffectMap::iterator itr = SpellScriptEffects.find(entry);

	if(itr != SpellScriptEffects.end())
	{
		LOG_ERROR("ScriptMgr tried to register more than 1 script effect handlers for Spell %u", entry);
		return;
	}

	SpellEntry* sp = dbcSpell.LookupEntryForced(entry);
	if(sp == NULL)
	{
		LOG_ERROR("ScriptMgr tried to register a script effect handler for Spell %u, which is invalid.", entry);
		return;
	}

	if(!sp->HasEffect(SPELL_EFFECT_SCRIPT_EFFECT) && !sp->HasEffect(SPELL_EFFECT_SEND_EVENT))
		LOG_ERROR("ScriptMgr has registered a script effect handler for Spell ID: %u ( %s ), but spell has no scripted effect!", entry, sp->Name);

	SpellScriptEffects.insert(std::pair< uint32, exp_handle_script_effect >(entry, callback));
}

CreatureAIScript* ScriptMgr::CreateAIScriptClassForEntry(Creature* pCreature)
{
	CreatureCreateMap::iterator itr = _creatures.find(pCreature->GetEntry());
	if(itr == _creatures.end())
		return NULL;

	exp_create_creature_ai function_ptr = itr->second;
	return (function_ptr)(pCreature);
}

GameObjectAIScript* ScriptMgr::CreateAIScriptClassForGameObject(uint32 uEntryId, GameObject* pGameObject)
{
	GameObjectCreateMap::iterator itr = _gameobjects.find(pGameObject->GetEntry());
	if(itr == _gameobjects.end())
		return NULL;

	exp_create_gameobject_ai function_ptr = itr->second;
	return (function_ptr)(pGameObject);
}

InstanceScript* ScriptMgr::CreateScriptClassForInstance(uint32 pMapId, MapMgr* pMapMgr)
{
	InstanceCreateMap::iterator Iter = mInstances.find(pMapMgr->GetMapId());
	if(Iter == mInstances.end())
		return NULL;
	exp_create_instance_ai function_ptr = Iter->second;
	return (function_ptr)(pMapMgr);
};

bool ScriptMgr::CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell)
{
	HandleDummySpellMap::iterator itr = _spells.find(uSpellId);
	if(itr == _spells.end())
		return false;

	exp_handle_dummy_spell function_ptr = itr->second;
	return (function_ptr)(i, pSpell);
}

bool ScriptMgr::HandleScriptedSpellEffect(uint32 SpellId, uint32 i, Spell* s)
{
	HandleScriptEffectMap::iterator itr = SpellScriptEffects.find(SpellId);
	if(itr == SpellScriptEffects.end())
		return false;

	exp_handle_script_effect ptr = itr->second;
	return (ptr)(i, s);
}

bool ScriptMgr::CallScriptedDummyAura(uint32 uSpellId, uint32 i, Aura* pAura, bool apply)
{
	HandleDummyAuraMap::iterator itr = _auras.find(uSpellId);
	if(itr == _auras.end())
		return false;

	exp_handle_dummy_aura function_ptr = itr->second;
	return (function_ptr)(i, pAura, apply);
}

bool ScriptMgr::CallScriptedItem(Item* pItem, Player* pPlayer)
{
	Arcemu::Gossip::Script* script = this->get_item_gossip(pItem->GetEntry());
	if(script != NULL)
	{
		script->OnHello(pItem, pPlayer);
		return true;
	}
	return false;
}

void ScriptMgr::register_item_gossip_script(uint32 entry, GossipScript* gs)
{
	register_item_gossip(entry, gs);
}

/* CreatureAI Stuff */
CreatureAIScript::CreatureAIScript(Creature* creature) : _unit(creature), linkedCreatureAI(NULL)
{

}

CreatureAIScript::~CreatureAIScript()
{
	//notify our linked creature that we are being deleted.
	if(linkedCreatureAI != NULL)
		linkedCreatureAI->LinkedCreatureDeleted();
}

void CreatureAIScript::RegisterAIUpdateEvent(uint32 frequency)
{
	//sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0,0);
	sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void CreatureAIScript::ModifyAIUpdateEvent(uint32 newfrequency)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(_unit, EVENT_SCRIPT_UPDATE_EVENT, newfrequency);
}

void CreatureAIScript::RemoveAIUpdateEvent()
{
	sEventMgr.RemoveEvents(_unit, EVENT_SCRIPT_UPDATE_EVENT);
}

void CreatureAIScript::LinkedCreatureDeleted()
{
	linkedCreatureAI = NULL;
}

void CreatureAIScript::SetLinkedCreature(CreatureAIScript* creatureAI)
{
	//notify our linked creature that we are not more linked
	if(linkedCreatureAI != NULL)
		linkedCreatureAI->LinkedCreatureDeleted();

	//link to the new creature
	linkedCreatureAI = creatureAI;
}

bool CreatureAIScript::IsAlive()
{
	return _unit->isAlive();
}

/* GameObjectAI Stuff */

GameObjectAIScript::GameObjectAIScript(GameObject* goinstance) : _gameobject(goinstance)
{

}

void GameObjectAIScript::ModifyAIUpdateEvent(uint32 newfrequency)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(_gameobject, EVENT_SCRIPT_UPDATE_EVENT, newfrequency);
}

void GameObjectAIScript::RemoveAIUpdateEvent()
{
	sEventMgr.RemoveEvents(_gameobject, EVENT_SCRIPT_UPDATE_EVENT);
}

void GameObjectAIScript::RegisterAIUpdateEvent(uint32 frequency)
{
	sEventMgr.AddEvent(_gameobject, &GameObject::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

/* InstanceAI Stuff */

InstanceScript::InstanceScript(MapMgr* pMapMgr) : mInstance(pMapMgr)
{
};

void InstanceScript::RegisterUpdateEvent(uint32 pFrequency)
{
	sEventMgr.AddEvent(mInstance, &MapMgr::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, pFrequency, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
};
void InstanceScript::ModifyUpdateEvent(uint32 pNewFrequency)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(mInstance, EVENT_SCRIPT_UPDATE_EVENT, pNewFrequency);
};
void InstanceScript::RemoveUpdateEvent()
{
	sEventMgr.RemoveEvents(mInstance, EVENT_SCRIPT_UPDATE_EVENT);
};

/* Hook Stuff */
void ScriptMgr::register_hook(ServerHookEvents event, void* function_pointer)
{
	ARCEMU_ASSERT(event < NUM_SERVER_HOOKS);
	_hooks[event].insert(function_pointer);
}

bool ScriptMgr::has_creature_script(uint32 entry) const
{
	return (_creatures.find(entry) != _creatures.end());
}

bool ScriptMgr::has_gameobject_script(uint32 entry) const
{
	return (_gameobjects.find(entry) != _gameobjects.end());
}

bool ScriptMgr::has_dummy_aura_script(uint32 entry) const
{
	return (_auras.find(entry) != _auras.end());
}

bool ScriptMgr::has_dummy_spell_script(uint32 entry) const
{
	return (_spells.find(entry) != _spells.end());
}

bool ScriptMgr::has_script_effect(uint32 entry) const
{
	return (SpellScriptEffects.find(entry) != SpellScriptEffects.end());
}

bool ScriptMgr::has_instance_script(uint32 id) const
{
	return (mInstances.find(id) != mInstances.end());
}

bool ScriptMgr::has_hook(ServerHookEvents evt, void* ptr) const
{
	return (_hooks[evt].size() != 0 && _hooks[evt].find(ptr) != _hooks[evt].end());
}

bool ScriptMgr::has_quest_script(uint32 entry) const
{
	Quest* q = QuestStorage.LookupEntry(entry);
	return (q == NULL || q->pQuestScript != NULL);
}

void ScriptMgr::register_creature_gossip(uint32 entry, Arcemu::Gossip::Script* script)
{
	GossipMap::iterator itr = creaturegossip_.find(entry);
	if(itr == creaturegossip_.end())
		creaturegossip_.insert(make_pair(entry, script));
	//keeping track of all created gossips to delete them all on shutdown
	_customgossipscripts.insert(script);
}

bool ScriptMgr::has_creature_gossip(uint32 entry) const
{
	return creaturegossip_.find(entry) != creaturegossip_.end();
}

Arcemu::Gossip::Script* ScriptMgr::get_creature_gossip(uint32 entry) const
{
	GossipMap::const_iterator itr = creaturegossip_.find(entry);
	if(itr != creaturegossip_.end())
		return itr->second;
	return NULL;
}

void ScriptMgr::register_item_gossip(uint32 entry, Arcemu::Gossip::Script* script)
{
	GossipMap::iterator itr = itemgossip_.find(entry);
	if(itr == itemgossip_.end())
		itemgossip_.insert(make_pair(entry, script));
	//keeping track of all created gossips to delete them all on shutdown
	_customgossipscripts.insert(script);
}

void ScriptMgr::register_go_gossip(uint32 entry, Arcemu::Gossip::Script* script)
{
	GossipMap::iterator itr = gogossip_.find(entry);
	if(itr == gogossip_.end())
		gogossip_.insert(make_pair(entry, script));
	//keeping track of all created gossips to delete them all on shutdown
	_customgossipscripts.insert(script);
}

bool ScriptMgr::has_item_gossip(uint32 entry) const
{
	return itemgossip_.find(entry) != itemgossip_.end();
}

bool ScriptMgr::has_go_gossip(uint32 entry) const
{
	return gogossip_.find(entry) != gogossip_.end();
}

Arcemu::Gossip::Script* ScriptMgr::get_go_gossip(uint32 entry) const
{
	GossipMap::const_iterator itr = gogossip_.find(entry);
	if(itr != gogossip_.end())
		return itr->second;
	return NULL;
}

Arcemu::Gossip::Script* ScriptMgr::get_item_gossip(uint32 entry) const
{
	GossipMap::const_iterator itr = itemgossip_.find(entry);
	if(itr != itemgossip_.end())
		return itr->second;
	return NULL;
}

void ScriptMgr::ReloadScriptEngines()
{
	//for all scripting engines that allow reloading, assuming there will be new scripting engines.
	exp_get_script_type version_function;
	exp_engine_reload engine_reloadfunc;

	for(DynamicLibraryMap::iterator itr = dynamiclibs.begin(); itr != dynamiclibs.end(); ++itr)
	{
		Arcemu::DynLib* dl = *itr;

		version_function = reinterpret_cast< exp_get_script_type >(dl->GetAddressForSymbol("_exp_get_script_type"));
		if(version_function == NULL)
			continue;

		if((version_function() & SCRIPT_TYPE_SCRIPT_ENGINE) != 0)
		{
			engine_reloadfunc = reinterpret_cast< exp_engine_reload >(dl->GetAddressForSymbol("_export_engine_reload"));
			if(engine_reloadfunc != NULL)
				engine_reloadfunc();
		}
	}
}

void ScriptMgr::UnloadScriptEngines()
{
	//for all scripting engines that allow unloading, assuming there will be new scripting engines.
	exp_get_script_type version_function;
	exp_engine_unload engine_unloadfunc;

	for(DynamicLibraryMap::iterator itr = dynamiclibs.begin(); itr != dynamiclibs.end(); ++itr)
	{
		Arcemu::DynLib* dl = *itr;

		version_function = reinterpret_cast< exp_get_script_type >(dl->GetAddressForSymbol("_exp_get_script_type"));
		if(version_function == NULL)
			continue;

		if((version_function() & SCRIPT_TYPE_SCRIPT_ENGINE) != 0)
		{
			engine_unloadfunc = reinterpret_cast< exp_engine_unload >(dl->GetAddressForSymbol("_exp_engine_unload"));
			if(engine_unloadfunc != NULL)
				engine_unloadfunc();
		}
	}
}



//support for Gossip scripts added before r4106 changes
void GossipScript::OnHello(Object* pObject, Player* Plr)
{
	GossipHello(pObject, Plr);
}

void GossipScript::OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char* EnteredCode)
{
	uint32 IntId = Id;

	if(Plr->CurrentGossipMenu != NULL)
	{
		GossipMenuItem item = Plr->CurrentGossipMenu->GetItem(Id);
		IntId = item.IntId;
	}

	GossipSelectOption(pObject, Plr, Id , IntId, EnteredCode);
}

void GossipScript::OnEnd(Object* pObject, Player* Plr)
{
	GossipEnd(pObject, Plr);
}

/* Hook Implementations */
bool HookInterface::OnNewCharacter(uint32 Race, uint32 Class, WorldSession* Session, const char* Name)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_NEW_CHARACTER];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnNewCharacter) * itr)(Race, Class, Session, Name);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnKillPlayer(Player* pPlayer, Player* pVictim)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_KILL_PLAYER];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnKillPlayer)*itr)(pPlayer, pVictim);
}

void HookInterface::OnFirstEnterWorld(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnFirstEnterWorld)*itr)(pPlayer);
}

void HookInterface::OnCharacterCreate(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_CHARACTER_CREATE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOCharacterCreate)*itr)(pPlayer);
}

void HookInterface::OnEnterWorld(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ENTER_WORLD];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEnterWorld)*itr)(pPlayer);
}

void HookInterface::OnGuildCreate(Player* pLeader, Guild* pGuild)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_GUILD_CREATE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnGuildCreate)*itr)(pLeader, pGuild);
}

void HookInterface::OnGuildJoin(Player* pPlayer, Guild* pGuild)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_GUILD_JOIN];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnGuildJoin)*itr)(pPlayer, pGuild);
}

void HookInterface::OnDeath(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_DEATH];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnDeath)*itr)(pPlayer);
}

bool HookInterface::OnRepop(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_REPOP];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnRepop) * itr)(pPlayer);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_EMOTE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEmote)*itr)(pPlayer, Emote, pUnit);
}

void HookInterface::OnEnterCombat(Player* pPlayer, Unit* pTarget)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ENTER_COMBAT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEnterCombat)*itr)(pPlayer, pTarget);
}

bool HookInterface::OnCastSpell(Player* pPlayer, SpellEntry* pSpell, Spell* spell)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_CAST_SPELL];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnCastSpell) * itr)(pPlayer, pSpell, spell);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

bool HookInterface::OnLogoutRequest(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnLogoutRequest) * itr)(pPlayer);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnLogout(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_LOGOUT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnLogout)*itr)(pPlayer);
}

void HookInterface::OnQuestAccept(Player* pPlayer, Quest* pQuest, Object* pQuestGiver)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_QUEST_ACCEPT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnQuestAccept)*itr)(pPlayer, pQuest, pQuestGiver);
}

void HookInterface::OnZone(Player* pPlayer, uint32 zone, uint32 oldZone)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ZONE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnZone)*itr)(pPlayer, zone, oldZone);
}

bool HookInterface::OnChat(Player* pPlayer, uint32 type, uint32 lang, const char* message, const char* misc)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_CHAT];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnChat) * itr)(pPlayer, type, lang, message, misc);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnLoot(Player* pPlayer, Unit* pTarget, uint32 money, uint32 itemId)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_LOOT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnLoot)*itr)(pPlayer, pTarget, money, itemId);
}

void HookInterface::OnObjectLoot(Player* pPlayer, Object* pTarget, uint32 money, uint32 itemId)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_OBJECTLOOT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnObjectLoot)*itr)(pPlayer, pTarget, money, itemId);
}

void HookInterface::OnFullLogin(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_FULL_LOGIN];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEnterWorld)*itr)(pPlayer);
}

void HookInterface::OnQuestCancelled(Player* pPlayer, Quest* pQuest)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_QUEST_CANCELLED];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnQuestCancel)*itr)(pPlayer, pQuest);
}

void HookInterface::OnQuestFinished(Player* pPlayer, Quest* pQuest, Object* pQuestGiver)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_QUEST_FINISHED];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnQuestFinished)*itr)(pPlayer, pQuest, pQuestGiver);
}

void HookInterface::OnHonorableKill(Player* pPlayer, Player* pKilled)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_HONORABLE_KILL];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnHonorableKill)*itr)(pPlayer, pKilled);
}

void HookInterface::OnArenaFinish(Player* pPlayer, ArenaTeam* pTeam, bool victory, bool rated)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ARENA_FINISH];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnArenaFinish)*itr)(pPlayer, pTeam, victory, rated);
}

void HookInterface::OnAreaTrigger(Player* pPlayer, uint32 areaTrigger)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_AREATRIGGER];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnAreaTrigger)*itr)(pPlayer, areaTrigger);
}

void HookInterface::OnPostLevelUp(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_POST_LEVELUP];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnPostLevelUp)*itr)(pPlayer);
}

bool HookInterface::OnPreUnitDie(Unit* killer, Unit* victim)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_PRE_DIE];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnPreUnitDie) * itr)(killer, victim);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}


void HookInterface::OnAdvanceSkillLine(Player* pPlayer, uint32 skillLine, uint32 current)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnAdvanceSkillLine)*itr)(pPlayer, skillLine, current);
}

void HookInterface::OnDuelFinished(Player* Winner, Player* Looser)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_DUEL_FINISHED];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnDuelFinished)*itr)(Winner, Looser);
}

void HookInterface::OnAuraRemove(Aura* aura)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_AURA_REMOVE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnAuraRemove)*itr)(aura);
}

bool HookInterface::OnResurrect(Player* pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_RESURRECT];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnResurrect) * itr)(pPlayer);
		if(rv == false)  // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}