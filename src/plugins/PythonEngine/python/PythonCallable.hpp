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
 
 #ifndef PYTHONCALL_H
 #define PYTHONCALL_H

#include "python/PythonTuple.hpp"
#include "python/PythonValue.hpp"
 
/// Represents a callable Python object (like a function)
 class PythonCallable : public PythonObject
 {
 public:
	 ///
	 /// Initialize with a function reference loaded from scripts
	 ///
	 /// Parameters
	 ///   callable  -  A Python callable (function)
	 /// 
	 /// Return value
	 ///  None
	 ///
	 PythonCallable( void* callable );

	 ~PythonCallable();
	 
	 ///
	 /// Call this callable, with a Tuple as argument. Returns a value object.
	 ///
	 /// Parameters
	 ///   args   -   A PythonTuple that contains the arguments for this Callable
	 ///
	 /// Return value
	 ///   Returns PythonValue object that contains the return value of the called function.
	 ///
	 PythonValue call( PythonTuple &args );

	 ///
	 /// Call this callable, with a Tuple argument.
	 /// Used as a convenience method for when a return value is not expected.
	 ///
	 /// Parameters
	 ///   args   -   A PythonTuple that contains the arguments for this Callable
	 ///
	 /// Return value
	 ///   None
	 ///
	 void callNoReturn( PythonTuple &args );
 };
 
 #endif
 