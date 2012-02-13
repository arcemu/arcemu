/*
 * ArcEmu, a MMORPG Server
 * 
 * Copyright (C) 2008-2012 ArcEmu Developers <http://arcemu.org/>
 *
 * See former copyright holders in CREDITS
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HONORHANDLER_H
#define HONORHANDLER_H

class HonorHandler
{
	public:
		static int32 CalculateHonorPointsForKill(uint32 playerLevel, uint32 victimLevel);
		static void RecalculateHonorFields(Player* pPlayer);
		static void AddHonorPointsToPlayer(Player* pPlayer, uint32 uAmount);
		static void OnPlayerKilled(Player* pPlayer, Player* pVictim);
};


#endif
