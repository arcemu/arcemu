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
 
#ifndef PYTHONVALUE_H_
#define PYTHONVALUE_H_

#include "python/PythonObject.hpp"

/// Represents a Python value object
class PythonValue : public PythonObject
{
public:
	PythonValue( PyObject *obj );
	PythonValue( const PythonValue &other );
	~PythonValue();

	/// Is this a None object?
	bool isNone();

	/// Is this a Boolean object?
	bool isBool();

	/// Retrieves the boolean value of the object
	bool getBoolValue();
};

#endif
