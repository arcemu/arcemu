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

#include "ArcPyObject.hpp"
#include "ArcPyMapMgr.hpp"

static PyObject* ArcPyObject_new( PyTypeObject *type, PyObject *args, PyObject *keywords )
{
	ArcPyObject *self = (ArcPyObject*)type->tp_alloc( type, 0 );
	self->objectPtr = NULL;
	return (PyObject*)self;
}

static int ArcPyObject_init( ArcPyObject *self, PyObject *args, PyObject *keywords )
{
	self->objectPtr = NULL;
	return 0;
}

static void ArcPyObject_dealloc( ArcPyObject* self )
{
	Py_TYPE( self )->tp_free( (PyObject*)self );
}



/// getZoneId
///   Returns the Id of the Zone where the Object is located
///
/// Parameters
///   None
///
/// Return value
///   Returns the Id of the Zone where the Object is located
///
/// Example
///   phase = object.getZoneId()
///
static PyObject* ArcPyObject_getZoneId( ArcPyObject *self, PyObject *args )
{
	Object *obj = self->objectPtr;
	return PyLong_FromUnsignedLong( obj->GetZoneId() );
}

/// getMapId
///   Returns the Id of the map where the Object is located
///
/// Parameters
///   None
///
/// Return value
///   Returns the Id of the map where the Object is located
///
/// Example
///   phase = object.getMapId()
///
static PyObject* ArcPyObject_getMapId( ArcPyObject *self, PyObject *args )
{
	Object *obj = self->objectPtr;
	return PyLong_FromUnsignedLong( obj->GetMapId() );
}


/// getPositionX
///   Returns the X component of the Object's position
///
/// Parameters
///   None
///
/// Return value
///   Returns the X component of the Object's position
///
/// Example
///   x = object.getPositionX()
///
static PyObject* ArcPyObject_getPositionX( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetPositionX() );
}

/// getPositionY
///   Returns the Y component of the Object's position
///
/// Parameters
///   None
///
/// Return value
///   Returns the Y component of the Object's position
///
/// Example
///   y = object.getPositionY()
///
static PyObject* ArcPyObject_getPositionY( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetPositionY() );
}


/// getPositionZ
///   Returns the Z component of the Object's position
///
/// Parameters
///   None
///
/// Return value
///   Returns the Z component of the Object's position
///
/// Example
///   z = object.getPositionZ()
///
static PyObject* ArcPyObject_getPositionZ( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetPositionZ() );
}

/// getOrientation
///   Returns the orientation (0 - 2PI) of the Object
///
/// Parameters
///   None
///
/// Return value
///   Returns the orientation (0 - 2PI) of the Object
///
/// Example
///   o = object.getOrientation()
///
static PyObject* ArcPyObject_getOrientation( ArcPyObject *self, PyObject *args )
{
	Object *object = self->objectPtr;
	return PyFloat_FromDouble( object->GetOrientation() );
}

/// setOrientation
///   Sets the orientation (0 - 2PI) of the Object
///
/// Parameters
///   orientation - The orientation (0 - 2PI) of the Object
///
/// Return value
///   None
///
/// Example
///   object.setOrientation( 3.14 )
///
static PyObject* ArcPyObject_setOrientation( ArcPyObject *self, PyObject *args )
{
	float orientation;
	if( !PyArg_ParseTuple( args, "f", &orientation ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a float parameter" );
		return NULL;
	}

	Object *object = self->objectPtr;
	object->SetOrientation( orientation );
	Py_RETURN_NONE;
}


/// calcDistance
///   Calculates the distance between two objects
///
/// Parameters
///   object   -   The other object
///
/// Return value
///   Returns the distance between this object and the other one.
///
/// Example
///   d = obj.calcDistance( other )
///
static PyObject* ArcPyObject_calcDistance( ArcPyObject *self, PyObject *args )
{
	PyObject *po;
	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyObject parameter" );
		return NULL;
	}

	if( !isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	return PyFloat_FromDouble( self->objectPtr->CalcDistance( apo->objectPtr ) );
}

/// isInFront
///   Tells if the object is in front of another
///
/// Parameters
///   object   -   The other object
///
/// Return value
///   Returns True if the object is in front of the other.
///   Returns False otherwise
///
/// Example
///   if object.isInFront( otherObject ):
///       print( "Object is in front of the other" )
///
static PyObject* ArcPyObject_isInFront( ArcPyObject *self, PyObject *args )
{
	PyObject *po;
	if( !PyArg_ParseTuple( args, "O", &po ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyObject parameter" );
		return NULL;
	}

	if( !isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	bool isInFront = self->objectPtr->isInFront( apo->objectPtr );
	if( isInFront )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


/// isInArc
///   Tells if the object is our specified arc
///
/// Parameters
///   object   -   The other object
///   angle    -   The angle
///
/// Return value
///   Returns True if the object is in the specified arc
///   Returns False otherwise
///
/// Example
///   if object.isInArc( otherObject, 90 ):
///       print( "Object is in arc" )
///
static PyObject* ArcPyObject_isInArc( ArcPyObject *self, PyObject *args )
{
	PyObject *po;
	float angle;

	if( !PyArg_ParseTuple( args, "Of", &po, &angle ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyObject, and an angle parameter" );
		return NULL;
	}

	if( !isArcPyObject( po ) )
	{
		PyErr_SetString( PyExc_TypeError, "This method requires an ArcPyObject parameter" );
		return NULL;
	}

	ArcPyObject *apo = (ArcPyObject*)po;
	bool isInArc = self->objectPtr->isInArc( apo->objectPtr, angle );
	if( isInArc )
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}


/// getObjectsInRange
///   Returns a list of objects that are in range
///
/// Parameters
///   None
///
/// Return value
///   Returns a list of objects that are in range
///
/// Example
///   objects = object.getObjectsInRange()
///
static PyObject* ArcPyObject_getObjectsInRange( ArcPyObject *self, PyObject *args )
{
	Object *obj = self->objectPtr;

	std::set< Object* > &objects = obj->GetInRangeObjects();

	if( objects.size() == 0 )
	{
		Py_RETURN_NONE;
	}

	PyObject* list = PyList_New( objects.size() );
	int count = 0;

	std::set< Object* >::iterator itr = objects.begin();
	while( itr != objects.end() )
	{
		PyList_SetItem( list, count, (PyObject*)createArcPyObject( *itr ) );
		count++;
		++itr;
	}

	return list;
}


/// getMapMgr
///   Returns the MapMgr of this Object
///
/// Parameters
///   None
///
/// Return value
///   Returns the MapMgr of this Object
///
/// Example
///   mapMgr = object.getMapMgr
///
static PyObject* ArcPyObject_getMapMgr( ArcPyObject *self, PyObject *args )
{
	Object *obj = self->objectPtr;

	MapMgr *mgr = obj->GetMapMgr();
	if( mgr != NULL )
		return (PyObject*)createArcPyMapMgr( mgr );
	else
		Py_RETURN_NONE;
}

/// setFloatValue
///   Sets an float field's value
///
/// Parameters
///   index    -   The field's index
///   value    -   The value that should be set
///
/// Return value
///   None
///
/// Example
///   object.setFloatValue( 123, 0.0 )
///
static PyObject* ArcPyObject_setFloatValue( ArcPyObject *self, PyObject *args )
{
	uint32 index;
	float value;

	if( !PyArg_ParseTuple( args, "kf", &index, &value ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an index, and a value parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;

	obj->SetFloatValue( index, value );

	Py_RETURN_NONE;
}

/// getFloatValue
///   Returns a float field's value
///
/// Parameters
///   index    -   The field's index
///
/// Return value
///   Returns the float field's value
///
/// Example
///   value = object.getFloatValue( 123 )
///
static PyObject* ArcPyObject_getFloatValue( ArcPyObject *self, PyObject *args )
{
	uint32 index;

	if( !PyArg_ParseTuple( args, "k", &index ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an index parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;
	return PyFloat_FromDouble( obj->GetFloatValue( index ) );
}

/// setUInt32Value
///   Sets an uint32 field's value
///
/// Parameters
///   index    -   The field's index
///   value    -   The value that should be set
///
/// Return value
///   None
///
/// Example
///   object.setUInt32Value( 123, 0 )
///
static PyObject* ArcPyObject_setUInt32Value( ArcPyObject *self, PyObject *args )
{
	uint32 index;
	uint32 value;

	if( !PyArg_ParseTuple( args, "kk", &index, &value ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an index, and a value parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;

	obj->SetUInt32Value( index, value );

	Py_RETURN_NONE;
}


/// getUInt32Value
///   Returns an uint32 field's value
///
/// Parameters
///   index    -   The field's index
///
/// Return value
///   Returns the value of the field
///
/// Example
///   value = object.getUInt32Value( 123 )
///
static PyObject* ArcPyObject_getUInt32Value( ArcPyObject *self, PyObject *args )
{
	uint32 index;

	if( !PyArg_ParseTuple( args, "k", &index ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an index parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;
	return PyLong_FromUnsignedLong( obj->GetUInt32Value( index ) );
}

/// setUInt64Value
///   Sets an uint64 field's value
///
/// Parameters
///   index    -   The field's index
///   value    -   The value that should be set
///
/// Return value
///   None
///
/// Example
///   object.setUInt64Value( 123, 0 )
///
static PyObject* ArcPyObject_setUInt64Value( ArcPyObject *self, PyObject *args )
{
	uint32 index;
	uint64 value;

	if( !PyArg_ParseTuple( args, "kK", &index, &value ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an index, and a value parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;

	obj->SetUInt64Value( index, value );

	Py_RETURN_NONE;
}

/// getUInt64Value
///   Returns an uint64 field's value
///
/// Parameters
///   index    -   The field's index
///
/// Return value
///   Returns the field's value
///
/// Example
///   value = object.getUInt64Value( 123 )
///
static PyObject* ArcPyObject_getUInt64Value( ArcPyObject *self, PyObject *args )
{
	uint32 index;

	if( !PyArg_ParseTuple( args, "k", &index ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires an index, and a value parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;
	return PyLong_FromUnsignedLongLong( obj->GetUInt64Value( index ) );
}

/// setScale
///   Set the size scale of the Object
///
/// Parameters
///   scale   -   The size scale of the Object
///
/// Return value
///   None
///
/// Example
///   unit.setScale( 5.0 ) # Make the Object 5x of normal
///   unit.setScale( 0.25 ) # Make the Object 1/4 of normal
///   unit.setScale( 1.0 ) # Make the Object normal
///
static PyObject* ArcPyObject_setScale( ArcPyObject *self, PyObject *args )
{
	float scale = 1.0f;

	if( !PyArg_ParseTuple( args, "f", &scale ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a scale parameter" );
		return NULL;
	}

	Object *object = self->objectPtr;
	object->SetScale( scale );

	Py_RETURN_NONE;
}

/// setByteFlags
///   Sets flags to a byte of a field
///
/// Parameters
///   index   -   The index of the field
///   byte    -   The byte of the field
///   flag    -   The flag to be set
///
/// Return value
///   None
///
/// Example
///   object.setByteFlags( 123, 1, 123 )
///
static PyObject* ArcPyObject_setByteFlags( ArcPyObject *self, PyObject *args )
{
	uint32 index;
	uint8 byte;
	uint8 flags;

	if( !PyArg_ParseTuple( args, "kbb", &index, &byte, &flags ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires index, byte, flags parameters" );
		return NULL;
	}

	Object *obj = self->objectPtr;

	obj->SetByteFlag( (uint16)index, byte, flags );

	Py_RETURN_NONE;
}


/// setPhase
///   Sets the phase of the Object
///
/// Parameters
///   phase   -   The new phase of the object
///
/// Return value
///   None
///
/// Example
///   object.setPhase( 257 )
///
static PyObject* ArcPyObject_setPhase( ArcPyObject *self, PyObject *args )
{
	uint32 phase;

	if( !PyArg_ParseTuple( args, "k", &phase ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a phase parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;
	obj->Phase( PHASE_SET, phase );

	Py_RETURN_NONE;
}


/// addPhase
///   Adds a new phase to the phases of the Object
///
/// Parameters
///   phase   -   The new phase to add
///
/// Return value
///   None
///
/// Example
///   object.addPhase( 2 )
///
static PyObject* ArcPyObject_addPhase( ArcPyObject *self, PyObject *args )
{
	uint32 phase;

	if( !PyArg_ParseTuple( args, "k", &phase ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a phase parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;
	obj->Phase( PHASE_ADD, phase );

	Py_RETURN_NONE;
}

/// removePhase
///   Removes a phase from the phases of the Object
///
/// Parameters
///   phase   -   The phase to remove
///
/// Return value
///   None
///
/// Example
///   object.removePhase( 2 )
///
static PyObject* ArcPyObject_removePhase( ArcPyObject *self, PyObject *args )
{
	uint32 phase;

	if( !PyArg_ParseTuple( args, "k", &phase ) )
	{
		PyErr_SetString( PyExc_ValueError, "This method requires a phase parameter" );
		return NULL;
	}

	Object *obj = self->objectPtr;
	obj->Phase( PHASE_DEL, phase );

	Py_RETURN_NONE;
}

/// getPhase
///   Returns the phases of the Object
///
/// Parameters
///   None
///
/// Return value
///   Returns the phases of the Object
///
/// Example
///   phase = object.getPhase()
///
static PyObject* ArcPyObject_getPhase( ArcPyObject *self, PyObject *args )
{
	Object *obj = self->objectPtr;
	return PyLong_FromUnsignedLong( obj->GetPhase() );
}

static PyMethodDef ArcPyObject_methods[] = 
{
	{ "getZoneId", (PyCFunction)ArcPyObject_getZoneId, METH_NOARGS, "Returns the Zone Id of the Object" },
	{ "getMapId", (PyCFunction)ArcPyObject_getMapId, METH_NOARGS, "Returns the Map Id of the Object" },
	{ "getPositionX", (PyCFunction)ArcPyObject_getPositionX, METH_VARARGS, "Returns the X coordinate of the Object" },
	{ "getPositionY", (PyCFunction)ArcPyObject_getPositionY, METH_VARARGS, "Returns the Y coordinate of the Object" },
	{ "getPositionZ", (PyCFunction)ArcPyObject_getPositionZ, METH_VARARGS, "Returns the Z coordinate of the Object" },
	{ "getOrientation", (PyCFunction)ArcPyObject_getOrientation, METH_NOARGS, "Returns the orientation (0-2PI) of the Object" },	
	{ "setOrientation", (PyCFunction)ArcPyObject_setOrientation, METH_VARARGS, "Sets the orientation (0-2PI) of the Object" },
	{ "calcDistance", (PyCFunction)ArcPyObject_calcDistance, METH_VARARGS, "Calculates the distance between two objects" },
	{ "isInFront", (PyCFunction)ArcPyObject_isInFront, METH_VARARGS, "Tells if the Object is in front of another" },
	{ "isInArc", (PyCFunction)ArcPyObject_isInArc, METH_VARARGS, "Tells if the Object is in the specified arc" },
	{ "getObjectsInRange", (PyCFunction)ArcPyObject_getObjectsInRange, METH_NOARGS, "Returns the objects in range of this object" },
	{ "getMapMgr", (PyCFunction)ArcPyObject_getMapMgr, METH_NOARGS, "Returns the MapMgr of the Object" },
	{ "setFloatValue", (PyCFunction)ArcPyObject_setFloatValue, METH_VARARGS, "Sets the value of a float field" },
	{ "getFloatValue", (PyCFunction)ArcPyObject_getFloatValue, METH_VARARGS, "Returns the value of a float field" },
	{ "setUInt32Value", (PyCFunction)ArcPyObject_setUInt32Value, METH_VARARGS, "Sets the value of an UInt32 field" },
	{ "getUInt32Value", (PyCFunction)ArcPyObject_getUInt32Value, METH_VARARGS, "Returns the value of an uint32 field" },
	{ "setUInt64Value", (PyCFunction)ArcPyObject_setUInt64Value, METH_VARARGS, "Sets the value of an UInt64 field" },
	{ "getUInt64Value", (PyCFunction)ArcPyObject_getUInt64Value, METH_VARARGS, "Returns the value of an UInt64 field" },
	{ "setScale", (PyCFunction)ArcPyObject_setScale, METH_VARARGS, "Sets the size scale of the Object" },
	{ "setByteFlags", (PyCFunction)ArcPyObject_setByteFlags, METH_VARARGS, "Sets flags to a byte of a field" },
	{ "setPhase", (PyCFunction)ArcPyObject_setPhase, METH_VARARGS, "Sets the phase of the object" },
	{ "addPhase", (PyCFunction)ArcPyObject_addPhase, METH_VARARGS, "Adds a new phase to the Object" },
	{ "removePhase", (PyCFunction)ArcPyObject_removePhase, METH_VARARGS, "Removes a phase from the Object" },
	{ "getPhase", (PyCFunction)ArcPyObject_getPhase, METH_VARARGS, "Retruns the phases of the object" },
	{NULL}
};

PyTypeObject ArcPyObjectType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	
	"ArcPyObject",					// tp_name
	sizeof( ArcPyObject ),			// tp_basicsize
	0,								// tp_itemsize
	(destructor)ArcPyObject_dealloc,	// tp_dealloc
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
	"Arcemu Object",					// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	ArcPyObject_methods,				// tp_methods
	0,								// tp_members
	0,								// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)ArcPyObject_init,		// tp_tp_init
	0,								// tp_alloc
	ArcPyObject_new,					// tp_new
};

int registerArcPyObject( PyObject *module )
{
	if( PyType_Ready( &ArcPyObjectType ) < 0 )
	{
		return -1;
	}
	
	Py_INCREF( &ArcPyObjectType );
	PyModule_AddObject( module, "Object", (PyObject*)&ArcPyObjectType);

	return 0;
}

ArcPyObject* createArcPyObject( Object *o )
{
	PyTypeObject *type = &ArcPyObjectType;
	ArcPyObject* object = (ArcPyObject*)type->tp_alloc( type, 0 );
	object->objectPtr = o;
	return object;
}

bool isArcPyObject( PyObject *obj )
{
	const char *typeName = Py_TYPE( obj )->tp_name;

	if( strcmp( typeName, "ArcPyObject" ) == 0 ||
		strcmp( typeName, "ArcPyItem" ) == 0 ||
		strcmp( typeName, "ArcPyGameObject" ) == 0 ||
		strcmp( typeName, "ArcPyUnit"  ) == 0 ||
		strcmp( typeName, "ArcPyCreature" ) == 0 ||
		strcmp( typeName, "ArcPyPlayer" ) == 0 )
	{
		return true;
	}

	return false;
}
