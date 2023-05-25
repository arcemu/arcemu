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

#include <Python.h>
#include <cstdio>

#include "StdAfx.h"

#include "ArcPyAura.hpp"
#include "ArcPyUnit.hpp"

static PyObject* ArcPyAura_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyAura *self = (ArcPyAura*)type->tp_alloc( type, 0 );
	self->auraPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyAura_init( ArcPyAura *self, PyObject *args, PyObject *keywords )
{
	self->auraPtr = NULL;
	return 0;
}

static void ArcPyAura_dealloc( ArcPyAura* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}

/// getSpellName
///   Returns the name of the spell that created this Aura
///
/// Parameters:
///   No parameters
///
/// Example:
///   name = a.getSpellName()
///
static PyObject* ArcPyAura_getSpellName( ArcPyAura *self, PyObject *args )
{
	Aura* aura = self->auraPtr;
	const char *spellName = aura->GetSpellProto()->Name;
	if( spellName == NULL )
		spellName = "NO NAME";

	PyObject *name = PyUnicode_FromString( spellName );	
	return name;
}


/// getSpellId
///   Returns the Id of the spell that created this Aura
///
/// Parameters:
///   No parameters
///
/// Return value:
///   Returns the Id of the spell that created this Aura
///
/// Example:
///   id = a.getSpellId()
///
static PyObject* ArcPyAura_getSpellId( ArcPyAura* self, PyObject* args )
{
	Aura* aura = self->auraPtr;
	PyObject *id = PyLong_FromLong( aura->GetSpellProto()->Id );
	return id;
}


/// getAuraSlot
///   Returns the Id of the Aura slot this Aura is in
///
/// Parameters:
///   No parameters
///
/// Return value:
///   Returns the Id of the Aura slot this Aura is in
///
/// Example:
///   slot = a.getAuraSlot()
///
static PyObject* ArcPyAura_getAuraSlot( ArcPyAura* self, PyObject* args )
{
	Aura* aura = self->auraPtr;
	PyObject *id = PyLong_FromLong( aura->GetAuraSlot() );
	return id;
}


/// getCasterGuid
///   Returns the GUID of the Aura's caster
///
/// Parameters:
///   No parameters
///
/// Return value:
///   Returns the Guid of the Aura's caster
///
/// Example:
///   guid = a.getCasterGuid()
///
static PyObject* ArcPyAura_getCasterGuid( ArcPyAura* self, PyObject* args )
{
	Aura *aura = self->auraPtr;
	PyObject *guid = PyLong_FromUnsignedLongLong( aura->GetCasterGUID() );
	return guid;
}


/// remove
///   Removes the Aura
///
/// Parameters:
///   No parameters
///
/// Return value:
///   None
///
/// Example:
///   a.remove()
///
static PyObject* ArcPyAura_remove( ArcPyAura* self, PyObject* args )
{
	Aura *aura = self->auraPtr;
	aura->Remove();

	Py_RETURN_NONE;
}

/// getTarget
///   Retrieves the target of the Aura
///
/// Parameters:
///   No parameters
///
/// Return value:
///   Returns target of the Aura as an ArcPyUnit
///
/// Example:
///   target = aura.getTarget()
///
static PyObject* ArcPyAura_getTarget( ArcPyAura* self, PyObject* args )
{
	Aura *aura = self->auraPtr;
	Unit *target = aura->GetTarget();

	return (PyObject*)createArcPyUnit( target );
}

static PyMethodDef ArcPyAura_methods[] = 
{
	{ "getSpellName", (PyCFunction)ArcPyAura_getSpellName, METH_NOARGS, "Returns the name of the Spell that applied this Aura" },
	{ "getSpellId", (PyCFunction)ArcPyAura_getSpellId, METH_NOARGS, "Returns the Id of the Spell that applied this Aura" },	
	{ "getAuraSlot", (PyCFunction)ArcPyAura_getAuraSlot, METH_NOARGS, "Returns the slot of this Aura" },
	{ "getCasterGuid", (PyCFunction)ArcPyAura_getCasterGuid, METH_NOARGS, "Returns the Aura caster's GUID" },
	{ "remove", (PyCFunction)ArcPyAura_remove, METH_NOARGS, "Removes the Aura" },
	{ "getTarget", (PyCFunction)ArcPyAura_getTarget, METH_NOARGS, "Retrieves the target of the Aura" },
	{NULL}
};

static PyTypeObject ArcPyAuraType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyAura",					// tp_name
	sizeof( ArcPyAura ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyAura_dealloc,	// tp_dealloc
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
	Py_TPFLAGS_DEFAULT,				// tp_flags
	"Arcemu Aura",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyAura_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyAura_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyAura_new,				// tp_new
};

int registerArcPyAura( PyObject *module )
{
	ArcPyAuraType.tp_new = ArcPyAura_new;

	if( PyType_Ready( &ArcPyAuraType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPyAuraType );
	PyModule_AddObject( module, "Aura", (PyObject*)&ArcPyAuraType);

	return 0;
}

ArcPyAura* createArcPyAura()
{
	PyTypeObject *type = &ArcPyAuraType;
	ArcPyAura* player = (ArcPyAura*)type->tp_alloc( type, 0 );
	return player;
}
