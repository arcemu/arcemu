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

#ifndef IUPDATABLE_HPP
#define IUPDATABLE_HPP

namespace Arcemu{
	//////////////////////////////////////////////////////////////////////////
	//class IUpdatable
	// Abstract case class for every updatable class.
	//
	/////////////////////////////////////////////////////////////////////////
	class IUpdatable{
	public:
		//////////////////////////////////////////////////////////////////////
		//virtual void Update( unsigned long time_passed )
		// Updates the Object, allowing it's time-continous features to work
		//
		//Parameters
		// unsigned long time_passed - Seconds passed since last update
		//
		//Return Value
		// None
		//
		//
		/////////////////////////////////////////////////////////////////////
		virtual void Update(unsigned long time_passed) = 0;
	};
}

#endif