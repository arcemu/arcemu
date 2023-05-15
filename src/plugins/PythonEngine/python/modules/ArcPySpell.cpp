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

#include <Python.h>
#include <cstdio>

#include "StdAfx.h"

#include "ArcPySpell.hpp"
#include "ArcPyObject.hpp"
#include "ArcPyUnit.hpp"
#include "ArcPyPlayer.hpp"

static PyObject* ArcPySpell_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPySpell *self = (ArcPySpell*)type->tp_alloc( type, 0 );
	self->spellPtr = NULL;
	return (PyObject*)self;
}

static int ArcPySpell_init( ArcPySpell *self, PyObject *args, PyObject *keywords )
{
	self->spellPtr = NULL;
	return 0;
}

static void ArcPySpell_dealloc( ArcPySpell* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}


/// getName
///   Returns the name of this Spell
///
/// Parameters
///   None
///
/// Return value
///   Returns a String that contains the name of this Spell
///
/// Example
///   name = spell.getName()
///
static PyObject* ArcPySpell_getName( ArcPySpell *self, PyObject *args )
{
	Spell* spell = self->spellPtr;
	const char *spellName = spell->GetProto()->Name;
	if( spellName == NULL )
		spellName = "NO NAME";

	PyObject *name = PyUnicode_FromString( spellName );	
	return name;
}

/// getCaster
///   Returns the object that cast this Spell
///
/// Parameters
///   None
///
/// Return value
///   Returns an ArcPyObject object that is the caster of this spell
///   Returns None otherwise.
///
/// Example
///   caster = spell.getCaster()
///
static PyObject* ArcPySpell_getCaster( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	Object *caster = spell->m_caster;

	ArcPyObject *apo = createArcPyObject( caster );
	return (PyObject*)apo;
}

/// getPlayerCaster
///   Returns the player who cast this Spell
///
/// Parameters
///   None
///
/// Return value
///   Returns an ArcPyPlayer object if a Player cast this spell.
///   Returns None otherwise.
///
/// Example
///   playerCaster = spell.getPlayerCaster()
///
static PyObject* ArcPySpell_getPlayerCaster( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	Player *playerCaster = spell->p_caster;
	if( playerCaster == NULL )
		Py_RETURN_NONE;

	ArcPyPlayer *app = createArcPyPlayer( playerCaster );
	return (PyObject*)app;
}

/// getTargetDestinationX
///   Returns the X coordinate of the Spell's target destination
///
/// Parameters
///   None
///
/// Return value
///   Returns the X coordinate of the Spell's target destination
///
/// Example
///   destX = spell.getTargetDestinationX()
///
static PyObject* ArcPySpell_getTargetDestinationX( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	return PyFloat_FromDouble( spell->m_targets.m_destX );
}

/// getTargetDestinationY
///   Returns the Y coordinate of the Spell's target destination
///
/// Parameters
///   None
///
/// Return value
///   Returns the Y coordinate of the Spell's target destination
///
/// Example
///   destY = spell.getTargetDestinationY()
///
static PyObject* ArcPySpell_getTargetDestinationY( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	return PyFloat_FromDouble( spell->m_targets.m_destY );
}

/// getTargetDestinationZ
///   Returns the Z coordinate of the Spell's target destination
///
/// Parameters
///   None
///
/// Return value
///   Returns the Z coordinate of the Spell's target destination
///
/// Example
///   destZ = spell.getTargetDestinationZ()
///
static PyObject* ArcPySpell_getTargetDestinationZ( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	return PyFloat_FromDouble( spell->m_targets.m_destZ );
}

/// getUnitTarget()
///   Returns the Unit target of the Spell
///
/// Parameters
///   None
///
/// Return value
///   Returns a Unit if the target is a Unit.
///   Returns None otherwise
///
/// Example
///   unit = spell.getUnitTarget()
///
static PyObject* ArcPySpell_getUnitTarget( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	Unit *unitTarget = spell->GetUnitTarget();

	if( unitTarget != NULL )
		return (PyObject*)createArcPyUnit( unitTarget );
	else
		Py_RETURN_NONE;
}


/// cancel()
///   Cancels the spell
///
/// Parameters
///   None
///
/// Return value
///   None
///
/// Example
///   spell.cancel()
///
static PyObject* ArcPySpell_cancel( ArcPySpell *self, PyObject *args )
{
	Spell *spell = self->spellPtr;
	spell->cancel();

	Py_RETURN_NONE;
}

static PyMethodDef ArcPySpell_methods[] = 
{
	{ "getName", (PyCFunction)ArcPySpell_getName, METH_NOARGS, "Returns the name of the Spell" },
	{ "getCaster", (PyCFunction)ArcPySpell_getCaster, METH_NOARGS, "Returns the Object that cast this spell." },
	{ "getPlayerCaster", (PyCFunction)ArcPySpell_getPlayerCaster, METH_NOARGS, "Returns the Player who cast this spell, or return None." },
	{ "getUnitTarget", (PyCFunction)ArcPySpell_getUnitTarget, METH_NOARGS, "Returns the Unit target of the Spell" },
	{ "getTargetDestinationX", (PyCFunction)ArcPySpell_getTargetDestinationX, METH_NOARGS, "Returns the X coordinate of the Spell's target destination" },
	{ "getTargetDestinationY", (PyCFunction)ArcPySpell_getTargetDestinationY, METH_NOARGS, "Returns the Y coordinate of the Spell's target destination" },
	{ "getTargetDestinationZ", (PyCFunction)ArcPySpell_getTargetDestinationZ, METH_NOARGS, "Returns the Z coordinate of the Spell's target destination" },
	{ "cancel", (PyCFunction)ArcPySpell_cancel, METH_NOARGS, "Cancels the spell" },
	{NULL}
};

static PyTypeObject ArcPySpellType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPySpell",					// tp_name
	sizeof( ArcPySpell ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPySpell_dealloc,	// tp_dealloc
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
	"Arcemu Spell",				// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPySpell_methods,			// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPySpell_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPySpell_new,				// tp_new
};

int registerArcPySpell( PyObject *module )
{
	ArcPySpellType.tp_new = ArcPySpell_new;

	if( PyType_Ready( &ArcPySpellType ) < 0 )
	{
		return -1;
	}

	Py_INCREF( &ArcPySpellType );
	PyModule_AddObject( module, "Spell", (PyObject*)&ArcPySpellType);

	return 0;
}

ArcPySpell* createArcPySpell()
{
	PyTypeObject *type = &ArcPySpellType;
	ArcPySpell* spell = (ArcPySpell*)type->tp_alloc( type, 0 );
	return spell;
}

