/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2012 <http://www.ArcEmu.org/>
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

#ifndef __MISCHANDLER_H
#define __MISCHANDLER_H

#pragma pack(push,1)
struct GraveyardTeleport
{
	uint32 ID;
	float X;
	float Y;
	float Z;
	float O;
	uint32 ZoneId;
	uint32 AdjacentZoneId;
	uint32 MapId;
	uint32 FactionID;
};
#pragma pack(pop)

#endif

