/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#include "RStdAfx.h"

SessionPacketHandler Session::Handlers[NUM_MSG_TYPES];
void Session::InitHandlers()
{
	memset(Handlers, 0, sizeof(void*) * NUM_MSG_TYPES);
	Handlers[CMSG_PLAYER_LOGIN] = &Session::HandlePlayerLogin;
	Handlers[CMSG_CHAR_ENUM] = &Session::HandleCharacterEnum;
	Handlers[CMSG_ITEM_QUERY_SINGLE] = &Session::HandleItemQuerySingleOpcode;
}

Session::Session(uint32 id) : m_sessionId(id)
{
	m_socket = 0;
	m_server = 0;
	m_accountId = 0;
	m_currentPlayer = 0;
	m_latency = 0;
	m_accountFlags = 0;
	m_build = 0;
	m_nextServer = 0;
}

void Session::Update()
{
	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_readQueue.Pop()))
	{
		opcode = pck->GetOpcode();

		/* can we handle it ourselves? */
		if(Session::Handlers[opcode] != 0)
		{
			(this->*Session::Handlers[opcode])(*pck);
		}
		else
		{
			/* no? pass it back to the worker server for handling. */
			if(m_server) m_server->SendWoWPacket(this, pck);
		}
		delete pck;
	}
}
