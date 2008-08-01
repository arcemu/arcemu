/*
 * arcemu MMORPG Server
 * Voice Chat Engine
 * Copyright (C) 2005-2007 Burlex <burlex@gmail.com>
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

#ifdef VOICE_CHAT

VoiceChatClientSocket::VoiceChatClientSocket(uint32 fd) : Socket(fd, 250000, 250000)
{
	remaining = 0;
	op = 0;
	next_ping = UNIXTIME + 15;
	last_pong = UNIXTIME;
}

void VoiceChatClientSocket::OnDisconnect()
{
	sVoiceChatHandler.SocketDisconnected();
}

void VoiceChatClientSocket::OnRead()
{
	WorldPacket *data;

	// uint16 op
	// uint16 size
	// <data>

	for(;;)
	{
		// no more data
		if( GetReadBuffer().GetSize() < 4 )
			break;

		GetReadBuffer().Read((uint8*)&op, 2);
		GetReadBuffer().Read((uint8*)&remaining, 2);

		if( GetReadBuffer().GetSize() < remaining )
			break;

		data = new WorldPacket(op, remaining);
		data->resize(remaining);
		GetReadBuffer().Read((uint8*)data->contents(), remaining);

		// handle the packet
		sVoiceChatHandler.OnRead(data);

		delete data;
		remaining = op = 0;
	}
}

void VoiceChatClientSocket::SendPacket(WorldPacket* data)
{
	//if((m_writeByteCount + len + 4) >= m_writeBufferSize)
	//if( GetWriteBuffer().GetSpace() < (len+4) )
	//if( m_writeByteCount + 4 + data->size() > m_writeBufferSize )
	if(	GetWriteBuffer().GetSpace() < (data->size()+4) )
	{
		Log.Error("VoiceChatHandler","!!! VOICE CHAT CLIENT SOCKET OVERLOAD !!!");
		return;
	}
	uint16 opcode = data->GetOpcode();
	uint32 sz = (uint32)data->size();
	bool rv;

	BurstBegin();
	rv = BurstSend((const uint8*)&opcode, 2);
	if(rv) BurstSend((const uint8*)&sz, 2);

	if( sz > 0 && rv )
	{
		rv = BurstSend((const uint8*)data->contents(), (uint32)data->size());
	}

	Log.Debug("VoiceChatHandler","sent packet of %u bytes with op %u, buffer len is now %u", data->size(), data->GetOpcode(), GetWriteBuffer().GetSize());
	if( rv )
		BurstPush();


	BurstEnd();
}

#endif		// VOICE_CHAT
