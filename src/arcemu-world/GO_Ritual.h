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
#ifndef GO_RITUAL_HPP
#define GO_RITUAL_HPP
namespace Arcemu{
	///////////////////////////////////////////////////////////
	//class GO_Ritual
	// Class that implements Type 18 (RITUAL) GameObjects
	//
	///////////////////////////////////////////////////////////
	class GO_Ritual : public GameObject{
	public:
		GO_Ritual();
		GO_Ritual(uint64 GUID);
		~GO_Ritual();
		void InitAI();
		uint32 *m_ritualmembers;
		uint32 m_ritualcaster;
		uint32 m_ritualtarget;
		uint16 m_ritualspell;
	};
}
#endif