<<<<<<< .working
<<<<<<< .working
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
=======
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
>>>>>>> .merge-right.r249
=======
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
>>>>>>> .merge-right.r249
