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

#include "creature/PythonCreatureAIScript.hpp"

#include "creature/PythonCreatureEventTypes.hpp"

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
		this->functions.setFunction( i, functions.getFunction( i ) );
	}
}

void PythonCreatureAIScript::OnCombatStart( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_ENTER_COMBAT;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnCombatStop( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_LEAVE_COMBAT;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnDamageTaken( Unit* attacker, uint32 amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_DAMAGE_TAKEN;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, attacker );
	args.setItem( 3, amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnCastSpell( uint32 spellId )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_CAST_SPELL;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItem( 2, spellId );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnTargetParried( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_TARGET_PARRIED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnTargetDodged( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_TARGET_DODGED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnTargetBlocked( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_TARGET_BLOCKED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );
	args.setItem( 3, amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnTargetCritHit( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_TARGET_CRIT_HIT;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );
	args.setItem( 3, amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnTargetDied( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_TARGET_DIED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}


void PythonCreatureAIScript::OnParried( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_PARRY;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnDodged( Unit* target )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_DODGED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnBlocked( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_BLOCKED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );
	args.setItem( 3, amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnCritHit( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_CRIT_HIT;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );
	args.setItem( 3, amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}


void PythonCreatureAIScript::OnHit( Unit* target, float amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_HIT;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, target );
	args.setItem( 3, (unsigned long)amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnDied( Unit* killer )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_DIED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, killer );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnAssistTargetDied( Unit* assistTarget )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_ASSIST_TARGET_DIED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, assistTarget );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnFear( Unit* feared, uint32 spellId )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_FEAR;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, feared );
	args.setItem( 3, spellId );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnFlee( Unit* flee )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_FLEE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemUnit( 2, flee );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnCallForHelp()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_CALL_FOR_HELP;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}


void PythonCreatureAIScript::OnLoad()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_LOAD;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnDespawn()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_DESPAWN;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnReachWP( uint32 waypointId, bool forwards )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_REACH_WP;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItem( 2, waypointId );
	args.setItemBool( 3, forwards );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnLootTaken( Player* player, ItemPrototype* itemPrototype )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_LOOT_TAKEN;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemPlayer( 2, player );
	args.setItem( 3, itemPrototype->ItemId );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::AIUpdate()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_AIUPDATE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnEmote( Player* player, EmoteType emote )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_EMOTE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItem( 1, eventType );
	args.setItemPlayer( 2, player );
	args.setItem( 3, emote );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnEnterVehicle()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_ENTER_VEHICLE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemUnit( 0, _unit );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnExitVehicle()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_EXIT_VEHICLE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemUnit( 0, _unit );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnFirstPassengerEntered( Unit* passenger )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_FIRST_PASSENGER_ENTERED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItemUnit( 1, passenger );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnVehicleFull()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_VEHICLE_FULL;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemUnit( 0, _unit );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnLastPassengerLeft( Unit* passenger )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_LAST_PASSENGER_LEFT;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItemUnit( 1, passenger );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnHealed( Unit* healer, uint32 spellId, int32 amount )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_HEALED;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 4 );
	args.setItemUnit( 0, _unit );
	args.setItemUnit( 1, healer );
	args.setItem( 2, spellId );
	args.setItem( 3, amount );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnApplyAura( Unit* caster, uint32 spellId )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_APPLY_AURA;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 3 );
	args.setItemUnit( 0, _unit );
	args.setItemUnit( 1, caster );
	args.setItem( 2, spellId );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnActivate( Unit *activator )
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_ACTIVATE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 2 );
	args.setItemUnit( 0, _unit );
	args.setItemUnit( 1, activator );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonCreatureAIScript::OnDeactivate()
{
	Guard g( ArcPython::getLock() );

	uint32 eventType = PYTHON_CREATURE_EVENT_ON_DEACTIVATE;

	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItemUnit( 0, _unit );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

