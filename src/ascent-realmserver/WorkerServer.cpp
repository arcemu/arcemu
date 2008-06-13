/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#include "RStdAfx.h"

WServerHandler WServer::PHandlers[IMSG_NUM_TYPES];

void WServer::InitHandlers()
{
	memset(PHandlers, 0, sizeof(void*) * IMSG_NUM_TYPES);
	PHandlers[ICMSG_REGISTER_WORKER] = &WServer::HandleRegisterWorker;
	PHandlers[ICMSG_WOW_PACKET] = &WServer::HandleWoWPacket;
	PHandlers[ICMSG_PLAYER_LOGIN_RESULT] = &WServer::HandlePlayerLoginResult;
	PHandlers[ICMSG_PLAYER_LOGOUT] = &WServer::HandlePlayerLogout;
	PHandlers[ICMSG_TELEPORT_REQUEST] = &WServer::HandleTeleportRequest;
}

WServer::WServer(uint32 id, WSSocket * s) : m_id(id), m_socket(s)
{

}

void WServer::HandleRegisterWorker(WorldPacket & pck)
{
	vector<uint32> preferred;
	uint32 build;
	pck >> build >> preferred;

	/* send a packed packet of all online players to this server */
	sClientMgr.SendPackedClientInfo(this);

	/* allocate initial instances for this worker */
	sClusterMgr.AllocateInitialInstances(this, preferred);
}

void WServer::HandleWoWPacket(WorldPacket & pck)
{
	uint32 sessionid, size;
	uint16 opcode;

	/* get session */
	pck >> sessionid >> opcode >> size;
	Session * session = sClientMgr.GetSession(sessionid);
	if(!session) return;

	/* write it to that session's output buffer */
	WorldSocket * s = session->GetSocket();
	if(s) s->OutPacket(opcode, size, size ? ((const void*)(pck.contents() + 10)) : 0);
}

void WServer::HandlePlayerLogout(WorldPacket & pck)
{
	uint32 sessionid, guid;
	pck >> sessionid >> guid;
	RPlayerInfo * pi = sClientMgr.GetRPlayer(guid);
	Session * s = sClientMgr.GetSession(sessionid);
	if(pi && s)
	{
		/* tell all other servers this player has gone offline */
		WorldPacket data(ISMSG_DESTROY_PLAYER_INFO, 4);
		data << guid;
		sClusterMgr.DistributePacketToAll(&data, this);

		/* clear the player from the session */
		s->ClearCurrentPlayer();
		s->ClearServers();

		/* destroy the playerinfo struct here */
		sClientMgr.DestroyRPlayerInfo(guid);
	}
}

void WServer::HandleTeleportRequest(WorldPacket & pck)
{
	WorldPacket data(ISMSG_TELEPORT_RESULT, 100);
	RPlayerInfo * pi;
	Session * s;
	Instance * dest;
	uint32 mapid, sessionid, instanceid;

	/* this packet is only used upon changing main maps! */
	pck >> sessionid >> mapid >> instanceid;	

	s = sClientMgr.GetSession(sessionid);
	if(s)
	{
		pi = s->GetPlayer();
		ASSERT(pi);

		/* find the destination server */
		if(instanceid == 0)
			dest = sClusterMgr.GetInstanceByMapId(mapid);
		else
			dest = sClusterMgr.GetInstanceByInstanceId(instanceid);

		/* server up? */
		if(!dest)
		{
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << uint32(0x02);	// INSTANCE_ABORT_NOT_FOUND
			s->SendPacket(&data);
		}
		else
		{
			/* server found! */
			LocationVector vec;
			pck >> vec >> vec.o;

			pi->MapId = mapid;
			pi->InstanceId = dest->InstanceId;
			pi->PositionX = vec.x;
			pi->PositionY = vec.y;

			if(dest->Server == s->GetServer())
			{
				/* we're not changing servers, the new instance is on the same server */
				data << sessionid << uint8(1) << mapid << instanceid << vec << vec.o;
				SendPacket(&data);
			}
			else
			{
				/* notify the old server to pack the player info together to send to the new server, and delete the player */
				data << sessionid << uint8(0) << mapid << instanceid << vec << vec.o;
				SendPacket(&data);
			}

			data.Initialize(ISMSG_PLAYER_INFO);
			pi->Pack(data);
			sClusterMgr.DistributePacketToAll(&data, this);

			data.Initialize(SMSG_NEW_WORLD);
			data << mapid << vec << vec.o;
			s->SendPacket(&data);
		}
	}
}
void WServer::HandlePlayerLoginResult(WorldPacket & pck)
{
	uint32 guid, sessionid;
	uint8 result;
	pck >> guid >> sessionid >> result;
	if(result)
	{
		Log.Success("WServer", "Worldserver %u reports successful login of player %u", m_id, guid);
		Session * s = sClientMgr.GetSession(sessionid);
		if(s)
		{
			/* update server */
			s->SetNextServer();
			
			/* pack together a player info packet and distribute it to all the other servers */
			ASSERT(s->GetPlayer());
			
			WorldPacket data(ISMSG_PLAYER_INFO, 100);
			s->GetPlayer()->Pack(data);
			sClusterMgr.DistributePacketToAll(&data, this);
		}
	}
	else
	{
		Log.Error("WServer", "Worldserver %u reports failed login of player %u", m_id, guid);
		Session * s = sClientMgr.GetSession(sessionid);
		if(s)
		{
			s->ClearCurrentPlayer();
			s->ClearServers();
		}

		sClientMgr.DestroyRPlayerInfo(guid);
	}
}

void WServer::Update()
{
	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_recvQueue.Pop()))
	{
		opcode = pck->GetOpcode();
		if(opcode < IMSG_NUM_TYPES && WServer::PHandlers[opcode] != 0)
			(this->*WServer::PHandlers[opcode])(*pck);
		else
			Log.Error("WServer", "Unhandled packet %u\n", opcode);
	}
}


