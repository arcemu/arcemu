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

#include "PythonCreatureEventTypes.hpp"
#include "PythonGOEventTypes.hpp"

#include <Python.h>

void registerArcemuConstants( PyObject *module )
{
	/// Server hook constants
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

	/// Languages
	PyModule_AddIntConstant( module, "LANG_UNIVERSAL", LANG_UNIVERSAL );
	PyModule_AddIntConstant( module, "LANG_ORCISH", LANG_ORCISH );
	PyModule_AddIntConstant( module, "LANG_DARNASSIAN", LANG_DARNASSIAN );
	PyModule_AddIntConstant( module, "LANG_TAURAHE", LANG_TAURAHE );
	/// 4
	/// 5
	PyModule_AddIntConstant( module, "LANG_DWARVISH", LANG_DWARVISH );
	PyModule_AddIntConstant( module, "LANG_COMMON", LANG_COMMON );
	PyModule_AddIntConstant( module, "LANG_DEMONIC", LANG_DEMONIC );
	PyModule_AddIntConstant( module, "LANG_TITAN", LANG_TITAN );
	PyModule_AddIntConstant( module, "LANG_THELASSIAN", LANG_THELASSIAN );
	PyModule_AddIntConstant( module, "LANG_DRACONIC", LANG_DRACONIC );
	PyModule_AddIntConstant( module, "LANG_KALIMAG", LANG_KALIMAG );
	PyModule_AddIntConstant( module, "LANG_GNOMISH", LANG_GNOMISH );
	PyModule_AddIntConstant( module, "LANG_TROLL", LANG_TROLL );

	PyModule_AddIntConstant( module, "LANG_GUTTERSPEAK", LANG_GUTTERSPEAK );
	PyModule_AddIntConstant( module, "LANG_TITAN", LANG_TITAN );
	PyModule_AddIntConstant( module, "LANG_DRAENEI", LANG_DRAENEI );

	/// Chat message types
	PyModule_AddIntConstant( module, "CHAT_MSG_SYSTEM", CHAT_MSG_SYSTEM );
	PyModule_AddIntConstant( module, "CHAT_MSG_SAY", CHAT_MSG_SAY );
	PyModule_AddIntConstant( module, "CHAT_MSG_PARTY", CHAT_MSG_PARTY );
	PyModule_AddIntConstant( module, "CHAT_MSG_RAID", CHAT_MSG_RAID );
	PyModule_AddIntConstant( module, "CHAT_MSG_YELL", CHAT_MSG_YELL );
	PyModule_AddIntConstant( module, "CHAT_MSG_WHISPER", CHAT_MSG_WHISPER );

	PyModule_AddIntConstant( module, "CHAT_MSG_MONSTER_SAY", CHAT_MSG_MONSTER_SAY );
	PyModule_AddIntConstant( module, "CHAT_MSG_MONSTER_PARTY", CHAT_MSG_MONSTER_PARTY );
	PyModule_AddIntConstant( module, "CHAT_MSG_MONSTER_YELL", CHAT_MSG_MONSTER_YELL );
	PyModule_AddIntConstant( module, "CHAT_MSG_MONSTER_WHISPER", CHAT_MSG_MONSTER_WHISPER );
	PyModule_AddIntConstant( module, "CHAT_MSG_MONSTER_EMOTE", CHAT_MSG_MONSTER_EMOTE );

	/// Gossip Icons
	PyModule_AddIntConstant( module, "ICON_CHAT", Arcemu::Gossip::ICON_CHAT );
	PyModule_AddIntConstant( module, "ICON_VENDOR", Arcemu::Gossip::ICON_VENDOR );
	PyModule_AddIntConstant( module, "ICON_FLIGHTMASTER", Arcemu::Gossip::ICON_FLIGHTMASTER );
	PyModule_AddIntConstant( module, "ICON_TRAINER", Arcemu::Gossip::ICON_TRAINER );
	PyModule_AddIntConstant( module, "ICON_MONEY_BAG", Arcemu::Gossip::ICON_MONEY_BAG );
	PyModule_AddIntConstant( module, "ICON_TALK", Arcemu::Gossip::ICON_TALK );
	PyModule_AddIntConstant( module, "ICON_TABARD", Arcemu::Gossip::ICON_TABARD );
	PyModule_AddIntConstant( module, "ICON_BATTLE", Arcemu::Gossip::ICON_BATTLE );
	PyModule_AddIntConstant( module, "ICON_DOT", Arcemu::Gossip::ICON_DOT );

	/// GameObject events
	PyModule_AddIntConstant( module, "GO_EVENT_ON_CREATE", PYTHON_GO_EVENT_ON_CREATE );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_SPAWN", PYTHON_GO_EVENT_ON_SPAWN );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_LOOT_TAKEN", PYTHON_GO_EVENT_ON_LOOT_TAKEN );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_USE", PYTHON_GO_EVENT_ON_USE );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_AIUPDATE", PYTHON_GO_EVENT_ON_AIUPDATE );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_DESPAWN", PYTHON_GO_EVENT_ON_DESPAWN );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_DAMAGED", PYTHON_GO_EVENT_ON_DAMAGED );
	PyModule_AddIntConstant( module, "GO_EVENT_ON_DESTROYED", PYTHON_GO_EVENT_ON_DESTROYED );

	/// Creature events
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_ENTER_COMBAT", PYTHON_CREATURE_EVENT_ON_ENTER_COMBAT );	
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_LEAVE_COMBAT", PYTHON_CREATURE_EVENT_ON_LEAVE_COMBAT );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_TARGET_DIED", PYTHON_CREATURE_EVENT_ON_TARGET_DIED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_DIED", PYTHON_CREATURE_EVENT_ON_DIED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_TARGET_PARRIED", PYTHON_CREATURE_EVENT_ON_TARGET_PARRIED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_TARGET_DODGED", PYTHON_CREATURE_EVENT_ON_TARGET_DODGED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_TARGET_BLOCKED", PYTHON_CREATURE_EVENT_ON_TARGET_BLOCKED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_TARGET_CRIT_HIT", PYTHON_CREATURE_EVENT_ON_TARGET_CRIT_HIT );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_PARRY", PYTHON_CREATURE_EVENT_ON_PARRY );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_DODGED", PYTHON_CREATURE_EVENT_ON_DODGED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_BLOCKED", PYTHON_CREATURE_EVENT_ON_BLOCKED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_CRIT_HIT", PYTHON_CREATURE_EVENT_ON_CRIT_HIT );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_HIT", PYTHON_CREATURE_EVENT_ON_HIT );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_ASSIST_TARGET_DIED", PYTHON_CREATURE_EVENT_ON_ASSIST_TARGET_DIED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_FEAR", PYTHON_CREATURE_EVENT_ON_FEAR );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_FLEE", PYTHON_CREATURE_EVENT_ON_FLEE );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_CALL_FOR_HELP", PYTHON_CREATURE_EVENT_ON_CALL_FOR_HELP );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_LOAD", PYTHON_CREATURE_EVENT_ON_LOAD );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_REACH_WP", PYTHON_CREATURE_EVENT_ON_REACH_WP );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_LOOT_TAKEN", PYTHON_CREATURE_EVENT_ON_LOOT_TAKEN );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_AIUPDATE", PYTHON_CREATURE_EVENT_ON_AIUPDATE );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_EMOTE", PYTHON_CREATURE_EVENT_ON_EMOTE );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_DAMAGE_TAKEN", PYTHON_CREATURE_EVENT_ON_DAMAGE_TAKEN );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_ENTER_VEHICLE", PYTHON_CREATURE_EVENT_ON_ENTER_VEHICLE );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_EXIT_VEHICLE", PYTHON_CREATURE_EVENT_ON_EXIT_VEHICLE );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_FIRST_PASSENGER_ENTERED", PYTHON_CREATURE_EVENT_ON_FIRST_PASSENGER_ENTERED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_VEHICLE_FULL", PYTHON_CREATURE_EVENT_ON_VEHICLE_FULL );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_LAST_PASSENGER_LEFT", PYTHON_CREATURE_EVENT_ON_LAST_PASSENGER_LEFT );


}
