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

#include "StdAfx.h"
#include "Python.h"
#include "PythonCreatureOOScript.hpp"
#include "python/modules/ArcPyUnit.hpp"
#include "python/modules/ArcPyPlayer.hpp"

#include "python/ArcPython.hpp"

PythonCreatureOOScript::PythonCreatureOOScript( Creature *creature, PyObject *obj ) :
CreatureAIScript( creature )
{
	this->obj = NULL;
	setObject( obj );
}

PythonCreatureOOScript::~PythonCreatureOOScript()
{
	if( obj != NULL )
	{
		Py_DECREF( obj );
		obj = NULL;
	}
}

void PythonCreatureOOScript::setObject( PyObject *obj )
{
	if( this->obj != NULL )
	{
		Py_DECREF( this->obj );
	}

	if( obj != NULL )
	{
		Py_INCREF( obj );
	}

	this->obj = obj;
}


void PythonCreatureOOScript::OnCombatStart( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnCombatStart", "O", apu );
}

void PythonCreatureOOScript::OnCombatStop( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnCombatStop", "O", apu );
}

void PythonCreatureOOScript::OnDamageTaken( Unit* attacker, uint32 amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( attacker );
	PyObject_CallMethod( obj, "OnDamageTaken", "OI", apu, amount );
}

void PythonCreatureOOScript::OnCastSpell( uint32 spellId )
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnCastSpell", "I", spellId );
}

void PythonCreatureOOScript::OnTargetParried( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnTargetParried", "O", apu );
}

void PythonCreatureOOScript::OnTargetDodged( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnTargetDodged", "O", apu );
}

void PythonCreatureOOScript::OnTargetBlocked( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnTargetBlocked", "Oi", apu, amount );
}

void PythonCreatureOOScript::OnTargetCritHit( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnTargetCritHit", "Oi", apu, amount );
}

void PythonCreatureOOScript::OnTargetDied( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnTargetDied", "O", apu );
}

void PythonCreatureOOScript::OnParried( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnParried", "O", apu );
}

void PythonCreatureOOScript::OnDodged( Unit* target )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnDodged", "O", apu );
}

void PythonCreatureOOScript::OnBlocked( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnBlocked", "Oi", apu, amount );
}

void PythonCreatureOOScript::OnCritHit( Unit* target, int32 amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnCritHit", "Oi", apu, amount );
}

void PythonCreatureOOScript::OnHit( Unit* target, float amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( target );
	PyObject_CallMethod( obj, "OnHit", "Of", apu, amount );
}

void PythonCreatureOOScript::OnDied( Unit* killer )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( killer );
	PyObject_CallMethod( obj, "OnDied", "O", apu );
}

void PythonCreatureOOScript::OnAssistTargetDied( Unit* assistTarget )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( assistTarget );
	PyObject_CallMethod( obj, "OnAssistTargetDied", "O", apu );
}

void PythonCreatureOOScript::OnFear( Unit* feared, uint32 spellId )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( feared );
	PyObject_CallMethod( obj, "OnFear", "Oi", apu, spellId );
}

void PythonCreatureOOScript::OnFlee( Unit* flee )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( flee );
	PyObject_CallMethod( obj, "OnFlee", "O", apu );
}

void PythonCreatureOOScript::OnCallForHelp()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnCallForHelp", NULL );
}

void PythonCreatureOOScript::OnLoad()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnLoad", NULL );
}

void PythonCreatureOOScript::OnDespawn()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnDespawn", NULL );
}

void PythonCreatureOOScript::OnReachWP( uint32 waypointId, bool forwards )
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnReachWP", "Ip", waypointId, forwards );
}

void PythonCreatureOOScript::OnLootTaken( Player* player, ItemPrototype* itemPrototype )
{
	Guard g( ArcPython::getLock() );

	PyObject *app = (PyObject*)createArcPyPlayer( player );

	PyObject_CallMethod( obj, "OnLootTaken", "OI", app, itemPrototype->ItemId );
}

void PythonCreatureOOScript::AIUpdate()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "AIUpdate", NULL );
}

void PythonCreatureOOScript::OnEmote( Player* player, EmoteType emote )
{
	Guard g( ArcPython::getLock() );

	PyObject *app = (PyObject*)createArcPyPlayer( player );
	PyObject_CallMethod( obj, "OnEmote", "OI", app, (uint32)emote );
}

void PythonCreatureOOScript::OnEnterVehicle()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnEnterVehicle", NULL );
}

void PythonCreatureOOScript::OnExitVehicle()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnExitVehicle", NULL );
}

void PythonCreatureOOScript::OnFirstPassengerEntered( Unit *passenger )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( passenger );
	PyObject_CallMethod( obj, "OnFirstPassengerEntered", "O", apu );
}

void PythonCreatureOOScript::OnVehicleFull()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnVehicleFull", NULL );
}

void PythonCreatureOOScript::OnLastPassengerLeft( Unit *passenger )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( passenger );
	PyObject_CallMethod( obj, "OnLastPassengerLeft", "O", apu );
}

void PythonCreatureOOScript::OnHealed( Unit* healer, uint32 spellId, int32 amount )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( healer );
	PyObject_CallMethod( obj, "OnHealed", "OIi", apu, spellId, amount );
}

void PythonCreatureOOScript::OnApplyAura( Unit* caster, uint32 spellId )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( caster );
	PyObject_CallMethod( obj, "OnApplyAura", "OI", apu, spellId );
}

void PythonCreatureOOScript::OnActivate( Unit* activator )
{
	Guard g( ArcPython::getLock() );

	PyObject *apu = (PyObject*)createArcPyUnit( activator );
	PyObject_CallMethod( obj, "OnActivate", "O", apu );
}

void PythonCreatureOOScript::OnDeactivate()
{
	Guard g( ArcPython::getLock() );

	PyObject_CallMethod( obj, "OnDeactivate", NULL );
}

