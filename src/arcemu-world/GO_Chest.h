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

#ifndef GO_CHEST_HPP
#define GO_CHEST_HPP

namespace Arcemu{
	///////////////////////////////////////////////////////////////////
	//class GO_Chest
	// class implementing type 3 (Chest) GameObjects
	//
	//////////////////////////////////////////////////////////////////
	class GO_Chest : public GO_Lootable{

	public:
		GO_Chest();


		GO_Chest(uint64 GUID);


		~GO_Chest();


		bool IsLootable(){ return true; }


		bool HasLoot();


		void Open();


		void Close();

	};
}
#endif