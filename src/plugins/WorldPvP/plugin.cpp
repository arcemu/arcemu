/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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
 
#include "plugin.h"
#include <Game/Scripting/ScriptSetup.h>
#include "git_version.h"

void setupEasternPlaguelands( ScriptMgr *mgr );
void setupHellfirePeninsula( ScriptMgr *mgr );
void setupNagrand( ScriptMgr *mgr );
void setupSilithus( ScriptMgr *mgr );
void setupTerokkarForest( ScriptMgr *mgr );
void setupZangarmarsh( ScriptMgr *mgr );

extern "C" SCRIPT_DECL const char* _exp_get_version()
{
	return BUILD_HASH_STR;
}

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register( ScriptMgr *mgr )
{
	setupEasternPlaguelands( mgr );
	setupHellfirePeninsula( mgr );
	setupNagrand( mgr );
	setupSilithus( mgr );
	setupTerokkarForest( mgr );
	setupZangarmarsh( mgr );
}
