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
#include "python/PythonTuple.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"
#include "python/Python.hpp"

#include "ServerHookRegistry.hpp"

#include "ServerHookHandler.hpp"

#include "python/modules/ArcPyUnit.hpp"
#include "python/modules/ArcPyPlayer.hpp"

void ServerHookHandler::hookOnKillPlayer( Player* killer, Player* victim )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_KILL_PLAYER, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		PythonTuple args( 2 );

		ArcPyPlayer *appKiller = createArcPyPlayer();
		appKiller->playerPtr = killer;

		ArcPyPlayer *appVictim = createArcPyPlayer();
		appVictim->playerPtr = victim;

		args.setItem( 0, PythonObject( (PyObject*)appKiller ) );		
		args.setItem( 1, PythonObject( (PyObject*)appVictim ) );

		PythonCallable callable( (PyObject*)(*itr) );
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
}

void ServerHookHandler::hookOnFirstEnterWorld( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnEnterWorld( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ENTER_WORLD, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnPlayerDeath( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_DEATH, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnPlayerRepop( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_REPOP, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnEmote( Player* player, uint32 emote, Unit* unit )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_EMOTE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		PythonTuple args( 3 );

		if( player != NULL )
		{
			ArcPyPlayer *app = createArcPyPlayer();
			app->playerPtr = player;
			PythonObject o( (PyObject*)app );
			args.setItem( 0, o );
		}
		else
		{
			args.setItemNone( 0 );
		}
		
		args.setItem( 1, emote );

		if( unit != NULL )
		{
			ArcPyUnit *apu = createArcPyUnit();
			apu->unitPtr = unit;
			PythonObject o( (PyObject*)apu );
			args.setItem( 2, o );
		}
		else
		{
			args.setItemNone( 2 );
		}

		PythonCallable callable( (PyObject*)(*itr) );
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
}

void ServerHookHandler::hookOnEnterCombat( Player* player, Unit* unit )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_ENTER_COMBAT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		PythonTuple args( 2 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		ArcPyUnit *apu = createArcPyUnit();
		apu->unitPtr = unit;

		args.setItem( 0, PythonObject( (PyObject*)app ) );		
		args.setItem( 1, PythonObject( (PyObject*)apu ) );

		PythonCallable callable( (PyObject*)(*itr) );
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
}

void ServerHookHandler::hookOnLogoutRequest( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnLogout( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_LOGOUT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnPlayerResurrect( Player* player )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_RESURRECT, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		void* handler = *itr;
		PythonTuple args( 1 );

		ArcPyPlayer *app = createArcPyPlayer();
		app->playerPtr = player;

		args.setItem( 0, PythonObject( (PyObject*)app ) );

		PythonCallable callable( (PyObject*)handler );
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
}

void ServerHookHandler::hookOnPreUnitDie( Unit* killer, Unit* victim )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( SERVER_HOOK_EVENT_ON_PRE_DIE, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		PythonTuple args( 2 );

		ArcPyUnit *apuKiller = createArcPyUnit();
		apuKiller->unitPtr = killer;

		ArcPyUnit *apuVictim = createArcPyUnit();
		apuVictim->unitPtr = victim;

		args.setItem( 0, PythonObject( (PyObject*)apuKiller ) );		
		args.setItem( 1, PythonObject( (PyObject*)apuVictim ) );

		PythonCallable callable( (PyObject*)(*itr) );
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
}

 