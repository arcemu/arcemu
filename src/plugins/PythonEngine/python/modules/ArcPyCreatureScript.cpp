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

#include "Python.h"
#include "structmember.h"

#include "ArcPyCreatureScript.hpp"
#include "ArcPyCreature.hpp"

static PyObject* ArcPyCreatureScript_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	PyObject *o = NULL;
	if( !PyArg_ParseTuple( args, "O", &o ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a Creature parameter" );
		return NULL;
	}

	if( strcmp( Py_TYPE( o )->tp_name, "ArcPyCreature" ) != 0 )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a Creature parameter" );
		return NULL;
	}

	ArcPyCreatureScript *self = (ArcPyCreatureScript*)type->tp_alloc( type, 0 );
	Py_INCREF( o );
	self->creature = o;

	return (PyObject*)self;
}

static int ArcPyCreatureScript_init( ArcPyCreatureScript *self, PyObject *args, PyObject *keywords )
{
	PyObject *o = NULL;
	if( !PyArg_ParseTuple( args, "O", &o ) )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a Creature parameter" );
		return -1;
	}

	if( strcmp( Py_TYPE( o )->tp_name, "ArcPyCreature" ) != 0 )
	{
		PyErr_SetString( PyExc_ValueError, "This function requires a Creature parameter" );
		return -1;
	}

	if( self->creature != NULL )
	{
		Py_DecRef( self->creature );
		self->creature = NULL;
	}

	self->creature = o;

	return 0;
}

static void ArcPyCreatureScript_dealloc( ArcPyCreatureScript* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

////////////////////////////////////////// Event handler methods /////////////////////////////////////////////////////////////////////////////

static PyObject* ArcPyCreatureScript_OnCombatStart( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnCombatStop( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnDamageTaken( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnCastSpell( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnTargetParried( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnTargetDodged( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnTargetBlocked( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnTargetCritHit( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnTargetDied( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnParried( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnDodged( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnBlocked( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnCritHit( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnHit( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnDied( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnAssistTargetDied( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnFear( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnFlee( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnCallForHelp( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnLoad( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnDespawn( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnReachWP( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnLootTaken( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_AIUpdate( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnEmote( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnEnterVehicle( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnExitVehicle( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnFirstPassengerEntered( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnVehicleFull( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnLastPassengerLeft( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnHealed( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnApplyAura( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnActivate( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

static PyObject* ArcPyCreatureScript_OnDeactivate( ArcPyCreatureScript *self, PyObject *args )
{
	Py_RETURN_NONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static PyMethodDef ArcPyCreatureScript_methods[] = 
{
	{ "OnCombatStart", (PyCFunction)ArcPyCreatureScript_OnCombatStart, METH_VARARGS, "" },
	{ "OnCombatStop", (PyCFunction)ArcPyCreatureScript_OnCombatStop, METH_VARARGS, "" },
	{ "OnDamageTaken", (PyCFunction)ArcPyCreatureScript_OnDamageTaken, METH_VARARGS, "" },
	{ "OnCastSpell", (PyCFunction)ArcPyCreatureScript_OnCastSpell, METH_VARARGS, "" },
	{ "OnTargetParried", (PyCFunction)ArcPyCreatureScript_OnTargetParried, METH_VARARGS, "" },
	{ "OnTargetDodged", (PyCFunction)ArcPyCreatureScript_OnTargetDodged, METH_VARARGS, "" },
	{ "OnTargetBlocked", (PyCFunction)ArcPyCreatureScript_OnTargetBlocked, METH_VARARGS, "" },
	{ "OnTargetCritHit", (PyCFunction)ArcPyCreatureScript_OnTargetCritHit, METH_VARARGS, "" },
	{ "OnTargetDied", (PyCFunction)ArcPyCreatureScript_OnTargetDied, METH_VARARGS, "" },
	{ "OnParried", (PyCFunction)ArcPyCreatureScript_OnParried, METH_VARARGS, "" },
	{ "OnDodged", (PyCFunction)ArcPyCreatureScript_OnDodged, METH_VARARGS, "" },
	{ "OnBlocked", (PyCFunction)ArcPyCreatureScript_OnBlocked, METH_VARARGS, "" },
	{ "OnCritHit", (PyCFunction)ArcPyCreatureScript_OnCritHit, METH_VARARGS, "" },
	{ "OnHit", (PyCFunction)ArcPyCreatureScript_OnHit, METH_VARARGS, "" },
	{ "OnDied", (PyCFunction)ArcPyCreatureScript_OnDied, METH_VARARGS, "" },
	{ "OnAssistTargetDied", (PyCFunction)ArcPyCreatureScript_OnAssistTargetDied, METH_VARARGS, "" },
	{ "OnFear", (PyCFunction)ArcPyCreatureScript_OnFear, METH_VARARGS, "" },
	{ "OnFlee", (PyCFunction)ArcPyCreatureScript_OnFlee, METH_VARARGS, "" },
	{ "OnCallForHelp", (PyCFunction)ArcPyCreatureScript_OnCallForHelp, METH_NOARGS, "" },
	{ "OnLoad", (PyCFunction)ArcPyCreatureScript_OnLoad, METH_NOARGS, "" },
	{ "OnDespawn", (PyCFunction)ArcPyCreatureScript_OnDespawn, METH_NOARGS, "" },
	{ "OnReachWP", (PyCFunction)ArcPyCreatureScript_OnReachWP, METH_VARARGS, "" },
	{ "OnLootTaken", (PyCFunction)ArcPyCreatureScript_OnLootTaken, METH_VARARGS, "" },
	{ "AIUpdate", (PyCFunction)ArcPyCreatureScript_AIUpdate, METH_VARARGS, "" },
	{ "OnEmote", (PyCFunction)ArcPyCreatureScript_OnEmote, METH_VARARGS, "" },	
	{ "OnEnterVehicle", (PyCFunction)ArcPyCreatureScript_OnEnterVehicle, METH_NOARGS, "" },
	{ "OnExitVehicle", (PyCFunction)ArcPyCreatureScript_OnExitVehicle, METH_NOARGS, "" },
	{ "OnFirstPassengerEntered", (PyCFunction)ArcPyCreatureScript_OnFirstPassengerEntered, METH_VARARGS, "" },
	{ "OnVehicleFull", (PyCFunction)ArcPyCreatureScript_OnVehicleFull, METH_NOARGS, "" },
	{ "OnLastPassengerLeft", (PyCFunction)ArcPyCreatureScript_OnLastPassengerLeft, METH_VARARGS, "" },
	{ "OnHealed", (PyCFunction)ArcPyCreatureScript_OnHealed, METH_VARARGS, "" },
	{ "OnApplyAura", (PyCFunction)ArcPyCreatureScript_OnApplyAura, METH_VARARGS, "" },
	{ "OnActivate", (PyCFunction)ArcPyCreatureScript_OnActivate, METH_VARARGS, "" },
	{ "OnDeactivate", (PyCFunction)ArcPyCreatureScript_OnDeactivate, METH_VARARGS, "" },
	{NULL}
};

static PyMemberDef ArcPyCreatureScript_members[] =
{
	{ "creature", T_OBJECT_EX, offsetof( ArcPyCreatureScript, creature ), 0, "Reference to the creature that is controlled by this script" },
	{ NULL }
};

PyTypeObject ArcPyCreatureScriptType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyCreatureScript",			// tp_name
	sizeof( ArcPyCreatureScript ),	// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyCreatureScript_dealloc,	// tp_dealloc
	0,								// tp_print
	0,								// tp_getattr
	0,								// tp_setattr
	0,								// tp_as_async
	0,								// tp_repr
	0,								// tp_as_number
	0,								// tp_as_sequence
	0,								// tp_as_mapping
	0,								// tp_hash
	0,								// tp_call
	0,								// tp_str
	0,								// tp_getattro
	0,								// tp_setattro
	0,								// tp_as_buffer
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	// tp_flags
	"ArcPyCreatureScript",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyCreatureScript_methods,	// tp_methods
	ArcPyCreatureScript_members,	// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyCreatureScript_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyCreatureScript_new,					// tp_new
};

int registerArcPyCreatureScript( PyObject *module )
{
	if( PyType_Ready( &ArcPyCreatureScriptType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyCreatureScriptType );
	PyModule_AddObject( module, "CreatureScript", (PyObject*)&ArcPyCreatureScriptType);

	return 0;
}
