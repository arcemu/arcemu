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

#ifndef ARCPYPLAYER_H_
#define ARCPYPLAYER_H_

#include "ArcPyUnit.hpp"

class Player;

/// Arcemu Player type for Python
typedef struct
{
	ArcPyUnit unit;
	Player* playerPtr;
} ArcPyPlayer;

/// Creates an empty ArcPyPlayer object
ArcPyPlayer* createArcPyPlayer( Player* p );

/// Tells if the Python Object is actually an ArcPyPlayer object
bool isArcPyPlayer( PyObject *obj );

#endif
