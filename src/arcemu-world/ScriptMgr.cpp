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
#ifndef WIN32
    #include <dlfcn.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <cstdlib>
    #include <cstring>
#endif

#include <svn_revision.h>
#define SCRIPTLIB_HIPART(x) ((x >> 16))
#define SCRIPTLIB_LOPART(x) ((x & 0x0000ffff))
#define SCRIPTLIB_VERSION_MINOR (BUILD_REVISION % 1000)
#define SCRIPTLIB_VERSION_MAJOR (BUILD_REVISION / 1000)

namespace worldstring{
    enum ws{
        BROWSEGOODS = 1,
        ISEEK       = 2,
        MAGE        = 3,
        SHAMAN      = 4,
        WARRIOR     = 5,
        PALADIN     = 6,
        WARLOCK     = 7,
        HUNTER      = 8,
        ROGUE       = 9,
        DRUID       = 10,
        PRIEST      = 11,
        TRAINING    = 12,
        BEASTTRAINING = 13

    };
}



initialiseSingleton(ScriptMgr);
initialiseSingleton(HookInterface);

ScriptMgr::ScriptMgr()
{
	DefaultGossipScript = new GossipScript();
}

ScriptMgr::~ScriptMgr()
{

}

struct ScriptingEngine
{
#ifdef WIN32
	HMODULE Handle;
#else
	void* Handle;
#endif
	exp_script_register InitializeCall;
	uint32 Type;
};

void ScriptMgr::LoadScripts()
{
	if(!HookInterface::getSingletonPtr())
		new HookInterface;

	Log.Success("Server","Loading External Script Libraries...");

	string start_path = Config.MainConfig.GetStringDefault( "Script", "BinaryLocation", "script_bin" ) + "\\";
	string search_path = start_path + "*.";

	vector< ScriptingEngine > ScriptEngines;

	/* Loading system for win32 */
#ifdef WIN32
	search_path += "dll";

	WIN32_FIND_DATA data;
	uint32 count = 0;
	HANDLE find_handle = FindFirstFile( search_path.c_str(), &data );
	if(find_handle == INVALID_HANDLE_VALUE)
		sLog.outError( "  No external scripts found! Server will continue to function with limited functionality." );
	else
	{
		do
		{
			string full_path = start_path + data.cFileName;
			HMODULE mod = LoadLibrary( full_path.c_str() );
			stringstream sstext;
			sstext << "  " << data.cFileName << " : 0x" << hex << reinterpret_cast< uint32* >( mod ) << dec << " : ";
			if( mod == 0 )
			{
				sstext << "error!";
				sLog.outError(sstext.str().c_str());
			}
			else
			{
				// find version import
				exp_get_version vcall = (exp_get_version)GetProcAddress(mod, "_exp_get_version");
				exp_script_register rcall = (exp_script_register)GetProcAddress(mod, "_exp_script_register");
				exp_get_script_type scall = (exp_get_script_type)GetProcAddress(mod, "_exp_get_script_type");
				if(vcall == 0 || rcall == 0 || scall == 0)
				{
					sstext << "version functions not found!";
					sLog.outError(sstext.str().c_str());
					FreeLibrary(mod);
				}
				else
				{
					uint32 version = vcall();
					uint32 stype = scall();
					if(SCRIPTLIB_LOPART(version) == static_cast<uint32>( SCRIPTLIB_VERSION_MINOR ) && SCRIPTLIB_HIPART(version) == static_cast<uint32>( SCRIPTLIB_VERSION_MAJOR ))
					{
						if( stype & SCRIPT_TYPE_SCRIPT_ENGINE )
						{
							sstext << "v"<< SCRIPTLIB_HIPART(version) << "." << SCRIPTLIB_LOPART(version) << " : ";
							sstext << "delayed load.";
							sLog.outBasic(sstext.str().c_str());

							ScriptingEngine se;
							se.Handle = mod;
							se.InitializeCall = rcall;
							se.Type = stype;

							ScriptEngines.push_back( se );
						}
						else
						{
							_handles.push_back(((SCRIPT_MODULE)mod));
							sstext << "v"<< SCRIPTLIB_HIPART(version) << "." << SCRIPTLIB_LOPART(version) << " : ";
							rcall(this);
							sstext << "loaded.";
							sLog.outBasic(sstext.str().c_str());
						}

						++count;
					}
					else
					{
						FreeLibrary(mod);
						sstext << "version mismatch!";
						sLog.outError(sstext.str().c_str());
					}
				}
			}
		}
		while(FindNextFile(find_handle, &data));
		FindClose(find_handle);
		Log.Success("Server","Loaded %u external libraries.", count);

		Log.Success("Server","Loading optional scripting engines...");
		for(vector<ScriptingEngine>::iterator itr = ScriptEngines.begin(); itr != ScriptEngines.end(); ++itr)
		{
			if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_LUA )
			{
				// lua :O
				if( sWorld.m_LuaEngine )
				{
					Log.Success("Server","Initializing LUA script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					FreeLibrary( itr->Handle );
				}
			}
			else
			{
				Log.Notice("Server","Unknown script engine type: 0x%.2X, please contact developers.", (*itr).Type );
				FreeLibrary( itr->Handle );
			}
		}
		Log.Success("Server","Done loading script engines...");
	}
#else
	/* Loading system for *nix */
	struct dirent ** list = NULL;
	int filecount = scandir(PREFIX "/lib/", &list, 0, 0);
	uint32 count = 0;

	if(!filecount || !list || filecount < 0)
		sLog.outError("  No external scripts found! Server will continue to function with limited functionality.");
	else
	{
char *ext;
		while(filecount--)
		{
			ext = strrchr(list[filecount]->d_name, '.');
#ifdef HAVE_DARWIN
			if (ext != NULL && strstr(list[filecount]->d_name, ".0.dylib") == NULL && !strcmp(ext, ".dylib")) {
#else
                        if (ext != NULL && !strcmp(ext, ".so")) {
#endif
				string full_path = "../lib/" + string(list[filecount]->d_name);
				SCRIPT_MODULE mod = dlopen(full_path.c_str(), RTLD_NOW);
				stringstream sstext;
				sstext << "  " << list[filecount]->d_name << " : 0x" << hex << mod << dec << " : ";
				if(mod == 0)
				{
					sstext << "error! " << dlerror();
					sLog.outError(sstext.str().c_str());
				}
				else
				{
					// find version import
					exp_get_version vcall = (exp_get_version)dlsym(mod, "_exp_get_version");
					exp_script_register rcall = (exp_script_register)dlsym(mod, "_exp_script_register");
					exp_get_script_type scall = (exp_get_script_type)dlsym(mod, "_exp_get_script_type");
					if(vcall == 0 || rcall == 0 || scall == 0)
					{
						sstext << "version functions not found!";
						sLog.outError(sstext.str().c_str());
						dlclose(mod);
					}
					else
					{
						int32 version = vcall();
						uint32 stype = scall();
						if(SCRIPTLIB_LOPART(version) == SCRIPTLIB_VERSION_MINOR && SCRIPTLIB_HIPART(version) == SCRIPTLIB_VERSION_MAJOR)
						{
							if( stype & SCRIPT_TYPE_SCRIPT_ENGINE )
							{
								sstext << "v"<< SCRIPTLIB_HIPART(version) << "." << SCRIPTLIB_LOPART(version) << " : ";
								sstext << "delayed load.";
								sLog.outBasic(sstext.str().c_str());

								ScriptingEngine se;
								se.Handle = mod;
								se.InitializeCall = rcall;
								se.Type = stype;

								ScriptEngines.push_back( se );
							}
							else
							{
								_handles.push_back(((SCRIPT_MODULE)mod));
								sstext << "v"<< SCRIPTLIB_HIPART(version) << "." << SCRIPTLIB_LOPART(version) << " : ";
								rcall(this);
								sstext << "loaded.";
								sLog.outBasic(sstext.str().c_str());						
							}

							++count;
						}
						else
						{
							dlclose(mod);
							sstext << "version mismatch!";
							sLog.outError(sstext.str().c_str());					
						}
					}
				}
			}
			free(list[filecount]);
		}
		free(list);
		sLog.outString("Loaded %u external libraries.", count);

		sLog.outString("Loading optional scripting engines...");
		for(vector<ScriptingEngine>::iterator itr = ScriptEngines.begin(); itr != ScriptEngines.end(); ++itr)
		{
			if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_LUA )
			{
				// lua :O
				if( sWorld.m_LuaEngine )
				{
					sLog.outString("   Initializing LUA script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					dlclose( itr->Handle );
				}
			}
			else
			{
				sLog.outError("  Unknown script engine type: 0x%.2X, please contact developers.", (*itr).Type );
				dlclose( itr->Handle );
			}
		}
		sLog.outString("Done loading script engines...");
	}
#endif
}

void ScriptMgr::UnloadScripts()
{
	if(HookInterface::getSingletonPtr())
		delete HookInterface::getSingletonPtr();

	for(CustomGossipScripts::iterator itr = _customgossipscripts.begin(); itr != _customgossipscripts.end(); ++itr)
		(*itr)->Destroy();
	_customgossipscripts.clear();
	delete this->DefaultGossipScript;
	this->DefaultGossipScript= NULL;

	for(QuestScripts::iterator itr = _questscripts.begin(); itr != _questscripts.end(); ++itr)
		delete *itr;
	_questscripts.clear();

	LibraryHandleMap::iterator itr = _handles.begin();
	for(; itr != _handles.end(); ++itr)
	{
#ifdef WIN32
		FreeLibrary(((HMODULE)*itr));
#else
		dlclose(*itr);
#endif
	}
	_handles.clear();
}

void ScriptMgr::DumpUnimplementedSpells(){
	std::ofstream of;

	sLog.outBasic("Dumping IDs for spells with unimplemented dummy/script effect(s)");
	uint32 count = 0;

	of.open( "unimplemented1.txt" );

	for( DBCStorage< SpellEntry >::iterator itr = dbcSpell.begin(); itr != dbcSpell.end(); ++itr ){
		SpellEntry *sp = *itr;

		if( !sp->HasEffect( SPELL_EFFECT_DUMMY ) && !sp->HasEffect( SPELL_EFFECT_SCRIPT_EFFECT ) && !sp->HasEffect( SPELL_EFFECT_SEND_EVENT ) )
			continue;

		HandleDummySpellMap::iterator sitr = _spells.find( sp->Id );
		if( sitr != _spells.end() )
			continue;

		HandleScriptEffectMap::iterator seitr = SpellScriptEffects.find( sp->Id );
		if( seitr != SpellScriptEffects.end() )
			continue;
		
		std::stringstream ss;
		ss << sp->Id;
		ss << std::endl;

		of.write( ss.str().c_str(), ss.str().length() );

		count++;
	}

	of.close();

	sLog.outBasic("Dumped %u IDs.", count );

	sLog.outBasic("Dumping IDs for spells with unimplemented dummy aura effect.");

	std::ofstream of2;
	of2.open( "unimplemented2.txt" );

	count = 0;

	for( DBCStorage< SpellEntry >::iterator itr = dbcSpell.begin(); itr != dbcSpell.end(); ++itr ){
		SpellEntry *sp = *itr;

		if( !sp->AppliesAura( SPELL_AURA_DUMMY ) )
			continue;

		HandleDummyAuraMap::iterator ditr = _auras.find( sp->Id );
		if( ditr != _auras.end() )
			continue;

		std::stringstream ss;
		ss << sp->Id;
		ss << std::endl;

		of2.write( ss.str().c_str(), ss.str().length() );

		count++;
	}

	of2.close();

	sLog.outBasic("Dumped %u IDs.", count );
}

void ScriptMgr::register_creature_script(uint32 entry, exp_create_creature_ai callback)
{
	if(_creatures.find(entry) != _creatures.end())
		sLog.outError("ScriptMgr is trying to register a script for Creature ID: %u even if there's already one for that Creature. Remove one of those scripts.", entry);

	_creatures.insert( CreatureCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback)
{
	if(_gameobjects.find(entry) != _gameobjects.end())
		sLog.outError("ScriptMgr is trying to register a script for GameObject ID: %u even if there's already one for that GameObject. Remove one of those scripts.", entry);

	_gameobjects.insert( GameObjectCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback)
{
	if( _auras.find(entry) != _auras.end() ){
		sLog.outError("ScriptMgr is trying to register a script for Aura ID: %u even if there's already one for that Aura. Remove one of those scripts.", entry);
	}

	SpellEntry *sp = dbcSpell.LookupEntryForced( entry );
	if( sp == NULL ){
		sLog.outError("ScriptMgr is trying to register a dummy aura handler for Spell ID: %u which is invalid.", entry );
		return;
	}

	if( !sp->AppliesAura( SPELL_AURA_DUMMY ) && !sp->AppliesAura( SPELL_AURA_PERIODIC_TRIGGER_DUMMY ) )
		sLog.outError("ScriptMgr has registered a dummy aura handler for Spell ID: %u ( %s ), but spell has no dummy aura!", entry, sp->Name );

	_auras.insert( HandleDummyAuraMap::value_type( entry, callback ) );
}

void ScriptMgr::register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback)
{
	if(_spells.find(entry) != _spells.end()){
		sLog.outError("ScriptMgr is trying to register a script for Spell ID: %u even if there's already one for that Spell. Remove one of those scripts.", entry);
		return;
	}

	SpellEntry *sp = dbcSpell.LookupEntryForced( entry );
	if( sp == NULL ){
		sLog.outError("ScriptMgr is trying to register a dummy handler for Spell ID: %u which is invalid.", entry );
		return;
	}

	if( !sp->HasEffect( SPELL_EFFECT_DUMMY ) )
		sLog.outError("ScriptMgr has registered a dummy handler for Spell ID: %u ( %s ), but spell has no dummy effect!", entry, sp->Name );

	_spells.insert( HandleDummySpellMap::value_type( entry, callback ) );
}

void ScriptMgr::register_gossip_script(uint32 entry, GossipScript * gs)
{
	CreatureInfo * ci = CreatureNameStorage.LookupEntry(entry);
	if(ci)
	{
		if(ci->gossip_script != DefaultGossipScript)
			sLog.outError("ScriptMgr is trying to register a gossip for Creature ID: %u even if there's already one for that Creature. Remove one of those gossips.", entry);

		ci->gossip_script = gs;
	}

	_customgossipscripts.insert(gs);
}

void ScriptMgr::register_go_gossip_script(uint32 entry, GossipScript * gs)
{
	GameObjectInfo * gi = GameObjectNameStorage.LookupEntry(entry);
	if(gi)
	{
		if(gi->gossip_script != NULL)
			sLog.outError("ScriptMgr is trying to register a gossip for GameObject ID: %u even if there's already one for that GameObject. Remove one of those gossips.", entry);

		gi->gossip_script = gs;
	}

	_customgossipscripts.insert(gs);
}

void ScriptMgr::register_quest_script(uint32 entry, QuestScript * qs)
{
	Quest * q = QuestStorage.LookupEntry( entry );
	if( q != NULL )
	{
		if(q->pQuestScript != NULL)
		sLog.outError("ScriptMgr is trying to register a script for Quest ID: %u even if there's already one for that Quest. Remove one of those scripts.", entry);

		q->pQuestScript = qs;
	}

	_questscripts.insert( qs );
}

void ScriptMgr::register_instance_script( uint32 pMapId, exp_create_instance_ai pCallback ) 
{ 
	if(mInstances.find(pMapId) != mInstances.end())
		sLog.outError("ScriptMgr is trying to register a script for Instance ID: %u even if there's already one for that Instance. Remove one of those scripts.", pMapId);

	mInstances.insert( InstanceCreateMap::value_type( pMapId, pCallback ) ); 
}; 

void ScriptMgr::register_creature_script( uint32* entries, exp_create_creature_ai callback ) 
{ 
	for( uint32 y = 0; entries[y] != 0; y++)
	{
		register_creature_script(entries[y], callback);
	}
};

void ScriptMgr::register_gameobject_script( uint32* entries, exp_create_gameobject_ai callback ) 
{ 
	for( uint32 y = 0; entries[y] != 0; y++)
	{
		register_gameobject_script(entries[y], callback);
	}
};

void ScriptMgr::register_dummy_aura( uint32* entries, exp_handle_dummy_aura callback ) 
{ 
	for( uint32 y = 0; entries[y] != 0; y++)
	{
		register_dummy_aura(entries[y], callback);
	}
};

void ScriptMgr::register_dummy_spell( uint32* entries, exp_handle_dummy_spell callback ) 
{ 
	for( uint32 y = 0; entries[y] != 0; y++)
	{
		register_dummy_spell(entries[y], callback);
	}
};

void ScriptMgr::register_script_effect( uint32* entries, exp_handle_script_effect callback ) 
{
	for( uint32 y = 0; entries[y] != 0; y++)
	{
		register_script_effect(entries[y], callback);
	}
};

void ScriptMgr::register_script_effect( uint32 entry, exp_handle_script_effect callback ){
	
	HandleScriptEffectMap::iterator itr = SpellScriptEffects.find( entry );
	
	if( itr != SpellScriptEffects.end() ){
		sLog.outError("ScriptMgr tried to register more than 1 script effect handlers for Spell %u", entry );
		return;
	}

	SpellEntry *sp = dbcSpell.LookupEntryForced( entry );
	if( sp == NULL ){
		sLog.outError("ScriptMgr tried to register a script effect handler for Spell %u, which is invalid.", entry );
		return;
	}

	if( !sp->HasEffect( SPELL_EFFECT_SCRIPT_EFFECT ) && !sp->HasEffect( SPELL_EFFECT_SEND_EVENT ) )
		sLog.outError("ScriptMgr has registered a script effect handler for Spell ID: %u ( %s ), but spell has no scripted effect!", entry, sp->Name );

	SpellScriptEffects.insert( std::pair< uint32, exp_handle_script_effect >( entry, callback ) );
}

CreatureAIScript* ScriptMgr::CreateAIScriptClassForEntry(Creature* pCreature)
{
	CreatureCreateMap::iterator itr = _creatures.find(pCreature->GetEntry());
	if(itr == _creatures.end())
		return NULL;

	exp_create_creature_ai function_ptr = itr->second;
	return (function_ptr)(pCreature);
}

GameObjectAIScript * ScriptMgr::CreateAIScriptClassForGameObject(uint32 uEntryId, GameObject* pGameObject)
{
	GameObjectCreateMap::iterator itr = _gameobjects.find(pGameObject->GetEntry());
	if(itr == _gameobjects.end())
		return NULL;

	exp_create_gameobject_ai function_ptr = itr->second;
	return (function_ptr)(pGameObject);
}

InstanceScript* ScriptMgr::CreateScriptClassForInstance( uint32 pMapId, MapMgr* pMapMgr ) 
{ 
	InstanceCreateMap::iterator Iter = mInstances.find( pMapMgr->GetMapId() ); 
		if ( Iter == mInstances.end() ) 
			return NULL; 
		exp_create_instance_ai function_ptr = Iter->second; 
			return ( function_ptr )( pMapMgr ); 
}; 

bool ScriptMgr::CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell)
{
	HandleDummySpellMap::iterator itr = _spells.find(uSpellId);
	if(itr == _spells.end())
		return false;

	exp_handle_dummy_spell function_ptr = itr->second;
	return (function_ptr)(i, pSpell);
}

bool ScriptMgr::HandleScriptedSpellEffect( uint32 SpellId, uint32 i, Spell *s ){
	HandleScriptEffectMap::iterator itr = SpellScriptEffects.find( SpellId );
	if( itr == SpellScriptEffects.end() )
		return false;

	exp_handle_script_effect ptr = itr->second;
	return (ptr)( i, s );
}

bool ScriptMgr::CallScriptedDummyAura(uint32 uSpellId, uint32 i, Aura* pAura, bool apply)
{
	HandleDummyAuraMap::iterator itr = _auras.find(uSpellId);
	if(itr == _auras.end())
		return false;

	exp_handle_dummy_aura function_ptr = itr->second;
	return (function_ptr)(i, pAura, apply);
}

bool ScriptMgr::CallScriptedItem(Item * pItem, Player * pPlayer)
{
	if(pItem->GetProto()->gossip_script)
	{
		pItem->GetProto()->gossip_script->GossipHello(pItem,pPlayer,true);
		return true;
	}
	
	return false;
}

void ScriptMgr::register_item_gossip_script(uint32 entry, GossipScript * gs)
{
	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(entry);
	if(proto)
	{
		if(proto->gossip_script != NULL)
		sLog.outError("ScriptMgr is trying to register a gossip for Item ID: %u even if there's already one for that Item. Remove one of those gossips.", entry);

		proto->gossip_script = gs;
	}

	_customgossipscripts.insert(gs);
}

/* CreatureAI Stuff */
CreatureAIScript::CreatureAIScript(Creature* creature) : _unit(creature), linkedCreatureAI(NULL)
{

}

CreatureAIScript::~CreatureAIScript()
{
	//notify our linked creature that we are being deleted.
	if( linkedCreatureAI != NULL )
		linkedCreatureAI->LinkedCreatureDeleted();
}

void CreatureAIScript::RegisterAIUpdateEvent(uint32 frequency)
{
	//sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0,0);
	sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
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
	if( linkedCreatureAI != NULL )
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



/* QuestScript Stuff */

/* Gossip Stuff*/

GossipScript::GossipScript()
{
	
}

void GossipScript::GossipEnd(Object* pObject, Player* Plr)
{
	Plr->CleanupGossipMenu();
}

bool CanTrainAt(Player * plr, Trainer * trn);
void GossipScript::GossipHello(Object* pObject, Player* Plr, bool AutoSend)
{
	GossipMenu *Menu;
	uint32 TextID = 2;
	if( !pObject->IsCreature() )
		return;
	Creature * pCreature = TO_CREATURE( pObject );

	uint32 Text = objmgr.GetGossipTextForNpc(pCreature->GetEntry());
	if(Text != 0)
	{
		GossipText * text = NpcTextStorage.LookupEntry(Text);
		if(text != 0)
			TextID = Text;
	}

	if (pCreature->isSpiritHealer())
	{
		Plr->GetSession()->SendSpiritHealerRequest(pCreature);
		return;
	}

	objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), TextID, Plr);
	
	if (pCreature->isVendor())
	{
		VendorRestrictionEntry * vendor = VendorRestrictionEntryStorage.LookupEntry(pCreature->GetProto()->Id);
		if( Plr->CanBuyAt(vendor) )
			Menu->AddItem(1, Plr->GetSession()->LocalizedWorldSrv(1), 1);
		else
			Menu->SetTextID(vendor->cannotbuyattextid);
	}
	
	if( pCreature->isTrainer() || pCreature->isProf() || pCreature->isClass() )
	{
		Trainer *pTrainer = pCreature->GetTrainer();
		if(!pTrainer)
		{
			if(AutoSend)
				Menu->SendTo(Plr);
			return;
		}
		string name = pCreature->GetCreatureInfo()->Name;
		string::size_type pos = name.find(" ");	  // only take first name

		if(pos != string::npos)
			name = name.substr(0, pos);

		if(CanTrainAt(Plr, pTrainer))
			Menu->SetTextID(pTrainer->Can_Train_Gossip_TextId);
		else
			Menu->SetTextID(pTrainer->Cannot_Train_GossipTextId);

		if( pTrainer->TrainerType != TRAINER_TYPE_PET )
		{
            // I seek
			string msg = string(Plr->GetSession()->LocalizedWorldSrv(2));

            
			if(pTrainer->RequiredClass)
			{
                
				switch(Plr->getClass())
				{
				case MAGE:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(3));
					break;
				case SHAMAN:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(4));
					break;
				case WARRIOR:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(5));
					break;
				case PALADIN:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(6));
					break;
				case WARLOCK:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(7));
					break;
				case HUNTER:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(8));
					break;
				case ROGUE:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(9));
					break;
				case DRUID:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(10));
					break;
				case PRIEST:
					msg += string(Plr->GetSession()->LocalizedWorldSrv(11));
					break;
                case DEATHKNIGHT:
                    msg += string(Plr->GetSession()->LocalizedWorldSrv(82));
                    break;
				}

                //training
				msg += " "+string(Plr->GetSession()->LocalizedWorldSrv(12))+", ";
				msg += name;
				msg += ".";

                // Sending the message itself
				Menu->AddItem(3, msg.c_str(), 2);

			}
			else
			{
                // training
				msg += string(Plr->GetSession()->LocalizedWorldSrv(12))+", ";
				msg += name;
				msg += ".";

				Menu->AddItem(3, msg.c_str(), 2);
			}
		}
		else
		{
			// Pet trainer menuitem
			Menu->AddItem(3, Plr->GetSession()->LocalizedWorldSrv(13), 2);
		}

        // talent reset menuitem and dual spec
		if (pTrainer->RequiredClass &&					  // class trainer
			pTrainer->RequiredClass == Plr->getClass() &&   // correct class
			pCreature->getLevel() > 10 &&				   // creature level
			pTrainer->TrainerType != TRAINER_TYPE_PET &&  // Pet Trainers do not respec hunter talents
			Plr->getLevel() >= 10 )						  // player level
		{
			Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(22), 11);
			if(Plr->getLevel() >= 40 && Plr->m_talentSpecsCount < 2)
				Menu->AddItem(0, "Learn about Dual Talent Specialization.", 15);
		}
	
        // pet untraining menuitem
		if (pTrainer->TrainerType == TRAINER_TYPE_PET &&	// pet trainer type
			Plr->getClass() == HUNTER &&					// hunter class
			Plr->GetSummon() != NULL )						// have pet
		{
			Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(23), 13);
		}

	} // end architype trainer / profession trainer

	if (pCreature->isTaxi())
		Menu->AddItem(2, Plr->GetSession()->LocalizedWorldSrv(14), 3);

	if (pCreature->isAuctioner())
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(15), 4);

	if (pCreature->isInnkeeper())
		Menu->AddItem(5, Plr->GetSession()->LocalizedWorldSrv(16), 5);

	if (pCreature->isBattleMaster())
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(21), 10);

	if (pCreature->isBanker())
	{
		if(pCreature->isQuestGiver())
			Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(17), 6);
		else
		{
			Plr->GetSession()->SendBankerList(pCreature);
			return;
		}
	}

	if (pCreature->isCharterGiver())
	{
		if( pCreature->isTabardDesigner() )
			Menu->AddItem(0, "How do I create a guild?", 8);
		else
			Menu->AddItem(0, "How do I create a arena team?", 8);
	}

	if (pCreature->isTabardDesigner())
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(20), 9);

	if ( pCreature->isStableMaster() && Plr->getClass() == HUNTER )
		Menu->AddItem(0, "I'd like to stable my pet here.", 17);

	if(AutoSend)
		Menu->SendTo(Plr);
}

void GossipScript::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
{
	if( !pObject->IsCreature() )
		return;

	Creature* pCreature = static_cast< Creature* >( pObject );

	sLog.outDebug("GossipSelectOption: Id = %u, IntId = %u", Id, IntId);

	switch( IntId )
	{
	case 1:
		// vendor
		Plr->GetSession()->SendInventoryList(pCreature);
		break;
	case 2:
		// trainer
		Plr->GetSession()->SendTrainerList(pCreature);
		break;
	case 3:
		// taxi
		Plr->GetSession()->SendTaxiList(pCreature);
		break;
	case 4:
		// auction
		Plr->GetSession()->SendAuctionList(pCreature);
		break;
	case 5:
		// innkeeper
		Plr->GetSession()->SendInnkeeperBind(pCreature);
		break;
	case 6:
		// banker
		Plr->GetSession()->SendBankerList(pCreature);
		break;
	case 7:
		// spirit
		Plr->GetSession()->SendSpiritHealerRequest(pCreature);
		break;
	case 8:
		// petition
		Plr->GetSession()->SendCharterRequest(pCreature);
		break;
	case 9:
		// guild crest
		Plr->GetSession()->SendTabardHelp(pCreature);
		break;
	case 10:
		// battlefield
		Plr->GetSession()->SendBattlegroundList(pCreature, 0);
		break;
	case 11:
		// switch to talent reset message
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 5674, Plr);
            // I understand
			Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(24), 12);
			Menu->SendTo(Plr);
		}break;
	case 12:
		// talent reset
		{
			Plr->Gossip_Complete();
			Plr->SendTalentResetConfirm();
		}break;
	case 13:
		// switch to untrain message
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 7722, Plr);
            Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(25), 14);
			Menu->SendTo(Plr);
		}break;
	case 14:
		// untrain pet
		{
			Plr->Gossip_Complete();
			Plr->SendPetUntrainConfirm();
		}break;

	case 15: // purchase dual spec
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 14136, Plr);
			Menu->AddMenuItem(0, "Purchase a Dual Talent Specialization.", 0, 16, "Are you sure you would like to purchase your second talent specialization", 10000000, false);
			Menu->SendTo(Plr);
		}break;

	case 16: // end dual spec dialog
		{
			if( !Plr->HasGold(10000000) )
			{
				Plr->GetSession()->SendNotification("You do not have enough gold to purchase a dual spec."); // I know this is not correct
				Plr->Gossip_Complete();
				return;
			}
			Plr->ModGold( -10000000 );
			Plr->m_talentSpecsCount = 2;
			Plr->Reset_Talents();
			Plr->CastSpell(Plr, 63624, true); // Show activate spec buttons
			Plr->CastSpell(Plr, 63706, true); // Allow primary spec to be activated
			Plr->CastSpell(Plr, 63707, true); // Allow secondary spec to be activated
			Plr->SaveToDB(false); // hai gm i bought dual spec but no werk plis gief mi 1000g back - GTFO you never bought anything
			Plr->Gossip_Complete();
		}
		break;

	case 17:
		Plr->GetSession()->SendStabledPetList(pCreature->GetGUID());
		break;

	default:
		sLog.outError("Unknown IntId %u on entry %u", IntId, pCreature->GetEntry());
		break;
	}	
}

void GossipScript::Destroy()
{
	delete this;
}

/* InstanceAI Stuff */ 

InstanceScript::InstanceScript( MapMgr* pMapMgr ) : mInstance( pMapMgr ) 
{
};

void InstanceScript::RegisterUpdateEvent( uint32 pFrequency ) 
{
	sEventMgr.AddEvent( mInstance, &MapMgr::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, pFrequency, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT ); 
};
void InstanceScript::ModifyUpdateEvent( uint32 pNewFrequency ) 
{
	sEventMgr.ModifyEventTimeAndTimeLeft( mInstance, EVENT_SCRIPT_UPDATE_EVENT, pNewFrequency );
};
void InstanceScript::RemoveUpdateEvent()
{
	sEventMgr.RemoveEvents( mInstance, EVENT_SCRIPT_UPDATE_EVENT ); 
};

/* Hook Stuff */
void ScriptMgr::register_hook(ServerHookEvents event, void * function_pointer)
{
	Arcemu::Util::ARCEMU_ASSERT(   event < NUM_SERVER_HOOKS);
	_hooks[event].push_back(function_pointer);
}

/* Hook Implementations */
bool HookInterface::OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_NEW_CHARACTER];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnNewCharacter)*itr)(Race, Class, Session, Name);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnKillPlayer(Player * pPlayer, Player * pVictim)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_KILL_PLAYER];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnKillPlayer)*itr)(pPlayer, pVictim);
}

void HookInterface::OnFirstEnterWorld(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnFirstEnterWorld)*itr)(pPlayer);
}

void HookInterface::OnCharacterCreate(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_CHARACTER_CREATE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOCharacterCreate)*itr)(pPlayer);
}

void HookInterface::OnEnterWorld(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ENTER_WORLD];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEnterWorld)*itr)(pPlayer);
}

void HookInterface::OnGuildCreate(Player * pLeader, Guild * pGuild)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_GUILD_CREATE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnGuildCreate)*itr)(pLeader, pGuild);
}

void HookInterface::OnGuildJoin(Player * pPlayer, Guild * pGuild)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_GUILD_JOIN];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnGuildJoin)*itr)(pPlayer, pGuild);
}

void HookInterface::OnDeath(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_DEATH];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnDeath)*itr)(pPlayer);
}

bool HookInterface::OnRepop(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_REPOP];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnRepop)*itr)(pPlayer);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnEmote(Player * pPlayer, uint32 Emote, Unit * pUnit)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_EMOTE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEmote)*itr)(pPlayer, Emote, pUnit);
}

void HookInterface::OnEnterCombat(Player * pPlayer, Unit * pTarget)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ENTER_COMBAT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEnterCombat)*itr)(pPlayer, pTarget);
}

bool HookInterface::OnCastSpell(Player * pPlayer, SpellEntry* pSpell, Spell * spell)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_CAST_SPELL];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnCastSpell)*itr)(pPlayer, pSpell, spell);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

bool HookInterface::OnLogoutRequest(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnLogoutRequest)*itr)(pPlayer);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnLogout(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_LOGOUT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnLogout)*itr)(pPlayer);
}

void HookInterface::OnQuestAccept(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_QUEST_ACCEPT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnQuestAccept)*itr)(pPlayer, pQuest, pQuestGiver);
}

void HookInterface::OnZone(Player * pPlayer, uint32 zone, uint32 oldZone)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ZONE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnZone)*itr)(pPlayer, zone, oldZone);
}

bool HookInterface::OnChat(Player * pPlayer, uint32 type, uint32 lang, const char * message, const char * misc)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_CHAT];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnChat)*itr)(pPlayer, type, lang, message, misc);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}

void HookInterface::OnLoot(Player * pPlayer, Unit * pTarget, uint32 money, uint32 itemId)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_LOOT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnLoot)*itr)(pPlayer, pTarget, money, itemId);
}

void HookInterface::OnObjectLoot(Player * pPlayer, Object * pTarget, uint32 money, uint32 itemId)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_OBJECTLOOT];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnObjectLoot)*itr)(pPlayer, pTarget, money, itemId);
}

void HookInterface::OnFullLogin(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_FULL_LOGIN];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnEnterWorld)*itr)(pPlayer);
}

void HookInterface::OnQuestCancelled(Player * pPlayer, Quest * pQuest)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_QUEST_CANCELLED];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnQuestCancel)*itr)(pPlayer, pQuest);
}

void HookInterface::OnQuestFinished(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_QUEST_FINISHED];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnQuestFinished)*itr)(pPlayer, pQuest, pQuestGiver);
}

void HookInterface::OnHonorableKill(Player * pPlayer, Player * pKilled)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_HONORABLE_KILL];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnHonorableKill)*itr)(pPlayer, pKilled);
}

void HookInterface::OnArenaFinish(Player * pPlayer, ArenaTeam* pTeam, bool victory, bool rated)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ARENA_FINISH];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnArenaFinish)*itr)(pPlayer, pTeam, victory, rated);
}

void HookInterface::OnAreaTrigger(Player * pPlayer, uint32 areaTrigger)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_AREATRIGGER];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnAreaTrigger)*itr)(pPlayer, areaTrigger);
}

void HookInterface::OnPostLevelUp(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_POST_LEVELUP];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnPostLevelUp)*itr)(pPlayer);
}

bool HookInterface::OnPreUnitDie(Unit *killer, Unit *victim)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_PRE_DIE];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnPreUnitDie)*itr)(killer, victim);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}
		

void HookInterface::OnAdvanceSkillLine(Player * pPlayer, uint32 skillLine, uint32 current)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE];
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
		((tOnAdvanceSkillLine)*itr)(pPlayer, skillLine, current);
}

void HookInterface::OnDuelFinished(Player * Winner, Player * Looser)
 {
       ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_DUEL_FINISHED];
       for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
               ((tOnDuelFinished)*itr)(Winner, Looser);
 }

void HookInterface::OnAuraRemove(Aura * aura)
 {
       ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_AURA_REMOVE];
       for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
               ((tOnAuraRemove)*itr)(aura);
 }

bool HookInterface::OnResurrect(Player * pPlayer)
{
	ServerHookList hookList = sScriptMgr._hooks[SERVER_HOOK_EVENT_ON_RESURRECT];
	bool ret_val = true;
	for(ServerHookList::iterator itr = hookList.begin(); itr != hookList.end(); ++itr)
	{
		bool rv = ((tOnResurrect)*itr)(pPlayer);
		if (rv == false) // never set ret_val back to true, once it's false
			ret_val = false;
	}
	return ret_val;
}