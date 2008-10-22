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

#ifdef GM_TICKET_MY_MASTER_COMPATIBLE

bool ChatHandler::HandleGMTicketListCommand(const char* args, WorldSession *m_session)
{
	Player *cplr = m_session->GetPlayer();

	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	chn->Say(cplr, "GmTicket 2", cplr, true);

	for(GmTicketList::iterator itr = objmgr.GM_TicketList.begin(); itr != objmgr.GM_TicketList.end(); itr++)
	{
		if((*itr)->deleted)
			continue;

		Player* plr = objmgr.GetPlayer((uint32)(*itr)->playerGuid);

		if( plr == NULL )
			continue;

//		Player* aplr = ((*itr)->assignedToPlayer == 0 ? NULL : objmgr.GetPlayer((uint32)(*itr)->assignedToPlayer));

		std::stringstream ss;

		uint32 zone = 0;
		if(plr->IsInWorld())
		{
			zone = plr->GetZoneId();
		}
		ss << "GmTicket 0," << (*itr)->name << "," << (*itr)->level << ",0," << zone;
		chn->Say(cplr, ss.str().c_str(), cplr, true);
	}

	return true;
}

bool ChatHandler::HandleGMTicketGetByIdCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;
	
	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;
	
	Player *plr = objmgr.GetPlayer(args, false);
	if(plr == NULL)
	{
		RedSystemMessage(m_session, "Player not found.");
		return true;
	}
	GM_Ticket *ticket = objmgr.GetGMTicketByPlayer(plr->GetGUID());
	if(ticket == NULL || ticket->deleted)
	{
		RedSystemMessage(m_session, "Ticket not found.");
		return true;
	}

	char* msg = new char[ticket->message.size() + 1];
	strcpy(msg, ticket->message.c_str());
	char * start = msg, *end;
	bool firstLine = true;
	for(;;)
	{
		end = strchr(start, '\n');
		if(!end)
			break;
	
		*end = '\0';

		std::stringstream ss;
		ss << "GmTicket " << (firstLine ? "3" : "4") << "," << ticket->name << "," << start;
		chn->Say(cplr, ss.str().c_str(), cplr, true);

		firstLine = false;

		start = end + 1;
	}
	if(*start != '\0')
	{
		std::stringstream ss;
		ss << "GmTicket " << (firstLine ? "3" : "4") << "," << ticket->name << "," << start;
		chn->Say(cplr, ss.str().c_str(), cplr, true);
	}
	delete[] msg;

	return true;
}

bool ChatHandler::HandleGMTicketRemoveByIdCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	Player *plr = objmgr.GetPlayer(args, true);
	if(plr == NULL)
	{
		RedSystemMessage(m_session, "Player not found.");
		return true;
	}
	GM_Ticket *ticket = objmgr.GetGMTicketByPlayer(plr->GetGUID());
	if(ticket == NULL || ticket->deleted)
	{
		RedSystemMessage(m_session, "Ticket not found.");
		return true;
	}

	std::stringstream ss;
	ss << "GmTicket 1," << ticket->name;
	chn->Say(cplr, ss.str().c_str(), NULL, true);

	objmgr.RemoveGMTicket(ticket->guid);

	if(!plr)
		return true;
	if(!plr->IsInWorld())
		return true;

	// Notify player about removing ticket
	WorldPacket data(SMSG_GMTICKET_DELETETICKET, 4);
	data << uint32(9);
	plr->GetSession()->SendPacket( &data );

	return true;
}

#else

bool ChatHandler::HandleGMTicketListCommand(const char* args, WorldSession *m_session)
{
	Player *cplr = m_session->GetPlayer();

	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	std::stringstream ss0;
	ss0 << "GmTicket:" << GM_TICKET_CHAT_OPCODE_LISTSTART;
	chn->Say(cplr, ss0.str().c_str(), cplr, true);

	for(GmTicketList::iterator itr = objmgr.GM_TicketList.begin(); itr != objmgr.GM_TicketList.end(); itr++)
	{
		if((*itr)->deleted)
			continue;

		Player* plr = objmgr.GetPlayer((uint32)(*itr)->playerGuid);
		
		Player* aplr = NULL;
		PlayerInfo* aplri = NULL;
		if((*itr)->assignedToPlayer != 0)
		{
			aplr = objmgr.GetPlayer((uint32)(*itr)->assignedToPlayer);
			if(aplr == NULL)
				aplri = objmgr.GetPlayerInfo((uint32)(*itr)->assignedToPlayer);
		}

		std::stringstream ss;
		ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_LISTENTRY;
		ss << ":" << (*itr)->guid;
		ss << ":" << (plr == NULL ? (*itr)->level : plr->getLevel());
		ss << ":" << (plr == NULL ? 0 : plr->IsInWorld());
		ss << ":" << (aplr == NULL ? (aplri == NULL ? "" : aplri->name) : aplr->GetName());
		ss << ":" << (plr == NULL ? (*itr)->name : plr->GetName());
		ss << ":" << (*itr)->comment;
		chn->Say(cplr, ss.str().c_str(), cplr, true);
	}

	return true;
}

bool ChatHandler::HandleGMTicketGetByIdCommand(const char* args, WorldSession *m_session)
{
	uint64 ticketGuid = (args ? atoi(args) : 0);
	if(!ticketGuid)
	{
		RedSystemMessage(m_session, "You must specify a ticket id.");
		return true;
	}
	
	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;
	
	GM_Ticket *ticket = objmgr.GetGMTicket(ticketGuid);
	if(ticket == NULL || ticket->deleted)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not found.", cplr, true);
		return true;
	}

	char* msg = new char[ticket->message.size() + 1];
	strcpy(msg, ticket->message.c_str());
	char * start = msg, *end;
	bool firstLine = true;
	for(;;)
	{
		end = strchr(start, '\n');
		if(!end)
			break;
	
		*end = '\0';

		std::stringstream ss;
		ss << "GmTicket:" << (firstLine ? GM_TICKET_CHAT_OPCODE_CONTENT : GM_TICKET_CHAT_OPCODE_APPENDCONTENT);
		ss << ":" << ticket->guid;
		ss << ":" << start;
		chn->Say(cplr, ss.str().c_str(), cplr, true);

		firstLine = false;

		start = end + 1;
	}
	if(*start != '\0')
	{
		std::stringstream ss;
		ss << "GmTicket:" << (firstLine ? GM_TICKET_CHAT_OPCODE_CONTENT : GM_TICKET_CHAT_OPCODE_APPENDCONTENT);
		ss << ":" << ticket->guid;
		ss << ":" << start;
		chn->Say(cplr, ss.str().c_str(), cplr, true);
	}
	delete[] msg;

	return true;
}

bool ChatHandler::HandleGMTicketRemoveByIdCommand(const char* args, WorldSession *m_session)
{
	uint64 ticketGuid = (args ? atoi(args) : 0);
	if(!ticketGuid)
	{
		RedSystemMessage(m_session, "You must specify a ticket id.");
		return true;
	}

	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	GM_Ticket *ticket = objmgr.GetGMTicket(ticketGuid);
	if(ticket == NULL || ticket->deleted)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not found.", cplr, true);
		return true;
	}

	if(ticket->assignedToPlayer != 0 && ticket->assignedToPlayer != cplr->GetGUID() && !cplr->GetSession()->CanUseCommand('z'))
	{
		chn->Say(cplr, "GmTicket:0:Ticket is assigned to another GM.", cplr, true);
		return true;
	}

	Player* plr = objmgr.GetPlayer((uint32)ticket->playerGuid);

	std::stringstream ss;
	ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_REMOVED;
	ss << ":" << ticket->guid;
	chn->Say(cplr, ss.str().c_str(), NULL, true);

	objmgr.RemoveGMTicket(ticket->guid);

	if(!plr)
		return true;
	if(!plr->IsInWorld())
		return true;

	// Notify player about removing ticket
	WorldPacket data(SMSG_GMTICKET_DELETETICKET, 4);
	data << uint32(9);
	plr->GetSession()->SendPacket( &data );

	return true;
}

bool ChatHandler::HandleGMTicketAssignToCommand(const char* args, WorldSession *m_session)
{
	uint64 ticketGuid;
	char guidString[100], name[100];
	int argc = sscanf(args,"%s %s", guidString, name);
	if(argc < 1 || argc > 2)
		return false;

	ticketGuid = atoi(guidString);
	if(!ticketGuid)
	{
		RedSystemMessage(m_session, "You must specify a ticket id.");
		return true;
	}

	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	GM_Ticket *ticket = objmgr.GetGMTicket(ticketGuid);
	if(ticket == NULL || ticket->deleted)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not found.", cplr, true);
		return true;
	}

	Player *plr = (argc == 1 ? cplr : objmgr.GetPlayer(name, false));
	if(plr == NULL)
	{
		chn->Say(cplr, "GmTicket:0:Player not found.", cplr, true);
		return true;
	}

	if(!plr->IsInWorld())
	{
		chn->Say(cplr, "GmTicket:0:Player isn't online.", cplr, true);
		return true;
	}

	if(plr->GetSession()->GetPermissionCount() == 0)
	{
		chn->Say(cplr, "GmTicket:0:Player is not a GM.", cplr, true);
		return true;
	}

	if(ticket->assignedToPlayer == plr->GetGUID())
	{
		chn->Say(cplr, "GmTicket:0:Ticket already assigned to this GM.", cplr, true);
		return true;
	}

	if(ticket->assignedToPlayer != 0 && ticket->assignedToPlayer != cplr->GetGUID())
	{
		Player *aplr = objmgr.GetPlayer((uint32)ticket->assignedToPlayer);
		if(aplr != NULL && aplr->IsInWorld() && !cplr->GetSession()->CanUseCommand('z'))
		{
			chn->Say(cplr, "GmTicket:0:Ticket already assigned to another GM.", cplr, true);
			return true;
		}
	}

	ticket->assignedToPlayer = plr->GetGUID();
	objmgr.UpdateGMTicket(ticket);

	std::stringstream ss;
	ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_ASSIGNED;
	ss << ":" << ticket->guid;
	ss << ":" << plr->GetName();
	chn->Say(cplr, ss.str().c_str(), NULL, true);

	return true;
}

bool ChatHandler::HandleGMTicketReleaseCommand(const char* args, WorldSession *m_session)
{
	uint64 ticketGuid = (args ? atoi(args) : 0);
	if(!ticketGuid)
	{
		RedSystemMessage(m_session, "You must specify a ticket id.");
		return true;
	}

	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	GM_Ticket *ticket = objmgr.GetGMTicket(ticketGuid);
	if(ticket == NULL || ticket->deleted)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not found.", cplr, true);
		return true;
	}

	if(ticket->assignedToPlayer == 0)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not assigned to a GM.", cplr, true);
		return true;
	}

	Player *plr = objmgr.GetPlayer((uint32)ticket->assignedToPlayer);
	if(!cplr->GetSession()->CanUseCommand('z') && plr != NULL && plr->IsInWorld() && plr->GetSession()->CanUseCommand('z'))
	{
		chn->Say(cplr, "GmTicket:0:You can not release tickets from admins.", cplr, true);
		return true;
	}
	
	ticket->assignedToPlayer = 0;
	objmgr.UpdateGMTicket(ticket);

	std::stringstream ss;
	ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_RELEASED;
	ss << ":" << ticket->guid;
	chn->Say(cplr, ss.str().c_str(), NULL, true);

	return true;
}

bool ChatHandler::HandleGMTicketCommentCommand(const char* args, WorldSession *m_session)
{
	uint64 ticketGuid;
	int argc = 1;
	char* comment = NULL;
	char* guidString = (char*)args;

	// Parse arguments
	char* space = (char*)strchr(args, ' ');
	if(space)
	{	
		*space = '\0';
		comment = space + 1;
		argc = 2;
	}

	ticketGuid = atoi(guidString);
	if(!ticketGuid)
	{
		RedSystemMessage(m_session, "You must specify a ticket id.");
		return true;
	}

	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	GM_Ticket *ticket = objmgr.GetGMTicket(ticketGuid);
	if(ticket == NULL || ticket->deleted)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not found.", cplr, true);
		return true;
	}

	if(ticket->assignedToPlayer != 0 && ticket->assignedToPlayer != cplr->GetGUID() && !cplr->GetSession()->CanUseCommand('z'))
	{
		chn->Say(cplr, "GmTicket:0:Ticket is assigned to another GM.", cplr, true);
		return true;
	}

	ticket->comment = (argc == 1 ? "" : comment);
	objmgr.UpdateGMTicket(ticket);

	std::stringstream ss;
	ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_COMMENT;
	ss << ":" << ticket->guid;
	ss << ":" << cplr->GetName();
	ss << ":" << ticket->comment;
	chn->Say(cplr, ss.str().c_str(), NULL, true);

	return true;
}

bool ChatHandler::HandleGMTicketDeletePermanentCommand(const char* args, WorldSession *m_session)
{
	uint64 ticketGuid = (args ? atoi(args) : 0);
	if(!ticketGuid)
	{
		RedSystemMessage(m_session, "You must specify a ticket id.");
		return true;
	}

	Player *cplr = m_session->GetPlayer();
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), cplr);
	if(!chn)
		return false;

	GM_Ticket *ticket = objmgr.GetGMTicket(ticketGuid);
	if(ticket == NULL)
	{
		chn->Say(cplr, "GmTicket:0:Ticket not found.", cplr, true);
		return true;
	}

	Player* plr = NULL;

	if(!ticket->deleted)
	{
		plr = objmgr.GetPlayer((uint32)ticket->playerGuid);
	
		std::stringstream ss;
		ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_REMOVED;
		ss << ":" << ticket->guid;
		chn->Say(cplr, ss.str().c_str(), NULL, true);

		objmgr.RemoveGMTicket(ticket->guid);
	}

	objmgr.DeleteGMTicketPermanently(ticket->guid);
	ticket = NULL;

	if(plr != NULL && plr->IsInWorld())
	{
		// Notify player about removing ticket
		WorldPacket data(SMSG_GMTICKET_DELETETICKET, 4);
		data << uint32(9);
		plr->GetSession()->SendPacket( &data );
	}
	return true;
}

#endif

bool ChatHandler::HandleGMTicketToggleTicketSystemStatusCommand(const char* args, WorldSession *m_session)
{
	if(sWorld.toggleGMTicketStatus())
		GreenSystemMessage(m_session, "TicketSystem enabled.");
	else
		GreenSystemMessage(m_session, "TicketSystem disabled.");
	return true;
}

