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

#ifndef GO_DOOR_HPP
#define GO_DOOR_HPP

namespace Arcemu{
	////////////////////////////////////////////////////////////////////////
	//class GO_Door
	// Implements Type 0 (DOOR) GameObjects
	//
	////////////////////////////////////////////////////////////////////////
	class GO_Door : public GameObject{
	public:
		GO_Door();


		GO_Door(uint64 GUID);


		~GO_Door();


		void InitAI();


		void Use(uint64 GUID);


		void Open();


		void Close();


		///////////////////////////////////////////////////////////////////
		//void SpecialOpen()
		// Opens the door with a special way. Like an explosion.
		//
		//Parameters
		// None
		//
		//Return Value
		// None
		//
		///////////////////////////////////////////////////////////////////
		void SpecialOpen();
	};
}

#endif