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

WorldSocket::WorldSocket(uint32 sessionid) : m_sessionId(sessionid)
{

}

WorldSocket::~WorldSocket()
{

}

void WorldSocket::Disconnect()
{

}

bool WorldSocket::IsConnected()
{
	return true;
}

void WorldSocket::OutPacket(uint16 opcode, uint16 len, const void* data)
{
	sClusterInterface.ForwardWoWPacket(opcode, len, data, m_sessionId);
}

#endif
