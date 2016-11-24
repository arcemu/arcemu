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

namespace Arcemu{
	/////////////////////////////////////////////////////////////////
	//class CObjectFactory
	// Factory class that instantiates and destroys all Objects
	//
	//
	/////////////////////////////////////////////////////////////////
	class CObjectFactory{
	public:
		CObjectFactory(){}
		~CObjectFactory(){}

		////////////////////////////////////////////////////////////
		//GameObject* CreateGameObject( uint32 Id, uint32 LowGUID )
		// Creates an instance of the GameObject class.
		//
		//Parameters
		// uint32 Id - Entry if of the GameObject
		// uint32 LowGUID - Unique ID of this instance
		//
		//Return Value
		// None
		//
		//
		//////////////////////////////////////////////////////////
		GameObject* CreateGameObject(uint32 Id, uint32 LowGUID);



		/////////////////////////////////////////////////////////
		//void DisposeOf( Object *o )
		// Disposes of the created Object
		//
		//Parameters
		// None
		//
		//Return Value
		// None
		//
		//
		////////////////////////////////////////////////////////
		void DisposeOf(Object *o);

	};
}