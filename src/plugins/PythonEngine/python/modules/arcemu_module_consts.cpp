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

#include "creature/PythonCreatureEventTypes.hpp"
#include "gameobject/PythonGOEventTypes.hpp"
#include "gossip/PythonGossipEventTypes.hpp"
#include "instance/PythonInstanceEventTypes.hpp"
#include "quest/PythonQuestEventTypes.hpp"

#include <Python.h>

void registerArcemuConstants( PyObject *module )
{
	/// Fields
	PyModule_AddIntConstant( module, "OBJECT_FIELD_GUID", OBJECT_FIELD_GUID );
	PyModule_AddIntConstant( module, "OBJECT_FIELD_ENTRY", OBJECT_FIELD_ENTRY );
	PyModule_AddIntConstant( module, "OBJECT_FIELD_SCALE_X", OBJECT_FIELD_SCALE_X );

	PyModule_AddIntConstant( module, "UNIT_FIELD_CREATEDBY", UNIT_FIELD_CREATEDBY );
	PyModule_AddIntConstant( module, "UNIT_FIELD_SUMMONEDBY", UNIT_FIELD_SUMMONEDBY );
	PyModule_AddIntConstant( module, "UNIT_CREATED_BY_SPELL", UNIT_CREATED_BY_SPELL );
	PyModule_AddIntConstant( module, "UNIT_FIELD_BYTES_0", UNIT_FIELD_BYTES_0 );
	PyModule_AddIntConstant( module, "UNIT_FIELD_BYTES_1", UNIT_FIELD_BYTES_1 );
	PyModule_AddIntConstant( module, "UNIT_FIELD_BYTES_2", UNIT_FIELD_BYTES_2 );

	/// UNIT_FIELD_BYTES_0
	PyModule_AddIntConstant( module, "RACE_BYTE", RACE_BYTE );
	PyModule_AddIntConstant( module, "CLASS_BYTE", CLASS_BYTE );
	PyModule_AddIntConstant( module, "GENDER_BYTE", GENDER_BYTE );
	PyModule_AddIntConstant( module, "POWER_TYPE_BYTE", POWER_TYPE_BYTE );

	/// UNIT_FIELD_BYTES_1
	PyModule_AddIntConstant( module, "STANDSTATE_BYTE", STANDSTATE_BYTE );
	PyModule_AddIntConstant( module, "PET_TALENT_POINTS_BYTE", PET_TALENT_POINTS_BYTE );
	PyModule_AddIntConstant( module, "STANDFLAGS_BYTE", STANDFLAGS_BYTE );
	PyModule_AddIntConstant( module, "VISIBILITY_BYTE", VISIBILITY_BYTE );

	/// UNIT_FIELD_BYTES_2
	PyModule_AddIntConstant( module, "SHEATSTATE_BYTE", SHEATSTATE_BYTE );
	PyModule_AddIntConstant( module, "PVPFLAGS_BYTE", PVPFLAGS_BYTE );
	PyModule_AddIntConstant( module, "PETFLAGS_BYTE", PETFLAGS_BYTE );
	PyModule_AddIntConstant( module, "SHAPESHIFTFORM_BYTE", SHAPESHIFTFORM_BYTE );

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
	PyModule_AddIntConstant( module, "SERVER_HOOK_EVENT_ON_TRANSPORT_ARRIVED", SERVER_HOOK_EVENT_ON_TRANSPORT_ARRIVED );

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

	/// Gossip events
	PyModule_AddIntConstant( module, "GOSSIP_EVENT_HELLO", PYTHON_GOSSIP_EVENT_HELLO );
	PyModule_AddIntConstant( module, "GOSSIP_EVENT_SELECT", PYTHON_GOSSIP_EVENT_SELECT );
	PyModule_AddIntConstant( module, "GOSSIP_EVENT_END", PYTHON_GOSSIP_EVENT_END );

	/// Gossip autosend
	PyModule_AddIntConstant( module, "GOSSIP_AUTOSEND_FALSE", 0 );
	PyModule_AddIntConstant( module, "GOSSIP_AUTOSEND_TRUE", 1 );

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
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_CAST_SPELL", PYTHON_CREATURE_EVENT_ON_CAST_SPELL );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_DESPAWN", PYTHON_CREATURE_EVENT_ON_DESPAWN );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_HEALED", PYTHON_CREATURE_EVENT_ON_HEALED );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_APPLY_AURA", PYTHON_CREATURE_EVENT_ON_APPLY_AURA );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_ACTIVATE", PYTHON_CREATURE_EVENT_ON_ACTIVATE );
	PyModule_AddIntConstant( module, "CREATURE_EVENT_ON_DEACTIVATE", PYTHON_CREATURE_EVENT_ON_DEACTIVATE );

	/// Quest events
	PyModule_AddIntConstant( module, "QUEST_EVENT_ON_ACCEPT", PYTHON_QUEST_EVENT_ON_ACCEPT );
	PyModule_AddIntConstant( module, "QUEST_EVENT_ON_COMPLETE", PYTHON_QUEST_EVENT_ON_COMPLETE );
	PyModule_AddIntConstant( module, "QUEST_EVENT_ON_CANCEL", PYTHON_QUEST_EVENT_ON_CANCEL );
	PyModule_AddIntConstant( module, "QUEST_EVENT_GAMEOBJECT_ACTIVATE", PYTHON_QUEST_EVENT_GAMEOBJECT_ACTIVATE );
	PyModule_AddIntConstant( module, "QUEST_EVENT_ON_CREATURE_KILL", PYTHON_QUEST_EVENT_ON_CREATURE_KILL );
	PyModule_AddIntConstant( module, "QUEST_EVENT_ON_EXPLORE_AREA", PYTHON_QUEST_EVENT_ON_EXPLORE_AREA );
	PyModule_AddIntConstant( module, "QUEST_EVENT_ON_PLAYER_ITEMPICKUP", PYTHON_QUEST_EVENT_ON_PLAYER_ITEMPICKUP );

	/// Instance events
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_PLAYER_DEATH", PYTHON_INSTANCE_EVENT_ON_PLAYER_DEATH );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_PLAYER_ENTER", PYTHON_INSTANCE_EVENT_ON_PLAYER_ENTER );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_AREA_TRIGGER", PYTHON_INSTANCE_EVENT_ON_AREA_TRIGGER );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_ZONE_CHANGE", PYTHON_INSTANCE_EVENT_ON_ZONE_CHANGE );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_CREATURE_DEATH", PYTHON_INSTANCE_EVENT_ON_CREATURE_DEATH );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_CREATURE_PUSH", PYTHON_INSTANCE_EVENT_ON_CREATURE_PUSH );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_GO_ACTIVATE", PYTHON_INSTANCE_EVENT_ON_GO_ACTIVATE );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ON_GO_PUSH", PYTHON_INSTANCE_EVENT_ON_GO_PUSH );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_ONLOAD", PYTHON_INSTANCE_EVENT_ONLOAD );
	PyModule_AddIntConstant( module, "INSTANCE_EVENT_DESTROY", PYTHON_INSTANCE_EVENT_DESTROY );

	/// Sheatstate
	PyModule_AddIntConstant( module, "SHEATSTATE_SHEATHED", 0 );
	PyModule_AddIntConstant( module, "SHEATSTATE_UNSHEATHED", 1 );

	/// Standstate
	PyModule_AddIntConstant( module, "STANDSTATE_STAND", STANDSTATE_STAND );
	PyModule_AddIntConstant( module, "STANDSTATE_SIT", STANDSTATE_SIT );
	PyModule_AddIntConstant( module, "STANDSTATE_SIT_CHAIR", STANDSTATE_SIT_CHAIR );
	PyModule_AddIntConstant( module, "STANDSTATE_SLEEP", STANDSTATE_SLEEP );
	PyModule_AddIntConstant( module, "STANDSTATE_SIT_LOW_CHAIR", STANDSTATE_SIT_LOW_CHAIR );
	PyModule_AddIntConstant( module, "STANDSTATE_SIT_MEDIUM_CHAIR", STANDSTATE_SIT_MEDIUM_CHAIR );
	PyModule_AddIntConstant( module, "STANDSTATE_SIT_HIGH_CHAIR", STANDSTATE_SIT_HIGH_CHAIR );
	PyModule_AddIntConstant( module, "STANDSTATE_DEAD", STANDSTATE_DEAD );
	PyModule_AddIntConstant( module, "STANDSTATE_KNEEL", STANDSTATE_KNEEL );

	/// Waypoint flags
	PyModule_AddIntConstant( module, "WAYPOINT_FLAG_WALK", WAYPOINT_FLAG_WALK );
	PyModule_AddIntConstant( module, "WAYPOINT_FLAG_RUN", WAYPOINT_FLAG_RUN );
	PyModule_AddIntConstant( module, "WAYPOINT_FLAG_FLY", WAYPOINT_FLAG_FLY );

	/// AI movement type
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_NONE", MOVEMENTTYPE_NONE );
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_RANDOMWP", MOVEMENTTYPE_RANDOMWP );
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_CIRCLEWP", MOVEMENTTYPE_CIRCLEWP );
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_WANTEDWP", MOVEMENTTYPE_WANTEDWP );
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_DONTMOVEWP", MOVEMENTTYPE_DONTMOVEWP );
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_QUEST", MOVEMENTTYPE_QUEST );
	PyModule_AddIntConstant( module, "MOVEMENTTYPE_FORWARDTHENSTOP", MOVEMENTTYPE_FORWARDTHANSTOP );

	/// Some emotes
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_NONE", EMOTE_ONESHOT_NONE );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_TALK", EMOTE_ONESHOT_TALK );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_BOW", EMOTE_ONESHOT_BOW );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_WAVE", EMOTE_ONESHOT_WAVE );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_CHEER", EMOTE_ONESHOT_CHEER );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_EXCLAMATION", EMOTE_ONESHOT_EXCLAMATION );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_QUESTION", EMOTE_ONESHOT_QUESTION );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_EAT", EMOTE_ONESHOT_EAT );
	PyModule_AddIntConstant( module, "EMOTE_STATE_DANCE", EMOTE_STATE_DANCE );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_LAUGH", EMOTE_ONESHOT_LAUGH );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_RUDE", EMOTE_ONESHOT_RUDE );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_ROAR", EMOTE_ONESHOT_ROAR );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_KNEEL", EMOTE_ONESHOT_KNEEL );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_KISS", EMOTE_ONESHOT_KISS );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_CRY", EMOTE_ONESHOT_CRY );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_CHICKEN", EMOTE_ONESHOT_CHICKEN );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_BEG", EMOTE_ONESHOT_BEG );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_APPLAUD", EMOTE_ONESHOT_APPLAUD );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_SHOUT", EMOTE_ONESHOT_SHOUT );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_FLEX", EMOTE_ONESHOT_FLEX );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_SHY", EMOTE_ONESHOT_SHY );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_POINT", EMOTE_ONESHOT_POINT );
	PyModule_AddIntConstant( module, "EMOTE_STATE_READYUNARMED", EMOTE_STATE_READYUNARMED );
	PyModule_AddIntConstant( module, "EMOTE_STATE_NONE", EMOTE_STATE_NONE );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_SALUTE", EMOTE_ONESHOT_SALUTE );
	PyModule_AddIntConstant( module, "EMOTE_ONESHOT_DANCE", EMOTE_ONESHOT_DANCE );

	/// NPC flags
	PyModule_AddIntConstant( module, "NPC_FLAG_NONE", UNIT_NPC_FLAG_NONE );
	PyModule_AddIntConstant( module, "NPC_FLAG_GOSSIP", UNIT_NPC_FLAG_GOSSIP );
	PyModule_AddIntConstant( module, "NPC_FLAG_QUESTGIVER", UNIT_NPC_FLAG_QUESTGIVER );
	PyModule_AddIntConstant( module, "NPC_FLAG_TRAINER", UNIT_NPC_FLAG_TRAINER );
	PyModule_AddIntConstant( module, "NPC_FLAG_TRAINER_CLASS", UNIT_NPC_FLAG_TRAINER_CLASS );
	PyModule_AddIntConstant( module, "NPC_FLAG_TRAINER_PROF", UNIT_NPC_FLAG_TRAINER_PROF );
	PyModule_AddIntConstant( module, "NPC_FLAG_VENDOR", UNIT_NPC_FLAG_VENDOR );
	PyModule_AddIntConstant( module, "NPC_FLAG_VENDOR_AMMO", UNIT_NPC_FLAG_VENDOR_AMMO );
	PyModule_AddIntConstant( module, "NPC_FLAG_VENDOR_FOOD", UNIT_NPC_FLAG_VENDOR_FOOD );
	PyModule_AddIntConstant( module, "NPC_FLAG_VENDOR_POISON", UNIT_NPC_FLAG_VENDOR_POISON );
	PyModule_AddIntConstant( module, "NPC_FLAG_VENDOR_REAGENT", UNIT_NPC_FLAG_VENDOR_REAGENT );
	PyModule_AddIntConstant( module, "NPC_FLAG_ARMORER", UNIT_NPC_FLAG_ARMORER );
	PyModule_AddIntConstant( module, "NPC_FLAG_TAXIVENDOR", UNIT_NPC_FLAG_TAXIVENDOR );
	PyModule_AddIntConstant( module, "NPC_FLAG_SPIRITHEALER", UNIT_NPC_FLAG_SPIRITHEALER );
	PyModule_AddIntConstant( module, "NPC_FLAG_SPIRITGUIDE", UNIT_NPC_FLAG_SPIRITGUIDE );
	PyModule_AddIntConstant( module, "NPC_FLAG_INNKEEPER", UNIT_NPC_FLAG_INNKEEPER );
	PyModule_AddIntConstant( module, "NPC_FLAG_BANKER", UNIT_NPC_FLAG_BANKER );
	PyModule_AddIntConstant( module, "NPC_FLAG_ARENACHARTER", UNIT_NPC_FLAG_ARENACHARTER );
	PyModule_AddIntConstant( module, "NPC_FLAG_TABARDCHANGER", UNIT_NPC_FLAG_TABARDCHANGER );
	PyModule_AddIntConstant( module, "NPC_FLAG_BATTLEFIELDPERSON", UNIT_NPC_FLAG_BATTLEFIELDPERSON );
	PyModule_AddIntConstant( module, "NPC_FLAG_AUCTIONEER", UNIT_NPC_FLAG_AUCTIONEER );
	PyModule_AddIntConstant( module, "NPC_FLAG_STABLEMASTER", UNIT_NPC_FLAG_STABLEMASTER );
	PyModule_AddIntConstant( module, "NPC_FLAG_GUILD_BANK", UNIT_NPC_FLAG_GUILD_BANK );
	PyModule_AddIntConstant( module, "NPC_FLAG_SPELLCLICK", UNIT_NPC_FLAG_SPELLCLICK );
	PyModule_AddIntConstant( module, "NPC_FLAG_PLAYER_VEHICLE", UNIT_NPC_FLAG_PLAYER_VEHICLE );
}
