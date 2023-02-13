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
#include "PythonValue.hpp"

#include <cstring>

PythonValue::PythonValue( PyObject *obj ) :
PythonObject( obj )
{
}

PythonValue::PythonValue( const PythonValue &other ) :
PythonObject( other.getObject() )
{
}

PythonValue::~PythonValue()
{
}

bool PythonValue::isNone()
{
	
	if( getObject() == &_Py_NoneStruct )
		return true;
	else
		return false;
}

bool PythonValue::isBool()
{
	if( PyBool_Check( getObject() ) )
		return true;
	else
		return false;
}

bool PythonValue::getBoolValue()
{
	/// Python has only one instance of a True and a False value
	/// Therefore we can compare to one of them here
	PyObject* tr = Py_True;

	if( getObject() == tr )
		return true;
	else
		return false;
}
