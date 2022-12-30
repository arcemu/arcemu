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
#include "PythonTuple.hpp"

PythonTuple::PythonTuple( unsigned long size )
{
	this->obj = PyTuple_New( size );
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

