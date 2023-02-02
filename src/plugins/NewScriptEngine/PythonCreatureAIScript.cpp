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

#include "PythonCreatureAIScript.hpp"

#include "PythonCreatureEventTypes.hpp"

#include "Python.h"
#include "python/ArcPython.hpp"
#include "python/PythonCallable.hpp"
#include "python/ArcPyTuple.hpp"

PythonCreatureAIScript::PythonCreatureAIScript( Creature* src, CreatureFunctionTuple &functions ) : CreatureAIScript( src )
{
	setFunctions( functions );
}

PythonCreatureAIScript::~PythonCreatureAIScript()
{
}

void PythonCreatureAIScript::setFunctions( CreatureFunctionTuple &functions )
{
	for( int i = 0; i < PYTHON_CREATURE_EVENT_COUNT; i++ )
	{
		this->functions.functions[ i ] = functions.functions[ i ];
	}
}

void PythonCreatureAIScript::OnCombatStart( Unit* target )
{
	Guard g( ArcPython::getLock() );

	if( functions.functions[ PYTHON_CREATURE_EVENT_ON_ENTER_COMBAT ] == NULL )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, PYTHON_CREATURE_EVENT_ON_ENTER_COMBAT );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.functions[ PYTHON_CREATURE_EVENT_ON_ENTER_COMBAT ] );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnCombatStop( Unit* target )
{
	Guard g( ArcPython::getLock() );

	if( functions.functions[ PYTHON_CREATURE_EVENT_ON_LEAVE_COMBAT ] == NULL )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, PYTHON_CREATURE_EVENT_ON_LEAVE_COMBAT );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.functions[ PYTHON_CREATURE_EVENT_ON_LEAVE_COMBAT ] );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnDamageTaken( Unit* attacker, uint32 amount )
{
	Guard g( ArcPython::getLock() );

	if( functions.functions[ PYTHON_CREATURE_EVENT_ON_DAMAGE_TAKEN ] == NULL )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, PYTHON_CREATURE_EVENT_ON_DAMAGE_TAKEN );
	args.setItemUnit( 2, attacker );
	args.setItem( 3, amount );

	PythonCallable callable( functions.functions[ PYTHON_CREATURE_EVENT_ON_DAMAGE_TAKEN ] );
	PythonValue result = callable.call( args );
	if( result.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		result.decref();
	}
}

void PythonCreatureAIScript::OnHit( Unit* target, float amount )
{
	Guard g( ArcPython::getLock() );

	if( functions.functions[ PYTHON_CREATURE_EVENT_ON_HIT ] == NULL )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, PYTHON_CREATURE_EVENT_ON_HIT );
	args.setItemUnit( 2, target );
	args.setItem( 3, (unsigned long)amount );

	PythonCallable::callNoReturn( functions.functions[ PYTHON_CREATURE_EVENT_ON_HIT ], args );
}

void PythonCreatureAIScript::OnDied( Unit* killer )
{
	Guard g( ArcPython::getLock() );

	if( functions.functions[ PYTHON_CREATURE_EVENT_ON_DIED ] == NULL )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, PYTHON_CREATURE_EVENT_ON_DIED );
	args.setItemUnit( 2, killer );

	PythonCallable::callNoReturn( functions.functions[ PYTHON_CREATURE_EVENT_ON_DIED ], args );
}

void PythonCreatureAIScript::OnLoad()
{
	Guard g( ArcPython::getLock() );

	if( functions.functions[ PYTHON_CREATURE_EVENT_ON_LOAD ] == NULL )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, PYTHON_CREATURE_EVENT_ON_LOAD );

	PythonCallable::callNoReturn( functions.functions[ PYTHON_CREATURE_EVENT_ON_LOAD ], args );
}


