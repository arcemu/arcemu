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
#include <ScriptSetup.h>

#ifdef WIN32
#pragma warning(disable:4129)
#pragma warning(disable:4244)
#endif

#if PLATFORM != PLATFORM_WIN32
#include <dirent.h>
#endif

ScriptMgr* m_scriptMgr = NULL;
Arcemu::Utility::TLSObject<PLUA_INSTANCE> lua_instance;
PLUA_INSTANCE LUA_COMPILER = NULL;

//static data initialization for our classes
const char* ScriptManager::mt = "LOADED_SCRIPTS";
const char* ReferenceHandler::refTable = "OBJECT_REFERENCES";

lua_engine::LuaScriptData lua_engine::compiled_scripts;
lua_engine::hooked_dummySpells lua_engine::_hooked_dummies;
lua_engine::ActiveStates lua_engine::activeStates;
FastMutex lua_engine::activestates_lock;
FastMutex lua_engine::scriptLock;


extern "C"
{
	SCRIPT_DECL uint32 _exp_get_script_type()
	{
		return SCRIPT_TYPE_SCRIPT_ENGINE;
	}
	SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
	{
		m_scriptMgr = mgr;
		lua_engine::startupEngine();
	}

	SCRIPT_DECL void _exp_engine_unload()
	{
		le::scriptLock.Acquire();
		//delete the scripts shared between the Lua instances
		for(le::LuaScriptData::iterator itr = le::compiled_scripts.begin(); itr != le::compiled_scripts.end(); ++itr)
			delete itr->second;
		le::compiled_scripts.clear();
		le::scriptLock.Release();
	}

	SCRIPT_DECL void _export_engine_reload()
	{
		lua_engine::restartEngine();
	}
};
void lua_engine::report(lua_State * L)
{
	int count = lua_gettop( L );

	while(count > 0)
	{
		const char* msg = lua_tostring(L, -1);
		LOG_ERROR(msg);
		lua_pop(L, 1);
		count--;
	}
}

void lua_engine::scriptload_searchdir(char* Dirname, deque<string>& store)
{
#ifdef WIN32
	Log.Success("LuaEngine", "Scanning Directory (%s)", Dirname);
	HANDLE hFile = NULL;
	WIN32_FIND_DATAA FindData;
	memset(&FindData, 0, sizeof(WIN32_FIND_DATAA));
	deque< std::string > directories;
	directories.push_back(Dirname);
	while(directories.size())
	{
		std::string current_directory = directories.front();
		std::string pattern = current_directory;
		pattern += "\\*.*";
		directories.pop_front();

		if(hFile != NULL)	//First let's close previous handles.
			FindClose(hFile);
		hFile = FindFirstFileA(pattern.c_str() , &FindData);
		if(hFile == INVALID_HANDLE_VALUE)
			continue;
		do
		{
			if((FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)   //ignore hidden files.
				continue;
			if(!strcmp(FindData.cFileName, "..") || !strcmp(FindData.cFileName, "."))
				continue;

			string item = current_directory;
			item += "\\";
			item += FindData.cFileName;
			if((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))   //store directories for later parsing.
				directories.push_back(item);
			else
			{
				size_t start = item.rfind("."); //find the '.' designating the extension
				if(start != string::npos)
				{
					string ext = item.substr(start + 1, (item.length() - (start + 1)));
					if(!ext.compare("lua"))
						store.push_back(item);
				}
			}

		}
		while(FindNextFileA(hFile, &FindData));
	}
#else
	char* pch = strrchr(Dirname, '/');
	if(strcmp(Dirname, "..") == 0 || strcmp(Dirname, ".") == 0) return;   //Against Endless-Loop
	if(pch != NULL && (strcmp(pch, "/..") == 0 || strcmp(pch, "/.") == 0 || strcmp(pch, "/.svn") == 0 || strcmp(pch, "/.git") == 0))
		return;
	struct dirent** list;
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
			err = true;
			Log.Error("LuaEngine", "Error opening %s: %s\n", dottedrelpath, strerror(errno));
		}
		else
			err = false;

		if(!err && S_ISDIR(attributes.st_mode))
			scriptload_searchdir((char*)dottedrelpath, store);  //Subdirectory
		else
		{
			char* ext = strrchr(list[filecount]->d_name, '.');
			if(ext != NULL && !strcmp(ext, ".lua"))
				store.push_back(dottedrelpath);
		}
		free(list[filecount]);
	}
	free(list);
#endif
}

void lua_engine::loadScripts()
{
	deque<string> found_scripts;
	scriptload_searchdir((char*)"scripts", found_scripts);
	//Read our scripts and cache their data.
	//We protect scripts while they are being modified.
	ptrdiff_t countofvalidscripts = 0, countofscripts = found_scripts.size();
	//Log.Success("LuaEngine","Found %u Lua scripts.", countofscripts);
	FILE* script_file = NULL;
	for(std::deque<string>::iterator itr = found_scripts.begin(); itr != found_scripts.end(); ++itr)
	{
		script_file = fopen((*itr).c_str(), "rb");
		if(script_file != NULL)
		{
			PLUA_SCRIPT script = new LUA_SCRIPT;
			int file_length = 0;
			fseek(script_file, 0, SEEK_END);
			file_length = ftell(script_file);
			rewind(script_file);
			script->data_ = (const void*)malloc(file_length + 1);
			memset((void*)script->data_, 0, file_length + 1);
			script->datasize_ = fread((void*)script->data_, 1, file_length, script_file);
			fclose(script_file);
			parseHeader(script);
			le::compiled_scripts.insert(make_pair((*itr), script));
		}
	}
	for(le::LuaScriptData::iterator it, itr = le::compiled_scripts.begin(); itr != le::compiled_scripts.end();)
	{
		it = itr++;
		//check if file has been loaded using the 'include' function
		if(LUA_COMPILER->scripts_.isLoaded(it->first))
		{
			++countofvalidscripts;
			continue;
		}
		if(lua_load(LUA_COMPILER->lu, le::readScript, it->second, it->first.c_str()) || lua_pcall(LUA_COMPILER->lu, 0, 0, 0))
		{
			report(LUA_COMPILER->lu);
			delete it->second;
			le::compiled_scripts.erase(it);
		}
		else
		{
			++countofvalidscripts;
			LUA_COMPILER->scripts_.add(it->first);
		}
	}
	Log.Success("LuaEngine", "Successfully loaded [%u/%u] scripts.", countofvalidscripts, countofscripts);
}

bool lua_engine::loadScript(const char* filename)
{
	PLUA_INSTANCE context = lua_instance.get();
	bool success = false;
	if(NULL == context)
		context = LUA_COMPILER;
	if(NULL != context && NULL != filename)
	{
		assert(context->lu != NULL);
		//get the current file directory and pre-pend it to the file name
		lua_Debug ar;
		std::string fullpath;

		ptrdiff_t function_level = 1;
		while(lua_getstack(context->lu, function_level, &ar))   //while we have a call stack, 1 of those functions has to have been defined in .lua file.
		{
			function_level++;
			if(lua_getinfo(context->lu, "S", &ar))
			{
				fullpath += ar.source;
				size_t start = fullpath.find('@'); //remove @ prefix if any.
				if(start != string::npos)
					fullpath = fullpath.substr(++start, (fullpath.length() - start));

				start = fullpath.rfind('\\'); //extract the directory
				start = (start == string::npos) ? fullpath.rfind('/') : start;
				if(start != string::npos)
				{
					fullpath = fullpath.substr(0, ++start);
					break; //we've found a directory, stop here.
				}
				fullpath.clear();
			}

		}
		assert(fullpath.length() > 0);

		fullpath += filename;
		FILE* _file = fopen(fullpath.c_str(), "rb");
		if(NULL != _file)
		{
			LUA_SCRIPT cached;
			size_t file_length = 0;
			fseek(_file, 0, SEEK_END);
			file_length = ftell(_file);
			rewind(_file);
			cached.data_ = (const void*)malloc(file_length);
			cached.datasize_ = fread((void*)cached.data_, 1, file_length, _file);
			fclose(_file);
			success = (0 == lua_load(context->lu, readScript, (void*)&cached , fullpath.c_str())) && (0 == lua_pcall(context->lu, 0, 0, 0));

		}
	}
	return success;
}

void lua_engine::BeginLuaFunctionCall(lua_function ref)
{
	ReferenceHandler::getReference(lua_state, (ptrdiff_t)ref);
}
bool lua_engine::ExecuteLuaFunction(int params, int res, variadic_parameter** results, bool getparams)
{
	bool ret = true;
	lua_State* lu = lua_state;
	int top = lua_gettop(lu);
	if(lua_type(lu, top - params) != LUA_TFUNCTION)
	{
		ret = false;
		if(params++ > 0)
		{
			for(int i = top; i > 0 && i >= (top - params); i--)
			{
				if(!lua_isnone(lu, i))
					lua_remove(lu, i);
			}
		}
		else
			lua_pop(lu, 1); //pop the nil pushed by BeginFunctionCall.
	}
	else
	{
		if(lua_pcall(lu, params, res, 0))
		{
			report(lu);
			ret = false;
		}
		//Get result arguments from lua.
		else if(getparams)
			*results = luabridge::tdstack<variadic_parameter*>::get(lu, (top - params));
	}
	return ret;
}

void lua_engine::EndLuaFunctionCall(int res)
{
	lua_State* lu = lua_state;
	for(int i = res; i > 0; i--)
	{
		if(!lua_isnone(lu, res))
			lua_remove(lu, res);
	}
}

/*
	This version only accepts actual Lua functions and no arguments. See LCF_Extra:CreateClosure(...) to pass arguments to this function.
	*/

void lua_engine::startupEngine()
{
	Log.Success("LuaEngineMgr", "LuaBridge interpreter %s: Loaded", ARCH);

	//Initialize our compiler.
	LUA_COMPILER = new LUA_INSTANCE;
	LUA_COMPILER->lu = lua_open();
	LUA_COMPILER->map = NULL;
	LUA_COMPILER->scripts_.state = LUA_COMPILER->lu;
	LUA_COMPILER->scripts_.initialize();
	//Since our objects load into "lua_instance" object.
	lua_instance = LUA_COMPILER;
	//Expose our wow object and functions to the compiler.
	loadState(LUA_COMPILER);
	//feed scripts to the compiler.
	le::scriptLock.Acquire();
	loadScripts();
	le::scriptLock.Release();

	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(uint32 i = 0; i < NUM_MAPS; ++i)
	{
		//check if the 'i' points to a valid map id.
		if(sInstanceMgr.GetMap(i) != NULL && !m_scriptMgr->has_instance_script(i))
		{
			//register that.
			m_scriptMgr->register_instance_script(i, createluainstance);
			//Since this dll is loaded after alot of mapmgr's have already been created, we need to tell all those existing mgrs to re-instantiate their instance script.
			MapMgr* mgr = sInstanceMgr.GetMapMgr(i);
			if(mgr != NULL && mgr->GetScript() == NULL)
				//make the mapmgr load the script on it's own thread.
				sEventMgr.AddEvent(mgr, &MapMgr::LoadInstanceScript, EVENT_MAPMGR_UPDATEOBJECTS, 1, 1, 0);
		}
	}
	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitBinding.begin(); itr != LUA_COMPILER->m_unitBinding.end(); ++itr)
		if(!m_scriptMgr->has_creature_script(itr->first))
			m_scriptMgr->register_creature_script(itr->first, createluacreature);

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goBinding.begin(); itr != LUA_COMPILER->m_goBinding.end(); ++itr)
		if(!m_scriptMgr->has_gameobject_script(itr->first))
			m_scriptMgr->register_gameobject_script(itr->first, createluagameobject);

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_questBinding.begin(); itr != LUA_COMPILER->m_questBinding.end(); ++itr)
	{
		if(!m_scriptMgr->has_quest_script(itr->first))
		{
			QuestScript* qs = createluaquest(itr->first);
			if(qs != NULL)
				m_scriptMgr->register_quest_script(itr->first, qs);
		}
	}
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitGossipBinding.begin(); itr != LUA_COMPILER->m_unitGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = createunitgossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_creature_gossip(itr->first, gs);
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_itemGossipBinding.begin(); itr != LUA_COMPILER->m_itemGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = createitemgossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_item_gossip(itr->first, gs);
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goGossipBinding.begin(); itr != LUA_COMPILER->m_goGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = creategogossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_go_gossip(itr->first, gs);
	}
	for(li::SpellFRefMap::iterator itr = LUA_COMPILER->m_dummySpells.begin(); itr != LUA_COMPILER->m_dummySpells.end(); ++itr)
	{
		if(!m_scriptMgr->has_dummy_spell_script(itr->first))
			m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
	}

	//big server hook chunk. it only hooks if there are functions present to save on unnecessary processing.
	RegisterHook(SERVER_HOOK_EVENT_ON_NEW_CHARACTER, (void*)LuaHookOnNewCharacter)
	RegisterHook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)LuaHookOnKillPlayer)
	RegisterHook(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, (void*)LuaHookOnFirstEnterWorld)
	RegisterHook(SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)LuaHookOnEnterWorld)
	RegisterHook(SERVER_HOOK_EVENT_ON_GUILD_JOIN, (void*)LuaHookOnGuildJoin)
	RegisterHook(SERVER_HOOK_EVENT_ON_DEATH, (void*)LuaHookOnDeath)
	RegisterHook(SERVER_HOOK_EVENT_ON_REPOP, (void*)LuaHookOnRepop)
	RegisterHook(SERVER_HOOK_EVENT_ON_EMOTE, (void*)LuaHookOnEmote)
	RegisterHook(SERVER_HOOK_EVENT_ON_ENTER_COMBAT, (void*)LuaHookOnEnterCombat)
	RegisterHook(SERVER_HOOK_EVENT_ON_CAST_SPELL, (void*)LuaHookOnCastSpell)
	RegisterHook(SERVER_HOOK_EVENT_ON_TICK, (void*)LuaHookOnTick)
	RegisterHook(SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, (void*)LuaHookOnLogoutRequest)
	RegisterHook(SERVER_HOOK_EVENT_ON_LOGOUT, (void*)LuaHookOnLogout)
	RegisterHook(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, (void*)LuaHookOnQuestAccept)
	RegisterHook(SERVER_HOOK_EVENT_ON_ZONE, (void*)LuaHookOnZone)
	RegisterHook(SERVER_HOOK_EVENT_ON_CHAT, (void*)LuaHookOnChat)
	RegisterHook(SERVER_HOOK_EVENT_ON_LOOT, (void*)LuaHookOnLoot)
	RegisterHook(SERVER_HOOK_EVENT_ON_GUILD_CREATE, (void*)LuaHookOnGuildCreate)
	//RegisterHook(SERVER_HOOK_EVENT_ON_ENTER_WORLD_2,(void*)LuaHookOnEnterWorld2)
	RegisterHook(SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, (void*)LuaHookOnCharacterCreate)
	RegisterHook(SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, (void*)LuaHookOnQuestCancelled)
	RegisterHook(SERVER_HOOK_EVENT_ON_QUEST_FINISHED, (void*)LuaHookOnQuestFinished)
	RegisterHook(SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)LuaHookOnHonorableKill)
	RegisterHook(SERVER_HOOK_EVENT_ON_ARENA_FINISH, (void*)LuaHookOnArenaFinish)
	RegisterHook(SERVER_HOOK_EVENT_ON_OBJECTLOOT, (void*)LuaHookOnObjectLoot)
	RegisterHook(SERVER_HOOK_EVENT_ON_AREATRIGGER, (void*)LuaHookOnAreaTrigger)
	RegisterHook(SERVER_HOOK_EVENT_ON_POST_LEVELUP, (void*)LuaHookOnPostLevelUp)
	RegisterHook(SERVER_HOOK_EVENT_ON_PRE_DIE, (void*)LuaHookOnPreUnitDie)
	RegisterHook(SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE, (void*)LuaHookOnAdvanceSkillLine)
	RegisterHook(SERVER_HOOK_EVENT_ON_DUEL_FINISHED, (void*)LuaHookOnDuelFinished)
	RegisterHook(SERVER_HOOK_EVENT_ON_AURA_REMOVE, (void*)LuaHookOnAuraRemove)
	RegisterHook(SERVER_HOOK_EVENT_ON_RESURRECT, (void*)LuaHookOnResurrect)
}

/*void lua_engine::shutdownEngine()
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
		Arcemu::Sleep(100);
	Log.Success("LuaEngine", "All lua states have successfully shutdown.");
}*/

void lua_engine::unload_resources(PLUA_INSTANCE lu)
{
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
		cleanup_varparam(it->second, lu->lu);
	lu->m_creatureFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_goFRefs.begin(); it != lu->m_goFRefs.end(); ++it)
		cleanup_varparam(it->second, lu->lu);
	lu->m_goFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_questFRefs.begin(); it != lu->m_questFRefs.end(); ++it)
		cleanup_varparam(it->second, lu->lu);
	lu->m_questFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_instanceFRefs.begin(); it != lu->m_instanceFRefs.end(); ++it)
		cleanup_varparam(it->second, lu->lu);
	lu->m_instanceFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_unitGossipFRefs.begin(); it != lu->m_unitGossipFRefs.end(); ++it)
		cleanup_varparam(it->second, lu->lu);
	lu->m_unitGossipFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_itemGossipFRefs.begin(); it != lu->m_itemGossipFRefs.end(); ++it)
		cleanup_varparam(it->second, lu->lu);
	lu->m_itemGossipFRefs.clear();

	for(li::ObjectFRefMap::iterator it = lu->m_goGossipFRefs.begin(); it != lu->m_goGossipFRefs.end(); ++it)
		cleanup_varparam(it->second, lu->lu);
	lu->m_goGossipFRefs.clear();

	lu->m_hooks.clear();

	for(li::References::iterator it = lu->m_globalFRefs.begin(); it != lu->m_globalFRefs.end(); ++it)
		cleanup_varparam((*it), lu->lu);
	lu->m_globalFRefs.clear();

	lu->coroutines_.clear();

	for(li::SpellFRefMap::iterator it = lu->m_dummySpells.begin(); it != lu->m_dummySpells.end(); ++it)
	{
		cleanup_varparam(it->second->params, lu->lu);
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
		delete itr->second;
	le::compiled_scripts.clear();

	assert(LUA_COMPILER->lu != NULL);
	lua_close(LUA_COMPILER->lu);
	LUA_COMPILER->lu = lua_open();
	LUA_COMPILER->scripts_.state = LUA_COMPILER->lu;
	LUA_COMPILER->scripts_.initialize();
	lua_instance = LUA_COMPILER;
	loadState(LUA_COMPILER);
	//re-compile the scripts.
	loadScripts();
	le::scriptLock.Release();
	//register any new bindings.
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitBinding.begin(); itr != LUA_COMPILER->m_unitBinding.end(); ++itr)
		if(!m_scriptMgr->has_creature_script(itr->first))
			m_scriptMgr->register_creature_script(itr->first, createluacreature);

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goBinding.begin(); itr != LUA_COMPILER->m_goBinding.end(); ++itr)
		if(!m_scriptMgr->has_gameobject_script(itr->first))
			m_scriptMgr->register_gameobject_script(itr->first, createluagameobject);

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_questBinding.begin(); itr != LUA_COMPILER->m_questBinding.end(); ++itr)
	{
		if(!m_scriptMgr->has_quest_script(itr->first))
		{
			QuestScript* qs = createluaquest(itr->first);
			if(qs != NULL)
				m_scriptMgr->register_quest_script(itr->first, qs);
		}
	}
	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_unitGossipBinding.begin(); itr != LUA_COMPILER->m_unitGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = createunitgossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_creature_gossip(itr->first, gs);
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_itemGossipBinding.begin(); itr != LUA_COMPILER->m_itemGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = createitemgossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_item_gossip(itr->first, gs);
	}

	for(li::ObjectBindingMap::iterator itr = LUA_COMPILER->m_goGossipBinding.begin(); itr != LUA_COMPILER->m_goGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = creategogossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_go_gossip(itr->first, gs);
	}
	for(li::SpellFRefMap::iterator itr = LUA_COMPILER->m_dummySpells.begin(); itr != LUA_COMPILER->m_dummySpells.end(); ++itr)
	{
		if(!m_scriptMgr->has_dummy_spell_script(itr->first))
			m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
	}

	//Tell all our active states to restart.
	for(le::ActiveStates::iterator itr = le::activeStates.begin(); itr != le::activeStates.end(); ++itr)
	{
		TimedEvent* ev = TimedEvent::Allocate((*itr), new CallBackFunctionP1<MapMgr*>(&le::restartThread, (*itr)), EVENT_MAPMGR_UPDATEOBJECTS, 1, 1);
		(*itr)->event_AddEvent(ev);
	}
}
void lua_engine::restartThread(MapMgr* map)
{
	//first grab the script lock.
	LuaGuard guard(le::scriptLock);
	//clean up our frefs and binding maps.
	PLUA_INSTANCE _li = lua_instance.get();
	le::unload_resources(_li);
	//close down the lua state, clearing up resources that were being used by the previously loaded scripts.
	if(_li->lu != NULL)
		lua_close(_li->lu);
	//open a brand new state.
	_li->lu = lua_open();
	_li->scripts_.state = _li->lu;
	_li->scripts_.initialize();
	//re-expose our wow objects and functions
	le::loadState(_li);
	//now we reload the scripts
	le::loadScripts(_li);
	//since we may have new bindings, update our contained interfaces to use the new bindings so that script changes may take effect

	//since we may have new bindings, lets register them w/ ScriptMgr.
	for(li::ObjectBindingMap::iterator itr = _li->m_unitBinding.begin(); itr != _li->m_unitBinding.end(); ++itr)
		if(!m_scriptMgr->has_creature_script(itr->first))
			m_scriptMgr->register_creature_script(itr->first, createluacreature);

	for(li::ObjectBindingMap::iterator itr = _li->m_goBinding.begin(); itr != _li->m_goBinding.end(); ++itr)
		if(!m_scriptMgr->has_gameobject_script(itr->first))
			m_scriptMgr->register_gameobject_script(itr->first, createluagameobject);

	for(li::ObjectBindingMap::iterator itr = _li->m_questBinding.begin(); itr != _li->m_questBinding.end(); ++itr)
	{
		if(!m_scriptMgr->has_quest_script(itr->first))
		{
			QuestScript* qs = createluaquest(itr->first);
			if(qs != NULL)
				m_scriptMgr->register_quest_script(itr->first, qs);
		}
	}
	for(li::ObjectBindingMap::iterator itr = _li->m_unitGossipBinding.begin(); itr != _li->m_unitGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = createunitgossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_creature_gossip(itr->first, gs);
	}

	for(li::ObjectBindingMap::iterator itr = _li->m_itemGossipBinding.begin(); itr != _li->m_itemGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = createitemgossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_item_gossip(itr->first, gs);
	}

	for(li::ObjectBindingMap::iterator itr = _li->m_goGossipBinding.begin(); itr != _li->m_goGossipBinding.end(); ++itr)
	{
		Arcemu::Gossip::Script* gs = creategogossipInterface(itr->first);
		if(gs != NULL)
			m_scriptMgr->register_go_gossip(itr->first, gs);
	}
	for(li::SpellFRefMap::iterator itr = _li->m_dummySpells.begin(); itr != _li->m_dummySpells.end(); ++itr)
	{
		if(!m_scriptMgr->has_dummy_spell_script(itr->first))
			m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
	}

	//since we may have new bindings, update our contained interfaces to use the new bindings so that script changes may take effect
	if(_li->m_creatureInterfaceMap.size())
	{
		for(li::ObjectBindingMap::iterator itr = _li->m_unitBinding.begin(); itr != _li->m_unitBinding.end(); ++itr)
		{
			std::pair<li::CreatureInterfaceMap::iterator, li::CreatureInterfaceMap::iterator> interfaces = _li->m_creatureInterfaceMap.equal_range(itr->first);
			for(; interfaces.first != interfaces.second; ++interfaces.first)
				interfaces.first->second->m_binding = itr->second;
		}
	}

	if(_li->m_goInterfaceMap.size())
	{
		for(li::ObjectBindingMap::iterator itr = _li->m_goBinding.begin(); itr != _li->m_goBinding.end(); ++itr)
		{
			std::pair<li::GOInterfaceMap::iterator, li::GOInterfaceMap::iterator> interfaces = _li->m_goInterfaceMap.equal_range(itr->first);
			for(; interfaces.first != interfaces.second; ++interfaces.first)
				interfaces.first->second->m_binding = itr->second;
		}
	}

	if(_li->m_questInterfaceMap.size())
	{
		for(li::ObjectBindingMap::iterator itr = _li->m_questBinding.begin(); itr != _li->m_questBinding.end(); ++itr)
		{
			std::pair<li::QuestInterfaceMap::iterator, li::QuestInterfaceMap::iterator> interfaces = _li->m_questInterfaceMap.equal_range(itr->first);
			for(; interfaces.first != interfaces.second; ++interfaces.first)
				interfaces.first->second->m_binding = itr->second;
		}
	}

	if(_li->m_unit_gossipInterfaceMap.size())
	{
		for(li::ObjectBindingMap::iterator itr = _li->m_unitGossipBinding.begin(); itr != _li->m_unitGossipBinding.end(); ++itr)
		{
			li::GossipInterfaceMap::iterator it = _li->m_unit_gossipInterfaceMap.find(itr->first),
			                                 itend = _li->m_unit_gossipInterfaceMap.end();
			if(it != itend)
				it->second->m_unit_gossip_binding = itr->second;
		}
	}

	if(_li->m_go_gossipInterfaceMap.size())
	{
		for(li::ObjectBindingMap::iterator itr = _li->m_goGossipBinding.begin(); itr != _li->m_goGossipBinding.end(); ++itr)
		{
			li::GossipInterfaceMap::iterator it = _li->m_go_gossipInterfaceMap.find(itr->first),
			                                 itend = _li->m_go_gossipInterfaceMap.end();
			if(it != itend)
				it->second->m_go_gossip_binding = itr->second;
		}
	}

	if(_li->m_item_gossipInterfaceMap.size())
	{
		for(li::ObjectBindingMap::iterator itr = _li->m_itemGossipBinding.begin(); itr != _li->m_itemGossipBinding.end(); ++itr)
		{
			li::GossipInterfaceMap::iterator it = _li->m_item_gossipInterfaceMap.find(itr->first),
			                                 itend = _li->m_item_gossipInterfaceMap.end();
			if(it != itend)
				it->second->m_item_gossip_binding = itr->second;
		}
	}
}
void lua_engine::shutdownThread(MapMgr* map)
{
	GET_LOCK;
	PLUA_INSTANCE ref = lua_instance.get();
	unload_resources(ref);
	lua_close(ref->lu);
	delete ref;
	lua_instance = NULL;
	LuaGuard guard(le::activestates_lock);
	le::ActiveStates::iterator itr = le::activeStates.find(map);
	if(itr != le::activeStates.end())
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
		bindQuestMethods(m);
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
	void loadScripts(PLUA_INSTANCE instance)
	{
		lua_State* lu = instance->lu;
		//LuaGuard guard(le::scriptLock);
		le::LuaScriptData::iterator it, itr = le::compiled_scripts.begin();
		for(; itr != le::compiled_scripts.end();)
		{
			it = itr++;
			if(instance->scripts_.isLoaded(it->first))
				continue;
			//check if our map id matches that of our script.
			if(it->second->maps_.size() && it->second->maps_.find((int32)instance->map->GetMapId()) == it->second->maps_.end())
				continue;
			if(lua_load(lu, le::readScript, it->second, it->first.c_str()) || lua_pcall(lu, 0, 0, 0))
			{
				report(lu);
				delete it->second;
				le::compiled_scripts.erase(it);
			}
			else
				instance->scripts_.add(it->first);
		}
	}
	const char* readScript(lua_State* L, void* ud, size_t* size)
	{
		PLUA_SCRIPT script = (PLUA_SCRIPT)ud;
		const char* ret = NULL;
		if(script->readpos_ >= script->datasize_)
		{
			*size = 0;
			script->readpos_ = 0;
		}
		else
		{
			ret = (const char*)(script->data_);
			*size = script->datasize_;
			script->readpos_ = script->datasize_;
		}
		return ret;
	}

	void parseHeader(PLUA_SCRIPT script)
	{
		if(script != NULL && script->datasize_)
		{
			std::string stringdata((const char*)script->data_);
			//locate header.
			size_t prefix = stringdata.find(ARCLUA_PREFIX);
			size_t suffix;
			if(prefix != string::npos)
			{
				suffix = stringdata.find(ARCLUA_SUFFIX, prefix);
				if(suffix != string::npos)
				{
					std::string header = stringdata.substr(prefix + ARCLUA_PREFIX_SIZE, (suffix - (prefix + ARCLUA_PREFIX_SIZE)));
					//search for 'map=' or 'MAP='
					size_t start = header.find("map=");
					if(start == string::npos)
						start = header.find("MAP=");
					if(start != string::npos)
					{
						//tokenize
						Arcemu::Utility::CTokenizer tokens(header.substr(start + 4, (header.length() - (start + 4))), ",");
						Arcemu::Utility::CTokenizer::iterator itr = tokens.start();
						for(; itr != tokens.end(); ++itr)
							script->maps_.insert(atoi((*itr).c_str()));
					}
				}
			}
		}
	}


	/*int dumpScript(lua_State * L, const void * data, size_t data_size, void* chunk)
	{
		if(data_size > 0)
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
		}
		return 0;
	}
	void dumpScripts2HDD()
	{
		std::string fullpath;
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
		Log.Notice("LuaEngine","Done dumping compile scripts.");
	}*/
}



