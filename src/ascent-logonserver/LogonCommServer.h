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

#ifndef __LOGON_COMM_SERVER_H
#define __LOGON_COMM_SERVER_H

#include <RC4Engine.h>

class LogonCommServerSocket : public Socket
{
	uint32 remaining;
	uint16 opcode;
	uint32 seed;
	RC4Engine sendCrypto;
	RC4Engine recvCrypto;
public:
	uint32 authenticated;
	bool use_crypto;

	LogonCommServerSocket(SOCKET fd);
	~LogonCommServerSocket();

	void OnRead();
	void OnDisconnect();
	void OnConnect();
	void SendPacket(WorldPacket * data);
	void HandlePacket(WorldPacket & recvData);

	void HandleRegister(WorldPacket & recvData);
	void HandlePing(WorldPacket & recvData);
	void HandleSessionRequest(WorldPacket & recvData);
	void HandleSQLExecute(WorldPacket & recvData);
	void HandleReloadAccounts(WorldPacket & recvData);
	void HandleAuthChallenge(WorldPacket & recvData);
	void HandleMappingReply(WorldPacket & recvData);
	void HandleUpdateMapping(WorldPacket & recvData);
	void HandleTestConsoleLogin(WorldPacket & recvData);
	void HandleDatabaseModify(WorldPacket& recvData);

	uint32 last_ping;
	bool removed;
	set<uint32> server_ids;
};

typedef void (LogonCommServerSocket::*logonpacket_handler)(WorldPacket&);

#endif
