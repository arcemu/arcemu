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

#ifndef ARCPYTHON_HPP_
#define ARCPYTHON_HPP_

#include "python/Python.hpp"

/// Python wrapper with Arcemu extensions
class ArcPython : public Python
{
public:
	ArcPython();
	~ArcPython();

	/// Returns Arcemu's Python global lock
	static Mutex& getLock();

private:
	/// Global lock that protects the Python interpreter
	/// Must be locked every time a thread accesses the interpreter
	static Mutex lock;
};

#endif
