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

#ifndef ARCPYCREATURE_H_
#define ARCPYCREATURE_H_

#include "ArcPyUnit.hpp"

class Creature;

/// Arcemu Creature type for Python
typedef struct
{
	ArcPyUnit unit;
	Creature* creaturePtr;
} ArcPyCreature;

#define ARCPYCREATURE_TYPE_NAME "ArcPyCreature"

/// Creates an empty ArcPyCreature object
ArcPyCreature* createArcPyCreature( Creature* creature );

/// Tells if the Python Object is actually an ArcPyCreature object
bool isArcPyPlayer( PyObject *obj );

#endif
