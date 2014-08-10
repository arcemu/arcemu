/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#ifndef GO_LOOTABLE_HPP
#define GO_LOOTABLE_HPP
namespace Arcemu{
	/////////////////////////////////////////////////////////////////////////
	//class GO_Lootable
	// Abstract Base Class for lootable GameObjects
	// ( fishing node, fishing hole, and chests )
	//
	/////////////////////////////////////////////////////////////////////////
	class GO_Lootable : public GameObject{
	public:
		GO_Lootable() : GameObject(){}
		GO_Lootable(uint64 GUID) : GameObject(GUID){}
		~GO_Lootable(){}
		///////////////////////////////////////////////////////////////////
		//virtual bool HasLoot()
		// Tells if the GO has loot
		//
		//Parameters
		// None
		//
		//Return Value
		// Returns true if it has loot.
		// Returns false otherwise.
		//
		//
		///////////////////////////////////////////////////////////////////
		virtual bool HasLoot() = 0;
		Loot loot;
	};
}
#endif