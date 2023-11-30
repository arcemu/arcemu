/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#ifndef MOVEMENTINFO_H_
#define MOVEMENTINFO_H_

class MovementInfo
{
	public:
		uint32 time;
		float pitch;// -1.55=looking down, 0=looking forward, +1.55=looking up
		float redirectSin;//on slip 8 is zero, on jump some other number
		float redirectCos, redirect2DSpeed;//9,10 changes if you are not on foot
		uint32 unk11, unk12;
		uint8 unk13;
		uint32 unklast;//something related to collision
		uint16 unk_230;

		float x, y, z, orientation;
		uint32 flags;
		float redirectVelocity;
		WoWGuid transGuid;
		float transX, transY, transZ, transO, transUnk;
		uint8 transUnk_2;

		MovementInfo();

		void init(WorldPacket & data);
		void write(WorldPacket & data);
};

#endif
