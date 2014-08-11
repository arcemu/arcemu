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

#ifndef GO_BUTTON_HPP
#define GO_BUTTON_HPP

namespace Arcemu{
	////////////////////////////////////////////////////////////////////////
	//class GO_Button
	// Implements Type 1 (BUTTON) GameObjects
	//
	////////////////////////////////////////////////////////////////////////
	class GO_Button : public GameObject{
	public:
		GO_Button();


		GO_Button(uint64 GUID);


		~GO_Button();


		void InitAI();


		void Open();


		void Close();
	};

}

#endif