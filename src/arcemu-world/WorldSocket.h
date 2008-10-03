/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

// Class WorldSocket - Main network code functions, handles
// reading/writing of all packets.

#ifndef __WORLDSOCKET_H
#define __WORLDSOCKET_H

/* Normal WorldSocket when not using clustering */
#ifndef CLUSTERING

#define WORLDSOCKET_SENDBUF_SIZE 131078
#define WORLDSOCKET_RECVBUF_SIZE 16384

class WorldPacket;
class SocketHandler;
class WorldSession;

enum OUTPACKET_RESULT
{
	OUTPACKET_RESULT_SUCCESS = 1,
	OUTPACKET_RESULT_NO_ROOM_IN_BUFFER = 2,
	OUTPACKET_RESULT_NOT_CONNECTED = 3,
	OUTPACKET_RESULT_SOCKET_ERROR = 4,
};

class SERVER_DECL WorldSocket : public Socket
{
public:
	WorldSocket(SOCKET fd);
	~WorldSocket();

	// vs8 fix - send null on empty buffer
	ARCEMU_INLINE void SendPacket(WorldPacket* packet) { if(!packet) return; OutPacket(packet->GetOpcode(), packet->size(), (packet->size() ? (const void*)packet->contents() : NULL)); }
	ARCEMU_INLINE void SendPacket(StackBufferBase * packet) { if(!packet) return; OutPacket(packet->GetOpcode(), packet->GetSize(), (packet->GetSize() ? (const void*)packet->GetBufferPointer() : NULL)); }

	void __fastcall OutPacket(uint16 opcode, size_t len, const void* data);
	OUTPACKET_RESULT __fastcall _OutPacket(uint16 opcode, size_t len, const void* data);
   
	ARCEMU_INLINE uint32 GetLatency() { return _latency; }

	void Authenticate();
	void InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid);

	void __fastcall UpdateQueuePosition(uint32 Position);

	void OnRead();
	void OnConnect();
	void OnDisconnect();

	ARCEMU_INLINE void SetSession(WorldSession * session) { mSession = session; }
	ARCEMU_INLINE WorldSession * GetSession() { return mSession; }
	bool Authed;

	void UpdateQueuedPackets();
	
protected:
	
	void _HandleAuthSession(WorldPacket* recvPacket);
	void _HandlePing(WorldPacket* recvPacket);

private:

	uint32 mOpcode;
	uint32 mRemaining;
	uint32 mSize;
	uint32 mSeed;
	uint32 mClientSeed;
	uint32 mClientBuild;
	uint32 mRequestID;

	WorldSession *mSession;
	WorldPacket * pAuthenticationPacket;
	FastQueue<WorldPacket*, DummyLock> _queue;
	Mutex queueLock;

	WowCrypt _crypt;
	uint32 _latency;
	bool mQueued;
	bool m_nagleEanbled;
	string * m_fullAccountName;
};

#endif

static inline void FastGUIDPack(ByteBuffer & buf, const uint64 & oldguid)
{
	// hehe speed freaks
	uint8 guidmask = 0;
	uint8 guidfields[9] = {0,0,0,0,0,0,0,0};
	
	int j = 1;
	uint8 * test = (uint8*)&oldguid;

	if (*test) //7*8
	{
		guidfields[j] = *test;
		guidmask |= 1;
		j++;
	}
	if (*(test+1)) //6*8
	{
		guidfields[j] = *(test+1);
		guidmask |= 2;
		j++;
	}
	if (*(test+2)) //5*8
	{
		guidfields[j] = *(test+2);
		guidmask |= 4;
		j++;
	}
	if (*(test+3)) //4*8
	{
		guidfields[j] = *(test+3);
		guidmask |= 8;
		j++;
	}
	if (*(test+4)) //3*8
	{
		guidfields[j] = *(test+4);
		guidmask |= 16;
		j++;
	}
	if (*(test+5))//2*8
	{
		guidfields[j] = *(test+5);
		guidmask |= 32;
		j++;
	}
	if (*(test+6))//1*8
	{
		guidfields[j] = *(test+6);
		guidmask |= 64;
		j++;
	}
	if (*(test+7)) //0*8
	{
		guidfields[j] = *(test+7);
		guidmask |= 128;
		j++;
	}
	guidfields[0] = guidmask;

	buf.append(guidfields,j);
}

//!!! warning. This presumes that all guids can be compressed at least 1 byte
//make sure you choose highguids acordingly
static inline unsigned int FastGUIDPack(const uint64 & oldguid, unsigned char * buffer, uint32 pos)
{
	// hehe speed freaks
	uint8 guidmask = 0;

	int j = 1 + pos;
#ifdef USING_BIG_ENDIAN
	uint64 t = swap64(oldguid);
	uint8 * test = (uint8*)&t;
#else
	uint8 * test = (uint8*)&oldguid;
#endif

	if (*test) //7*8
	{
		buffer[j] = *test;
		guidmask |= 1;
		j++;
	}
	if (*(test+1)) //6*8
	{
		buffer[j] = *(test+1);
		guidmask |= 2;
		j++;
	}
	if (*(test+2)) //5*8
	{
		buffer[j] = *(test+2);
		guidmask |= 4;
		j++;
	}
	if (*(test+3)) //4*8
	{
		buffer[j] = *(test+3);
		guidmask |= 8;
		j++;
	}
	if (*(test+4)) //3*8
	{
		buffer[j] = *(test+4);
		guidmask |= 16;
		j++;
	}
	if (*(test+5))//2*8
	{
		buffer[j] = *(test+5);
		guidmask |= 32;
		j++;
	}
	if (*(test+6))//1*8
	{
		buffer[j] = *(test+6);
		guidmask |= 64;
		j++;
	}
	if (*(test+7)) //0*8
	{
		buffer[j] = *(test+7);
		guidmask |= 128;
		j++;
	}
	buffer[pos] = guidmask;
	return (j - pos);
}

/* Modified/Simplified WorldSocket for use with clustering */
#ifdef CLUSTERING
class WorldSocket
{
public:
	WorldSocket(uint32 sessionid);
	~WorldSocket();

	void Disconnect();
	bool IsConnected();
	ARCEMU_INLINE string GetRemoteIP() { return string(inet_ntoa(m_address.sin_addr)); }
	ARCEMU_INLINE uint32 GetRemotePort() { return ntohs(m_address.sin_port); }

	ARCEMU_INLINE void SendPacket(WorldPacket* packet) { if(!packet) return; OutPacket(packet->GetOpcode(), (uint16)packet->size(), (packet->size() ? (const void*)packet->contents() : NULL)); }
	ARCEMU_INLINE void SendPacket(StackBufferBase * packet) { if(!packet) return; OutPacket(packet->GetOpcode(), packet->GetSize(), (packet->GetSize() ? (const void*)packet->GetBufferPointer() : NULL)); }
	void __fastcall OutPacket(uint16 opcode, uint16 len, const void* data);
	ARCEMU_INLINE uint32 GetSessionId() { return m_sessionId; }

protected:
	uint32 m_sessionId;
	sockaddr_in m_address;
};

#endif
#endif
