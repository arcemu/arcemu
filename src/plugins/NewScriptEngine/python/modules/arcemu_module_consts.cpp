/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

void registerArcemuConstants( PyObject *module )
{
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_NEW_CHARACTER", SERVER_HOOK_EVENT_ON_NEW_CHARACTER );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_KILL_PLAYER", SERVER_HOOK_EVENT_ON_KILL_PLAYER );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD", SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_ENTER_WORLD", SERVER_HOOK_EVENT_ON_ENTER_WORLD );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_GUILD_JOIN", SERVER_HOOK_EVENT_ON_GUILD_JOIN );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_DEATH", SERVER_HOOK_EVENT_ON_DEATH );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_REPOP", SERVER_HOOK_EVENT_ON_REPOP );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_EMOTE", SERVER_HOOK_EVENT_ON_EMOTE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_ENTER_COMBAT", SERVER_HOOK_EVENT_ON_ENTER_COMBAT );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_CAST_SPELL", SERVER_HOOK_EVENT_ON_CAST_SPELL );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_TICK", SERVER_HOOK_EVENT_ON_TICK );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST", SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_LOGOUT", SERVER_HOOK_EVENT_ON_LOGOUT );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_QUEST_ACCEPT", SERVER_HOOK_EVENT_ON_QUEST_ACCEPT );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_ZONE", SERVER_HOOK_EVENT_ON_ZONE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_CHAT", SERVER_HOOK_EVENT_ON_CHAT );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_LOOT", SERVER_HOOK_EVENT_ON_LOOT );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_GUILD_CREATE", SERVER_HOOK_EVENT_ON_GUILD_CREATE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_FULL_LOGIN", SERVER_HOOK_EVENT_ON_FULL_LOGIN );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_CHARACTER_CREATE", SERVER_HOOK_EVENT_ON_CHARACTER_CREATE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_QUEST_CANCELLED", SERVER_HOOK_EVENT_ON_QUEST_CANCELLED );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_QUEST_FINISHED", SERVER_HOOK_EVENT_ON_QUEST_FINISHED );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_HONORABLE_KILL", SERVER_HOOK_EVENT_ON_HONORABLE_KILL );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_ARENA_FINISH", SERVER_HOOK_EVENT_ON_ARENA_FINISH );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_OBJECTLOOT", SERVER_HOOK_EVENT_ON_OBJECTLOOT );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_AREATRIGGER", SERVER_HOOK_EVENT_ON_AREATRIGGER );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_POST_LEVELUP", SERVER_HOOK_EVENT_ON_POST_LEVELUP );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_PRE_DIE", SERVER_HOOK_EVENT_ON_PRE_DIE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE", SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_DUEL_FINISHED", SERVER_HOOK_EVENT_ON_DUEL_FINISHED );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_AURA_REMOVE", SERVER_HOOK_EVENT_ON_AURA_REMOVE );
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_RESURRECT", SERVER_HOOK_EVENT_ON_RESURRECT );
}
