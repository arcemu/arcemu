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

#ifndef PYTHON_TUPLE_H
#define PYTHON_TUPLE_H

#include "python/PythonObject.hpp"

/// A Python Tuple object
///   A Tuple is an object that can contain exactly N arbitrary typed values
///
class PythonTuple
{
public:
	///
	/// Initializes this PythonTuple object
	/// 
	/// Parameters
	///   size   -  The size (number of components) of this PythonTuple object
	///
	/// Return value
	///   None
	///
	PythonTuple( unsigned long size );

	~PythonTuple();

	/// Set a long value at the specified index
	void setItem( unsigned long idx, long value );

	/// Set a Python object value at the specified index
	void setItem( unsigned long idx, PythonObject &object );

	/// Set a C-String value at the specified index
	void setItem( unsigned long idx, const char* value );
	
	/// Set a boolean value at the specified index
	void setItemBool( unsigned long idx, bool value );

	/// Set a None value at the specified index
	void setItemNone( unsigned long idx );

	/// Returns the size (number of components) of this Tuple
	unsigned long getSize(){ return size; }

	/// Returns the underlying Python object
	PyObject* getObject() const{ return obj; }

private:
	/// The wrapped Python Tuple object
	PyObject *obj;

	/// The size (number of components) of this tuple
	unsigned long size;
};

#endif
