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
 
#ifndef PYTHONOBJECT_H_
#define PYTHONOBJECT_H_

/// Represents / Wraps a Python Object, hiding Python details
class PythonObject
{
public:
	///
	/// Initializes this PythonObject instance
	///
	/// Parameters
	///   obj   -   The PyObject that will be wrapped
	///
	/// Return value
	///   None
	///
	PythonObject( PyObject* obj );

	~PythonObject();

	/// Increase the reference count of the object
	void incref();

	/// Decrease the reference count of the object.
	/// When it reaches zero the object is destroyed
	void decref();

	/// Is there an object in this wrapper class?
	bool isEmpty()
	{
		if( obj == NULL )
			return true;
		else
			return false;
	}

	/// Get the wrapped Python object
	PyObject* getObject() const{ return obj; }

private:
	/// The wrapped PyObject
	PyObject* obj;
};

#endif
