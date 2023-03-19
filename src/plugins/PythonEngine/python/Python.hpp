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

#ifndef PYTHON_WRAPPER_H_
#define PYTHON_WRAPPER_H_

/// Python C++ wrapper class
class Python
{
public:
	/// Initializes the Python interpreter
	Python();

	/// Deinitializes the Python interpreter
	~Python();

	///
	/// Print the last Python exception's error message if there' any
	///
	/// Parameters
	///   None
	///
	/// Return value
	///   None
	///
	static void printError();

	///
	/// Sets the Python standard library path
	///
	/// Parameters
	///   path   -  The path to the directory which contains the Python standard library files
	///
	/// Return value
	///   None
	///
	static void setPath( const wchar_t *path );

	///
	/// Execute a Python script. Output will go to the console.
	///
	/// Parameters
	///   fileName   -  The path to a Python file
	///
	/// Return value
	///   Returns 0 if there was no problem.
	///   Returns -1 otherwise.
	///
	int runSimpleFile( const char *fileName );
};

#endif
