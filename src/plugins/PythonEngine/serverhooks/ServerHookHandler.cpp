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

#include "python/PythonObject.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"
#include "python/ArcPython.hpp"

#include "python/ArcPyTuple.hpp"

#include "serverhooks/ServerHookRegistry.hpp"
#include "serverhooks/ServerHookHandler.hpp"

#include "python/modules/ArcPyAura.hpp"
#include "python/modules/ArcPyGameObject.hpp"
#include "python/modules/ArcPyGuild.hpp"
#include "python/modules/ArcPyItem.hpp"
#include "python/modules/ArcPyUnit.hpp"
#include "python/modules/ArcPyPlayer.hpp"
#include "python/modules/ArcPyQuest.hpp"
#include "python/modules/ArcPySpell.hpp"
#include "python/modules/ArcPyWorldSession.hpp"

bool ServerHookHandler::hookOnNewCharacter( uint32 charRace, uint32 charClass, WorldSession* session, const char *name )
{
	Guard g( ArcPython::getLock() );

	bool returnValue = true;

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_NEW_CHARACTER, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		ArcPyTuple args( 4 );

		args.setItem( 0, charRace );
		args.setItem( 1, charClass );
		args.setItemWorldSession( 2, session );
		args.setItem( 3, name );

		PythonCallable callable( *itr );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			Python::printError();
		}
		else
		{
			if( value.isBool() && !value.getBoolValue() )
			{
				returnValue = false;
			}
			value.decref();
		}
	}

	return returnValue;
}

void ServerHookHandler::hookOnKillPlayer( Player* killer, Player* victim )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_KILL_PLAYER, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		ArcPyTuple args( 2 );

		args.setItemPlayer( 0, killer );		
		args.setItemPlayer( 1, victim );

		PythonCallable callable( *itr );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnFirstEnterWorld( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );

		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnEnterWorld( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ENTER_WORLD, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );

		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnGuildJoin( Player* player, Guild* guild )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_GUILD_JOIN, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 2 );

		args.setItemPlayer( 0, player );
		args.setItemGuild( 1, guild );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnPlayerDeath( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_DEATH, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );

		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

bool ServerHookHandler::hookOnPlayerRepop( Player* player )
{
	Guard g( ArcPython::getLock() );

	bool returnValue = true;

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_REPOP, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );

		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			Python::printError();
		}
		else
		{
			if( value.isBool() && !value.getBoolValue() )
			{
				returnValue = false;
			}
			value.decref();
		}
	}

	return returnValue;
}

void ServerHookHandler::hookOnEmote( Player* player, uint32 emote, Unit* unit )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_EMOTE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		ArcPyTuple args( 3 );

		if( player != NULL )
		{
			args.setItemPlayer( 0, player );
		}
		else
		{
			args.setItemNone( 0 );
		}
		
		args.setItem( 1, emote );

		if( unit != NULL )
		{
			args.setItemUnit( 2, unit );
		}
		else
		{
			args.setItemNone( 2 );
		}

		PythonCallable callable( *itr );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnEnterCombat( Player* player, Unit* unit )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ENTER_COMBAT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		ArcPyTuple args( 2 );

		args.setItemPlayer( 0, player );		
		args.setItemUnit( 1, unit );

		PythonCallable callable( *itr );
		callable.callNoReturn( args );
	}
}

bool ServerHookHandler::hookOnCastSpell( Player* player, SpellEntry* spe, Spell* spell )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_CAST_SPELL, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 3 );
		args.setItemPlayer( 0, player );
		args.setItem( 1, spe->Id );
		args.setItemSpell( 2, spell );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}

	return true;
}

bool ServerHookHandler::hookOnLogoutRequest( Player* player )
{
	Guard g( ArcPython::getLock() );

	bool returnValue = true;

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );
		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			Python::printError();
		}
		else
		{
			if( value.isBool() && ! value.getBoolValue() )
			{
				returnValue = false;
			}
			value.decref();
		}
	}

	return returnValue;
}

void ServerHookHandler::hookOnLogout( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_LOGOUT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );

		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnAcceptQuest( Player* player, Quest* quest, Object* questGiver )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 3 );

		args.setItemPlayer( 0, player );
		args.setItemQuest( 1, quest );

		PyObject *param = NULL;

		/// GM commands!
		if( questGiver != NULL )
		{
			uint8 qgt = questGiver->GetTypeId();
			switch( qgt )
			{
			case TYPEID_GAMEOBJECT:
				{
					args.setItemGameObject( 2, (GameObject*)questGiver );
				}
				break;

			case TYPEID_UNIT:
				{
					args.setItemUnit( 2, (Unit*)questGiver );
				}
				break;

			case TYPEID_ITEM:
				{
					args.setItemPlayer( 2, (Player*)questGiver );
					break;
				}

			default:
				{
					args.setItemNone( 2 );
					break;
				}
			}
		}
		else
		{
			args.setItemNone( 2 );
		}

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnZoneChange( Player* player, uint32 oldZone, uint32 newZone )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ZONE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 3 );

		args.setItemPlayer( 0, player );
		args.setItem( 1, oldZone );
		args.setItem( 2, newZone );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

bool ServerHookHandler::hookOnChatMessage( Player* player, uint32 type, uint32 lang, const char* message, const char* misc )
{
	Guard g( ArcPython::getLock() );

	bool returnValue = true;

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_CHAT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;

		ArcPyTuple args( 5 );
		args.setItemPlayer( 0, player );
		args.setItem( 1, type );
		args.setItem( 2, lang );
		args.setItem( 3, message );

		if( misc != NULL )
			args.setItem( 4, misc );
		else
			args.setItem( 4, "" );

		PythonCallable callable( handler );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			Python::printError();
		}
		else
		{
			if( value.isBool() && !value.getBoolValue() )
			{
				returnValue = false;
			}

			value.decref();
		}
	}

	return returnValue;
}

void ServerHookHandler::hookOnLoot( Player* player, Unit* unit, uint32 money, uint32 itemId )
{
	/// Why is this called when it's not a Unit we're looting?!
	if( unit == NULL )
		return;

	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_LOOT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 4 );

		args.setItemPlayer( 0, player );
		args.setItemUnit( 1, unit );
		args.setItem( 2, money );
		args.setItem( 3, itemId );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnGuildCreate( Player* leader, Guild* guild )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_GUILD_CREATE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 2 );
		args.setItemPlayer( 0, leader );
		args.setItemGuild( 1, guild );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnFullLogin( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_FULL_LOGIN, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 1 );
		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnCharacterCreated( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 1 );
		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnQuestCancelled( Player* player, Quest* quest )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 2 );
		args.setItemPlayer( 0, player );
		args.setItemQuest( 1, quest );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnQuestFinished( Player* player, Quest* quest, Object *questFinisher )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_QUEST_FINISHED, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 3 );

		args.setItemPlayer( 0, player );
		args.setItemQuest( 1, quest );

		uint8 qgt = questFinisher->GetTypeId();
		switch( qgt )
		{
		case TYPEID_GAMEOBJECT:
			{
				args.setItemGameObject( 2, (GameObject*)questFinisher );
			}
			break;

		case TYPEID_UNIT:
			{
				args.setItemUnit( 2, (Unit*)questFinisher );
			}
			break;

		case TYPEID_ITEM:
			{
				args.setItemItem( 2, (Item*)questFinisher );
			}
			break;

		default:
			{
				args.setItemNone( 2 );
			}
			break;
		}

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnHonorableKill( Player* killer, Player *victim )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_HONORABLE_KILL, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 2 );
		args.setItemPlayer( 0, killer );
		args.setItemPlayer( 1, victim );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnArenaFinish( Player* player, ArenaTeam* arenaTeam, bool victory, bool rated )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ARENA_FINISH, handlers );
	
	/// Because of debug commands sometimes there's no arena team!
	string teamName;
	if( arenaTeam != NULL )
	{
		teamName = arenaTeam->m_name;
	}

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 4 );
		args.setItemPlayer( 0, player );
		args.setItem( 1, teamName.c_str() );
		args.setItemBool( 2, victory );
		args.setItemBool( 3, rated );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnObjectLoot( Player* player, Object* target, uint32 money, uint32 itemId )
{
	if( target == NULL )
		return;

	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_OBJECTLOOT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 4 );

		args.setItemPlayer( 0, player );

		switch( target->GetTypeId() )
		{
		case TYPEID_UNIT:
			{
				args.setItemUnit( 1, (Unit*)target );
				break;
			}

		case TYPEID_PLAYER:
			{
				args.setItemPlayer( 1, (Player*)target );
				break;
			}

		case TYPEID_GAMEOBJECT:
			{
				args.setItemGameObject( 1, (GameObject*)target );
				break;
			}

		case TYPEID_ITEM:
			{
				args.setItemItem( 1, (Item*)target );
				break;
			}

		default:
			{
				args.setItemNone( 1 );
				break;
			}
		}

		
		args.setItem( 2, money );
		args.setItem( 3, itemId );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnAreaTrigger( Player* player, uint32 areaTriggerId )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_AREATRIGGER, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 2 );
		args.setItemPlayer( 0, player );
		args.setItem( 1, areaTriggerId );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

bool ServerHookHandler::hookOnPlayerResurrect( Player* player )
{
	Guard g( ArcPython::getLock() );

	bool returnValue = true;

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_RESURRECT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;

		ArcPyTuple args( 1 );
		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			Python::printError();
		}
		else
		{
			if( value.isBool() && !value.getBoolValue() )
			{
				returnValue = false;
			}
			value.decref();
		}
	}

	return true;
}

void ServerHookHandler::hookOnLevelUp( Player* player )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_POST_LEVELUP, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		
		ArcPyTuple args( 1 );
		args.setItemPlayer( 0, player );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

bool ServerHookHandler::hookOnPreUnitDie( Unit* killer, Unit* victim )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_PRE_DIE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		ArcPyTuple args( 2 );
		args.setItemUnit( 0, killer );		
		args.setItemUnit( 1, victim );

		PythonCallable callable( *itr );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			Python::printError();
		}
		else
		{
			value.decref();
		}
	}

	return true;
}

void ServerHookHandler::hookOnAdvanceSkillLine( Player* player, uint32 skill, uint32 value )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;

		ArcPyTuple args( 3 );
		args.setItemPlayer( 0, player );
		args.setItem( 1, skill );
		args.setItem( 2, value );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnDuelFinished( Player* winner, Player* loser )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_DUEL_FINISHED, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;

		ArcPyTuple args( 2 );
		args.setItemPlayer( 0, winner );
		args.setItemPlayer( 1, loser );		

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnAuraRemove( Aura* aura )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_AURA_REMOVE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		ArcPyTuple args( 1 );

		args.setItemAura( 0, aura );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}

void ServerHookHandler::hookOnTransportArrived( GameObject *go, uint32 route )
{
	Guard g( ArcPython::getLock() );

	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_TRANSPORT_ARRIVED, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;

		ArcPyTuple args( 2 );
		args.setItemGameObject( 0, go );
		args.setItem( 1, route );

		PythonCallable callable( handler );
		callable.callNoReturn( args );
	}
}
