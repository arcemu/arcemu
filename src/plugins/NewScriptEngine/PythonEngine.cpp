/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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
#include <Python.h>
#include "PythonEngine.hpp"
#include "python/Python.hpp"
#include "ServerHookRegistry.hpp"
#include "ServerHookHandler.hpp"

void register_arcemu_extensions();


PythonEngine::PythonEngine( ScriptMgr *mgr )
{
	register_arcemu_extensions();

	Python::setPath( L"./pythonlibs" );
	python = new Python();
	this->mgr = mgr;
}

PythonEngine::~PythonEngine()
{
	delete python;
	python = NULL;
}

void PythonEngine::onStartup()
{
	loadScripts();

	registerHooks();
}

int PythonEngine::loadScript( const char *fileName )
{
	int val = python->runSimpleFile( fileName );
	if( val == 0 )
	{
	    LOG_BASIC( "Loaded %s", fileName );
	}
	else
	{
	    LOG_BASIC( "Failed to load %s", fileName );
	}

	return val;
}

int PythonEngine::loadScripts()
{
	LOG_BASIC( "Loading Python scripts..." );
	int c = 0;

	if( loadScript( "pythonscripts/test_script.py" ) == 0 )
	    c++;

	LOG_BASIC( "Loaded %d Python scripts.", c );

	return 0;
}

#define REGISTER_SERVER_HOOK( HOOK, FUNCTION ) if( ServerHookRegistry::hasHooksForEvent( HOOK ) ){ mgr->register_hook( HOOK, (FUNCTION) ); }

void PythonEngine::registerHooks()
{
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_NEW_CHARACTER, (void*)(&ServerHookHandler::hookOnNewCharacter) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)(&ServerHookHandler::hookOnKillPlayer) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, (void*)(&ServerHookHandler::hookOnFirstEnterWorld) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)(&ServerHookHandler::hookOnEnterWorld) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_GUILD_JOIN, (void*)(&ServerHookHandler::hookOnGuildJoin) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_DEATH, (void*)(&ServerHookHandler::hookOnPlayerDeath) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_REPOP, (void*)(&ServerHookHandler::hookOnPlayerRepop) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_EMOTE, (void*)(&ServerHookHandler::hookOnEmote) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ENTER_COMBAT, (void*)(&ServerHookHandler::hookOnEnterCombat) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_CAST_SPELL, (void*)(&ServerHookHandler::hookOnCastSpell ) );
	/// Will not implement SERVER_HOOK_EVENT_ON_TICK
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, (void*)(&ServerHookHandler::hookOnLogoutRequest) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)(&ServerHookHandler::hookOnLogout) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, (void*)(&ServerHookHandler::hookOnAcceptQuest) );

	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ZONE, (void*)(&ServerHookHandler::hookOnZoneChange) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_CHAT, (void*)(&ServerHookHandler::hookOnChatMessage) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_LOOT, (void*)(&ServerHookHandler::hookOnLoot) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_GUILD_CREATE, (void*)(&ServerHookHandler::hookOnGuildCreate) );

	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_FULL_LOGIN, (void*)(&ServerHookHandler::hookOnFullLogin) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, (void*)(&ServerHookHandler::hookOnCharacterCreated) );

	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)(&ServerHookHandler::hookOnHonorableKill) );

	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_POST_LEVELUP, (void*)(&ServerHookHandler::hookOnLevelUp) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_PRE_DIE, (void*)(&ServerHookHandler::hookOnPreUnitDie) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE, (void*)(&ServerHookHandler::hookOnAdvanceSkillLine) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_DUEL_FINISHED, (void*)(&ServerHookHandler::hookOnDuelFinished) );

	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_RESURRECT, (void*)(&ServerHookHandler::hookOnPlayerResurrect) );
	
	
}
