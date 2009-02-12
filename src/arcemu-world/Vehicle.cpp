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

/*
 *  Vehicle GM Commands (.vehicle)
 *		- spawn {creature id}
 *		- 
 *
 *	Related Packets
 *		{CLIENT} Packet: (0x027A) CMSG_FAR_SIGHT PacketSize = 1, Data = 01
 * 
 */


#include "StdAfx.h"

Vehicle::Vehicle(uint64 guid) : Creature(guid)
{
	m_passengers.clear();
}

Vehicle::~Vehicle()
{

}

void Vehicle::Load(uint32 vehicleid)
{
	/*
	VehicleEntry* v = dbcVehicle.LookupEntry(vehicleid);
	if (v == NULL)
		return;

	m_VehicleEntry = vehicleid;
	*/
	m_passengers.clear();
}

void Vehicle::AddPassenger(Player * player, uint8 seat)
{
	if (!player->IsInWorld())
		return;

	if (seat == 0xFF) //choose a seat for us
	{
		for (uint8 i=0; i < GetOccupancyLimit(); ++i)
			if (m_passengers[i] == NULL)
			{
				seat = i;
				break;
			}

		if (seat == 0xFF) //can't find a seat
			return;
	}
	else
	{
		if (seat >= GetOccupancyLimit() || m_passengers[seat] != NULL)
			return;
	}

	/*
	VehicleEntry* v = dbcVehicle.LookupEntry(m_VehicleEntry);
	if (v == NULL)
		return;
	VehicleSeatEntry* seatentry = dbcVehicleSeat.LookupEntry(v->seatEntry[seat]);
	if (seatentry == NULL)
		return;
	*/

	m_passengers[seat] = player;

	// Set this vehicle and seat to player
	player->SetVehicle(this, seat);

	//Time to move the unit...
	player->Root();

	WorldPacket data(MSG_MOVE_TELEPORT, 50);
	data << player->GetNewGUID();
	data << uint32(0x200); //transport data
	data << uint16(0);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << GetGUID();
	data << float(0.0f); //seatentry->offsetX;
	data << float(0.0f); //seatentry->offsetY;
	data << float(0.0f); // seatentry->offsetZ;
	data << float(0.0f); // orientation???
	data << getMSTime(); //WTF IS THIS
	data << seat;
	data << uint32(0);

	player->SendMessageToSet(&data, false);

	player->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());
	WorldPacket ack(0x49D, 0);
	if (player->GetSession() != NULL)
		player->GetSession()->SendPacket(&ack);
	player->SetPlayerStatus(TRANSFER_PENDING);
	player->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());

	data.Initialize(MSG_MOVE_TELEPORT_ACK);
	data << player->GetNewGUID();
	data << uint32(0); //counter
	data << uint32(0x200); //transport data
	data << uint16(0);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << GetGUID();
	data << float(0.0f); //seatentry->offsetX;
	data << float(0.0f); //seatentry->offsetY;
	data << float(0.0f); //seatentry->offsetZ;
	data << float(0.0f); //orientation ??
	data <<getMSTime(); //WTF IS THIS
	data << seat;
	data << uint32(0);

	player->GetSession()->SendPacket(&data);

	if (seat == 0) //todo: what if its uncontrollable (skybreaker flightpath, rocket mount from k3)
	{
		//give control to player
		data.Initialize(SMSG_FORCE_RUN_SPEED_CHANGE);
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(1);
		data << m_runSpeed; //speed;
		player->GetSession()->SendPacket(&data);

		data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
		data << GetNewGUID() << uint8(1);
		player->GetSession()->SendPacket(&data);
			m_controller = player;
		player->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, player->GetGUID());
		SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

		//set active mover
		player->GetSession()->SetActiveMover(GetNewGUID());

		data.Initialize(SMSG_PET_SPELLS);
			
		data << GetGUID();
		data << uint32(0);
		data << uint32(0);
		data << uint32(0x00000101);

		for(uint32 i=0; i<10; ++i)
		{
			data << uint16(0);
			data << uint8(0);
			data << uint8(8 + i);
		}

		data << uint8(0);
		data << uint8(0);
		player->GetSession()->SendPacket(&data);
	}
}

void Vehicle::RemovePassenger(Player * player)
{
	//shared_ptr<Vehicle> sthis = SPCAST(shared_from_this(), Vehicle); //don't allow deletion during execution

	if (player == NULL || player->GetVehicle() != this ||
		player->GetVehicleSeat() >= GetPassengerCount() ||
		m_passengers[player->GetVehicleSeat()] != player)
			return;

	WorldPacket data(MSG_MOVE_HEARTBEAT, 50);
	data.Initialize(MSG_MOVE_HEARTBEAT);
	data << player->GetNewGUID();
	data << uint32(0x800);
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	player->SendMessageToSet(&data, false);

	player->SetUInt64Value(PLAYER_FARSIGHT, 0);
	player->SetUInt64Value(UNIT_FIELD_CHARM, 0);
	player->SetPlayerStatus(TRANSFER_PENDING);
	player->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
	data.Initialize(MSG_MOVE_TELEPORT_ACK);
	data << player->GetNewGUID();
	data << uint32(1); //counter
	data << uint32(0x800);
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	player->GetSession()->SendPacket(&data);

	if (player == m_controller)
	{
		m_controller = NULL;

		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
		RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

		player->GetSession()->SetActiveMover(player->GetNewGUID());
		data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
		data << GetNewGUID() << uint8(0);
		player->GetSession()->SendPacket(&data);

	}

	//clear refs
	uint8 seat = player->GetVehicleSeat();
	player->ResetVehicleSettings();
	m_passengers[seat] = NULL;
}

void Vehicle::MoveVehicle(float x, float y, float z, float o)
{
	SetPosition(x, y, z, o);

	int8 passengercount = GetPassengerCount();
	for (int8 i=0; i < passengercount; ++i)
		if (m_passengers[i] != NULL)
			m_passengers[i]->SetPosition(x, y, z, o);
}

void Vehicle::setDeathState(DeathState s)
{
	Creature::setDeathState(s);

	int8 passengercount = GetPassengerCount();
	for (int8 i=0; i < passengercount; ++i)
		if (m_passengers[i] != NULL)
			RemovePassenger(m_passengers[i]);
}

void Vehicle::PossessVehicle(bool possess)
{

	//SetWorldState(TEST_1, 0);
	// Mount Seige Vehicle (SOTA, Attacker)
	/*
 *		WorldState 0x0DE5 = 0x190;
 *		WorldState 0x0DE7 = 0x06;
 *		WorldState 0x0DE8 = 0x28;
 *		WorldState 0x0DE9 = 0x00;
 *		WorldState 0x0DE9 = 0x00;
 *		WorldState 0x0DE8 = 0x28;
 *		WorldState 0x0DE8 = 0x04;
 *
 *	Unmount Seige Vehice (SOTA, Attacker)
 *		WorldState 0x0DE5 = 0x18B;
 *		WorldState 0x0DE7 = 0x06;
 *		WorldState 0x0DE8 = 0x23;
 *		WorldState 0x0DE9 = 0x00;
 *		WorldState 0x0DE9 = 0x05;
 *		WorldState 0x0DE8 = 0x1E;
 *		WorldState 0x0DE8 = 0x03;
 */
}

void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	//i dont give a crap what this packet structure is :P
	if (GetPlayer() == NULL || GetPlayer()->GetVehicle() == NULL)
		return;

	GetPlayer()->GetVehicle()->RemovePassenger(GetPlayer());
}

bool ChatHandler::HandleVehicleSpawn(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehiclePossess(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleUnpossess(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleMoveSpeed(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleTurnSpeed(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleProtectileSpeed(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleTurnRadians(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleMove(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}

bool ChatHandler::HandleVehicleFire(const char * args, WorldSession * m_session)
{
	RedSystemMessage(m_session, "Not Implemented.");
	return true;
}
