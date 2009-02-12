/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

#ifndef __VEHICLE_H
#define __VEHICLE_H

class SERVER_DECL Vehicle : public Creature
{
public:
	Vehicle(uint64 guid);
	~Vehicle();

	void MoveVehicle();

	vector<Player *> m_passengers;
	Player * m_controller;
	//uint32 m_VehicleEntry; //dbc entry

	bool IsVehicle() { return true; }
	uint32 GetOccupancyLimit() { return 1; } // CHANGE THIS
	int8 GetPassengerCount() { return m_passengers.size(); }

	void Load(uint32 vehicleid);
	void AddPassenger(Player * player, uint8 seat = 0xFF);
	void RemovePassenger(Player * player);
	void MoveVehicle(float x, float y, float z, float o);

	void PossessVehicle(bool possess);

	void setDeathState(DeathState s);
};

#endif
