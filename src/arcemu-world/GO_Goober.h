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

#ifndef GO_GOOBER_HPP
#define GO_GOOBER_HPP

namespace Arcemu{
	////////////////////////////////////////////////////////////////////
	//clas GO_Goober
	// Class implementing Type 10 (GOOBER) GameObjects
	//
	////////////////////////////////////////////////////////////////////
	class GO_Goober : public GameObject{
	public:
		GO_Goober();


		GO_Goober(uint64 GUID);


		~GO_Goober();


		void InitAI();


		void Use(uint64 GUID);


		void Open();


		void Close();

	private:
		SpellEntry *spell;

	};
}

#endif