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


