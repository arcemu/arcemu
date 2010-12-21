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

#include "StdAfx.h"
#include "LUAEngine.h"
#include <ScriptSetup.h>

#ifdef WIN32
#pragma warning(disable:4129)
#pragma warning(disable:4244)
#endif

#if PLATFORM != PLATFORM_WIN32
#include <dirent.h>
#endif

ScriptMgr * m_scriptMgr = NULL;
///THREAD PLUA_INSTANCE lua_instance = NULL;
PLUA_INSTANCE LUA_COMPILER = NULL;
SHAREDPTR_POOL sp_pool;
extern "C"
{
	 SCRIPT_DECL uint32 _exp_get_script_type()
	{ 
		return SCRIPT_TYPE_SCRIPT_ENGINE | SCRIPT_TYPE_SCRIPT_ENGINE_LUA;
	}
	SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
	{
		m_scriptMgr = mgr;
		lua_engine::startupEngine();
	}
	SCRIPT_DECL void _export_engine_reload()
	{
		lua_engine::restartEngine();
	}
};
void lua_engine::report(lua_State * L)
{
	const char * msg= lua_tostring(L,-1);
	int count = 5;
	while(msg && --count > 0)
	{
		lua_pop(L,1);
		printf("\t%s\n",msg);
		msg=lua_tostring(L,-1);
	}
}

void lua_engine::scriptload_searchdir(char * Dirname, std::set<string>& store)
{
#ifdef WIN32 
	Log.Success("LuaEngine", "Scanning Directory \"%s\"", Dirname);
	HANDLE hFile;
	WIN32_FIND_DATAA FindData;
	memset(&FindData,0,sizeof(WIN32_FIND_DATAA));

	char SearchName[MAX_PATH];
	        
	strcpy(SearchName,Dirname);
	strcat(SearchName, "\\*.*" );

	hFile = FindFirstFileA(SearchName,&FindData);
	FindNextFileA(hFile, &FindData);
	    
	while( FindNextFileA(hFile, &FindData) )
	{
		if( (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  && !(FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ) //Credits for this 'if' go to Cebernic from ArcScripts Team. Thanks, you saved me some work ;-)
		{
			strcpy(SearchName,Dirname);
			strcat(SearchName, "\\" );
			strcat(SearchName,FindData.cFileName);
			scriptload_searchdir(SearchName, store);
		}
		else
		{
			string fname = Dirname;
			fname += "\\";
			fname += FindData.cFileName;

			int len = fname.length();
			int i=0;
			char ext[MAX_PATH];
					  
			while(len > 0)
			{  
				ext[i++] = fname[--len];
				if(fname[len] == '.' )
		  			break;
	  		}
	  		ext[i++] = '\0';
	  		if ( !_stricmp(ext,"aul." ) )
				store.insert(fname);
		}
	}
	FindClose(hFile);
#else
	char *pch=strrchr(Dirname,'/');
	if (strcmp(Dirname, "..")==0 || strcmp(Dirname, ".")==0) return; //Against Endless-Loop
	if (pch != NULL && (strcmp(pch, "/..")==0 || strcmp(pch, "/.")==0 || strcmp(pch, "/.svn")==0)) return;
	struct dirent ** list;
	int filecount = scandir(Dirname, &list, 0, 0);

	if(filecount <= 0 || !list)
		return;

	struct stat attributes;
	bool err;
	Log.Success("LuaEngine", "Scanning Directory %s", Dirname);
	while(filecount--)
	{
		char dottedrelpath[200];
		sprintf(dottedrelpath, "%s/%s", Dirname, list[filecount]->d_name);
		if(stat(dottedrelpath, &attributes) == -1) 
		{
			err=true;
			Log.Error("LuaEngine","Error opening %s: %s\n", dottedrelpath, strerror(errno));
		}
		else 
			err=false;

		if (!err && S_ISDIR(attributes.st_mode))
			scriptload_searchdir((char *)dottedrelpath, store); //Subdirectory
		else
		{
			char* ext = strrchr(list[filecount]->d_name, '.');
			if(ext != NULL && !strcmp(ext, ".lua"))
				store.insert(dottedrelpath);
		}
		free(list[filecount]);
	}
	free(list);
#endif
}

void lua_engine::loadScripts()
{
	std::set<string> found_scripts;
	Log.Notice("LuaEngine", "Scanning Script-Directories...");
	char * script_dir = "scripts";
	scriptload_searchdir( script_dir, found_scripts);
	Log.Notice("LuaEngine","Found %u Lua scripts.", found_scripts.size() );
	//Read our scripts and cache their data.
	//We protect scripts while they are being modified.
	int cnt = 0;
	FILE * script_file = NULL;
	for(std::set<string>::iterator itr = found_scripts.begin(); itr != found_scripts.end(); ++itr)
	{
		script_file = fopen( (*itr).c_str(), "rb");
		if(script_file != NULL)
		{
			PLUA_SCRIPT script = new LUA_SCRIPT;
			script->read_pos = 0;
			int file_length = 0;
			fseek(script_file, 0, SEEK_END);
			file_length = ftell(script_file);
			rewind(script_file);
			script->dump_data = (const void*)malloc(file_length);
			memset( (void*)script->dump_data, 0, file_length);
			script->dump_size = fread( (void*)script->dump_data, 1, file_length, script_file);
			fclose(script_file);
			le::compiled_scripts.insert( make_pair( (*itr), script) );
		}
	}
	for(le::LuaScriptData::iterator it, itr = le::compiled_scripts.begin(); itr != le::compiled_scripts.end(); )
	{
		it = itr++;
		if(lua_load(LUA_COMPILER->lu, le::readScript, it->second, it->first.c_str() ) || lua_pcall(LUA_COMPILER->lu, 0, 0, 0 ) )
		{
			report(LUA_COMPILER->lu);
			free( (void*)it->second->dump_data);
			delete it->second;
			le::compiled_scripts.erase(it);
		}
		else
			Log.Success("LuaEngine","loaded %s",it->first.c_str() );
	}
	Log.Success("LuaEngine", "Successfully loaded %u scripts.", cnt);
}

void lua_engine::BeginLuaFunctionCall(lua_function ref)
{
	//lua_settop(lu, 0);
	
	lua_getref(lua_state,(int)ref);
}
bool lua_engine::ExecuteLuaFunction(int params, int res, variadic_parameter * results, bool getparams)
{
	bool ret = true;
	lua_State * lu = lua_state;
	int top = lua_gettop(lu);
	if(lua_type(lu, top-params) != LUA_TFUNCTION)
	{
		ret = false;
		//Paroxysm : Stack Cleaning here, not sure what causes that crash in luaH_getstr, maybe due to lack of stack space. Anyway, experimental.
		if(params > 0)
		{
			for(int i = top; i >= (top-params); i--)
			{
				if(!lua_isnone(lu,i) )
					lua_remove(lu,i);
			}
		}
	}
	else
	{
		if(lua_pcall(lu,params,res,0) )
		{
			report(lu);
			ret = false;
		}
		//Get result arguments from lua.
		else if(getparams)
			results = luabridge::tdstack<variadic_parameter*>::get(lu,1);
	}
	return ret;
}
void lua_engine::EndLuaFunctionCall(int res) 
{
	lua_State * lu = lua_state;
	for(int i = res; i > 0; i--)
	{
		if(!lua_isnone(lu,res))
			lua_remove(lu,res);
	}
}

/*
	This version only accepts actual Lua functions and no arguments. See LCF_Extra:CreateClosure(...) to pass arguments to this function.
	*/

void lua_engine::startupEngine()
{
	Log.Notice("LuaEngineMgr", "Spawning Lua Engine...");
/*#ifdef WIN32
	Log.Color(TGREEN);
	printf(" \_\_                        \_\_  \_\_                  \_\_\_\_\_\_                 \n");
	//Log.Color(TGREEN);
	printf("/\\ \\                      /\\ \\/\\ \\                /\\  \_  \\                 \n");
	//Log.Color(TGREEN);
	printf("\\ \\ \\      \_\_  \_\_     \_\_  \\ \\ \\\_\\ \\  \_\_  \_\_  \_\_\_\_\_\\ \\ \\\_\\ \\  \_\_\_\_\_  \_\_\_    \n");
	//Log.Color(TGREEN);
	printf(" \\ \\ \\  \_\_/\\ \\/\\ \\  /'\_\_`\\ \\ \\  \_  \\/\\ \\/\\ \\/\\  \_\_`\\ \\  \_\_ \\/\\  \_\_\\/'\_\_\_\\  \n");
	//Log.Color(TGREEN);
	printf("  \\ \\ \\\_\\ \\ \\ \\\_\\ \\/\\ \\\_\\.\\\_\\ \\ \\ \\ \\ \\ \\\_\\ \\ \\ \\\_\\ \\ \\ \\/\\ \\ \\ \\//\\ \\\_\_/  \n");
	//Log.Color(TGREEN);
	printf("   \\ \\\_\_\_\_/\\ \\\_\_\_\_/\\ \\\_\_/ \\\_\\\\ \\\_\\ \\\_\\/`\_\_\_\_ \\ \\  \_\_/\\ \\\_\\ \\\_\\ \\\_\\\\ \\\_\_\_\_\\ \n");
	//Log.Color(TGREEN);
	printf("    \\/\_\_\_/  \\/\_\_\_/  \\/\_\_/\\/\_/ \\/\_/\\/\_/`/\_\_\_// \\ \\ \\/  \\/\_/\\/\_/\\/\_/ \\/\_\_\_\_/ \n");
	//Log.Color(TGREEN);
	printf("                                         /\\\_\_\_/\\ \\\_\\                       \n");
	//Log.Color(TGREEN);
	printf("                                         \\/\_\_/  \\/\_/                      \n");
	Log.Color(TNORMAL);
	#else
	Log.Color(TGREEN);
	
	#endif
	Log.Line("");
	Log.Color(TNORMAL);
	Log.Line("");*/
	Log.Notice("LuaEngineMgr","~~~LuaHypArc~~~");
	Log.Notice("LuaEngineMgr", "LuaHypArc Lua Engine %s: Loaded", ARCH);

	//Initialize our compiler.
	LUA_COMPILER = new LUA_INSTANCE;
	LUA_COMPILER->lu = lua_open();
	LUA_COMPILER->map = NULL;
	//Since our objects load into "lua_instance" object.
	lua_instance = LUA_COMPILER;
	//Expose our wow object and functions to the compiler.
	loadState(LUA_COMPILER);
	//feed scripts to the compiler.
	le::scriptLock.Acquire();
	loadScripts();
	le::scriptLock.Release();
	//dumpScripts2HDD();
	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitBinding.begin(); itr != LUA_COMPILER->m_unitBinding.end(); ++itr)
		m_scriptMgr->register_creature_script( itr->first, createluacreature );

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goBinding.begin(); itr != LUA_COMPILER->m_goBinding.end(); ++itr)
		m_scriptMgr->register_gameobject_script( itr->first, createluagameobject );

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_questBinding.begin(); itr != LUA_COMPILER->m_questBinding.end(); ++itr)
	{
		QuestScript * qs = createluaquest( itr->first );
		if( qs != NULL )
			m_scriptMgr->register_quest_script( itr->first, qs );
	}
	//Lua Instances are what make thread based lua engines work, since map threads are the ones that try to execute lua code, we can keep track of them 
	//By making register a lua instance. Through this instance, we can be able to control that map's lua state.
	for(uint32 i = 0; i < NUM_MAPS; ++i)
	{
		MapMgr * mgr = sInstanceMgr.GetMapMgr(i);
		if(mgr != NULL)
		{
			m_scriptMgr->register_instance_script(i, createluainstance );
			//make the mapmgr load the script on it's own thread.
			if(mgr->GetScript() == NULL)
				sEventMgr.AddEvent(mgr, &MapMgr::LoadInstanceScript, EVENT_MAPMGR_UPDATEOBJECTS, 1, 1, 0);
		}
	}

    for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitGossipBinding.begin(); itr != LUA_COMPILER->m_unitGossipBinding.end(); ++itr)
 	{
		GossipScript * gs = createunitgossipInterface( itr->first );
 		if( gs != NULL )
			m_scriptMgr->register_gossip_script( itr->first, gs );
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_itemGossipBinding.begin(); itr != LUA_COMPILER->m_itemGossipBinding.end(); ++itr)
	{
		GossipScript * gs = createitemgossipInterface( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_item_gossip_script( itr->first, gs );
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goGossipBinding.begin(); itr != LUA_COMPILER->m_goGossipBinding.end(); ++itr)
	{
		GossipScript * gs = creategogossipInterface( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_go_gossip_script( itr->first, gs );
	}
	for(li::SpellFRefMap::iterator itr = LUA_COMPILER->m_dummySpells.begin(); itr != LUA_COMPILER->m_dummySpells.end(); ++itr)
	{
		m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
		le::_hooked_dummies.insert(itr->first);
	}
	//big server hook chunk. it only hooks if there are functions present to save on unnecessary processing.
	RegisterHook(SERVER_HOOK_NEW_CHARACTER,(void*)LuaHookOnNewCharacter)
	RegisterHook(SERVER_HOOK_KILL_PLAYER,(void*)LuaHookOnKillPlayer)
	RegisterHook(SERVER_HOOK_FIRST_ENTER_WORLD,(void*)LuaHookOnFirstEnterWorld)
	RegisterHook(SERVER_HOOK_ENTER_WORLD,(void*)LuaHookOnEnterWorld)
	RegisterHook(SERVER_HOOK_GUILD_JOIN,(void*)LuaHookOnGuildJoin)
	RegisterHook(SERVER_HOOK_DEATH,(void*)LuaHookOnDeath)
	RegisterHook(SERVER_HOOK_REPOP,(void*)LuaHookOnRepop)
	RegisterHook(SERVER_HOOK_EMOTE,(void*)LuaHookOnEmote)
	RegisterHook(SERVER_HOOK_ENTER_COMBAT,(void*)LuaHookOnEnterCombat)
	RegisterHook(SERVER_HOOK_CAST_SPELL,(void*)LuaHookOnCastSpell)
	RegisterHook(SERVER_HOOK_TICK,(void*)LuaHookOnTick)
	RegisterHook(SERVER_HOOK_LOGOUT_REQUEST,(void*)LuaHookOnLogoutRequest)
	RegisterHook(SERVER_HOOK_LOGOUT,(void*)LuaHookOnLogout)
	RegisterHook(SERVER_HOOK_QUEST_ACCEPT,(void*)LuaHookOnQuestAccept)
	RegisterHook(SERVER_HOOK_ZONE,(void*)LuaHookOnZone)
	RegisterHook(SERVER_HOOK_CHAT,(void*)LuaHookOnChat)
	RegisterHook(SERVER_HOOK_LOOT,(void*)LuaHookOnLoot)
	RegisterHook(SERVER_HOOK_GUILD_CREATE,(void*)LuaHookOnGuildCreate)
	RegisterHook(SERVER_HOOK_ENTER_WORLD_2,(void*)LuaHookOnEnterWorld2)
	RegisterHook(SERVER_HOOK_CHARACTER_CREATE,(void*)LuaHookOnCharacterCreate)
	RegisterHook(SERVER_HOOK_QUEST_CANCELLED,(void*)LuaHookOnQuestCancelled)
	RegisterHook(SERVER_HOOK_QUEST_FINISHED,(void*)LuaHookOnQuestFinished)
	RegisterHook(SERVER_HOOK_HONORABLE_KILL,(void*)LuaHookOnHonorableKill)
	RegisterHook(SERVER_HOOK_ARENA_FINISH,(void*)LuaHookOnArenaFinish)
	RegisterHook(SERVER_HOOK_OBJECTLOOT,(void*)LuaHookOnObjectLoot)
	RegisterHook(SERVER_HOOK_AREATRIGGER,(void*)LuaHookOnAreaTrigger)
	RegisterHook(SERVER_HOOK_POST_LEVELUP,(void*)LuaHookOnPostLevelUp)
	RegisterHook(SERVER_HOOK_PRE_DIE,(void*)LuaHookOnPreUnitDie)
	RegisterHook(SERVER_HOOK_ADVANCE_SKILLLINE,(void*)LuaHookOnAdvanceSkillLine)
	RegisterHook(SERVER_HOOK_DUEL_FINISHED,(void*)LuaHookOnDuelFinished)
	RegisterHook(SERVER_HOOK_AURA_REMOVE,(void*)LuaHookOnAuraRemove)
	RegisterHook(SERVER_HOOK_RESURRECT,(void*)LuaHookOnResurrect)
}

void lua_engine::shutdownEngine()
{
	Log.Notice("LuaEngine", "LuaEngine is shutting down.");
	delete LUA_COMPILER;
	LUA_COMPILER = NULL;
	activestates_lock.Acquire();
	for(ActiveStates::iterator itr = activeStates.begin(); itr != activeStates.end(); ++itr)
	{
		TimedEvent * ev = TimedEvent::Allocate( (*itr), new CallBackFunctionP1<MapMgr*>(&le::shutdownThread, (*itr) ), EVENT_MAPMGR_UPDATEOBJECTS, 1, 1);
		(*itr)->event_AddEvent(ev);
	}
	activestates_lock.Release();
	while(activeStates.size() )
		Sleep(100);
	Log.Success("LuaEngine", "All lua states have successfully shutdown.");
}

void lua_engine::unload_resources()
{
	PLUA_INSTANCE lu = lua_instance;
	DestroyAllLuaEvents(lu);

	//Clean up the bindings, no need to unref them because we are going to close down the lua state anyway.
	for(li::ObjectBindingMap::iterator it = lu->m_unitBinding.begin(); it != lu->m_unitBinding.end(); ++it)
		delete it->second;
	lu->m_unitBinding.clear();

	for(li::ObjectBindingMap::iterator it = lu->m_goBinding.begin(); it != lu->m_goBinding.end(); ++it)
		delete it->second;
	lu->m_goBinding.clear();

	for(li::ObjectBindingMap::iterator it = lu->m_questBinding.begin(); it != lu->m_questBinding.end(); ++it)
		delete it->second;
	lu->m_questBinding.clear();

	for(li::ObjectBindingMap::iterator it = lu->m_instanceBinding.begin(); it != lu->m_instanceBinding.end(); ++it)
		delete it->second;
	lu->m_instanceBinding.clear();

	for(li::ObjectBindingMap::iterator it = lu->m_unitGossipBinding.begin(); it != lu->m_unitGossipBinding.end(); ++it)
		delete it->second;
	lu->m_unitGossipBinding.clear();

	for(li::ObjectBindingMap::iterator it = lu->m_goGossipBinding.begin(); it != lu->m_goGossipBinding.end(); ++it)
		delete it->second;
	lu->m_goGossipBinding.clear();

	for(li::ObjectBindingMap::iterator it = lu->m_itemGossipBinding.begin(); it != lu->m_itemGossipBinding.end(); ++it)
		delete it->second;
	lu->m_itemGossipBinding.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_creatureFRefs.begin(); it != lu->m_creatureFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_creatureFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_goFRefs.begin(); it != lu->m_goFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_goFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_questFRefs.begin(); it != lu->m_questFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_questFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_instanceFRefs.begin(); it != lu->m_instanceFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_instanceFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_unitGossipFRefs.begin(); it != lu->m_unitGossipFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_unitGossipFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_itemGossipFRefs.begin(); it != lu->m_itemGossipFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_itemGossipFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_goGossipFRefs.begin(); it != lu->m_goGossipFRefs.end(); ++it)
		cleanup_varparam(it->second, lua_state);
	lu->m_goGossipFRefs.clear();

	lu->m_hooks.clear();

	for(li::References::iterator it = lu->m_globalFRefs.begin(); it != lu->m_globalFRefs.end(); ++it)
		cleanup_varparam( (*it), lua_state);
	lu->m_globalFRefs.clear();

	for(li::References::iterator it = lu->pendingThreads.begin(); it != lu->pendingThreads.end(); ++it)
		cleanup_varparam( (*it), lua_state);
	lu->pendingThreads.clear();

	for(li::SpellFRefMap::iterator it = lu->m_dummySpells.begin(); it != lu->m_dummySpells.end(); ++it)
	{
		cleanup_varparam( it->second->params, lua_state);
		delete it->second;
	}
	lu->m_dummySpells.clear();
}
void lua_engine::restartEngine()
{
	//lock scripts
	Log.Notice("LuaEngine", "LuaEngine is restarting. ");
	le::scriptLock.Acquire();
	for(le::LuaScriptData::iterator itr = le::compiled_scripts.begin(); itr != le::compiled_scripts.end(); ++itr)
	{
		free( (void*)itr->second->dump_data);
		delete itr->second;
	}
	le::compiled_scripts.clear();
	LUA_COMPILER->lu = lua_open();
	lua_instance = LUA_COMPILER;
	loadState(LUA_COMPILER);
	//re-compile the scripts.
	loadScripts();
	le::scriptLock.Release();
	//register any new bindings.
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitBinding.begin(); itr != LUA_COMPILER->m_unitBinding.end(); ++itr)
		m_scriptMgr->register_creature_script( itr->first, createluacreature );

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goBinding.begin(); itr != LUA_COMPILER->m_goBinding.end(); ++itr)
		m_scriptMgr->register_gameobject_script( itr->first, createluagameobject );

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_questBinding.begin(); itr != LUA_COMPILER->m_questBinding.end(); ++itr)
	{
		QuestScript * qs = createluaquest( itr->first );
		if( qs != NULL )
			m_scriptMgr->register_quest_script( itr->first, qs );
	}
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitGossipBinding.begin(); itr != LUA_COMPILER->m_unitGossipBinding.end(); ++itr)
	{
		GossipScript * gs = createunitgossipInterface( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_gossip_script( itr->first, gs );
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_itemGossipBinding.begin(); itr != LUA_COMPILER->m_itemGossipBinding.end(); ++itr)
	{
		GossipScript * gs = createitemgossipInterface( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_item_gossip_script( itr->first, gs );
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goGossipBinding.begin(); itr != LUA_COMPILER->m_goGossipBinding.end(); ++itr)
	{
		GossipScript * gs = creategogossipInterface( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_go_gossip_script( itr->first, gs );
	}

	for(li::SpellFRefMap::iterator itr = LUA_COMPILER->m_dummySpells.begin(); itr != LUA_COMPILER->m_dummySpells.end(); ++itr)
	{
		if(le::_hooked_dummies.find(itr->first) == le::_hooked_dummies.end() )
		{
			m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
			le::_hooked_dummies.insert(itr->first);
		}
	}

	//Tell all our active states to restart.
	for(le::ActiveStates::iterator itr = le::activeStates.begin(); itr != le::activeStates.end(); ++itr)
	{
		TimedEvent * ev = TimedEvent::Allocate( (*itr), new CallBackFunctionP1<MapMgr*>(&le::restartThread, (*itr) ), EVENT_MAPMGR_UPDATEOBJECTS, 1, 1);
		(*itr)->event_AddEvent(ev);
	}
}
void lua_engine::restartThread(MapMgr * map)
{
	//first grab the script lock.
	LuaGuard guard(le::scriptLock);
	//clean up our frefs and binding maps.
	le::unload_resources();
	//close down the lua state, clearing up resources that were being used by the previously loaded scripts.
	lua_close(lua_state);
	//open a brand new state.
	lua_state = lua_open();
	//re-expose our wow objects and functions
	le::loadState(lua_instance);
	//now we reload the scripts
	le::loadScripts(lua_state);
	//since we may have new bindings, update our contained interfaces to use the new bindings so that script changes may take effect
	for(li::ObjectBindingMap::iterator itr = lua_instance->m_unitBinding.begin(); itr != lua_instance->m_unitBinding.end(); ++itr)
	{
		li::CreatureInterfaceMap::iterator it = lua_instance->m_creatureInterfaceMap.find(itr->first),
			itend = lua_instance->m_creatureInterfaceMap.upper_bound(itr->first);
		//assign the new binding.
		for(; it != itend; ++it)
			it->second->m_binding = itr->second;
	}
	for(li::ObjectBindingMap::iterator itr = lua_instance->m_goBinding.begin(); itr != lua_instance->m_goBinding.end(); ++itr)
	{
		li::GOInterfaceMap::iterator it = lua_instance->m_goInterfaceMap.find(itr->first),
			itend = lua_instance->m_goInterfaceMap.upper_bound(itr->first);
		for(; it != itend; ++it)
			it->second->m_binding = itr->second;
	}
	for(li::ObjectBindingMap::iterator itr = lua_instance->m_questBinding.begin(); itr != lua_instance->m_questBinding.end(); ++itr)
	{
		li::QuestInterfaceMap::iterator it = lua_instance->m_questInterfaceMap.find(itr->first),
			itend = lua_instance->m_questInterfaceMap.upper_bound(itr->first);
		for(; it != itend; ++it)
			it->second->m_binding = itr->second;
	}
	for(li::ObjectBindingMap::iterator itr = lua_instance->m_unitGossipBinding.begin(); itr != lua_instance->m_unitGossipBinding.end(); ++itr)
	{
		li::GossipInterfaceMap::iterator it = lua_instance->m_unit_gossipInterfaceMap.find(itr->first),
			itend = lua_instance->m_unit_gossipInterfaceMap.upper_bound(itr->first);
		for(; it != itend; ++it)
			it->second->m_unit_gossip_binding = itr->second;
	}
	for(li::ObjectBindingMap::iterator itr = lua_instance->m_goGossipBinding.begin(); itr != lua_instance->m_goGossipBinding.end(); ++itr)
	{
		li::GossipInterfaceMap::iterator it = lua_instance->m_go_gossipInterfaceMap.find(itr->first),
			itend = lua_instance->m_go_gossipInterfaceMap.upper_bound(itr->first);
		for(; it != itend; ++it)
			it->second->m_go_gossip_binding = itr->second;
	}
	for(li::ObjectBindingMap::iterator itr = lua_instance->m_itemGossipBinding.begin(); itr != lua_instance->m_itemGossipBinding.end(); ++itr)
	{
		li::GossipInterfaceMap::iterator it = lua_instance->m_item_gossipInterfaceMap.find(itr->first),
			itend = lua_instance->m_item_gossipInterfaceMap.upper_bound(itr->first);
		for(; it != itend; ++it)
			it->second->m_item_gossip_binding = itr->second;
	}
}
void lua_engine::shutdownThread(MapMgr* map)
{
	GET_LOCK;
	unload_resources();
	lua_close(lua_state);
	delete lua_instance;
	lua_instance = NULL;
	LuaGuard guard(le::activestates_lock);
	le::ActiveStates::iterator itr = le::activeStates.find(map);
	if(itr != le::activeStates.end() )
		le::activeStates.erase(itr);
};
namespace lua_engine
{
	void loadState(PLUA_INSTANCE instance)
	{
		luaL_openlibs(instance->lu);
		luabridge::module m(instance->lu);
		bindRegisterMethods(m);
		bindObjectMethods(m);
		bindUnitMethods(m);
		bindCreatureMethods(m);
		bindPlayerMethods(m);
		bindGameobjectMethods(m);
		bindSQLMethods(m);
		bindSpellMethods(m);
		bindPacketMethods(m);
		bindAuraMethods(m);
		bindTaxiMethods(m);
		bindItemMethods(m);
		bindGlobalMethods(m);
		bindMapMethods(m);
	}

	//	Feed cached lua scripts to the lua loader
	void loadScripts(lua_State * lu)
	{
		int cnt = 0;
		//LuaGuard guard(le::scriptLock);
		le::LuaScriptData::iterator it,itr = le::compiled_scripts.begin();
		for(; itr != le::compiled_scripts.end();)
		{
			it = itr++;
			if(lua_load(lu, le::readScript, it->second, it->first.c_str() ) || lua_pcall(lu, 0, 0, 0 ) )
			{
				report(lu);
				free( (void*)it->second->dump_data);
				delete it->second;
				le::compiled_scripts.erase(it);
			}
		}
	}
	const char * readScript(lua_State * L, void * ud, size_t * size)
	{
		PLUA_SCRIPT script = (PLUA_SCRIPT)ud;
		const char * ret = NULL;
		if(script->read_pos >= script->dump_size)
		{
			*size = 0;
			script->read_pos = 0;
		}
		else
		{
			ret = (const char*)script->dump_data;
			*size = script->dump_size;
			script->read_pos = script->dump_size;
		}
		return ret;
	}
	int dumpScript(lua_State * L, const void * data, size_t data_size, void* chunk)
	{
		/*if(data_size > 0)
		{
			PLUA_BINARYCHUNK bin_chunk = (PLUA_BINARYCHUNK)chunk;
			LUA_DUMPNODE * prev_node = NULL, * dump_node = bin_chunk->head_chunk;
			while(dump_node != NULL)
			{
				prev_node = dump_node;
				dump_node = dump_node->next_chunk;
			}
			dump_node = new LUA_DUMPNODE;
			dump_node->chunk_size = data_size;
			dump_node->chunk_data = malloc(data_size);
			dump_node->next_chunk = NULL;
			memcpy(dump_node->chunk_data, data, data_size);
			if(bin_chunk->head_chunk == NULL)
				bin_chunk->head_chunk = dump_node;
			else
				prev_node->next_chunk = dump_node;
			bin_chunk->chunk_cnt++;
		}*/
		return 0;
	}
	void dumpScripts2HDD()
	{
		/*std::string fullpath;
		Log.Notice("LuaEngine","Dumping compiled scripts to HDD.");
		for(LuaScriptData::iterator itr = compiled_scripts.begin(); itr != compiled_scripts.end(); ++itr)
		{
			fullpath = itr->first+".luac";
			FILE * outfile = fopen(fullpath.c_str(), "ab");
			if(outfile != NULL)
			{
				fwrite(itr->second->dump_data, itr->second->dump_size, 1, outfile);
				fclose(outfile);
			}
		}
		Log.Notice("LuaEngine","Done dumping compile scripts.");*/
	}
}

