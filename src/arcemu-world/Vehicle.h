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

#define MAX_PASSENGERS 16
#define MAX_GOBJECTS 16
#define MAX_VEHICLE_SPELLS 16

class SERVER_DECL Vehicle : public Creature
{
	// Vehicle specific data
	Player * m_passengers[MAX_PASSENGERS];
	Player * m_controller;
	int8	 m_passengerCount;
	uint32	 m_fire_SpellIds[MAX_VEHICLE_SPELLS];

	// DBC Support
	//uint32 m_VehicleEntry; //dbc entry

	// FUTURE --- NOT USED --- DON'T USE --- DON'T COMMENT OUT - Thanks
	// Game objects in fixed relative positions to the vehicle
	// Custom stuff for future development
	GameObject * m_gameobjects[MAX_GOBJECTS];  

public:
	Vehicle(uint64 guid);
	~Vehicle();

	// Override superclass method that returns false
	bool IsVehicle() { return true; }

	// Packet Sends
	void RelocateToVehicle(Player * player);
	void SendFarsightPacket(Player * player, bool enabled);
	void SendRideSpellPacket(Player * player);
	void SendRidePacket(Player * player);
	void SendHeartbeatPacket(Player *player);

	// Clears the passenger
	void ClearPassengers();
	void ClearGameObjects();

	// Get information about the vehicle
	int8 GetOccupancyLimit() { return 1; } // CHANGE THIS
	int8 GetPassengerCount() { return m_passengerCount; }

	// Passenger Control
	void AddPassenger(Player * player, int8 seat = -1);
	void RemovePassenger(Player * player);

	// Vehicle Control
	void MoveVehicle(float x, float y, float z, float o);
	void MoveVehicle();

	// Set the vehicle's death state
	void SetDeathState(DeathState s);
};

#endif
