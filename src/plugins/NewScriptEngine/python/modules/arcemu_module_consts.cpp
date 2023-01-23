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
}
