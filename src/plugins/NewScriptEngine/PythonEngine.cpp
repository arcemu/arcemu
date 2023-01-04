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

#include "serverhooks.hpp"

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
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)python_hookOnEnterWorld );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_DEATH, (void*)python_hookOnPlayerDeath );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_REPOP, (void*)python_hookOnPlayerRepop );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_EMOTE, (void*)python_hookOnEmote );
	REGISTER_SERVER_HOOK( SERVER_HOOK_EVENT_ON_ENTER_COMBAT, (void*)python_hookOnEnterCombat );
}
