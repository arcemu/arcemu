/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"

enum GMticketType
{
	GM_TICKET_TYPE_STUCK				= 1,
	GM_TICKET_TYPE_BEHAVIOR_HARASSMENT	= 2,
	GM_TICKET_TYPE_GUILD				= 3,
	GM_TICKET_TYPE_ITEM					= 4,
	GM_TICKET_TYPE_ENVIRONMENTAL		= 5,
	GM_TICKET_TYPE_NON_QUEST_CREEP		= 6,
	GM_TICKET_TYPE_QUEST_QUEST_NPC		= 7,
	GM_TICKET_TYPE_TECHNICAL			= 8,
	GM_TICKET_TYPE_ACCOUNT_BILLING		= 9,
	GM_TICKET_TYPE_CHARACTER			= 10,
};

void WorldSession::HandleGMTicketCreateOpcode(WorldPacket & recv_data)
{
	uint32 map;
	float x, y, z;
	std::string message = "";
	std::string message2 = "";
	GM_Ticket *ticket = new GM_Ticket;
	WorldPacket data(SMSG_GMTICKET_CREATE, 4);

	// recv Data
	recv_data >> map;
	recv_data >> x;
	recv_data >> y;
	recv_data >> z;
	recv_data >> message;
	recv_data >> message2;

	// Remove pending tickets
	objmgr.RemoveGMTicketByPlayer(GetPlayer()->GetGUID());

	ticket->guid = objmgr.GenerateTicketID();
	ticket->playerGuid = GetPlayer()->GetGUID();
	ticket->map = map;
	ticket->posX = x;
	ticket->posY = y;
	ticket->posZ = z;
	ticket->message = message;
	ticket->timestamp = (uint32)UNIXTIME;
	ticket->name = GetPlayer()->GetName();
	ticket->level = GetPlayer()->getLevel();
	ticket->deleted = false;
	ticket->assignedToPlayer = 0;
	ticket->comment = "";

	// Add a new one
	objmgr.AddGMTicket(ticket, false);

	// Response - no errors
	data << uint32(2);

	SendPacket(&data);

	// send message indicating new ticket
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(),GetPlayer());
	if(chn)
	{
		std::stringstream ss;
#ifdef GM_TICKET_MY_MASTER_COMPATIBLE
		ss << "GmTicket 5, " << ticket->name;
#else
		ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_NEWTICKET;
		ss << ":" << ticket->guid;
		ss << ":" << ticket->level;
		ss << ":" << ticket->name;
#endif
		chn->Say(_player, ss.str().c_str(), NULL, true);
	}
}

void WorldSession::HandleGMTicketUpdateOpcode(WorldPacket & recv_data)
{
	std::string message = "";
	WorldPacket data(SMSG_GMTICKET_UPDATETEXT, 4);

	// recv Data
	recv_data >> message;

	// Update Ticket
	GM_Ticket *ticket = objmgr.GetGMTicketByPlayer(GetPlayer()->GetGUID());
	if(!ticket) // Player doesnt have a GM Ticket yet
	{
		// Response - error couldnt find existing Ticket
		data << uint32(1);

		SendPacket(&data);
		return;
	}
	ticket->message = message;
	ticket->timestamp = (uint32)UNIXTIME;

	objmgr.UpdateGMTicket(ticket);

	// Response - no errors
	data << uint32(2);

	SendPacket(&data);

#ifndef GM_TICKET_MY_MASTER_COMPATIBLE
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(),GetPlayer());
	if(chn)
	{
		std::stringstream ss;
		ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_UPDATED;
		ss << ":" << ticket->guid;
		chn->Say(_player, ss.str().c_str(), NULL, true);
	}
#endif
}

void WorldSession::HandleGMTicketDeleteOpcode(WorldPacket & recv_data)
{
	GM_Ticket* ticket = objmgr.GetGMTicketByPlayer(GetPlayer()->GetGUID());

	// Remove Tickets from Player
	objmgr.RemoveGMTicketByPlayer(GetPlayer()->GetGUID());

	// Response - no errors
	WorldPacket data(SMSG_GMTICKET_DELETETICKET, 4);
	data << uint32(9);
	SendPacket(&data);

	// send message to gm_sync_chan
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), GetPlayer());
	if(chn && ticket != NULL)
	{
		std::stringstream ss;
#ifdef GM_TICKET_MY_MASTER_COMPATIBLE
		ss << "GmTicket 1," << ticket->name;
#else
		ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_REMOVED;
		ss << ":" << ticket->guid;
#endif
		chn->Say(_player, ss.str().c_str(), NULL, true);
	}
}

void WorldSession::HandleGMTicketGetTicketOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_GMTICKET_GETTICKET, 400);
	// no data

	// get Current Ticket
	GM_Ticket *ticket = objmgr.GetGMTicketByPlayer(GetPlayer()->GetGUID());

	if(!ticket) // no Current Ticket
	{
		data << uint32(10);
		SendPacket(&data);
		return;
	}

	// Send current Ticket
	data << uint32(6); // unk
	data << ticket->message.c_str();
	data << (uint8)ticket->map;

	SendPacket(&data);
}


void WorldSession::HandleGMTicketSystemStatusOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_GMTICKET_SYSTEMSTATUS, 4);

	// no data

	// Response - System is working Fine
	if(sWorld.getGMTicketStatus())
		data << uint32(1);
	else
		data << uint32(0);

	SendPacket(&data);
}

void WorldSession::HandleGMTicketToggleSystemStatusOpcode(WorldPacket & recv_data)
{
	if(!HasGMPermissions())
		return;

	sWorld.toggleGMTicketStatus();
}
