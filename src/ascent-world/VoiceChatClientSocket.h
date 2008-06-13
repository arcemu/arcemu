/*
 * Ascent MMORPG Server
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

#ifndef _VOICECHATCLIENTSOCKET_H
#define _VOICECHATCLIENTSOCKET_H

#ifdef VOICE_CHAT

class VoiceChatClientSocket : public Socket
{
	uint16 op;
	uint16 remaining;
public:
	VoiceChatClientSocket(uint32 fd);
	void OnDisconnect();
	void OnRead();
	void SendPacket(WorldPacket* data);
	time_t next_ping;
	time_t last_pong;
};

#endif		// VOICE_CHAT
#endif		// _VOICECHATCLIENTSOCKET_H

