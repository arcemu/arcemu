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

#include "StdAfx.h"
#ifdef CLUSTERING

WSClient::WSClient(SOCKET fd) : Socket(fd, 1024576, 1024576)
{
	_remaining = 0;
	_cmd = 0;
}

WSClient::~WSClient()
{

}

void WSClient::OnRead()
{
	for(;;)
	{
		if(!_cmd)
		{
			if(GetReadBufferSize() < 6)
				break;

			Read(2, (uint8*)&_cmd);
			Read(4, (uint8*)&_remaining);
		}

		if(_remaining && GetReadBufferSize() < _remaining)
			break;

		if(_cmd == ISMSG_WOW_PACKET)
		{
			/* optimized version for packet passing, to reduce latency! ;) */
			uint32 sid = *(uint32*)&m_readBuffer[0];
			uint16 op  = *(uint16*)&m_readBuffer[4];
			uint32 sz  = *(uint32*)&m_readBuffer[6];			
			WorldSession * session = sClusterInterface.GetSession(sid);
			if(session != NULL)
			{
				WorldPacket * pck = new WorldPacket(op, sz);
				pck->resize(sz);
				memcpy((void*)pck->contents(), &m_readBuffer[10], sz);
				session->QueuePacket(pck);
			}
			RemoveReadBufferBytes(sz + 10/*header*/, false);
			_cmd = 0;
			continue;
		}

		WorldPacket * pck = new WorldPacket(_cmd, _remaining);
		_cmd = 0;
		pck->resize(_remaining);
		Read(_remaining, (uint8*)pck->contents());

		/* we could handle auth here */
		switch(_cmd)
		{
		case ISMSG_AUTH_REQUEST:
			sClusterInterface.HandleAuthRequest(*pck);
			delete pck;
			break;
		default:
			sClusterInterface.QueuePacket(pck);
		}		
	}
}

void WSClient::OnConnect()
{
	sClusterInterface.SetSocket(this);
}

void WSClient::SendPacket(WorldPacket * data)
{
	bool rv;
	uint32 size = data->size();
	uint16 opcode = data->GetOpcode();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)data->contents(), size);

	if(rv) BurstPush();
	BurstEnd();
}

#endif
