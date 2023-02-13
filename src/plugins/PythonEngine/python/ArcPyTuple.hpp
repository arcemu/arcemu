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

#ifndef ARCPY_TUPLE_H
#define ARCPY_TUPLE_H

#include "python/PythonTuple.hpp"

/// A Python Tuple with Arcemu value support
class ArcPyTuple : public PythonTuple
{
public:
	ArcPyTuple( unsigned long size );
	~ArcPyTuple();

	/// Set an Aura value at the specified index
	void setItemAura( unsigned long idx, Aura* aura );

	/// Set a GameObject value at the specified index
	void setItemGameObject( unsigned long idx, GameObject* gameObject );

	/// Set a Guild value at the specified index
	void setItemGuild( unsigned long idx, Guild* guild );

	/// Set an Item value at the specified index
	void setItemItem( unsigned long idx, Item* item );

	/// Set a Player value at the specified index
	void setItemPlayer( unsigned long idx, Player* player );

	/// Set a Quest value at the specified index
	void setItemQuest( unsigned long idx, Quest* quest );

	/// Set a Spell value at the specified index
	void setItemSpell( unsigned long idx, Spell* spell );

	/// Set a Unit value at the specified index
	void setItemUnit( unsigned long idx, Unit* unit );

	/// Set a WorldSession value at the specified index
	void setItemWorldSession( unsigned long idx, WorldSession* worldSession );	
};

#endif
