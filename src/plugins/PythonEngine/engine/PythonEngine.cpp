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
#include "engine/PythonEngine.hpp"
#include "python/ArcPython.hpp"
#include "serverhooks/ServerHookRegistry.hpp"
#include "serverhooks/ServerHookHandler.hpp"
#include "engine/FunctionRegistry.hpp"
#include "creature/CreatureFunctionRegisterer.hpp"
#include "creature/CreatureGossipScriptRegisterer.hpp"
#include "creature/CreatureGossipScriptReleaser.hpp"
#include "gameobject/GOScriptRegisterer.hpp"
#include "gameobject/GOGossipScriptRegisterer.hpp"
#include "gameobject/GOGossipScriptReleaser.hpp"
#include "instance/InstanceScriptRegisterer.hpp"
#include "item/ItemGossipScriptRegisterer.hpp"
#include "item/ItemGossipScriptReleaser.hpp"
#include "quest/QuestScriptRegisterer.hpp"
#include "creature/PythonCreatureAIScriptFactory.hpp"
#include "gameobject/PythonGameObjectAIScriptFactory.hpp"
#include "instance/PythonInstanceScriptFactory.hpp"
#include "quest/PythonQuestScriptFactory.hpp"
#include "spells/DummySpellHandlerRegisterer.hpp"
#include "spells/ScriptedEffectHandlerRegisterer.hpp"
#include "spells/DummyAuraHandlerRegisterer.hpp"

void register_arcemu_extensions();

PythonEngine::PythonEngine( ScriptMgr *mgr )
{
	/// First we extend Python with our own types, constants, and functions
	register_arcemu_extensions();

	/// Then start Python
	Python::setPath( L"./pythonlibs" );
	python = new ArcPython();
	this->mgr = mgr;
}

PythonEngine::~PythonEngine()
{
	/// First release the functions we loaded from scripts
	PythonCreatureAIScriptFactory::onShutdown();
	PythonGameObjectAIScriptFactory::onShutdown();
	PythonQuestScriptFactory::onShutDown();
	PythonInstanceScriptFactory::onShutdown();
	ServerHookRegistry::releaseHooks();
	FunctionRegistry::releaseFunctions();

	/// then shut Python down
	delete python;
	python = NULL;
}

void PythonEngine::onStartup()
{
	/// First load scripts
	loadScripts();

	/// then register the functions from the loaded scripts
	registerHooks();
	registerScripts();
}

void PythonEngine::onReload()
{
	/// We don't want any script execution, or Python state change while reloading
	Guard g( ArcPython::getLock() );

	/// First release functions that we loaded
	ServerHookRegistry::releaseHooks();	
	CreatureGossipScriptReleaser creatureGossipReleaser( mgr );
	FunctionRegistry::visitCreatureGossipFunctions( &creatureGossipReleaser );
	GOGossipScriptReleaser goGossipReleaser( mgr );
	FunctionRegistry::visitGOGossipFunctions( &goGossipReleaser );
	ItemGossipScriptReleaser itemGossipReleaser( mgr );
	FunctionRegistry::visitItemGossipFunctions( &itemGossipReleaser );
	FunctionRegistry::releaseFunctions();

	/// Load current scripts
	loadScripts();

	/// Register functions from loaded scripts
	registerScripts();

	/// Unfortunately there's no way to remove the script classes that we have already created for spawned entities
	/// So the best we can do is to change the functions in them, or set NULL if the script function was removed
	PythonCreatureAIScriptFactory::onReload();
	PythonGameObjectAIScriptFactory::onReload();
	PythonInstanceScriptFactory::onReload();
	PythonQuestScriptFactory::onReload();
}

int PythonEngine::loadScript( const char *fileName )
{
	//sLog.Success( "APE", "Loading %s...", fileName );

	/// This is where magic happens:
	/// Before starting it, we extended Python with functions that register hook and event handlers.
	/// When executing the Python scripts Python loads the functions we defined, and runs the functions we call in the scripts
	/// If a hook, or event handler registry function is called Python calls the functions we added, and those functions register the handler functions in the engine,
	///  keeping the function reference to the handlers, so that we can create script classes and call them when needed
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

	/// Find files with the extensions .py
	std::vector< std::string > pythonFiles;
	Arcemu::FileUtils::findFilesByExtension( "pythonscripts", "py", pythonFiles ); 

	/// Run all the files we've found
	for( std::vector< std::string >::iterator itr = pythonFiles.begin(); itr != pythonFiles.end(); ++itr )
	{
		const std::string &fileName = *itr;
		if( loadScript( fileName.c_str() ) == 0 )
			c++;
	}

	sLog.Success( "APE", "Loaded %d Python scripts.", c );

	return c;
}

#define REGISTER_SERVER_HOOK( HOOK, FUNCTION ) if( ServerHookRegistry::hasHooksForEvent( HOOK ) ){ mgr->register_hook( HOOK, (FUNCTION) ); }

void PythonEngine::registerHooks()
{
	/// We register the hook handler functions in Arcemu here
	/// Aremu calls these when specific events happen, and these functions call the event handlers we loaded from Python scripts

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
	/// Will not implement SERVER_HOOK_EVENT_ON_TICK, as it is not implemented in Arcemu either
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
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_TRANSPORT_ARRIVED, (void*)(&ServerHookHandler::hookOnTransportArrived) );
}

void PythonEngine::registerScripts()
{
	/// We register the functions that we loaded from Python scripts here

	CreatureGossipScriptRegisterer registerer( this->mgr );
	FunctionRegistry::visitCreatureGossipFunctions( &registerer );

	GOGossipScriptRegisterer goGossipRegisterer( this->mgr );
	FunctionRegistry::visitGOGossipFunctions( &goGossipRegisterer );

	ItemGossipScriptRegisterer itemGossipRegisterer( this->mgr );
	FunctionRegistry::visitItemGossipFunctions( &itemGossipRegisterer );

	InstanceScriptRegisterer instanceScriptRegisterer( this->mgr );
	FunctionRegistry::visitInstanceEventFunctions( &instanceScriptRegisterer );

	GOScriptRegisterer goScriptRegisterer( this->mgr );
	FunctionRegistry::visitGOEventFunctions( &goScriptRegisterer );

	CreatureFunctionRegisterer creatureFunctionRegisterer( this->mgr );
	FunctionRegistry::visitCreatureEventFunctions( &creatureFunctionRegisterer );

	QuestScriptRegisterer questScriptRegisterer( this->mgr );
	FunctionRegistry::visitQuestEventFunctions( &questScriptRegisterer );

	DummySpellHandlerRegisterer dummySpellRegisterer( this->mgr );
	FunctionRegistry::visitDummySpellHandlerFunctions( &dummySpellRegisterer );

	ScriptedEffectHandlerRegisterer scriptedEffectRegisterer( this->mgr );
	FunctionRegistry::visitScriptedEffectHandlerFunctions( &scriptedEffectRegisterer );

	DummyAuraHandlerRegisterer dummyAuraRegisterer( this->mgr );
	FunctionRegistry::visitDummyAuraHandlerFunctions( &dummyAuraRegisterer );
}

