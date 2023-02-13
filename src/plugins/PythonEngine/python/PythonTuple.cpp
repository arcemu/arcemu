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

#include "PythonObject.hpp"
#include "PythonTuple.hpp"

PythonTuple::PythonTuple( unsigned long size )
{
	this->obj = PyTuple_New( size );
	size = size;
}

PythonTuple::~PythonTuple()
{
	Py_DECREF( obj );
	obj = NULL;
	size = 0;
}


void PythonTuple::setItem( unsigned long idx, long value )
{
	PyTuple_SetItem( obj, idx, PyLong_FromLong( value ) );
}

void PythonTuple::setItem( unsigned long idx, PythonObject &pyobj )
{
	PyTuple_SetItem( obj, idx, pyobj.getObject() );
}

void PythonTuple::setItem( unsigned long idx, const char* value )
{
	PyObject *string = Py_BuildValue( "s", value );
	PyTuple_SetItem( obj, idx, string );
}

void PythonTuple::setItemBool( unsigned long idx, bool boolValue )
{
	PyObject *value = NULL;
	if( boolValue )
		value = Py_True;
	else
		value = Py_False;
	
	Py_IncRef( value );

	PyTuple_SetItem( obj, idx, value );
}

void PythonTuple::setItemNone( unsigned long idx )
{
	Py_INCREF( Py_None );
	PyTuple_SetItem( obj, idx, Py_None );
}
