/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2022 Arcemu Team <http://www.ArcEmu.org/>
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
#include <Game/Scripting/ScriptSetup.h>
#include "engine/PythonEngine.hpp"

static PythonEngine *pythonEngine = NULL;

/// These are the functions that Arcemu calls when starting, stopping, reloading the plugin

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_SCRIPT_ENGINE;
}

extern "C" SCRIPT_DECL void _exp_script_register( ScriptMgr *mgr )
{
	if( pythonEngine == NULL )
	{
		sLog.Success( "APE", "Starting up...");
		pythonEngine = new PythonEngine(mgr);
		pythonEngine->onStartup();
		sLog.Success( "APE", "Startup complete.");
	}
	else
	{
		sLog.Error( "APE", "Trying to start up after starting up." );
	}
}

extern "C" SCRIPT_DECL void _exp_engine_unload()
{
	if( pythonEngine != NULL )
	{
		sLog.Success( "APE", "Shutting down...");
		delete pythonEngine;
		pythonEngine = NULL;
		sLog.Success( "APE", "Shutdown complete.");
	}
	else
	{
		sLog.Error( "APE", "Trying to shut down before starting up." );
	}
}

extern "C" SCRIPT_DECL void _export_engine_reload()
{
	if( pythonEngine != NULL )
	{
		sLog.Success( "APE", "Reloading");
		pythonEngine->onReload();
		sLog.Success( "APE", "Done reloading.");
	}
	else
	{
		sLog.Error( "APE", "Trying to reload before starting up." );
	}
}

