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

#ifndef ARCPYUNIT_H_
#define ARCPYUNIT_H_

#include "ArcPyObject.hpp"

extern PyTypeObject ArcPyUnitType;

class Unit;

/// Arcemu Unit type for Python
typedef struct
{
	ArcPyObject Object;
	Unit* unitPtr;
} ArcPyUnit;

/// Create an empty ArcPyUnit object
ArcPyUnit* createArcPyUnit( Unit* u );

/// Tells if this PyObject is an ArcPyUnit or something that can be casted to ArcPyUnit
bool isArcPyUnit( PyObject *obj );

#endif
