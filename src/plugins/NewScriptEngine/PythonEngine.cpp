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
#include "python/ArcPython.hpp"
#include "ServerHookRegistry.hpp"
#include "ServerHookHandler.hpp"

#include "FunctionRegistry.hpp"

#include "CreatureGossipScriptRegisterer.hpp"
#include "CreatureGossipScriptReleaser.hpp"

#include "ItemGossipScriptRegisterer.hpp"
#include "ItemGossipScriptReleaser.hpp"

void register_arcemu_extensions();


PythonEngine::PythonEngine( ScriptMgr *mgr )
{
	register_arcemu_extensions();

	Python::setPath( L"./pythonlibs" );
	python = new ArcPython();
	this->mgr = mgr;
}

PythonEngine::~PythonEngine()
{
	ServerHookRegistry::releaseHooks();
	FunctionRegistry::releaseFunctions();

	delete python;
	python = NULL;
}

void PythonEngine::onStartup()
{
	loadScripts();

	registerHooks();

	registerGossipScripts();
}

void PythonEngine::onReload()
{
	Guard g( ArcPython::getLock() );

	ServerHookRegistry::releaseHooks();
	
	CreatureGossipScriptReleaser creatureGossipReleaser( mgr );
	FunctionRegistry::visitCreatureGossipFunctions( &creatureGossipReleaser );

	ItemGossipScriptReleaser itemGossipReleaser( mgr );
	FunctionRegistry::visitItemGossipFunctions( &itemGossipReleaser );

	FunctionRegistry::releaseFunctions();

	loadScripts();

	registerGossipScripts();
}

int PythonEngine::loadScript( const char *fileName )
{
	sLog.Success( "APE", "Loading %s...", fileName );

	int val = python->runSimpleFile( fileName );
	if( val == 0 )
	{
	    sLog.Success( "APE", "Loaded %s", fileName );
	}
	else
	{
	    sLog.Error( "APE", "Failed to load %s", fileName );
	}

	return val;
}

int PythonEngine::loadScripts()
{
	sLog.Success( "APE", "Loading Python scripts..." );
	int c = 0;

	std::vector< std::string > pythonFiles;
	Arcemu::FileUtils::findFilesByExtension( "pythonscripts", "py", pythonFiles ); 

	for( std::vector< std::string >::iterator itr = pythonFiles.begin(); itr != pythonFiles.end(); ++itr )
	{
		const std::string &fileName = *itr;
		if( loadScript( fileName.c_str() ) == 0 )
			c++;
	}

	sLog.Success( "APE", "Loaded %d Python scripts.", c );

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
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, (void*)(&ServerHookHandler::hookOnQuestCancelled) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_QUEST_FINISHED, (void*)(&ServerHookHandler::hookOnQuestFinished) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)(&ServerHookHandler::hookOnHonorableKill) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ARENA_FINISH, (void*)(&ServerHookHandler::hookOnArenaFinish) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_OBJECTLOOT, (void*)(&ServerHookHandler::hookOnObjectLoot) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_AREATRIGGER, (void*)(&ServerHookHandler::hookOnAreaTrigger) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_POST_LEVELUP, (void*)(&ServerHookHandler::hookOnLevelUp) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_PRE_DIE, (void*)(&ServerHookHandler::hookOnPreUnitDie) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE, (void*)(&ServerHookHandler::hookOnAdvanceSkillLine) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_DUEL_FINISHED, (void*)(&ServerHookHandler::hookOnDuelFinished) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_AURA_REMOVE, (void*)(&ServerHookHandler::hookOnAuraRemove) );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_RESURRECT, (void*)(&ServerHookHandler::hookOnPlayerResurrect) );	
}

void PythonEngine::registerGossipScripts()
{
	CreatureGossipScriptRegisterer registerer( this->mgr );
	FunctionRegistry::visitCreatureGossipFunctions( &registerer );

	ItemGossipScriptRegisterer itemGossipRegisterer( this->mgr );
	FunctionRegistry::visitItemGossipFunctions( &itemGossipRegisterer );
}

