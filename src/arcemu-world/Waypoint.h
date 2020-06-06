/*
 * ArcEmu MMORPG Server
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

#ifndef WAYPOINT_H_
#define WAYPOINT_H_

struct WayPoint
{
	WayPoint()
	{
		o = 0.0f;
	}
	uint32 id;
	float x;
	float y;
	float z;
	float o;
	uint32 waittime; //ms
	uint32 flags;
	bool forwardemoteoneshot;
	uint32 forwardemoteid;
	bool backwardemoteoneshot;
	uint32 backwardemoteid;
	uint32 forwardskinid;
	uint32 backwardskinid;

};

typedef std::vector<WayPoint*> WayPointMap;

#endif
