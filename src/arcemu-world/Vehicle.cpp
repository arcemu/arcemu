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
	sLog.outDebug("Vehicle Constructed");
	ClearGameObjects();
	ClearPassengers();
}

Vehicle::~Vehicle()
{
	sLog.outDebug("Vehicle Removed");
}

// Remove game objects before calling
void Vehicle::ClearGameObjects()
{
	for (int i = 0; i < MAX_GOBJECTS; i++)
		m_gameobjects[i] = NULL;
}

// Remove passengers before calling
void Vehicle::ClearPassengers()
{
	for (int i = 0; i < MAX_PASSENGERS; i++)
		m_passengers[i] = NULL;
	m_controller = NULL;
	m_passengerCount = 0;
}

void Vehicle::SendHeartbeatPacket(Player *player)
{
	WorldPacket data;
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
}

void Vehicle::SendRidePacket(Player * player)
{
	// ADD Player
	/*
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

	*/

	// Remove Player
	WorldPacket data;
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
}

void Vehicle::RelocateToVehicle(Player * player)
{
	player->SetPlayerStatus(TRANSFER_PENDING);
	player->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
}

void Vehicle::SendFarsightPacket(Player * player, bool enabled)
{
	if (enabled)
	{
		player->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());
		WorldPacket ack(0x49D, 0);
		if (player->GetSession() != NULL)
			player->GetSession()->SendPacket(&ack);
	}
	else
	{
		player->SetUInt64Value(PLAYER_FARSIGHT, 0);
		player->SetUInt64Value(UNIT_FIELD_CHARM, 0);
	}
}

void Vehicle::SendRideSpellPacket(Player * player)
{
	WorldPacket data;
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

void Vehicle::AddPassenger(Player * player, int8 seat)
{
	if (!player->IsInWorld())
		return;

	if (seat == -1) //choose a seat for us
	{
		for (uint8 i=0; i < GetOccupancyLimit(); i++)
			if (m_passengers[i] == NULL)
			{
				seat = i;
				break;
			}

		if (seat == -1) //can't find a seat
			return;
		seat = 0;
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

	sLog.outDebug("AddPassenger");

	m_passengers[seat] = player;

	// Set this vehicle and seat to player
	player->SetVehicle(this, seat);

	// Time to move into the unit...
	//player->Root();

	SendFarsightPacket(player, true);
	RelocateToVehicle(player);

	/*
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
	*/

	if (seat == 0) //todo: what if its uncontrollable (skybreaker flightpath, rocket mount from k3)
	{
		//give control to player
		/*
		data.Initialize(SMSG_FORCE_RUN_SPEED_CHANGE);
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(1);
		data << m_runSpeed; //speed;
		player->GetSession()->SendPacket(&data);
		*/

		WorldPacket data;
		data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
		data << GetNewGUID() << uint8(1);
		player->GetSession()->SendPacket(&data);
			m_controller = player;
		player->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, player->GetGUID());
		SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

		// Set player world states
		// UNIX (LINKING PROBLEM WITH SetWorldState as INLINE)
		/*
		player->GetMapMgr()->SetWorldState(0x0DE5, 0x190);
		player->GetMapMgr()->SetWorldState(0x0DE7, 0x06);
 		player->GetMapMgr()->SetWorldState(0x0DE8, 0x28);
 		player->GetMapMgr()->SetWorldState(0x0DE9, 0x00);
 		player->GetMapMgr()->SetWorldState(0x0DE9, 0x00);
 		player->GetMapMgr()->SetWorldState(0x0DE8, 0x28);
 		player->GetMapMgr()->SetWorldState(0x0DE8, 0x04);
		*/

		 //set active mover
		player->GetSession()->SetActiveMover(GetNewGUID());

		// Show the vehicle action bar ???
		SendRideSpellPacket(player);
	}
}

void Vehicle::RemovePassenger(Player * player)
{
	if (player == NULL || player->GetVehicle() != this ||
		player->GetVehicleSeat() >= GetPassengerCount() ||
		m_passengers[player->GetVehicleSeat()] != player)
			return;


	// Set player's world state
	/*
 	player->GetMapMgr()->SetWorldState(0x0DE5, 0x18B);
 	player->GetMapMgr()->SetWorldState(0x0DE7, 0x06);
 	player->GetMapMgr()->SetWorldState(0x0DE8, 0x23);
 	player->GetMapMgr()->SetWorldState(0x0DE9, 0x00);
  	player->GetMapMgr()->SetWorldState(0x0DE9, 0x05);
  	player->GetMapMgr()->SetWorldState(0x0DE8, 0x1E);
  	player->GetMapMgr()->SetWorldState(0x0DE8, 0x03);
	*/

	SendFarsightPacket(player, false);
	RelocateToVehicle(player);

	if (player == m_controller)
	{
		WorldPacket data;
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

void Vehicle::SetDeathState(DeathState s)
{
	Creature::setDeathState(s);

	int8 passengercount = GetPassengerCount();
	for (int8 i=0; i < passengercount; ++i)
		if (m_passengers[i] != NULL)
			RemovePassenger(m_passengers[i]);
}

void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	//i dont give a crap what this packet structure is :P
	if (GetPlayer() == NULL || GetPlayer()->GetVehicle() == NULL)
		return;

	GetPlayer()->GetVehicle()->RemovePassenger(GetPlayer());
}


/*******************************************************************************
 *	GM Commands
 *******************************************************************************/

bool ChatHandler::HandleVehicleSpawn(const char * args, WorldSession * m_session)
{
	uint32 entry = atol(args);
	uint8 gender = 0; 

	if(entry == 0)
		return false;

	CreatureProto * proto = CreatureProtoStorage.LookupEntry(entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(entry);
	if(proto == 0 || info == 0)
	{
		RedSystemMessage(m_session, "Invalid entry id.");
		return true;
	}

	CreatureSpawn * sp = new CreatureSpawn;
	//sp->displayid = info->DisplayID;
	gender = (uint8)info->GenerateModelId(&sp->displayid);
 	sp->entry = entry; 
	sp->entry = entry;
	sp->form = 0;
	sp->id = objmgr.GenerateCreatureSpawnID();
	sp->movetype = 0;
	sp->x = m_session->GetPlayer()->GetPositionX();
	sp->y = m_session->GetPlayer()->GetPositionY();
	sp->z = m_session->GetPlayer()->GetPositionZ();
	sp->o = m_session->GetPlayer()->GetOrientation();
	sp->emote_state = 0;
	sp->flags = 0;
	sp->factionid = proto->Faction;
	sp->bytes0 = sp->setbyte(0,2,gender);
	sp->bytes1 = 0;
	sp->bytes2 = 0;
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;
	sp->MountedDisplayID = 0;
	sp->Item1SlotDisplay = 0;
	sp->Item2SlotDisplay = 0;
	sp->Item3SlotDisplay = 0;


	Creature * p = m_session->GetPlayer()->GetMapMgr()->CreateCreature(entry, true);
	ASSERT(p);
	p->Load(sp, (uint32)NULL, NULL);
	p->m_loadedFromDB = true;
	p->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	
	uint32 x = m_session->GetPlayer()->GetMapMgr()->GetPosX(m_session->GetPlayer()->GetPositionX());
	uint32 y = m_session->GetPlayer()->GetMapMgr()->GetPosY(m_session->GetPlayer()->GetPositionY());

	// Add spawn to map
	m_session->GetPlayer()->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(
		x,
		y)->CreatureSpawns.push_back(sp);

	MapCell * mCell = m_session->GetPlayer()->GetMapMgr()->GetCell( x, y );

	if( mCell != NULL )
		mCell->SetLoaded();

	BlueSystemMessage(m_session, "Spawned a vehicle `%s` with entry %u at %f %f %f on map %u", info->Name, 
		entry, sp->x, sp->y, sp->z, m_session->GetPlayer()->GetMapId());

	sGMLog.writefromsession(m_session, "spawned a %s at %u %f %f %f", info->Name, m_session->GetPlayer()->GetMapId(),sp->x,sp->y,sp->z);

	return true;
}

bool ChatHandler::HandleVehiclePossess(const char * args, WorldSession * m_session)
{
	Creature * creature = this->getSelectedCreature(m_session);
	if (!creature || !creature->IsVehicle())
	{
		SystemMessage(m_session, "Select a vehicle first");
		return true;
	}

	Vehicle * vehicle = (Vehicle *)creature;
	Player * player = m_session->GetPlayer();

	vehicle->AddPassenger(player);
	player->Possess(vehicle);
	
	GreenSystemMessage(m_session, "Possessing Selected Vehicle...");
	return true;
}

bool ChatHandler::HandleVehicleUnpossess(const char * args, WorldSession * m_session)
{
	Player * player = m_session->GetPlayer();
	Vehicle * vehicle = player->GetVehicle();
	if (!vehicle)
	{
		SystemMessage(m_session, "You're not in possession of a vehicle");
		return true;
	}
	player->UnPossess();
	vehicle->RemovePassenger(player);
	GreenSystemMessage(m_session, "Unpossessing Current Vehicle...");
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
