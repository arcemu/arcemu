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

class WServer;
class Session;
class WSSocket : public Socket
{
	bool _authenticated;
	uint32 _remaining;
	uint16 _cmd;
	WServer * _ws;
public:
	uint32 m_id;

	WSSocket(SOCKET fd);
	~WSSocket();

	void SendPacket(WorldPacket * pck);
	void SendWoWPacket(Session * from, WorldPacket * pck);
	void OnRead();

	void HandleAuthRequest(WorldPacket & pck);
	void HandleRegisterWorker(WorldPacket & pck);
	void OnConnect();

};


