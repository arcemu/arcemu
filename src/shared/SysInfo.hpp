/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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


#ifndef ARCEMU_SYSINFO_HPP
#define ARCEMU_SYSINFO_HPP

namespace Arcemu{
	////////////////////////////////////////////////////////
	//class SysInfo
	//  Class with static methods capable of retrieving
	//  some basic system information
	//
	///////////////////////////////////////////////////////
	class SysInfo{
	public:
		////////////////////////////////////////////////////////////////////
		//static int GetCPUCount()
		//  Tells the number of CPUs installed in the system
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the number of CPUs
		//
		//
		////////////////////////////////////////////////////////////////////
		static long GetCPUCount();


		////////////////////////////////////////////////////////////////////
		//static unsigned long long GetCPUUsage()
		//  Tells the CPU time ( kernel and user )spent on executing
		//  the caller process.
		//
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the used CPU time in microseconds.
		//
		//
		////////////////////////////////////////////////////////////////////
		static unsigned long long GetCPUUsage();


		////////////////////////////////////////////////////////////////////
		//static unsigned long GetRAMUsage()
		//  Tells the RAM usage of the caller process.
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the number of bytes used by the caller process
		//
		//
		////////////////////////////////////////////////////////////////////
		static unsigned long long GetRAMUsage();


		////////////////////////////////////////////////////////////////////
		//static unsigned long long GetTickCount()
		//  Returns the time elapsed since some starting point, with
		//  milliseconds precision.
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the milliseconds elapsed since some point in the past
		//
		//
		////////////////////////////////////////////////////////////////////
		static unsigned long long GetTickCount();

	};
}

#endif
