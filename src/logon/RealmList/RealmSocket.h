/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef __LOGON_COMM_SERVER_H
#define __LOGON_COMM_SERVER_H

#include <Crypto/RC4Engine.h>

class RealmSocket : public Socket
{
		uint32 remaining;
		uint16 opcode;
		uint32 seed;
		RC4Engine sendCrypto;
		RC4Engine recvCrypto;
	public:
		uint32 authenticated;
		bool use_crypto;

		RealmSocket(SOCKET fd);
		~RealmSocket();

		void OnRead();
		void OnDisconnect();
		void OnConnect();
		void SendPacket(PacketBuffer* data);
		void HandlePacket(PacketBuffer & recvData);

		void HandleRegister(PacketBuffer & recvData);
		void HandlePing(PacketBuffer & recvData);
		void HandleSessionRequest(PacketBuffer & recvData);
		void HandleAuthChallenge(PacketBuffer & recvData);
		void HandleMappingReply(PacketBuffer & recvData);
		void HandleUpdateMapping(PacketBuffer & recvData);
		void HandleTestConsoleLogin(PacketBuffer & recvData);
		void HandleDatabaseModify(PacketBuffer & recvData);
		void HandlePopulationRespond(PacketBuffer & recvData);

		void RefreshRealmsPop();

		Arcemu::Threading::AtomicCounter last_ping;
		bool removed;
		set<uint32> server_ids;
};

typedef void (RealmSocket::*logonpacket_handler)(PacketBuffer &);

#endif
