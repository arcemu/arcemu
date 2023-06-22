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

#ifndef ARCEMU_OBJECTLOCATOR_H_
#define ARCEMU_OBJECTLOCATOR_H_

class Object;
class Unit;

/// Locates Objects around the parent Object
class SERVER_DECL ObjectLocator
{
public:
	ObjectLocator( Object *object )
	{
		this->object = object;
	}

	//////////////////////////////////////////////////////////////////////////
	// findClosestFriendly()
	//   Finds the closest friendly Unit
	//
	// Parameters
	//   None
	//
	// Return value
	//   Returns the closest friendly Unit that is in range.
	//   Returns NULL if there's no such Unit in range.
	//
	//
	//////////////////////////////////////////////////////////////////////////
	Unit* findClosestFriendly();

private:
	Object *object;
};

#endif
