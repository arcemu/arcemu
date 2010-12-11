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
LuaEngine g_luaMgr;
	

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{ 
	return SCRIPT_TYPE_SCRIPT_ENGINE | SCRIPT_TYPE_SCRIPT_ENGINE_LUA;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	m_scriptMgr = mgr;
	sLuaMgr.Startup();
}
extern "C" SCRIPT_DECL void _export_engine_reload()
{
	sLuaMgr.Restart();
}

template<typename T> const char * GetTClassName() { return "UNKNOWN"; }
template<> const char * GetTClassName<Unit>() { return "Unit"; }
template<> const char * GetTClassName<Item>() { return "Item"; }
template<> const char * GetTClassName<GameObject>() { return "GameObject"; }
template<> const char * GetTClassName<WorldPacket>() { return "LuaPacket"; }
template<> const char * GetTClassName<TaxiPath>() { return "LuaTaxi"; }
template<> const char * GetTClassName<Spell>() { return "Spell"; }
template<> const char * GetTClassName<Field>() { return "SQL_Field"; }
template<> const char * GetTClassName<QueryResult>() { return "SQL_QResult"; }
template<> const char * GetTClassName<Aura>() { return "LuaAura"; }

template<typename T> RegType<T>* GetMethodTable();
template<> RegType<Unit>* GetMethodTable<Unit>();
template<> RegType<Item>* GetMethodTable<Item>();
template<> RegType<GameObject>* GetMethodTable<GameObject>();
template<> RegType<WorldPacket>* GetMethodTable<WorldPacket>();
template<> RegType<TaxiPath>* GetMethodTable<TaxiPath>();
template<> RegType<Spell>* GetMethodTable<Spell>();
template<> RegType<Field>* GetMethodTable<Field>();
template<> RegType<QueryResult>* GetMethodTable<QueryResult>();
template<> RegType<Aura>* GetMethodTable<Aura>();

void report(lua_State * L)
{
	const char * msg= lua_tostring(L,-1);
	while(msg)
	{
		lua_pop(L,-1);
		printf("\t%s\n", msg);
		msg=lua_tostring(L,-1);
	}
}

void LuaEngine::ScriptLoadDir(char* Dirname, LUALoadScripts *pak)
{
	#ifdef WIN32
		Log.Notice("LuaEngine", "Scanning Directory %s", Dirname);
		HANDLE hFile;
		WIN32_FIND_DATA FindData;
		memset(&FindData,0,sizeof(FindData));

		char SearchName[MAX_PATH];
	        
		strcpy(SearchName,Dirname);
		strcat(SearchName,"\\*.*");

		hFile = FindFirstFile(SearchName,&FindData);
		FindNextFile(hFile, &FindData);
	    
		while( FindNextFile(hFile, &FindData) )
		{
			if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) //Credits for this 'if' go to Cebernic from ArcScripts Team. Thanks, you saved me some work ;-)
			{
				strcpy(SearchName,Dirname);
				strcat(SearchName,"\\");
				strcat(SearchName,FindData.cFileName);
				ScriptLoadDir(SearchName, pak);
			}
			else
			{
						string fname = Dirname;
						fname += "\\";
						fname += FindData.cFileName;

			  		size_t len = strlen(fname.c_str());
					  int i=0;
					  char ext[MAX_PATH];
					  
					  while(len > 0)
					  {  
					   ext[i++] = fname[--len];
					   if(fname[len] == '.')
		  		   break;
	  			  }
	  			  ext[i++] = '\0';
	  			  if ( !_stricmp(ext,"aul.") ) pak->luaFiles.insert(fname);
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
		Log.Notice("LuaEngine", "Scanning Directory %s", Dirname);
		while(filecount--)
		{
			char dottedrelpath[200];
			sprintf(dottedrelpath, "%s/%s", Dirname, list[filecount]->d_name);
			if(stat(dottedrelpath, &attributes) == -1) {
				err=true;
				Log.Error("LuaEngine","Error opening %s: %s", dottedrelpath, strerror(errno));
			} else err=false;

			if (!err && S_ISDIR(attributes.st_mode))
			{
				ScriptLoadDir((char *)dottedrelpath, pak); //Subdirectory
			} else {
				char* ext = strrchr(list[filecount]->d_name, '.');
				if(ext != NULL && !strcmp(ext, ".lua"))
				{
					pak->luaFiles.insert(dottedrelpath);
				}
			}

			free(list[filecount]);
		}
		free(list);
	#endif
}

void LuaEngine::LoadScripts()
{
	LUALoadScripts rtn;
	Log.Notice("LuaEngine", "Scanning Script-Directories...");
	ScriptLoadDir((char*)"scripts", &rtn);

	unsigned int cnt_uncomp=0;

	luaL_openlibs(lu);
	RegisterCoreFunctions();
	Log.Notice("LuaEngine", "Loading Scripts...");

	char filename[200];

	for(set<string>::iterator itr = rtn.luaFiles.begin(); itr != rtn.luaFiles.end(); ++itr)
	{ 
		strcpy(filename, itr->c_str());
		if(luaL_loadfile(lu, filename) != 0)
		{
			Log.Error("LuaEngine", "loading %s failed.(could not load)", itr->c_str());
			report(lu);
		}
		else
		{
			if(lua_pcall(lu, 0, 0, 0) != 0)
			{
				Log.Error("LuaEngine", "%s failed.(could not run)", itr->c_str());
				report(lu);
			}
			else
					Log.Debug("LuaEngine", "loaded %s.", itr->c_str());
		}
		cnt_uncomp++;
	}
	Log.Notice("LuaEngine","Loaded %u Lua scripts.", cnt_uncomp);
}


/*******************************************************************************
	FUNCTION CALL METHODS
*******************************************************************************/

void LuaEngine::BeginCall(uint16 fReference)
{
	lua_settop(lu,0); //stack should be empty
	lua_getref(lu,fReference);
}
bool LuaEngine::ExecuteCall(uint8 params, uint8 res)
{
	bool ret = true;
	int top = lua_gettop(lu);
	if(strcmp(luaL_typename(lu,top-params),"function") )
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
	}
	return ret;
}
void LuaEngine::EndCall(uint8 res) 
{
	for(int i = res; i > 0; i--)
	{
		if(!lua_isnone(lu,res))
			lua_remove(lu,res);
	}
}
/*******************************************************************************
	END FUNCTION CALL METHODS
*******************************************************************************/

/******************************************************************************
	PUSH METHODS
******************************************************************************/

void LuaEngine::PUSH_UNIT(Object * unit, lua_State * L) 
{
	Unit * pUnit = NULL;
	if(unit != NULL && unit->IsUnit() ) 
		pUnit = TO_UNIT(unit);
	if(L == NULL)
		ArcLuna<Unit>::push(lu,pUnit);
	else
		ArcLuna<Unit>::push(L,pUnit);
}
void LuaEngine::PUSH_GO(Object *go, lua_State *L)
{
	GameObject * pGo = NULL;
	if(go != NULL && go->IsGameObject() )
		pGo = TO< GameObject* >(go);
	if(L == NULL)
		ArcLuna<GameObject>::push(lu,pGo);
	else
		ArcLuna<GameObject>::push(L,pGo);
}
void LuaEngine::PUSH_ITEM(Object * item, lua_State *L)
{
	Item * pItem = NULL;
	if(item != NULL && (item->IsItem() || item->IsContainer()))
		pItem = TO< Item* >(item);
	if(L == NULL)
		ArcLuna<Item>::push(lu,pItem);
	else
		ArcLuna<Item>::push(L,pItem);
}
void LuaEngine::PUSH_GUID(uint64 guid, lua_State * L) 
{
	if(L == NULL)
		GUID_MGR::push(lu,guid);
	else
		GUID_MGR::push(L,guid);
}
void LuaEngine::PUSH_PACKET(WorldPacket * pack, lua_State * L) 
{
	if(L == NULL)
		ArcLuna<WorldPacket>::push(lu,pack,true);
	else
		ArcLuna<WorldPacket>::push(L,pack,true);
}
void LuaEngine::PUSH_TAXIPATH(TaxiPath * tp, lua_State * L) 
{
	if(L == NULL)
		ArcLuna<TaxiPath>::push(lu,tp,true);
	else
		ArcLuna<TaxiPath>::push(L,tp,true);
}
void LuaEngine::PUSH_SPELL(Spell * sp, lua_State * L) 
{
	if(L == NULL)
		ArcLuna<Spell>::push(lu,sp);
	else
		ArcLuna<Spell>::push(L,sp);
}
void LuaEngine::PUSH_SQLFIELD(Field *field, lua_State *L)
{
	if(L == NULL)
		ArcLuna<Field>::push(lu,field);
	else
		ArcLuna<Field>::push(L,field);
}
void LuaEngine::PUSH_SQLRESULT(QueryResult * res, lua_State * L)
{
	if(L == NULL)
		ArcLuna<QueryResult>::push(lu,res,true);
	else
		ArcLuna<QueryResult>::push(L,res,true);
}
void LuaEngine::PUSH_AURA(Aura * aura, lua_State * L)
{
	if(L == NULL)
		ArcLuna<Aura>::push(lu,aura);
	else
		ArcLuna<Aura>::push(L,aura);
}

/*******************************************************************************
	END PUSH METHODS
*******************************************************************************/

void LuaEngine::HyperCallFunction(const char * FuncName, int ref) //hyper as in hypersniper :3
{
	GET_LOCK
	string sFuncName = string(FuncName);
	char * copy = strdup(FuncName);
	char * token = strtok(copy,".:");
	int top = 1;
	bool colon = false;
	//REMEMBER: top is always 1
	lua_settop(lu,0); //stack should be empty
	if (strpbrk(FuncName,".:") == NULL ) //stack: empty
		lua_getglobal(lu,FuncName);  //stack: function
	else
	{
		lua_getglobal(lu, "_G"); //start out with the global table.  //stack: _G
		while (token != NULL)
		{
			lua_getfield(lu, -1, token); //get the (hopefully) table/func  //stack: _G, subtable/func/nil
			if ((int)sFuncName.find(token)-1 > 0) //if it isn't the first token
			{
				if (sFuncName.at(sFuncName.find(token)-1) == ':')
					colon = true;
			}

			if (lua_isfunction(lu,-1) && !lua_iscfunction(lu,-1)) //if it's a Lua function //stack: _G/subt, func
			{
				if (colon) //stack: subt, func
				{
					lua_pushvalue(lu, -2); //make the table the first arg //stack: subt, func, subt
					lua_remove(lu,top); //stack: func, subt
				}
				else
					lua_replace(lu,top); //stack: func
				break; //we don't need anything else
			}
			else if(lua_istable(lu,-1) ) //stack: _G/subt, subtable
			{
				token = strtok(NULL,".:"); //stack: _G/subt, subtable
				lua_replace(lu, top); //stack: subtable
			}
		}
	}
	lua_rawgeti(lu, LUA_REGISTRYINDEX, ref);
	lua_State * M = lua_tothread(lu, -1); //repeats, args
	int thread = lua_gettop(lu);
	int repeats = luaL_checkinteger(M, 1); //repeats, args
	int nargs = lua_gettop(M)-1;
	if (nargs != 0) //if we HAVE args...
	{
		for (int i = 2; i <= nargs+1; i++)
		{
			lua_pushvalue(M,i);
		}
		lua_xmove(M, lu, nargs);
	}
	if(repeats != 0)
	{
		if (--repeats == 0) //free stuff, then
		{
			free((void*)FuncName);
			luaL_unref(lu, LUA_REGISTRYINDEX, ref);
			hash_map<int, EventInfoHolder*>::iterator itr = sLuaMgr.m_registeredTimedEvents.find(ref);
			sLuaMgr.m_registeredTimedEvents.erase(itr);
		}
		else
		{
			lua_remove(M, 1); //args
			lua_pushinteger(M, repeats); //args, repeats
			lua_insert(M, 1); //repeats, args
		}
	}
	lua_remove(lu, thread); //now we can remove the thread object
	int r = lua_pcall(lu,nargs+(colon ? 1 : 0),0,0);
	if (r)
		report(lu);

	free((void*)copy);
	lua_settop(lu,0);
	RELEASE_LOCK
}

/*
	This version only accepts actual Lua functions and no arguments. See LCF_Extra:CreateClosure(...) to pass arguments to this function.
	*/
static int CreateLuaEvent(lua_State * L)
{
	GET_LOCK
	int delay = luaL_checkinteger(L,2);
	int repeats = luaL_checkinteger(L,3);
	if(!strcmp(luaL_typename(L,1),"function") || delay > 0)
	{
		lua_settop(L,1);
		int functionRef = lua_ref(L,true);
		TimedEvent * ev = TimedEvent::Allocate(World::getSingletonPtr(),new CallbackP1<LuaEngine,int>(&sLuaMgr,&LuaEngine::CallFunctionByReference,functionRef),0,delay,repeats);
		ev->eventType  = LUA_EVENTS_END+functionRef; //Create custom reference by adding the ref number to the max lua event type to get a unique reference for every function.
		sWorld.event_AddEvent(ev);
		sLuaMgr.getFunctionRefs().insert(functionRef);
		lua_pushinteger(L,functionRef);
	}
	else
		lua_pushnil(L);
	RELEASE_LOCK
	return 1;
}
void LuaEngine::CallFunctionByReference(int ref)
{
	GET_LOCK
		
	lua_getref(lu,ref);
	if(lua_pcall(lu,0,0,0) )
		report(lu);
	RELEASE_LOCK
}
void LuaEngine::DestroyAllLuaEvents()
{
	GET_LOCK
	//Clean up for all events.
	set<int>::iterator itr = m_functionRefs.begin();
	for(; itr != m_functionRefs.end(); ++itr)
	{
		sEventMgr.RemoveEvents(World::getSingletonPtr(),(*itr)+LUA_EVENTS_END);
		lua_unref(lu,(*itr));
	}
	m_functionRefs.clear();
	RELEASE_LOCK
}
static int ModifyLuaEventInterval(lua_State * L)
{
	GET_LOCK
	int ref = luaL_checkinteger(L,1);
	int newinterval = luaL_checkinteger(L,2);
	ref+= LUA_EVENTS_END;
	//Easy interval modification.
	sEventMgr.ModifyEventTime(World::getSingletonPtr(),ref,newinterval);
	RELEASE_LOCK
	return 0;
}
static int DestroyLuaEvent(lua_State * L)
{
	//Simply remove the reference, CallFunctionByReference will find the reference has been freed and skip any processing.
	GET_LOCK
	int ref = luaL_checkinteger(L,1);
	lua_unref(L,ref);
	sLuaMgr.getFunctionRefs().erase(ref);
	sEventMgr.RemoveEvents(World::getSingletonPtr(),ref+LUA_EVENTS_END);
	RELEASE_LOCK
	return 0;
}
static int ExtractfRefFromCString(lua_State * L,const char * functionName)
{
	int functionRef = 0;
	int top = lua_gettop(L);
	if(functionName != NULL)
	{
		char * copy = strdup(functionName);
		char * token = strtok(copy, ".:");
		if (strpbrk(functionName,".:") == NULL)
		{
			lua_getglobal(L,functionName);
			if (lua_isfunction(L,-1) && !lua_iscfunction(L,-1))
				functionRef = lua_ref(L,true);
			else
				luaL_error(L,"Reference creation failed! (%s) is not a valid Lua function. \n",functionName);
		}
		else
		{
			lua_getglobal(L,"_G");
			while (token != NULL)
			{
				lua_getfield(L,-1,token);
				if (lua_isfunction(L,-1) && !lua_iscfunction(L,-1))
				{
					functionRef = lua_ref(L,true);
					break;
				}
				else if (lua_istable(L,-1) )
				{
					token = strtok(NULL,".:");
					continue;
				}
				else
				{
					luaL_error(L,"Reference creation failed! (%s) is not a valid Lua function. \n",functionName);
					break;
				}
			}
		}
		free((void*)copy);
		lua_settop(L,top);
	}
	return functionRef;
}

static int RegisterServerHook(lua_State * L);
static int RegisterUnitEvent(lua_State * L);
static int RegisterQuestEvent(lua_State * L);
static int RegisterGameObjectEvent(lua_State * L);
static int RegisterUnitGossipEvent(lua_State * L);
static int RegisterItemGossipEvent(lua_State * L);
static int RegisterGOGossipEvent(lua_State * L);
static int SuspendLuaThread(lua_State * L);
static int RegisterTimedEvent(lua_State * L);
static int RemoveTimedEvents(lua_State * L);
static int RegisterDummySpell(lua_State * L);
static int RegisterInstanceEvent(lua_State * L);
void RegisterGlobalFunctions(lua_State*);

void LuaEngine::RegisterCoreFunctions()
{
	lua_register(lu,"RegisterUnitEvent",RegisterUnitEvent);
	lua_register(lu,"RegisterGameObjectEvent",RegisterGameObjectEvent);
	lua_register(lu,"RegisterQuestEvent",RegisterQuestEvent);
	lua_register(lu,"RegisterUnitGossipEvent",RegisterUnitGossipEvent);
	lua_register(lu,"RegisterItemGossipEvent",RegisterItemGossipEvent);
	lua_register(lu,"RegisterGOGossipEvent",RegisterGOGossipEvent);
	lua_register(lu,"RegisterServerHook",RegisterServerHook);
	lua_register(lu,"SuspendThread",&SuspendLuaThread);
	lua_register(lu,"RegisterTimedEvent",&RegisterTimedEvent);
	lua_register(lu,"RemoveTimedEvents",&RemoveTimedEvents);
	lua_register(lu,"RegisterDummySpell",&RegisterDummySpell);
	lua_register(lu,"RegisterInstanceEvent",&RegisterInstanceEvent);

	lua_register(lu,"CreateLuaEvent", &CreateLuaEvent);
	lua_register(lu,"ModifyLuaEventInterval",&ModifyLuaEventInterval);
	lua_register(lu,"DestroyLuaEvent",&DestroyLuaEvent);

	RegisterGlobalFunctions(lu);

	ArcLuna<Unit>::Register(lu);
	ArcLuna<Item>::Register(lu);
	ArcLuna<GameObject>::Register(lu);
	ArcLuna<WorldPacket>::Register(lu);
	ArcLuna<TaxiPath>::Register(lu);
	ArcLuna<Spell>::Register(lu);
	ArcLuna<Field>::Register(lu);
	ArcLuna<QueryResult>::Register(lu);
	ArcLuna<Aura>::Register(lu);
	
	GUID_MGR::Register(lu);
	
	//set the suspendluathread a coroutine function
	lua_getglobal(lu,"coroutine");
	if(lua_istable(lu,-1) )
	{
		lua_pushcfunction(lu,SuspendLuaThread);
		lua_setfield(lu,-2,"wait");
		lua_pushcfunction(lu,SuspendLuaThread);
		lua_setfield(lu,-2,"WAIT");
	}
	lua_pop(lu,1);
}

static int RegisterServerHook(lua_State * L)
{
	uint16 functionRef = 0;
	//Maximum passed in arguments, consider rest as garbage
	lua_settop(L,2);
	uint32 ev = luaL_checkint(L, 1);
	const char * typeName = luaL_typename(L,2);
	if(!ev || typeName == NULL) return 0;
	//For functions directly passed in, skip all that code and register the reference.
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,2));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_SERVHOOK,0,ev,functionRef);
	lua_pop(L,2);
	return 0;
}

static int RegisterDummySpell(lua_State * L)
{
	uint16 functionRef = 0;
	uint32 entry = luaL_checkint(L, 1);
	const char * typeName = luaL_typename(L, 2);
	lua_settop(L,2);

	if(!entry || typeName == NULL) return 0;

	if (m_luaDummySpells.find(entry) != m_luaDummySpells.end())
	{
		luaL_error(L,"LuaEngineMgr : RegisterDummySpell failed! Spell %d already has a registered Lua function!",entry);
	}
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,2));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_DUMMYSPELL,entry,0,functionRef);
	lua_pop(L,2);
	return 0;
}

static int RegisterUnitEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!entry || !ev || typeName == NULL ) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_UNIT,entry,ev,functionRef);
	lua_pop(L,3);
	return 0;
}

static int RegisterInstanceEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int map = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!map || !ev || typeName == NULL ) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_INSTANCE,map,ev,functionRef);
	lua_pop(L,3);
	return 0;
}

static int RegisterQuestEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!entry || !ev || typeName == NULL) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_QUEST,entry,ev,functionRef);
	lua_pop(L,3);
	return 0;
}

static int RegisterGameObjectEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!entry || !ev || typeName == NULL) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_GO,entry,ev,functionRef);
	lua_pop(L,3);
	return 0;
}

static int RegisterUnitGossipEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!entry || !ev || typeName == NULL) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_UNIT_GOSSIP,entry,ev,functionRef);
	lua_pop(L,3);
	return 0;
}
static int RegisterItemGossipEvent(lua_State * L)
 {
 	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!entry || !ev || typeName == NULL) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_ITEM_GOSSIP,entry,ev,functionRef);
	lua_pop(L,3);
	return 0;
 }
static int RegisterGOGossipEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);

	if(!entry || !ev || typeName == NULL) return 0;
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		sLuaMgr.RegisterEvent(REGTYPE_GO_GOSSIP,entry,ev,functionRef);
	lua_pop(L,3);
	return 0;
}

static int SuspendLuaThread(lua_State * L) {
	lua_State * thread = (lua_isthread(L,1)) ? lua_tothread(L,1) : NULL;
	if(thread == NULL) {
		return luaL_error(L,"LuaEngineMgr","SuspendLuaThread expected Lua coroutine, got NULL. \n");
	}
	int waitime = luaL_checkinteger(L,2);
	if(waitime <= 0) {
		return luaL_error(L,"LuaEngineMgr","SuspendLuaThread expected timer > 0 instead got (%d) \n",waitime);
	}
	lua_pushvalue(L,1);
	int ref = luaL_ref(L,LUA_REGISTRYINDEX);
	if(ref == LUA_REFNIL || ref == LUA_NOREF)
		return luaL_error(L,"Error in SuspendLuaThread! Failed to create a valid reference.");
	TimedEvent * evt = TimedEvent::Allocate(thread,new CallbackP1<LuaEngine,int>(&g_luaMgr,&LuaEngine::ResumeLuaThread,ref),0,waitime,1);
	sWorld.event_AddEvent(evt);
	lua_remove(L,1); // remove thread object
	lua_remove(L,1); // remove timer.
	//All that remains now are the extra arguments passed to this function.
	lua_xmove(L,thread,lua_gettop(L));
	g_luaMgr.getThreadRefs().insert(ref);
	return lua_yield(thread,lua_gettop(L));
}

static int RegisterTimedEvent(lua_State * L) //in this case, L == lu
{
	const char * funcName = strdup(luaL_checkstring(L,1));
	int delay = luaL_checkint(L,2);
	int repeats = luaL_checkint(L,3);
	if (!delay || repeats < 0 || !funcName)
	{
		lua_pushnumber(L, LUA_REFNIL);
		return 1;
	}
	lua_remove(L, 1); 
	lua_remove(L, 1);//repeats, args
	lua_State * thread = lua_newthread(L); //repeats, args, thread
	lua_insert(L,1); //thread, repeats, args
	lua_xmove(L,thread,lua_gettop(L)-1); //thread
	int ref = luaL_ref(L, LUA_REGISTRYINDEX); //empty
	if(ref == LUA_REFNIL || ref == LUA_NOREF)
		return luaL_error(L,"Error in RegisterTimedEvent! Failed to create a valid reference.");
	TimedEvent *te = TimedEvent::Allocate(&sLuaMgr, new CallbackP2<LuaEngine, const char*, int>(&sLuaMgr, &LuaEngine::HyperCallFunction, funcName, ref), EVENT_LUA_TIMED, delay, repeats);
	EventInfoHolder * ek = new EventInfoHolder;
	ek->funcName = funcName;
	ek->te = te;
	sLuaMgr.m_registeredTimedEvents.insert( pair<int, EventInfoHolder*>(ref, ek) );
	sLuaEventMgr.event_AddEvent(te);
	lua_settop(L,0);
	lua_pushnumber(L, ref);
	return 1;
}


static int RemoveTimedEvents(lua_State * L) //in this case, L == lu
{
	sLuaEventMgr.RemoveEvents();
	return 0;
}


//all of these run similarly, they execute OnServerHook for all the functions in their respective event's list.
bool LuaHookOnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_NEW_CHARACTER].begin(); itr != EventAsToFuncName[SERVER_HOOK_NEW_CHARACTER].end(); ++itr) 
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_NEW_CHARACTER);
		sLuaMgr.PUSH_STRING(Name);
		sLuaMgr.PUSH_UINT(Race);
		sLuaMgr.PUSH_UINT(Class);
		if(sLuaMgr.ExecuteCall(4,1) )
		{
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

void LuaHookOnKillPlayer(Player * pPlayer, Player * pVictim)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_KILL_PLAYER].begin(); itr != EventAsToFuncName[SERVER_HOOK_KILL_PLAYER].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_KILL_PLAYER);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UNIT(pVictim);
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnFirstEnterWorld(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_FIRST_ENTER_WORLD].begin(); itr != EventAsToFuncName[SERVER_HOOK_FIRST_ENTER_WORLD].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_FIRST_ENTER_WORLD);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

void LuaHookOnEnterWorld(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_ENTER_WORLD].begin(); itr != EventAsToFuncName[SERVER_HOOK_ENTER_WORLD].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_ENTER_WORLD);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

void LuaHookOnGuildJoin(Player * pPlayer, Guild * pGuild)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_GUILD_JOIN].begin(); itr != EventAsToFuncName[SERVER_HOOK_GUILD_JOIN].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_GUILD_JOIN);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_STRING(pGuild->GetGuildName());
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnDeath(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_DEATH].begin(); itr != EventAsToFuncName[SERVER_HOOK_DEATH].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_DEATH);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

bool LuaHookOnRepop(Player * pPlayer)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_REPOP].begin(); itr != EventAsToFuncName[SERVER_HOOK_REPOP].end(); ++itr) 
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_REPOP);
		sLuaMgr.PUSH_UNIT(pPlayer);
		if (sLuaMgr.ExecuteCall(2,1)) 
		{
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

void LuaHookOnEmote(Player * pPlayer, uint32 Emote, Unit * pUnit)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_EMOTE].begin(); itr != EventAsToFuncName[SERVER_HOOK_EMOTE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_EMOTE);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UNIT(pUnit);
		sLuaMgr.PUSH_UINT(Emote);
		sLuaMgr.ExecuteCall(4);
	}
	RELEASE_LOCK
}

void LuaHookOnEnterCombat(Player * pPlayer, Unit * pTarget)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_ENTER_COMBAT].begin(); itr != EventAsToFuncName[SERVER_HOOK_ENTER_COMBAT].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_ENTER_COMBAT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UNIT(pTarget);
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

bool LuaHookOnCastSpell(Player * pPlayer, SpellEntry* pSpell, Spell* spell)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_CAST_SPELL].begin(); itr != EventAsToFuncName[SERVER_HOOK_CAST_SPELL].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_CAST_SPELL);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(pSpell->Id);
		sLuaMgr.PUSH_SPELL(spell);
		if( sLuaMgr.ExecuteCall(4,1) ) {
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

void LuaHookOnTick()
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_TICK].begin(); itr != EventAsToFuncName[SERVER_HOOK_TICK].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.ExecuteCall();
	}
	RELEASE_LOCK
}

bool LuaHookOnLogoutRequest(Player * pPlayer)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_LOGOUT_REQUEST].begin(); itr != EventAsToFuncName[SERVER_HOOK_LOGOUT_REQUEST].end(); itr++)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_LOGOUT_REQUEST);
		sLuaMgr.PUSH_UNIT(pPlayer);
		if(sLuaMgr.ExecuteCall(2,1) )
		{
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

void LuaHookOnLogout(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_LOGOUT].begin(); itr != EventAsToFuncName[SERVER_HOOK_LOGOUT].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_LOGOUT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

void LuaHookOnQuestAccept(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_QUEST_ACCEPT].begin(); itr != EventAsToFuncName[SERVER_HOOK_QUEST_ACCEPT].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_QUEST_ACCEPT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(pQuest->id);
		if(!pQuestGiver)
			sLuaMgr.PUSH_NIL();
		else if(pQuestGiver->IsUnit() )
			sLuaMgr.PUSH_UNIT(pQuestGiver);
		else if(pQuestGiver->IsGameObject() )
			sLuaMgr.PUSH_GO(pQuestGiver);
		else if(pQuestGiver->IsItem())
			sLuaMgr.PUSH_ITEM(pQuestGiver);
		else
			sLuaMgr.PUSH_NIL();
		sLuaMgr.ExecuteCall(4);
	}
	RELEASE_LOCK
}

void LuaHookOnZone(Player * pPlayer, uint32 Zone, uint32 oldZone)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_ZONE].begin(); itr != EventAsToFuncName[SERVER_HOOK_ZONE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_ZONE);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(Zone);
		sLuaMgr.PUSH_UINT(oldZone);
		sLuaMgr.ExecuteCall(4);
	}
	RELEASE_LOCK
}

bool LuaHookOnChat(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_CHAT].begin(); itr != EventAsToFuncName[SERVER_HOOK_CHAT].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_CHAT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_STRING(Message);
		sLuaMgr.PUSH_UINT(Type);
		sLuaMgr.PUSH_UINT(Lang);
		sLuaMgr.PUSH_STRING(Misc);
		if( sLuaMgr.ExecuteCall(6,1)) {
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

void LuaHookOnLoot(Player * pPlayer, Unit * pTarget, uint32 Money, uint32 ItemId)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_LOOT].begin(); itr != EventAsToFuncName[SERVER_HOOK_LOOT].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_LOOT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UNIT(pTarget);
		sLuaMgr.PUSH_UINT(Money);
		sLuaMgr.PUSH_UINT(ItemId);
		sLuaMgr.ExecuteCall(5);
	}
	RELEASE_LOCK
}

void LuaHookOnGuildCreate(Player * pLeader, Guild * pGuild)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_GUILD_CREATE].begin(); itr != EventAsToFuncName[SERVER_HOOK_GUILD_CREATE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_GUILD_CREATE);
		sLuaMgr.PUSH_UNIT(pLeader);
		sLuaMgr.PUSH_STRING(pGuild->GetGuildName());
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnEnterWorld2(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_ENTER_WORLD_2].begin(); itr != EventAsToFuncName[SERVER_HOOK_ENTER_WORLD_2].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_ENTER_WORLD_2);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

void LuaHookOnCharacterCreate(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_CHARACTER_CREATE].begin(); itr != EventAsToFuncName[SERVER_HOOK_CHARACTER_CREATE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_CHARACTER_CREATE);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

void LuaHookOnQuestCancelled(Player * pPlayer, Quest * pQuest)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_QUEST_CANCELLED].begin(); itr != EventAsToFuncName[SERVER_HOOK_QUEST_CANCELLED].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_QUEST_CANCELLED);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(pQuest->id);
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnQuestFinished(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_QUEST_FINISHED].begin(); itr != EventAsToFuncName[SERVER_HOOK_QUEST_FINISHED].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_QUEST_FINISHED);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(pQuest->id);
		if(!pQuestGiver)
			sLuaMgr.PUSH_NIL();
		else if(pQuestGiver->IsUnit() )
			sLuaMgr.PUSH_UNIT(pQuestGiver);
		else if(pQuestGiver->IsGameObject() )
			sLuaMgr.PUSH_GO(pQuestGiver);
		else if(pQuestGiver->IsItem())
			sLuaMgr.PUSH_ITEM(pQuestGiver);
		else
			sLuaMgr.PUSH_NIL();
		sLuaMgr.ExecuteCall(4);
	}
	RELEASE_LOCK
}

void LuaHookOnHonorableKill(Player * pPlayer, Player * pKilled)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_HONORABLE_KILL].begin(); itr != EventAsToFuncName[SERVER_HOOK_HONORABLE_KILL].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_HONORABLE_KILL);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UNIT(pKilled);
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnArenaFinish(Player * pPlayer, ArenaTeam* pTeam, bool victory, bool rated)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_ARENA_FINISH].begin(); itr != EventAsToFuncName[SERVER_HOOK_ARENA_FINISH].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_ARENA_FINISH);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_STRING(pTeam->m_name.c_str());
		sLuaMgr.PUSH_BOOL(victory);
		sLuaMgr.PUSH_BOOL(rated);
		sLuaMgr.ExecuteCall(5);
	}
	RELEASE_LOCK
}

void LuaHookOnObjectLoot(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_OBJECTLOOT].begin(); itr != EventAsToFuncName[SERVER_HOOK_OBJECTLOOT].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_OBJECTLOOT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UNIT(pTarget);
		sLuaMgr.PUSH_UINT(Money);
		sLuaMgr.PUSH_UINT(ItemId);
		sLuaMgr.ExecuteCall(5);
	}
	RELEASE_LOCK
}

void LuaHookOnAreaTrigger(Player * pPlayer, uint32 areaTrigger)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_AREATRIGGER].begin(); itr != EventAsToFuncName[SERVER_HOOK_AREATRIGGER].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_AREATRIGGER);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(areaTrigger);
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnPostLevelUp(Player * pPlayer)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_POST_LEVELUP].begin(); itr != EventAsToFuncName[SERVER_HOOK_POST_LEVELUP].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_POST_LEVELUP);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

bool LuaHookOnPreUnitDie(Unit *Killer, Unit *Victim)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_PRE_DIE].begin(); itr != EventAsToFuncName[SERVER_HOOK_PRE_DIE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_PRE_DIE);
		sLuaMgr.PUSH_UNIT(Killer);
		sLuaMgr.PUSH_UNIT(Victim);
		if( sLuaMgr.ExecuteCall(3,1)) {
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

void LuaHookOnAdvanceSkillLine(Player * pPlayer, uint32 SkillLine, uint32 Current)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_ADVANCE_SKILLLINE].begin(); itr != EventAsToFuncName[SERVER_HOOK_ADVANCE_SKILLLINE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_ADVANCE_SKILLLINE);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(SkillLine);
		sLuaMgr.PUSH_UINT(Current);
		sLuaMgr.ExecuteCall(4);
	}
	RELEASE_LOCK
}

void LuaHookOnDuelFinished(Player * pWinner, Player * pLoser)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_DUEL_FINISHED].begin(); itr != EventAsToFuncName[SERVER_HOOK_DUEL_FINISHED].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_DUEL_FINISHED);
		sLuaMgr.PUSH_UNIT(pWinner);
		sLuaMgr.PUSH_UNIT(pLoser);
		sLuaMgr.ExecuteCall(3);
	}
	RELEASE_LOCK
}

void LuaHookOnAuraRemove(Aura * aura)
{
	GET_LOCK
	for(std::vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_AURA_REMOVE].begin(); itr != EventAsToFuncName[SERVER_HOOK_AURA_REMOVE].end(); ++itr)
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_AURA_REMOVE);
		sLuaMgr.PUSH_AURA(aura);
		sLuaMgr.ExecuteCall(2);
	}
	RELEASE_LOCK
}

bool LuaHookOnResurrect(Player * pPlayer)
{
	GET_LOCK
	bool result = true;
	for(vector<uint16>::iterator itr = EventAsToFuncName[SERVER_HOOK_RESURRECT].begin(); itr != EventAsToFuncName[SERVER_HOOK_RESURRECT].end(); ++itr) 
	{
		sLuaMgr.BeginCall((*itr));
		sLuaMgr.PUSH_INT(SERVER_HOOK_RESURRECT);
		sLuaMgr.PUSH_UNIT(pPlayer);
		if (sLuaMgr.ExecuteCall(2,1)) 
		{
			lua_State * L = sLuaMgr.getluState();
			if(!lua_isnoneornil(L,1) && !lua_toboolean(L,1) )
				result = false;
			sLuaMgr.EndCall(1);
		}
	}
	RELEASE_LOCK
	return result;
}

bool LuaOnDummySpell(uint32 effectIndex, Spell * pSpell)
{
	GET_LOCK
	sLuaMgr.BeginCall(m_luaDummySpells[pSpell->GetProto()->Id]);
	sLuaMgr.PUSH_UINT(effectIndex);
	sLuaMgr.PUSH_SPELL(pSpell);
	sLuaMgr.ExecuteCall(2);
	RELEASE_LOCK
	return true;
}

class LuaCreature : public CreatureAIScript
{
public:
	LuaCreature(Creature* creature) : CreatureAIScript(creature), m_binding(NULL) {}
	~LuaCreature()
	{}
	ARCEMU_INLINE void SetUnit(Creature * ncrc) { _unit = ncrc; }
	void OnCombatStart(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_ENTER_COMBAT]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_ENTER_COMBAT);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}

	void OnCombatStop(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_LEAVE_COMBAT]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_LEAVE_COMBAT);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}

	void OnTargetDied(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_TARGET_DIED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_TARGET_DIED);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}

	void OnDied(Unit *mKiller)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_DIED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_DIED);
		sLuaMgr.PUSH_UNIT(mKiller);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}
	void OnTargetParried(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_TARGET_PARRIED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_TARGET_PARRIED);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}
	void OnTargetDodged(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_TARGET_DODGED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_TARGET_DODGED);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}
	void OnTargetBlocked(Unit* mTarget, int32 iAmount)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_TARGET_BLOCKED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_TARGET_BLOCKED);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_INT(iAmount);
		sLuaMgr.ExecuteCall(4);

		RELEASE_LOCK
	}
	void OnTargetCritHit(Unit* mTarget, int32 fAmount)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_TARGET_CRIT_HIT]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_TARGET_CRIT_HIT);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_INT(fAmount);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	void OnParried(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_PARRY]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_PARRY);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}
	void OnDodged(Unit* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_DODGED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_DODGED);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	}
	void OnBlocked(Unit* mTarget, int32 iAmount)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_BLOCKED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_BLOCKED);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_INT(iAmount);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	void OnCritHit(Unit* mTarget, int32 fAmount)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_CRIT_HIT]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_CRIT_HIT);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_INT(fAmount);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	void OnHit(Unit* mTarget, float fAmount)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_HIT]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_HIT);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_FLOAT(fAmount);
		sLuaMgr.ExecuteCall(4);

		RELEASE_LOCK
	}
	void OnAssistTargetDied(Unit* mAssistTarget)
	{
		
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_ASSIST_TARGET_DIED]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_ASSIST_TARGET_DIED);
		sLuaMgr.PUSH_UNIT(mAssistTarget);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}
	void OnFear(Unit* mFeared, uint32 iSpellId)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_FEAR]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_FEAR);
		sLuaMgr.PUSH_UNIT(mFeared);
		sLuaMgr.PUSH_UINT(iSpellId);
		sLuaMgr.ExecuteCall(4);

		RELEASE_LOCK
	}
	void OnFlee(Unit* mFlee)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_FLEE]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_FLEE);
		sLuaMgr.PUSH_UNIT(mFlee);
		sLuaMgr.ExecuteCall(3);

		RELEASE_LOCK
	}
	void OnCallForHelp()
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_CALL_FOR_HELP]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_CALL_FOR_HELP);
		sLuaMgr.ExecuteCall(2);

		RELEASE_LOCK
	}
	void OnLoad()
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_LOAD]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_LOAD);
		sLuaMgr.ExecuteCall(2);

		RELEASE_LOCK
		uint32 iid = _unit->GetInstanceID();
		if (_unit->GetMapMgr() == NULL || _unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
			iid = 0;
		OnLoadInfo.push_back(_unit->GetMapId());
		OnLoadInfo.push_back(iid);
		OnLoadInfo.push_back(GET_LOWGUID_PART(_unit->GetGUID()));
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_REACH_WP]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_REACH_WP);
		sLuaMgr.PUSH_UINT(iWaypointId);
		sLuaMgr.PUSH_BOOL(bForwards);
		sLuaMgr.ExecuteCall(4);

		RELEASE_LOCK
	}
	void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_LOOT_TAKEN]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_LOOT_TAKEN);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(pItemPrototype->ItemId);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	void AIUpdate()
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_AIUPDATE]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_AIUPDATE);
		sLuaMgr.ExecuteCall(2);

		RELEASE_LOCK
	}
	void OnEmote(Player * pPlayer, EmoteType Emote)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_EMOTE]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_EMOTE);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_INT((int32)Emote);
		sLuaMgr.ExecuteCall(4);

		RELEASE_LOCK
	}
	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[CREATURE_EVENT_ON_DAMAGE_TAKEN]);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.PUSH_INT(CREATURE_EVENT_ON_DAMAGE_TAKEN);
		sLuaMgr.PUSH_UNIT(mAttacker);
		sLuaMgr.PUSH_UINT(fAmount);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	void StringFunctionCall(int fRef)
	{
		
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(fRef);
		sLuaMgr.PUSH_UNIT(_unit);
		sLuaMgr.ExecuteCall(1);
		RELEASE_LOCK
	}
	void Destroy()
	{
		{
				typedef std::multimap<uint32,LuaCreature*> CMAP;
			CMAP & cMap = sLuaMgr.getLuCreatureMap();
			CMAP::iterator itr = cMap.find(_unit->GetEntry());
			CMAP::iterator itend = cMap.upper_bound(_unit->GetEntry());
			CMAP::iterator it;
			for(;itr != cMap.end() && itr != itend;)
			{
				it = itr++;
				if(it->second != NULL && it->second == this)
					cMap.erase(it);
			}
		}
		{
			//Function Ref clean up
			std::map< uint64,std::set<int> > & objRefs = sLuaMgr.getObjectFunctionRefs();
			std::map< uint64,std::set<int> >::iterator itr = objRefs.find(_unit->GetGUID());
			if(itr != objRefs.end() )
			{
				std::set<int> & refs = itr->second;
				for(std::set<int>::iterator it = refs.begin(); it != refs.end(); ++it)
				{
					lua_unref(sLuaMgr.getluState(),(*it));
					sEventMgr.RemoveEvents(_unit,(*it)+EVENT_LUA_CREATURE_EVENTS);
				}
				refs.clear();
			}
		}
		delete this;
	}
	LuaObjectBinding * m_binding;
};

class LuaGameObject : public GameObjectAIScript
{
public:
	LuaGameObject(GameObject * go) : GameObjectAIScript(go), m_binding(NULL) {}
	~LuaGameObject() {}
	ARCEMU_INLINE GameObject * getGO() { return _gameobject; }
	void OnCreate()
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[GAMEOBJECT_EVENT_ON_CREATE]);
		sLuaMgr.PUSH_GO(_gameobject);
		sLuaMgr.ExecuteCall(1);

		RELEASE_LOCK
	}
	void OnSpawn()
	{
		
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[GAMEOBJECT_EVENT_ON_SPAWN]);
		sLuaMgr.PUSH_GO(_gameobject);
		sLuaMgr.ExecuteCall(1);

		RELEASE_LOCK
	}
	void OnDespawn()
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[GAMEOBJECT_EVENT_ON_DESPAWN]);
		sLuaMgr.PUSH_GO(_gameobject);
		sLuaMgr.ExecuteCall(1);
		RELEASE_LOCK
	}
	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{
		
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[GAMEOBJECT_EVENT_ON_LOOT_TAKEN]);
		sLuaMgr.PUSH_GO(_gameobject);
		sLuaMgr.PUSH_UINT(GAMEOBJECT_EVENT_ON_LOOT_TAKEN);
		sLuaMgr.PUSH_UNIT(pLooter);
		sLuaMgr.PUSH_UINT(pItemInfo->ItemId);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	void OnActivate(Player * pPlayer)
	{
		CHECK_BINDING_ACQUIRELOCK

		sLuaMgr.BeginCall(m_binding->m_functionReferences[GAMEOBJECT_EVENT_ON_USE]);
		sLuaMgr.PUSH_GO(_gameobject);
		sLuaMgr.PUSH_UINT(GAMEOBJECT_EVENT_ON_USE);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	}
	
	void AIUpdate()
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[GAMEOBJECT_EVENT_AIUPDATE]);
		sLuaMgr.PUSH_GO(_gameobject);
		sLuaMgr.ExecuteCall(1);
		RELEASE_LOCK
	}
	void Destroy ()
	{
		typedef std::multimap<uint32,LuaGameObject*> GMAP;
		GMAP & gMap = sLuaMgr.getLuGameObjectMap();
		GMAP::iterator itr = gMap.find(_gameobject->GetEntry());
		GMAP::iterator itend = gMap.upper_bound(_gameobject->GetEntry());
		GMAP::iterator it;
		//uint64 guid = _gameobject->GetGUID(); Unused?
		for(; itr != itend;)
		{
			it = itr++;
			if(it->second != NULL && it->second == this)
				gMap.erase(it);
		}

		std::map< uint64,std::set<int> > & objRefs = sLuaMgr.getObjectFunctionRefs();
		std::map< uint64,std::set<int> >::iterator itr2 = objRefs.find(_gameobject->GetGUID());
		if(itr2 != objRefs.end() )
		{
			std::set<int> & refs = itr2->second;
			for(std::set<int>::iterator it = refs.begin(); it != refs.end(); ++it)
				lua_unref( sLuaMgr.getluState(), (*it) );
			refs.clear();
		}
		delete this;
	}
	LuaObjectBinding * m_binding;
};

class LuaGossip : public GossipScript
{
public:
	LuaGossip() : GossipScript(), m_unit_gossip_binding(NULL),m_item_gossip_binding(NULL),m_go_gossip_binding(NULL) {}
	~LuaGossip() {
		typedef HM_NAMESPACE::hash_map<uint32, LuaGossip*> MapType;
		MapType gMap;
		if(this->m_go_gossip_binding != NULL)
		{
			gMap = g_luaMgr.getGameObjectGossipInterfaceMap();
			for(MapType::iterator itr = gMap.begin(); itr != gMap.end(); ++itr)
			{
				if(itr->second == this) {
					gMap.erase(itr);
					break;
				}
			}
		}
		else if(this->m_unit_gossip_binding != NULL)
		{
			gMap = g_luaMgr.getUnitGossipInterfaceMap();
			for(MapType::iterator itr = gMap.begin(); itr != gMap.end(); ++itr)
			{
				if(itr->second == this)
				{
					gMap.erase(itr);
					break;
				}
			}
		}
		else if(this->m_item_gossip_binding != NULL)
		{
			gMap = g_luaMgr.getItemGossipInterfaceMap();
			for(MapType::iterator itr = gMap.begin(); itr != gMap.end(); ++itr)
			{
				if(itr->second == this)
				{
					gMap.erase(itr);
					break;
				}
			}
		}
	}

	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GET_LOCK
		if(pObject->IsCreature())
        {
			if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }

			sLuaMgr.BeginCall(m_unit_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_TALK]);
			sLuaMgr.PUSH_UNIT(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_TALK);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.PUSH_BOOL(AutoSend);
			sLuaMgr.ExecuteCall(4);
        }
        else if(pObject->IsItem())
        {
			if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }

			sLuaMgr.BeginCall(m_item_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_TALK]);
			sLuaMgr.PUSH_ITEM(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_TALK);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.PUSH_BOOL(AutoSend);
			sLuaMgr.ExecuteCall(4);
        }
		else if(pObject->IsGameObject())
        {
			if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }

            sLuaMgr.BeginCall(m_go_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_TALK]);
			sLuaMgr.PUSH_GO(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_TALK);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.PUSH_BOOL(AutoSend);
			sLuaMgr.ExecuteCall(4);
        }
		RELEASE_LOCK
	}

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		GET_LOCK
		if(pObject->IsCreature())
        {
			if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }

			sLuaMgr.BeginCall(m_unit_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_SELECT_OPTION]);
			sLuaMgr.PUSH_UNIT(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_SELECT_OPTION);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.PUSH_UINT(Id);
			sLuaMgr.PUSH_UINT(IntId);
			sLuaMgr.PUSH_STRING(EnteredCode);
			sLuaMgr.ExecuteCall(6);
        }
        else if(pObject->IsItem())
        {
			if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }
			sLuaMgr.BeginCall(m_item_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_SELECT_OPTION]);
			sLuaMgr.PUSH_ITEM(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_SELECT_OPTION);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.PUSH_UINT(Id);
			sLuaMgr.PUSH_UINT(IntId);
			sLuaMgr.PUSH_STRING(EnteredCode);
			sLuaMgr.ExecuteCall(6);
        }
        else if(pObject->IsGameObject())
        {
			if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }
            sLuaMgr.BeginCall(m_go_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_SELECT_OPTION]);
			sLuaMgr.PUSH_GO(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_SELECT_OPTION);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.PUSH_UINT(Id);
			sLuaMgr.PUSH_UINT(IntId);
			sLuaMgr.PUSH_STRING(EnteredCode);
			sLuaMgr.ExecuteCall(6);
        }
		RELEASE_LOCK
	}

	void GossipEnd(Object* pObject, Player* Plr)
	{
		GET_LOCK
		if(pObject->IsCreature())
        {
			if(m_unit_gossip_binding == NULL) { RELEASE_LOCK; return; }
			sLuaMgr.BeginCall(m_unit_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_END]);
			sLuaMgr.PUSH_UNIT(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_END);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.ExecuteCall(3);
        }
        else if(pObject->IsItem())
        {
			if(m_item_gossip_binding == NULL) { RELEASE_LOCK; return; }
			sLuaMgr.BeginCall(m_item_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_END]);
			sLuaMgr.PUSH_ITEM(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_END);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.ExecuteCall(3);
        }
        else if(pObject->IsGameObject())
        {
			if(m_go_gossip_binding == NULL) { RELEASE_LOCK; return; }
            sLuaMgr.BeginCall(m_go_gossip_binding->m_functionReferences[GOSSIP_EVENT_ON_END]);
			sLuaMgr.PUSH_GO(pObject);
			sLuaMgr.PUSH_UINT(GOSSIP_EVENT_ON_END);
			sLuaMgr.PUSH_UNIT(Plr);
			sLuaMgr.ExecuteCall(3);
        }
		RELEASE_LOCK
	}

	LuaObjectBinding * m_unit_gossip_binding;
	LuaObjectBinding * m_item_gossip_binding;
    LuaObjectBinding * m_go_gossip_binding;
};

class LuaQuest : public QuestScript
{
public:
	LuaQuest() : QuestScript() {}
	~LuaQuest()
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaQuest*> QuestType;
		QuestType qMap = g_luaMgr.getLuQuestMap();
		for(QuestType::iterator itr = qMap.begin(); itr != qMap.end(); ++itr)
		{
			if(itr->second == this)
			{
				qMap.erase(itr);
				break;
			}
		}
	}

	void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_ON_ACCEPT]);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_UINT(qLogEntry->GetQuest()->id);
		sLuaMgr.ExecuteCall(2);
		RELEASE_LOCK
	}

	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_ON_COMPLETE]);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_UINT(qLogEntry->GetQuest()->id);
		sLuaMgr.ExecuteCall(2);
		RELEASE_LOCK
	}
	void OnQuestCancel(Player* mTarget)
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_ON_CANCEL]);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.ExecuteCall(1);
		RELEASE_LOCK
	}
	void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_GAMEOBJECT_ACTIVATE]);
		sLuaMgr.PUSH_UINT(entry);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_UINT(qLogEntry->GetQuest()->id);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	}
	void OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_ON_CREATURE_KILL]);
		sLuaMgr.PUSH_UINT(entry);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_UINT(qLogEntry->GetQuest()->id);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	}
	void OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_ON_EXPLORE_AREA]);
		sLuaMgr.PUSH_UINT(areaId);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_UINT(qLogEntry->GetQuest()->id);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	}
	void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[QUEST_EVENT_ON_PLAYER_ITEMPICKUP]);
		sLuaMgr.PUSH_UINT(itemId);
		sLuaMgr.PUSH_UINT(totalCount);
		sLuaMgr.PUSH_UNIT(mTarget);
		sLuaMgr.PUSH_UINT(qLogEntry->GetQuest()->id);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	}
	LuaObjectBinding * m_binding;
};

class LuaInstance : public InstanceScript
{
public:
	LuaInstance( MapMgr* pMapMgr ) : InstanceScript( pMapMgr ), m_instanceId( pMapMgr->GetInstanceID() ) {}
	~LuaInstance() {}

	// Player
	void OnPlayerDeath( Player* pVictim, Unit* pKiller ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_PLAYER_DEATH]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_UNIT(pVictim);
		sLuaMgr.PUSH_UNIT(pKiller);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	};
   
	// Area and AreaTrigger
	void OnPlayerEnter( Player* pPlayer ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_PLAYER_ENTER]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(2);
		RELEASE_LOCK
	};
	void OnAreaTrigger( Player* pPlayer, uint32 uAreaId ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_AREA_TRIGGER]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(uAreaId);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	};
	void OnZoneChange( Player* pPlayer, uint32 uNewZone, uint32 uOldZone ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_ZONE_CHANGE]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.PUSH_UINT(uNewZone);
		sLuaMgr.PUSH_UINT(uOldZone);
		sLuaMgr.ExecuteCall(4);
		RELEASE_LOCK
	};

	// Creature / GameObject - part of it is simple reimplementation for easier use Creature / GO < --- > Script
	void OnCreatureDeath( Creature* pVictim, Unit* pKiller ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_CREATURE_DEATH]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_UNIT(pVictim);
		sLuaMgr.PUSH_UNIT(pKiller);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	};

	void OnCreaturePushToWorld( Creature* pCreature ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_CREATURE_PUSH]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_UNIT(pCreature);
		sLuaMgr.ExecuteCall(2);
		RELEASE_LOCK
	};

	void OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_GO_ACTIVATE]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_GO(pGameObject);
		sLuaMgr.PUSH_UNIT(pPlayer);
		sLuaMgr.ExecuteCall(3);
		RELEASE_LOCK
	};
	
	void OnGameObjectPushToWorld( GameObject* pGameObject ) 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ON_GO_PUSH]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.PUSH_GO(pGameObject);
		sLuaMgr.ExecuteCall(2);
		RELEASE_LOCK
	};

	// Standard virtual methods
	void OnLoad()
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_ONLOAD]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.ExecuteCall(1);
		RELEASE_LOCK
	};

	void Destroy() 
	{
		CHECK_BINDING_ACQUIRELOCK
		sLuaMgr.BeginCall(m_binding->m_functionReferences[INSTANCE_EVENT_DESTROY]);
		sLuaMgr.PUSH_UINT(m_instanceId);
		sLuaMgr.ExecuteCall(1);
		RELEASE_LOCK

		typedef HM_NAMESPACE::hash_map<uint32,LuaInstance*> IMAP;
		IMAP & iMap = sLuaMgr.getLuInstanceMap();
		for(IMAP::iterator itr = iMap.begin(); itr != iMap.end(); ++itr)
		{
			if(itr->second == this)
			{
				iMap.erase(itr);
				break;
			}
		}
		delete this;
	};

	uint32 m_instanceId;
	LuaObjectBinding * m_binding;
};

CreatureAIScript * CreateLuaCreature(Creature * src)
{
	LuaCreature * script = NULL;
	if(src != NULL)
	{
		uint32 id = src->GetEntry();
		LuaObjectBinding * pBinding = sLuaMgr.getUnitBinding(id);
		if( pBinding != NULL)
		{
			typedef std::multimap<uint32,LuaCreature*> CRCMAP;
			CRCMAP & cMap = sLuaMgr.getLuCreatureMap();
			script = new LuaCreature(src);
			cMap.insert(make_pair(id,script));
			script->m_binding = pBinding;
		}
	}
	return script;
}

GameObjectAIScript * CreateLuaGameObject(GameObject * src)
{
	LuaGameObject * script = NULL;
	if(src != NULL) 
	{
		uint32 id = src->GetInfo()->ID;
		LuaObjectBinding * pBinding = NULL;
		pBinding = sLuaMgr.getGameObjectBinding(id);
		if( pBinding != NULL) 
		{
			typedef multimap<uint32,LuaGameObject*> GMAP;
			GMAP & gMap = sLuaMgr.getLuGameObjectMap();
			script = new LuaGameObject(src);
			gMap.insert(make_pair(id,script));
			script->m_binding = pBinding;
		}
	}
	return script;
}

QuestScript * CreateLuaQuestScript(uint32 id)
{
	LuaQuest * pLua = NULL;
	LuaObjectBinding * pBinding = sLuaMgr.getQuestBinding( id );
	if(pBinding != NULL)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaQuest*> QMAP;
		QMAP & qMap = sLuaMgr.getLuQuestMap();
		QMAP::iterator itr = qMap.find(id);
		if(itr != qMap.end() )
		{
			if(itr->second == NULL)
				pLua = itr->second = new LuaQuest();
			else
				pLua = itr->second;
		}
		else
		{
			pLua = new LuaQuest();
			qMap.insert(make_pair(id,pLua));
		}
		pLua->m_binding = pBinding;
	}
	return pLua;
}

InstanceScript * CreateLuaInstance(MapMgr* pMapMgr)
{
	LuaInstance * pLua = NULL;
	uint32 id = pMapMgr->GetMapId();
	LuaObjectBinding * pBinding = sLuaMgr.getInstanceBinding(id);
	if (pBinding != NULL)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaInstance*> IMAP;
		IMAP & iMap = sLuaMgr.getLuInstanceMap();
		IMAP::iterator itr = iMap.find(id);
		if (itr != iMap.end())
		{
			if (itr->second == NULL)
				pLua = itr->second = new LuaInstance(pMapMgr);
			else
				pLua = itr->second;
		}
		else
		{
			pLua = new LuaInstance(pMapMgr);
			iMap.insert(make_pair(id,pLua));
		}
		pLua->m_binding = pBinding;
	}
	return pLua;
}

GossipScript * CreateLuaUnitGossipScript(uint32 id)
{
	LuaGossip * pLua = NULL;
    LuaObjectBinding * pBinding = sLuaMgr.getLuaUnitGossipBinding( id );
	if( pBinding != NULL )
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaGossip*> GMAP;
		GMAP & gMap = sLuaMgr.getUnitGossipInterfaceMap();
		GMAP::iterator itr = gMap.find(id);
		if(itr != gMap.end() )
		{
			if(itr->second == NULL)
				pLua = itr->second = new LuaGossip();
			else
				pLua = itr->second;
		}
		else
		{
			pLua = new LuaGossip();
			gMap.insert(make_pair(id,pLua));
		}
		pLua->m_unit_gossip_binding = pBinding;
	}
	return pLua;
}
GossipScript * CreateLuaItemGossipScript(uint32 id)
 {
	LuaGossip * pLua = NULL;
    LuaObjectBinding * pBinding = sLuaMgr.getLuaItemGossipBinding( id );
 	if( pBinding != NULL )
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaGossip*> GMAP;
		GMAP & gMap = sLuaMgr.getItemGossipInterfaceMap();
		GMAP::iterator itr = gMap.find(id);
		if(itr != gMap.end() )
		{
			if(itr->second == NULL)
				pLua = itr->second = new LuaGossip();
			else
				pLua = itr->second;
		}
		else
		{
			pLua = new LuaGossip();
			gMap.insert(make_pair(id,pLua));

		}
		pLua->m_item_gossip_binding = pBinding;
	}
 	return pLua;
 }
GossipScript * CreateLuaGOGossipScript(uint32 id)
{
	LuaGossip * pLua = NULL;
    LuaObjectBinding * pBinding = g_luaMgr.getLuaGOGossipBinding( id );
	if( pBinding != NULL )
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaGossip*> GMAP;
		GMAP & gMap = sLuaMgr.getGameObjectGossipInterfaceMap();
		GMAP::iterator itr = gMap.find(id);
		if(itr != gMap.end() )
		{
			if(itr->second == NULL)
				pLua = itr->second = new LuaGossip();
			else
				pLua = itr->second;
		}
		else
		{
			pLua = new LuaGossip();
			gMap.insert(make_pair(id,pLua));
		}
		pLua->m_go_gossip_binding = pBinding;
	}
	return pLua;
}

void LuaEngine::Startup()
{
	Log.Notice("LuaEngineMgr", "Spawning Lua Engine...");
	Log.Notice("LuaEngineMgr", "~~~LuaHypArc~~~");
	Log.Notice("LuaEngineMgr", "LuaHypArc Lua Engine %s: Loaded", ARCH);
	//Create a new global state that will server as the lua universe.
	lu = lua_open();

	LoadScripts();

	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(LuaObjectBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		m_scriptMgr->register_creature_script( itr->first, CreateLuaCreature );
		sLuaMgr.getLuCreatureMap().insert(make_pair(itr->first,(LuaCreature*)NULL));
	}

	for(LuaObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		m_scriptMgr->register_gameobject_script( itr->first, CreateLuaGameObject );
		sLuaMgr.getLuGameObjectMap().insert(make_pair(itr->first,(LuaGameObject*)NULL));
	}

	for(LuaObjectBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		QuestScript * qs = CreateLuaQuestScript( itr->first );
		if( qs != NULL )
		{
			m_scriptMgr->register_quest_script( itr->first, qs );
			sLuaMgr.getLuQuestMap().insert(make_pair(itr->first,(LuaQuest*)NULL));
		}
	}

	for(LuaObjectBindingMap::iterator itr = m_instanceBinding.begin(); itr != m_instanceBinding.end(); ++itr)
	{
		m_scriptMgr->register_instance_script( itr->first, CreateLuaInstance );
		sLuaMgr.getLuInstanceMap().insert(make_pair(itr->first,(LuaInstance*)NULL));
	}

    for(LuaObjectBindingMap::iterator itr = m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
 	{
		GossipScript * gs = CreateLuaUnitGossipScript( itr->first );
 		if( gs != NULL )
		{
 			m_scriptMgr->register_gossip_script( itr->first, gs );
			sLuaMgr.getUnitGossipInterfaceMap().insert(make_pair(itr->first,(LuaGossip*)NULL));
		}
	}

    for(LuaObjectBindingMap::iterator itr = m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaItemGossipScript( itr->first );
		if( gs != NULL )
		{
			m_scriptMgr->register_item_gossip_script( itr->first, gs );
			sLuaMgr.getItemGossipInterfaceMap().insert(make_pair(itr->first,(LuaGossip*)NULL));
		}
    }

    for(LuaObjectBindingMap::iterator itr = m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaGOGossipScript( itr->first );
		if( gs != NULL )
		{
			m_scriptMgr->register_go_gossip_script( itr->first, gs );
			sLuaMgr.getGameObjectGossipInterfaceMap().insert(make_pair(itr->first,(LuaGossip*)NULL));
		}
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

	for (std::map<uint32,uint16>::iterator itr = m_luaDummySpells.begin(); itr != m_luaDummySpells.end(); ++itr)
	{
		if (find(sLuaMgr.HookInfo.dummyHooks.begin(), sLuaMgr.HookInfo.dummyHooks.end(), itr->first) == sLuaMgr.HookInfo.dummyHooks.end())
		{
			m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
			sLuaMgr.HookInfo.dummyHooks.push_back(itr->first);
		}
	}
}
void LuaEngine::RegisterEvent(uint8 regtype, uint32 id, uint32 evt, uint16 functionRef) 
{
	switch(regtype) 
	{
		case REGTYPE_UNIT: 
			{
				if(id && evt && evt < CREATURE_EVENT_COUNT) {
					LuaObjectBinding * bind = getUnitBinding(id);
					if(bind == NULL) {
						LuaObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaObjectBinding));
						nbind.m_functionReferences[evt] = functionRef;
						m_unitBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->m_functionReferences[evt] > 0)
							lua_unref(lu,bind->m_functionReferences[evt]);
						bind->m_functionReferences[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_GO:
			{
				if(id && evt && evt < GAMEOBJECT_EVENT_COUNT) {
					LuaObjectBinding * bind = getGameObjectBinding(id);
					if(bind == NULL) {
						LuaObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaObjectBinding));
						nbind.m_functionReferences[evt] = functionRef;
						m_gameobjectBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->m_functionReferences[evt] > 0)
							lua_unref(lu,bind->m_functionReferences[evt]);
						bind->m_functionReferences[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_QUEST:
			{
				if(id && evt && evt < QUEST_EVENT_COUNT) {
					LuaObjectBinding * bind = getQuestBinding(id);
					if(bind == NULL) {
						LuaObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaObjectBinding));
						nbind.m_functionReferences[evt] = functionRef;
						m_questBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->m_functionReferences[evt] > 0)
							lua_unref(lu,bind->m_functionReferences[evt]);
						bind->m_functionReferences[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_SERVHOOK:
			{
				if(evt < SERVER_HOOK_COUNT)
					EventAsToFuncName[evt].push_back(functionRef);
			}break;
		case REGTYPE_DUMMYSPELL: 
			{
				if (id)
					m_luaDummySpells.insert( pair<uint32,uint16>(id,functionRef) );
			}break;
		case REGTYPE_INSTANCE: 
			{
				if(id && evt && evt < INSTANCE_EVENT_COUNT) {
					LuaObjectBinding * bind = getInstanceBinding(id);
					if(bind == NULL) {
						LuaObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaObjectBinding));
						nbind.m_functionReferences[evt] = functionRef;
						m_instanceBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->m_functionReferences[evt] > 0)
							lua_unref(lu,bind->m_functionReferences[evt]);
						bind->m_functionReferences[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_UNIT_GOSSIP:
			{
				if(id && evt && evt < GOSSIP_EVENT_COUNT) {
					LuaObjectBinding * bind = getLuaUnitGossipBinding(id);
					if(bind == NULL) {
						LuaObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaObjectBinding));
						nbind.m_functionReferences[evt] = functionRef;
						m_unit_gossipBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->m_functionReferences[evt] > 0)
							lua_unref(lu,bind->m_functionReferences[evt]);
						bind->m_functionReferences[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_ITEM_GOSSIP:
		{
			if(id && evt && evt < GOSSIP_EVENT_COUNT) {
				LuaObjectBinding * bind = getLuaItemGossipBinding(id);
				if(bind == NULL) {
					LuaObjectBinding nbind;
					memset(&nbind,0,sizeof(LuaObjectBinding));
					nbind.m_functionReferences[evt] = functionRef;
					m_item_gossipBinding.insert(make_pair(id,nbind));
				}
				else
				{
					if(bind->m_functionReferences[evt] > 0)
						lua_unref(lu,bind->m_functionReferences[evt]);
					bind->m_functionReferences[evt] = functionRef;
				}
			}
		}break;
		case REGTYPE_GO_GOSSIP:
		{
			if(id && evt && evt < GOSSIP_EVENT_COUNT) {
				LuaObjectBinding * bind = getLuaGOGossipBinding(id);
				if(bind == NULL) {
					LuaObjectBinding nbind;
					memset(&nbind,0,sizeof(LuaObjectBinding));
					nbind.m_functionReferences[evt] = functionRef;
					m_go_gossipBinding.insert(make_pair(id,nbind));
				}
				else
				{
					if(bind->m_functionReferences[evt] > 0)
						lua_unref(lu,bind->m_functionReferences[evt]);
					bind->m_functionReferences[evt] = functionRef;
				}
			}
		}break;
	}
}

void LuaEngine::Unload()
{
	RemoveTimedEvents(lu);
	DestroyAllLuaEvents(); // stop all pending events.
	// clean up the engine of any existing defined variables
	for(LuaObjectBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		for(int i = 0; i < CREATURE_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_unitBinding.clear();
	for(LuaObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		for(int i = 0; i < GAMEOBJECT_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_gameobjectBinding.clear();
	for(LuaObjectBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		for(int i = 0; i < QUEST_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_questBinding.clear();
	for(LuaObjectBindingMap::iterator itr = m_instanceBinding.begin(); itr != m_instanceBinding.end(); ++itr)
	{
		for(int i = 0; i < INSTANCE_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_instanceBinding.clear();
	for(LuaObjectBindingMap::iterator itr = m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
	{
		for(int i = 0; i < GOSSIP_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_unit_gossipBinding.clear();
	for(LuaObjectBindingMap::iterator itr = m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		for(int i = 0; i < GOSSIP_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_item_gossipBinding.clear();
	for(LuaObjectBindingMap::iterator itr = m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		for(int i = 0; i < GOSSIP_EVENT_COUNT; ++i)
		{
			if(itr->second.m_functionReferences[i] > 0)
				lua_unref(lu,itr->second.m_functionReferences[i]);
		}
	}
	m_go_gossipBinding.clear();
	//Serv hooks : had forgotten these.
	for(int i = 0; i < SERVER_HOOK_COUNT; ++i)
	{
		vector<uint16> & next = EventAsToFuncName[i];
		for(vector<uint16>::iterator itr = next.begin(); itr != next.end(); ++itr)
			lua_unref(lu,(*itr));
		next.clear();
	}
	for(map<uint32,uint16>::iterator itr = m_luaDummySpells.begin(); itr != m_luaDummySpells.end(); ++itr)
		lua_unref(lu,itr->second);
	m_luaDummySpells.clear();
	for(set<int>::iterator itr = m_pendingThreads.begin(); itr != m_pendingThreads.end(); ++itr)
		lua_unref(lu,(*itr));
	m_pendingThreads.clear();
	m_functionRefs.clear();

	lua_close(lu);
}
void LuaEngine::Restart()
{
	Log.Notice("LuaEngineMgr","Restarting Engine.");
	GET_LOCK
	getcoLock().Acquire();
	Unload();
	lu = lua_open();
	LoadScripts();
	for(LuaObjectBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		typedef multimap<uint32,LuaCreature*> CMAP;
		CMAP & cMap = sLuaMgr.getLuCreatureMap();
		CMAP::iterator it = cMap.find(itr->first);
		CMAP::iterator itend = cMap.upper_bound(itr->first);
		if(it == cMap.end() )
		{
			m_scriptMgr->register_creature_script(itr->first,CreateLuaCreature);
			cMap.insert(make_pair(itr->first,(LuaCreature*)NULL));
		}
		else
		{
			for(;it != itend; ++it) 
			{
				if(it->second != NULL)
					it->second->m_binding = &itr->second;
			}
		}
	}
	for(LuaObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		typedef multimap<uint32,LuaGameObject*> GMAP;
		GMAP & gMap = sLuaMgr.getLuGameObjectMap();
		GMAP::iterator it = gMap.find(itr->first);
		GMAP::iterator itend = gMap.upper_bound(itr->first);
		if(it == gMap.end() )
		{
			m_scriptMgr->register_gameobject_script(itr->first,CreateLuaGameObject);
			gMap.insert(make_pair(itr->first,(LuaGameObject*)NULL));
		}
		else
		{
			for(;it != itend; ++it)
			{
				if(it->second != NULL)
					it->second->m_binding = &itr->second;
			}
		}
	}
	for(LuaObjectBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaQuest*> QMAP;
		QMAP & qMap = sLuaMgr.getLuQuestMap();
		QMAP::iterator it = qMap.find(itr->first);
		if(it == qMap.end())
		{
			m_scriptMgr->register_quest_script(itr->first,CreateLuaQuestScript(itr->first));
			qMap.insert(make_pair(itr->first,(LuaQuest*)NULL));
		}
		else
		{
			LuaQuest * q_interface = it->second;
			if(q_interface != NULL)
				q_interface->m_binding = &itr->second;
		}
	}
	for(LuaObjectBindingMap::iterator itr = m_instanceBinding.begin(); itr != m_instanceBinding.end(); ++itr)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaInstance*> IMAP;
		IMAP & iMap = sLuaMgr.getLuInstanceMap();
		IMAP::iterator it = iMap.find(itr->first);
		if(it == iMap.end())
		{
			m_scriptMgr->register_instance_script(itr->first, CreateLuaInstance);
			iMap.insert(make_pair(itr->first,(LuaInstance*)NULL));
		}
		else
		{
			if(it->second != NULL)
				it->second->m_binding = &itr->second;
		}
	}
	for(LuaObjectBindingMap::iterator itr = this->m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaGossip*> GMAP;
		GMAP & gMap = sLuaMgr.getUnitGossipInterfaceMap();
		GMAP::iterator it = gMap.find(itr->first);
		if(it == gMap.end() ) 
		{
			GossipScript * gs = CreateLuaUnitGossipScript(itr->first);
			if(gs != NULL)
			{
				m_scriptMgr->register_gossip_script(itr->first,gs);
				gMap.insert(make_pair(itr->first,(LuaGossip*)NULL));
			}
		}
		else
		{
			LuaGossip * u_gossip = it->second;
			if(u_gossip != NULL)
				u_gossip->m_unit_gossip_binding = &itr->second;
		}
	}
	for(LuaObjectBindingMap::iterator itr = this->m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaGossip*> GMAP;
		GMAP & gMap = sLuaMgr.getItemGossipInterfaceMap();
		GMAP::iterator it = gMap.find(itr->first);
		if(it == gMap.end() ) 
		{
			GossipScript * gs = CreateLuaItemGossipScript(itr->first);
			if(gs != NULL)
			{
				m_scriptMgr->register_item_gossip_script(itr->first,gs);
				gMap.insert(make_pair(itr->first,(LuaGossip*)NULL));
			}
		}
		else
		{
			LuaGossip * i_gossip = it->second;
			if(i_gossip != NULL)
				i_gossip->m_item_gossip_binding = &itr->second;
		}
	}
	for(LuaObjectBindingMap::iterator itr = this->m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		typedef HM_NAMESPACE::hash_map<uint32,LuaGossip*> GMAP;
		GMAP & gMap = sLuaMgr.getGameObjectGossipInterfaceMap();
		GMAP::iterator it = gMap.find(itr->first);
		if(it == gMap.end() ) 
		{
			GossipScript * gs = CreateLuaGOGossipScript(itr->first);
			if(gs != NULL)
			{
				m_scriptMgr->register_go_gossip_script(itr->first,gs);
				gMap.insert(make_pair(itr->first,(LuaGossip*)NULL));
			}
		}
		else
		{
			LuaGossip * g_gossip = it->second;
			if(g_gossip != NULL)
				g_gossip->m_go_gossip_binding = &itr->second;
		}
	}
	/*
		BIG SERV HOOK CHUNK EEK 
		*/
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

	for (std::map<uint32,uint16>::iterator itr = m_luaDummySpells.begin(); itr != m_luaDummySpells.end(); ++itr)
	{
		if (find(sLuaMgr.HookInfo.dummyHooks.begin(), sLuaMgr.HookInfo.dummyHooks.end(), itr->first) == sLuaMgr.HookInfo.dummyHooks.end())
		{
			m_scriptMgr->register_dummy_spell(itr->first, &LuaOnDummySpell);
			sLuaMgr.HookInfo.dummyHooks.push_back(itr->first);
		}
	}
	RELEASE_LOCK
	getcoLock().Release();

	//hyper: do OnSpawns for spawned creatures.
	vector<uint32> temp = OnLoadInfo;
	OnLoadInfo.clear();
	for ( vector<uint32>::iterator itr = temp.begin(); itr != temp.end(); itr += 3)
	{
		//*itr = mapid; *(itr+1) = iid; *(itr+2) = lowguid
		MapMgr * mgr = NULL;
		if (*(itr+1) == 0) //no instance
			mgr = sInstanceMgr.GetMapMgr(*itr);
		else
		{
			Instance *inst = sInstanceMgr.GetInstanceByIds(*itr, *(itr+1));
			if (inst != NULL)
				mgr = inst->m_mapMgr;
		}
		if (mgr != NULL)
		{
			Creature * unit = mgr->GetCreature(*(itr+2));
			if (unit != NULL && unit->IsInWorld() && unit->GetScript() != NULL)
				unit->GetScript()->OnLoad();
		}
	}
	temp.clear();

	Log.Notice("LuaEngineMgr","Done restarting engine.");
}

void LuaEngine::ResumeLuaThread(int ref) {
	getcoLock().Acquire();
	lua_State * expectedThread = NULL;
	lua_rawgeti(lu,LUA_REGISTRYINDEX,ref);
	if(lua_isthread(lu,-1) )
		expectedThread = lua_tothread(lu,-1);
	if(expectedThread != NULL) 
	{
		//push ourself on the stack
		lua_pushthread(expectedThread);
		//move the thread to the main lu state(and pop it off)
		lua_xmove(expectedThread,lu,1);
		if(lua_rawequal(lu,-1,-2) )
		{
			lua_pop(lu,2);
			int res = lua_resume(expectedThread,lua_gettop(expectedThread));
			if(res != LUA_YIELD && res)
				report(expectedThread);
		}
		else
			lua_pop(lu,2);
		luaL_unref(lu,LUA_REGISTRYINDEX,ref);
	}
	getcoLock().Release();
}


/************************************************************************/
/* SCRIPT FUNCTION IMPLEMENTATION                                       */
/************************************************************************/

#define TEST_UNIT() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsCreature()) { return 0; }
#define TEST_UNIT_RET() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsCreature()) { lua_pushboolean(L,0); return 1; }

#define TEST_PLAYER() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsPlayer()) { return 0; }
#define TEST_PLAYER_RET() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsPlayer()) { lua_pushboolean(L,0); return 1; }

#define TEST_UNITPLAYER() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsUnit()) { return 0; }
#define TEST_UNITPLAYER_RET() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsUnit()) { lua_pushboolean(L,0); return 1; }

#define TEST_GO() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsGameObject()) { return 0; }
#define TEST_GO_RET() if(ptr == NULL || !ptr->IsInWorld() || !ptr->IsGameObject()) { lua_pushboolean(L,0); return 1; }

#define RET_NIL( ){ lua_pushnil(L); return 1; }
#define RET_BOOL(exp) { (exp) ? lua_pushboolean(L,1) : lua_pushboolean(L,0); return 1; }
#define RET_STRING(str) { lua_pushstring(L,(str)); return 1; }
#define RET_NUMBER(number) { lua_pushnumber(L,(number)); return 1; }
#define RET_INT(integer) { lua_pushinteger(L,(integer)); return 1; }

// Simplicity macros.
#define CHECK_UNIT(L,narg) sLuaMgr.CHECK_UNIT(L,narg)
#define CHECK_PLAYER(L,narg) TO_PLAYER(CHECK_UNIT(L,narg))
#define CHECK_GO(L,narg) sLuaMgr.CHECK_GO(L,narg)
#define CHECK_ITEM(L,narg) sLuaMgr.CHECK_ITEM(L,narg)
#define CHECK_PACKET(L,narg) sLuaMgr.CHECK_PACKET(L,narg)
#define CHECK_GUID(L, narg) sLuaMgr.CHECK_GUID(L,narg)
#define CHECK_OBJECT(L, narg) sLuaMgr.CHECK_OBJECT(L,narg)
#define CHECK_TAXIPATH(L, narg) sLuaMgr.CHECK_TAXIPATH(L,narg)
#define CHECK_SPELL(L, narg) sLuaMgr.CHECK_SPELL(L,narg)
#define CHECK_OBJECT(L, narg) sLuaMgr.CHECK_OBJECT(L,narg)
#define CHECK_AURA(L, narg) sLuaMgr.CHECK_AURA(L,narg)

//Its coming soon ^.^
//#define CHECK_SPELL(L,narg) ArcLuna<Spell>::check(L),(narg))
//This is used alot when checking for coords but Lua handles only doubles.
#define CHECK_FLOAT(L,narg) (lua_isnoneornil(L,(narg)) ) ? 0.00f : (float)luaL_checknumber(L,(narg)); 
#define CHECK_ULONG(L,narg) (uint32)luaL_checknumber((L),(narg))
#define CHECK_USHORT(L, narg) (uint16)luaL_checkinteger((L),(narg))
#define CHECK_BOOL(L,narg) (lua_toboolean((L),(narg)) > 0) ? true : false

#define PUSH_UNIT(L, unit) sLuaMgr.PUSH_UNIT(TO_UNIT(unit),L)
#define PUSH_GO(L, go) sLuaMgr.PUSH_GO(TO< GameObject* >(go),L)
#define PUSH_PACKET(L,pack) sLuaMgr.PUSH_PACKET(pack,L)
#define PUSH_ITEM(L,item) sLuaMgr.PUSH_ITEM(TO< Item* >(item),L)
#define PUSH_GUID(L, obj) sLuaMgr.PUSH_GUID(obj,L)
#define PUSH_TAXIPATH(L, tp) sLuaMgr.PUSH_TAXIPATH(tp,L)
#define PUSH_SPELL(L, sp) sLuaMgr.PUSH_SPELL(sp,L)
#define PUSH_SQLFIELD(L, field) sLuaMgr.PUSH_SQLFIELD(field,L)
#define PUSH_SQLRESULT(L, res) sLuaMgr.PUSH_SQLRESULT(res,L)
#define PUSH_AURA(L, aura) sLuaMgr.PUSH_AURA(aura,L)

//I know its not a good idea to do it like that BUT it is the easiest way. I will make it better in steps:
#include "LUAFunctions.h"
#include "FunctionTables.h"

