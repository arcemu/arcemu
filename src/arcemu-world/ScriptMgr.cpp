/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

	Log.Notice("Server","Loading External Script Libraries...");
	sLog.outString("");

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
			printf( "  %s : 0x%p : ", data.cFileName, reinterpret_cast< uint32* >( mod ));
			if( mod == 0 )
			{
				printf( "error!\n" );
			}
			else
			{
				// find version import
				exp_get_version vcall = (exp_get_version)GetProcAddress(mod, "_exp_get_version");
				exp_script_register rcall = (exp_script_register)GetProcAddress(mod, "_exp_script_register");
				exp_get_script_type scall = (exp_get_script_type)GetProcAddress(mod, "_exp_get_script_type");
				if(vcall == 0 || rcall == 0 || scall == 0)
				{
					printf("version functions not found!\n");
					FreeLibrary(mod);
				}
				else
				{
					uint32 version = vcall();
					uint32 stype = scall();
					if(SCRIPTLIB_LOPART(version) == SCRIPTLIB_VERSION_MINOR && SCRIPTLIB_HIPART(version) == SCRIPTLIB_VERSION_MAJOR)
					{
						if( stype & SCRIPT_TYPE_SCRIPT_ENGINE )
						{
							printf("v%u.%u : ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
							printf("delayed load.\n");

							ScriptingEngine se;
							se.Handle = mod;
							se.InitializeCall = rcall;
							se.Type = stype;

							ScriptEngines.push_back( se );
						}
						else
						{
							_handles.push_back(((SCRIPT_MODULE)mod));
							printf("v%u.%u : ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
							rcall(this);
							printf("loaded.\n");						
						}

						++count;
					}
					else
					{
						FreeLibrary(mod);
						printf("version mismatch!\n");						
					}
				}
			}
		}
		while(FindNextFile(find_handle, &data));
		FindClose(find_handle);
		sLog.outString("");
		Log.Notice("Server","Loaded %u external libraries.", count);
		sLog.outString("");

		Log.Notice("Server","Loading optional scripting engines...");
		for(vector<ScriptingEngine>::iterator itr = ScriptEngines.begin(); itr != ScriptEngines.end(); ++itr)
		{
			if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_LUA )
			{
				// lua :O
				if( sWorld.m_LuaEngine )
				{
					Log.Notice("Server","Initializing LUA script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					FreeLibrary( itr->Handle );
				}
			}
			else if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_AS )
			{
				if( sWorld.m_ASEngine )
				{
					Log.Notice("Server","Initializing AngelScript script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					FreeLibrary( (*itr).Handle );
				}
			}
			else
			{
				Log.Notice("Server","Unknown script engine type: 0x%.2X, please contact developers.", (*itr).Type );
				FreeLibrary( itr->Handle );
			}
		}
		Log.Notice("Server","Done loading script engines...");
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
				printf("  %s : 0x%p : ", list[filecount]->d_name, mod);
				if(mod == 0)
					printf("error! [%s]\n", dlerror());
				else
				{
					// find version import
					exp_get_version vcall = (exp_get_version)dlsym(mod, "_exp_get_version");
					exp_script_register rcall = (exp_script_register)dlsym(mod, "_exp_script_register");
					exp_get_script_type scall = (exp_get_script_type)dlsym(mod, "_exp_get_script_type");
					if(vcall == 0 || rcall == 0 || scall == 0)
					{
						printf("version functions not found!\n");
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
								printf("v%u.%u : ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
								printf("delayed load.\n");

								ScriptingEngine se;
								se.Handle = mod;
								se.InitializeCall = rcall;
								se.Type = stype;

								ScriptEngines.push_back( se );
							}
							else
							{
								_handles.push_back(((SCRIPT_MODULE)mod));
								printf("v%u.%u : ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
								rcall(this);
								printf("loaded.\n");						
							}

							++count;
						}
						else
						{
							dlclose(mod);
							printf("version mismatch!\n");						
						}
					}
				}
			}
			free(list[filecount]);
		}
		free(list);
		sLog.outString("");
		sLog.outString("Loaded %u external libraries.", count);
		sLog.outString("");

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
			else if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_AS )
			{
				if( sWorld.m_ASEngine )
				{
					sLog.outString("   Initializing AngelScript script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					dlclose( (*itr).Handle );
				}
			}
			else
			{
				sLog.outString("  Unknown script engine type: 0x%.2X, please contact developers.", (*itr).Type );
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
	this->DefaultGossipScript=NULL;

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

void ScriptMgr::register_creature_script(uint32 entry, exp_create_creature_ai callback)
{
	_creatures.insert( CreatureCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback)
{
	_gameobjects.insert( GameObjectCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback)
{
	_auras.insert( HandleDummyAuraMap::value_type( entry, callback ) );
}

void ScriptMgr::register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback)
{
	_spells.insert( HandleDummySpellMap::value_type( entry, callback ) );
}

void ScriptMgr::register_gossip_script(uint32 entry, GossipScript * gs)
{
	CreatureInfo * ci = CreatureNameStorage.LookupEntry(entry);
	if(ci)
		ci->gossip_script = gs;

	_customgossipscripts.insert(gs);
}

void ScriptMgr::register_go_gossip_script(uint32 entry, GossipScript * gs)
{
	GameObjectInfo * gi = GameObjectNameStorage.LookupEntry(entry);
	if(gi)
		gi->gossip_script = gs;

	_customgossipscripts.insert(gs);
}

void ScriptMgr::register_quest_script(uint32 entry, QuestScript * qs)
{
	Quest * q = QuestStorage.LookupEntry( entry );
	if( q != NULL )
		q->pQuestScript = qs;

	_questscripts.insert( qs );
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

bool ScriptMgr::CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell)
{
	HandleDummySpellMap::iterator itr = _spells.find(uSpellId);
	if(itr == _spells.end())
		return false;

	exp_handle_dummy_spell function_ptr = itr->second;
	return (function_ptr)(i, pSpell);
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
	if(pItem->GetProto() && pItem->GetProto()->gossip_script)
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
		proto->gossip_script = gs;

	_customgossipscripts.insert(gs);
}

/* CreatureAI Stuff */
CreatureAIScript::CreatureAIScript(Creature* creature) : _unit(creature)
{

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
	sEventMgr.AddEvent(_gameobject, &GameObject::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0,0);
}


/* InstanceAI Stuff */

InstanceScript::InstanceScript(MapMgr *instance) : _instance(instance)
{
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
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?static_cast< Creature* >( pObject ):NULL;
	if(!pCreature)
		return;

	Trainer *pTrainer = pCreature->GetTrainer();
	uint32 Text = objmgr.GetGossipTextForNpc(pCreature->GetEntry());
	if(Text != 0)
	{
		GossipText * text = NpcTextStorage.LookupEntry(Text);
		if(text != 0)
			TextID = Text;
	}

	objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), TextID, Plr);
	
	uint32 flags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);

	if(flags & UNIT_NPC_FLAG_VENDOR)
		Menu->AddItem(1, Plr->GetSession()->LocalizedWorldSrv(1), 1);
	
	if((flags & UNIT_NPC_FLAG_TRAINER || flags & UNIT_NPC_FLAG_TRAINER_PROF || flags & UNIT_NPC_FLAG_TRAINER_CLASS) && pTrainer != 0)
	{
		string name = pCreature->GetCreatureInfo()->Name;
		string::size_type pos = name.find(" ");	  // only take first name
		if(pos != string::npos)
			name = name.substr(0, pos);

		if(CanTrainAt(Plr, pTrainer))
			Menu->SetTextID(pTrainer->Can_Train_Gossip_TextId);
		else
			Menu->SetTextID(pTrainer->Cannot_Train_GossipTextId);
        if(pTrainer->TrainerType != TRAINER_TYPE_PET)
		{
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
				}
				msg += " "+string(Plr->GetSession()->LocalizedWorldSrv(12))+", ";
				msg += name;
				msg += ".";

				Menu->AddItem(3, msg.c_str(), 2);

			}
			else
			{
				msg += string(Plr->GetSession()->LocalizedWorldSrv(12))+", ";
				msg += name;
				msg += ".";

				Menu->AddItem(3, msg.c_str(), 2);
			}
		}
		else
		{
			
			Menu->AddItem(3, Plr->GetSession()->LocalizedWorldSrv(13), 2);
		}
	}

	if(flags & UNIT_NPC_FLAG_TAXIVENDOR)
		Menu->AddItem(2, Plr->GetSession()->LocalizedWorldSrv(14), 3);

	if(flags & UNIT_NPC_FLAG_AUCTIONEER)
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(15), 4);

	if(flags & UNIT_NPC_FLAG_INNKEEPER)
		Menu->AddItem(5, Plr->GetSession()->LocalizedWorldSrv(16), 5);

	if(flags & UNIT_NPC_FLAG_BANKER)
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(17), 6);

	if(flags & UNIT_NPC_FLAG_SPIRITHEALER)
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(18), 7);

	if(flags & UNIT_NPC_FLAG_ARENACHARTER)
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(19), 8);

	if(flags & UNIT_NPC_FLAG_TABARDCHANGER)
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(20), 9);

	if(flags & UNIT_NPC_FLAG_BATTLEFIELDPERSON)
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(21), 10);

	if( pTrainer &&
		pTrainer->RequiredClass &&					  // class trainer
		pTrainer->RequiredClass == Plr->getClass() &&   // correct class
		pCreature->getLevel() > 10 &&				   // creature level
		pTrainer->TrainerType != TRAINER_TYPE_PET &&  // Pet Trainers do not respec hunter talents
		Plr->getLevel() > 10 )						  // player level
	{
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(22), 11);
	}
	
	if( pTrainer &&
		pTrainer->TrainerType == TRAINER_TYPE_PET &&	// pet trainer type
		Plr->getClass() == HUNTER &&					// hunter class
		Plr->GetSummon() != NULL )						// have pet
	{
		Menu->AddItem(0, Plr->GetSession()->LocalizedWorldSrv(23), 13);
	}

	if(AutoSend)
		Menu->SendTo(Plr);
}

void GossipScript::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
{
	Creature* pCreature = static_cast< Creature* >( pObject );
	if( pObject->GetTypeId() != TYPEID_UNIT )
		return;

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

	default:
		sLog.outError("Unknown IntId %u on entry %u", IntId, pCreature->GetEntry());
		break;
	}	
}

void GossipScript::Destroy()
{
	delete this;
}

void ScriptMgr::register_hook(ServerHookEvents event, void * function_pointer)
{
	ASSERT(event < NUM_SERVER_HOOKS);
	_hooks[event].push_back(function_pointer);
}

/* Hook Implementations */
#define OUTER_LOOP_BEGIN(type, fptr_type) if(!sScriptMgr._hooks[type].size()) { \
	return; } \
	fptr_type call; \
	for(ServerHookList::iterator itr = sScriptMgr._hooks[type].begin(); itr != sScriptMgr._hooks[type].end(); ++itr) { \
	call = ((fptr_type)*itr);

#define OUTER_LOOP_END }

#define OUTER_LOOP_BEGIN_COND(type, fptr_type) if(!sScriptMgr._hooks[type].size()) { \
	return true; } \
	fptr_type call; \
	bool ret_val = true; \
	for(ServerHookList::iterator itr = sScriptMgr._hooks[type].begin(); itr != sScriptMgr._hooks[type].end(); ++itr) { \
		call = ((fptr_type)*itr);

#define OUTER_LOOP_END_COND } return ret_val;

bool HookInterface::OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_NEW_CHARACTER, tOnNewCharacter)
		ret_val = (call)(Race, Class, Session, Name);
	OUTER_LOOP_END_COND
}

void HookInterface::OnKillPlayer(Player * pPlayer, Player * pVictim)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_KILL_PLAYER, tOnKillPlayer)
		(call)(pPlayer, pVictim);
	OUTER_LOOP_END
}

void HookInterface::OnFirstEnterWorld(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, tOnFirstEnterWorld)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnCharacterCreate(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, tOCharacterCreate)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnEnterWorld(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ENTER_WORLD, tOnEnterWorld)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnGuildCreate(Player * pLeader, Guild * pGuild)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_GUILD_CREATE, tOnGuildCreate)
		(call)(pLeader, pGuild);
	OUTER_LOOP_END
}

void HookInterface::OnGuildJoin(Player * pPlayer, Guild * pGuild)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_GUILD_JOIN, tOnGuildJoin)
		(call)(pPlayer, pGuild);
	OUTER_LOOP_END
}

void HookInterface::OnDeath(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_DEATH, tOnDeath)
		(call)(pPlayer);
	OUTER_LOOP_END
}

bool HookInterface::OnRepop(Player * pPlayer)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_REPOP, tOnRepop)
		ret_val = (call)(pPlayer);
	OUTER_LOOP_END_COND
}

void HookInterface::OnEmote(Player * pPlayer, uint32 Emote, Unit * pUnit)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_EMOTE, tOnEmote)
		(call)(pPlayer, Emote, pUnit);
	OUTER_LOOP_END
}

void HookInterface::OnEnterCombat(Player * pPlayer, Unit * pTarget)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ENTER_COMBAT, tOnEnterCombat)
		(call)(pPlayer, pTarget);
	OUTER_LOOP_END
}

bool HookInterface::OnCastSpell(Player * pPlayer, SpellEntry* pSpell)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_CAST_SPELL, tOnCastSpell)
		ret_val = (call)(pPlayer, pSpell);
	OUTER_LOOP_END_COND
}

bool HookInterface::OnLogoutRequest(Player * pPlayer)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, tOnLogoutRequest)
		ret_val = (call)(pPlayer);
	OUTER_LOOP_END_COND
}

void HookInterface::OnLogout(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_LOGOUT, tOnLogout)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnQuestAccept(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, tOnQuestAccept)
		(call)(pPlayer, pQuest, pQuestGiver);
	OUTER_LOOP_END
}

void HookInterface::OnZone(Player * pPlayer, uint32 Zone)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ZONE, tOnZone)
		(call)(pPlayer, Zone);
	OUTER_LOOP_END
}

bool HookInterface::OnChat(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_CHAT, tOnChat)
		ret_val = (call)(pPlayer, Type, Lang, Message, Misc);
	OUTER_LOOP_END_COND
}

void HookInterface::OnLoot(Player * pPlayer, Unit * pTarget, uint32 Money, uint32 ItemId)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_LOOT, tOnLoot)
		(call)(pPlayer, pTarget, Money, ItemId);
	OUTER_LOOP_END
}

void HookInterface::OnObjectLoot(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_OBJECTLOOT, tOnObjectLoot)
		(call)(pPlayer, pTarget, Money, ItemId);
	OUTER_LOOP_END
}

void HookInterface::OnEnterWorld2(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ENTER_WORLD_2, tOnEnterWorld)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnQuestCancelled(Player * pPlayer, Quest * pQuest)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, tOnQuestCancel)
		(call)(pPlayer, pQuest);
	OUTER_LOOP_END
}

void HookInterface::OnQuestFinished(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_QUEST_FINISHED, tOnQuestFinished)
		(call)(pPlayer, pQuest, pQuestGiver);
	OUTER_LOOP_END
}

void HookInterface::OnHonorableKill(Player * pPlayer, Player * pKilled)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_HONORABLE_KILL, tOnHonorableKill)
		(call)(pPlayer, pKilled);
	OUTER_LOOP_END
}

void HookInterface::OnArenaFinish(Player * pPlayer, ArenaTeam* pTeam, bool victory, bool rated)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ARENA_FINISH, tOnArenaFinish)
		(call)(pPlayer, pTeam, victory, rated);
	OUTER_LOOP_END
}

void HookInterface::OnAreaTrigger(Player * pPlayer, uint32 areaTrigger)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_AREATRIGGER, tOnAreaTrigger)
		(call)(pPlayer, areaTrigger);
	OUTER_LOOP_END
}

void HookInterface::OnPostLevelUp(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_POST_LEVELUP, tOnPostLevelUp)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnPreUnitDie(Unit *Killer, Unit *Victim)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_PRE_DIE, tOnPreUnitDie)
		(call)(Killer, Victim);
	OUTER_LOOP_END
}

void HookInterface::OnAdvanceSkillLine(Player * pPlayer, uint32 SkillLine, uint32 Current)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE, tOnAdvanceSkillLine)
		(call)(pPlayer, SkillLine, Current);
	OUTER_LOOP_END
}
