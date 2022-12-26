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
#include "engine.h"

#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_SCRIPT_ENGINE;
}

extern "C" SCRIPT_DECL void _exp_script_register( const ScriptMgr *mgr )
{
	LOG_BASIC("NSE: Script register called");
}

extern "C" SCRIPT_DECL void _exp_engine_unload()
{
	LOG_BASIC("NSE: Engine unload called");
}

extern "C" SCRIPT_DECL void _export_engine_reload()
{
	LOG_BASIC("NSE: Engine reload called");
}


namespace Arcemu
{
	namespace Scripting
	{
		NewScriptEngine::NewScriptEngine()
		{
		}
	};
};
