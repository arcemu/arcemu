/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"
#include "AuthCodes.h"

/* echo send/received packets to console */
//#define ECHO_PACKET_LOG_TO_CONSOLE 1

#pragma pack(push, 1)
struct ClientPktHeader
{
	uint16 size;
	uint32 cmd;
};

struct ServerPktHeader
{
	uint16 size;
	uint16 cmd;
};
#pragma pack(pop)

WorldSocket::WorldSocket(SOCKET fd)
:
Socket(fd, sWorld.SocketSendBufSize, sWorld.SocketRecvBufSize),
Authed(false),
mOpcode(0),
mRemaining(0),
mSize(0),
mSeed(RandomUInt()),
mRequestID(0),
mSession(NULL),
pAuthenticationPacket(NULL),
_latency(0),
mQueued(false),
m_nagleEanbled(false),
m_fullAccountName(NULL)
{

}

WorldSocket::~WorldSocket()
{
	WorldPacket * pck;
	queueLock.Acquire();
	while( (pck = _queue.Pop()) != NULL )
	{
		delete pck;
	}
	queueLock.Release();

	if(pAuthenticationPacket)
		delete pAuthenticationPacket;

	if(mSession)
	{
		mSession->SetSocket(NULL);
		mSession= NULL;
	}

	if( m_fullAccountName != NULL )
	{
		delete m_fullAccountName;
		m_fullAccountName = NULL;
	}
}

void WorldSocket::OnDisconnect()
{
	if(mSession)
	{
		mSession->SetSocket(0);
		mSession= NULL;
	}

	if(mRequestID != 0)
	{
		sLogonCommHandler.UnauthedSocketClose(mRequestID);
		mRequestID = 0;
	}

	if(mQueued)
	{
		sWorld.RemoveQueuedSocket(this);	// Remove from queued sockets.
		mQueued=false;
	}
}

void WorldSocket::OutPacket(uint16 opcode, size_t len, const void* data)
{
	OUTPACKET_RESULT res;
	if( (len + 10) > WORLDSOCKET_SENDBUF_SIZE )
	{
		sLog.outError("WARNING: Tried to send a packet of %u bytes (which is too large) to a socket. Opcode was: %u (0x%03X)", (unsigned int)len, (unsigned int)opcode, (unsigned int)opcode);
		return;
	}

	res = _OutPacket(opcode, len, data);
	if(res == OUTPACKET_RESULT_SUCCESS)
		return;

	if(res == OUTPACKET_RESULT_NO_ROOM_IN_BUFFER)
	{
		/* queue the packet */
		queueLock.Acquire();
		WorldPacket * pck = new WorldPacket(opcode, len);
		if(len) pck->append((const uint8*)data, len);
		_queue.Push(pck);
		queueLock.Release();
	}
}

void WorldSocket::UpdateQueuedPackets()
{
	queueLock.Acquire();
	if(!_queue.HasItems())
	{
		queueLock.Release();
		return;
	}

	WorldPacket * pck;
	while( (pck = _queue.front()) != NULL )
	{
		/* try to push out as many as you can */
		switch(_OutPacket(pck->GetOpcode(), pck->size(), pck->size() ? pck->contents() : NULL))
		{
		case OUTPACKET_RESULT_SUCCESS:
			{
				delete pck;
				_queue.pop_front();
			}break;

		case OUTPACKET_RESULT_NO_ROOM_IN_BUFFER:
			{
				/* still connected */
				queueLock.Release();
				return;
			}break;

		default:
			{
				/* kill everything in the buffer */
				while( (pck == _queue.Pop()) != 0 )
				{
					delete pck;
				}
				queueLock.Release();
				return;
			}break;
		}
	}
	queueLock.Release();
}

OUTPACKET_RESULT WorldSocket::_OutPacket(uint16 opcode, size_t len, const void* data)
{
	bool rv;
	if(!IsConnected())
		return OUTPACKET_RESULT_NOT_CONNECTED;

	BurstBegin();
	//if((m_writeByteCount + len + 4) >= m_writeBufferSize)
	if( writeBuffer.GetSpace() < (len+4) )
	{
		BurstEnd();
		return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
	}

	// Packet logger :)
	sWorldLog.LogPacket((uint32)len, opcode, (const uint8*)data, 1, (mSession ? mSession->GetAccountId() : 0) );

	// Encrypt the packet
	// First, create the header.
	ServerPktHeader Header;

	Header.cmd = opcode;
	Header.size = ntohs((uint16)len + 2);

	_crypt.EncryptSend((uint8*)&Header, sizeof (ServerPktHeader));

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&Header, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(len > 0 && rv)
	{
		rv = BurstSend((const uint8*)data, (uint32)len);
	}

	if(rv) BurstPush();
	BurstEnd();
	return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
}

void WorldSocket::OnConnect()
{
	sWorld.mAcceptedConnections++;
	_latency = getMSTime();

	WorldPacket wp( SMSG_AUTH_CHALLENGE, 24 );

	wp << uint32( 1 );
	wp << uint32( mSeed );
	wp << uint32( 0xC0FFEEEE );
	wp << uint32( 0x00BABE00 );
	wp << uint32( 0xDF1697E5 );
	wp << uint32( 0x1234ABCD );

	SendPacket( &wp );

}

void WorldSocket::_HandleAuthSession(WorldPacket* recvPacket)
{
	std::string account;
	uint32 unk2, unk3;
	uint64 unk4;
	uint32 unk5, unk6, unk7;
	
	_latency = getMSTime() - _latency;

	try
	{
		*recvPacket >> mClientBuild;
		*recvPacket >> unk2;
		*recvPacket >> account;
		*recvPacket >> unk3;
		*recvPacket >> mClientSeed;
		*recvPacket >> unk4;
		*recvPacket >> unk5;
		*recvPacket >> unk6;
		*recvPacket >> unk7;
	}
	catch(ByteBuffer::error &)
	{
		sLog.outDetail("Incomplete copy of AUTH_SESSION Received.");
		return;
	}

	// Send out a request for this account.
	mRequestID = sLogonCommHandler.ClientConnected(account, this);
	
	if(mRequestID == 0xFFFFFFFF)
	{
		Disconnect();
		return;
	}

	// shitty hash !
	m_fullAccountName = new string( account );

	// Set the authentication packet 
    pAuthenticationPacket = recvPacket;
}

void WorldSocket::InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid)
{
	if(requestid != mRequestID)
		return;

	uint32 error;
	recvData >> error;

	if(error != 0 || pAuthenticationPacket == NULL)
	{
		// something happened wrong @ the logon server
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		return;
	}

	// Extract account information from the packet.
	string AccountName;
	const string * ForcedPermissions;
	uint32 AccountID;
	string GMFlags;
	uint8 AccountFlags;
	string lang = "enUS";
	uint32 i;
	
	recvData >> AccountID >> AccountName >> GMFlags >> AccountFlags;
	ForcedPermissions = sLogonCommHandler.GetForcedPermissions(AccountName);
	if( ForcedPermissions != NULL )
		GMFlags.assign(ForcedPermissions->c_str());

	sLog.outDebug( " >> got information packet from logon: `%s` ID %u (request %u)", AccountName.c_str(), AccountID, mRequestID);

	mRequestID = 0;
	// Pull the session key.
//	uint8 K[40];
	recvData.read(K, 40);
	
	_crypt.Init(K);
	
	//checking if player is already connected
	//disconnect current player and login this one(blizzlike)

	if(recvData.rpos() != recvData.wpos())
		recvData.read((uint8*)lang.data(), 4);

	WorldSession *session = sWorld.FindSession( AccountID );
	if( session)
	{
		// AUTH_FAILED = 0x0D
		session->Disconnect();
		
		// clear the logout timer so he times out straight away
		session->SetLogoutTimer(1);

		// we must send authentication failed here.
		// the stupid newb can relog his client.
		// otherwise accounts dupe up and disasters happen.
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
		return;
	}

	Sha1Hash sha;

	uint8 digest[20];
	pAuthenticationPacket->read(digest, 20);

	uint32 t = 0;
	if( m_fullAccountName == NULL )				// should never happen !
		sha.UpdateData(AccountName);
	else
	{
		sha.UpdateData(*m_fullAccountName);
		
		// this is unused now. we may as well free up the memory.
		delete m_fullAccountName;
		m_fullAccountName = NULL;
	}

	sha.UpdateData((uint8 *)&t, 4);
	sha.UpdateData((uint8 *)&mClientSeed, 4);
	sha.UpdateData((uint8 *)&mSeed, 4);
	sha.UpdateData((uint8 *)&K, 40);
	sha.Finalize();

	if (memcmp(sha.GetDigest(), digest, 20))
	{
		// AUTH_UNKNOWN_ACCOUNT = 21
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
		return;
	}

	// Allocate session
	WorldSession * pSession = new WorldSession(AccountID, AccountName, this);
	mSession = pSession;
	Arcemu::Util::ARCEMU_ASSERT(   mSession != NULL );
	// aquire delete mutex
	pSession->deleteMutex.Acquire();
	
	// Set session properties
	pSession->SetClientBuild(mClientBuild);
	pSession->LoadSecurity(GMFlags);
	pSession->SetAccountFlags(AccountFlags);
	pSession->m_lastPing = (uint32)UNIXTIME;
	pSession->language = sLocalizationMgr.GetLanguageId(lang);

	if(recvData.rpos() != recvData.wpos())
		recvData >> pSession->m_muted;

	for(i = 0; i < 8; ++i)
		pSession->SetAccountData(i, NULL, true, 0);

	if(sWorld.m_useAccountData)
	{
		QueryResult * pResult = CharacterDatabase.Query("SELECT * FROM account_data WHERE acct = %u", AccountID);
		if( pResult == NULL )
			CharacterDatabase.Execute("INSERT INTO account_data VALUES(%u, '', '', '', '', '', '', '', '', '')", AccountID);
		else
		{
			size_t len;
			const char * data;
			char * d;
			for(i = 0; i < 8; ++i)
			{
				data = pResult->Fetch()[1+i].GetString();
				len = data ? strlen(data) : 0;
				if(len > 1)
				{
					d = new char[len+1];
					memcpy(d, data, len+1);
					pSession->SetAccountData(i, d, true, (uint32)len);
				}
			}

			delete pResult;
		}
	}

	Log.Debug("Auth", "%s from %s:%u [%ums]", AccountName.c_str(), GetRemoteIP().c_str(), GetRemotePort(), _latency);
#ifdef SESSION_CAP
	if( sWorld.GetSessionCount() >= SESSION_CAP )
	{
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		Disconnect();
		return;
	}
#endif

	// Check for queue.
	uint32 playerLimit = sWorld.GetPlayerLimit();
	if( (sWorld.GetSessionCount() < playerLimit) || pSession->HasGMPermissions() ) {
		Authenticate();
	} else if( playerLimit > 0 ){
		// Queued, sucker.
		uint32 Position = sWorld.AddQueuedSocket(this);
		mQueued = true;
		Log.Debug("Queue", "%s added to queue in position %u", AccountName.c_str(), Position);

		// Send packet so we know what we're doing
		UpdateQueuePosition(Position);
	} else {
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0E"); // AUTH_REJECT = 14
		Disconnect();
	}

	// release delete mutex
	pSession->deleteMutex.Release();
}

void WorldSocket::Authenticate()
{
	Arcemu::Util::ARCEMU_ASSERT(   pAuthenticationPacket != NULL );
	mQueued = false;

	if( mSession == NULL )
		return;

	if( mSession->HasFlag( ACCOUNT_FLAG_XPACK_02 ) )
		OutPacket( SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x02" );
	else if( mSession->HasFlag( ACCOUNT_FLAG_XPACK_01 ) )
		OutPacket( SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x01" );
	else
		OutPacket( SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x00" );

	sAddonMgr.SendAddonInfoPacket( pAuthenticationPacket, static_cast< uint32 >( pAuthenticationPacket->rpos() ), mSession );
	mSession->_latency = _latency;

	delete pAuthenticationPacket;
	pAuthenticationPacket = NULL;

	sWorld.AddSession( mSession );
	sWorld.AddGlobalSession( mSession );
	
	if( mSession->HasGMPermissions() )
		sWorld.gmList.insert( mSession );

}

void WorldSocket::UpdateQueuePosition(uint32 Position)
{
// cebernic: Displays re-correctly until 2.4.3,there will not be always 0
	WorldPacket QueuePacket(SMSG_AUTH_RESPONSE, 16);
	QueuePacket << uint8(0x1B) << uint8(0x2C) << uint8(0x73) << uint8(0) << uint8(0);
	QueuePacket << uint32(0) << uint8(0);// << uint8(0);
	QueuePacket << Position;
//	QueuePacket << uint8(1);
	SendPacket(&QueuePacket);
}

void WorldSocket::_HandlePing(WorldPacket* recvPacket)
{
	uint32 ping;
	if(recvPacket->size() < 4)
	{
		sLog.outError("Socket closed due to incomplete ping packet.");
		Disconnect();
		return;
	}

	*recvPacket >> ping;
	*recvPacket >> _latency;

	if(mSession)
	{
		mSession->_latency = _latency;
		mSession->m_lastPing = (uint32)UNIXTIME;

		// reset the move time diff calculator, don't worry it will be re-calculated next movement packet.
		mSession->m_clientTimeDelay = 0;
	}

	OutPacket(SMSG_PONG, 4, &ping);

#ifdef WIN32
	// Dynamically change nagle buffering status based on latency.
	//if(_latency >= 250)
	// I think 350 is better, in a MMO 350 latency isn't that big that we need to worry about reducing the number of packets being sent.
	if(_latency >= 350)
	{
		if(!m_nagleEanbled)
		{
			u_long arg = 0;
			setsockopt(GetFd(), 0x6, 0x1, (const char*)&arg, sizeof(arg));
			m_nagleEanbled = true;
		}
	}
	else
	{
		if(m_nagleEanbled)
		{
			u_long arg = 1;
			setsockopt(GetFd(), 0x6, 0x1, (const char*)&arg, sizeof(arg));
			m_nagleEanbled = false;
		}
	}
#endif
}

void WorldSocket::OnRead()
{
	for(;;)
	{
		// Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(readBuffer.GetSize() < 6)
			{
				// No header in the packet, let's wait.
				return;
			}

			// Copy from packet buffer into header local var
			ClientPktHeader Header;
			readBuffer.Read((uint8*)&Header, 6);

			// Decrypt the header
			_crypt.DecryptRecv((uint8*)&Header, sizeof (ClientPktHeader));

			mRemaining = mSize = ntohs(Header.size) - 4;
			mOpcode = Header.cmd;
		}

		WorldPacket * Packet;

		if(mRemaining > 0)
		{
			if( readBuffer.GetSize() < mRemaining )
			{
				// We have a fragmented packet. Wait for the complete one before proceeding.
				return;
			}
		}

		Packet = new WorldPacket( static_cast<uint16>( mOpcode ), mSize);
		Packet->resize(mSize);

		if(mRemaining > 0)
		{
			// Copy from packet buffer into our actual buffer.
			///Read(mRemaining, (uint8*)Packet->contents());
			readBuffer.Read((uint8*)Packet->contents(), mRemaining);
		}

		sWorldLog.LogPacket(mSize, static_cast<uint16>( mOpcode ), mSize ? Packet->contents() : NULL, 0, (mSession ? mSession->GetAccountId() : 0) );
		mRemaining = mSize = mOpcode = 0;

		// Check for packets that we handle
		switch(Packet->GetOpcode())
		{
		case CMSG_PING:
			{
				_HandlePing(Packet);
				delete Packet;
			}break;
		case CMSG_AUTH_SESSION:
			{
				_HandleAuthSession(Packet);
			}break;
		default:
			{
				if(mSession) mSession->QueuePacket(Packet);
				else delete Packet;
			}break;
		}
	}
}



void WorldLog::LogPacket(uint32 len, uint16 opcode, const uint8* data, uint8 direction, uint32 accountid)
{
#ifdef ECHO_PACKET_LOG_TO_CONSOLE
	sLog.outString("[%s]: %s %s (0x%03X) of %u bytes.", direction ? "SERVER" : "CLIENT", direction ? "sent" : "received",
		LookupName(opcode, g_worldOpcodeNames), opcode, len);
#endif

	if(bEnabled)
	{
		mutex.Acquire();
		unsigned int line = 1;
		unsigned int countpos = 0;
		uint16 lenght = static_cast<uint16>( len );
		unsigned int count = 0;

		fprintf(m_file, "{%s} Packet: (0x%04X) %s PacketSize = %u stamp = %u accountid = %u\n", (direction ? "SERVER" : "CLIENT"), opcode,
			LookupName(opcode, g_worldOpcodeNames), lenght, getMSTime(), accountid );
		fprintf(m_file, "|------------------------------------------------|----------------|\n");
		fprintf(m_file, "|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|\n");
		fprintf(m_file, "|------------------------------------------------|----------------|\n");

		if(lenght > 0)
		{
			fprintf(m_file, "|");
			for (count = 0 ; count < lenght ; count++)
			{
				if (countpos == 16)
				{
					countpos = 0;

					fprintf(m_file, "|");

					for (unsigned int a = count-16; a < count;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(m_file, ".");
						else
							fprintf(m_file, "%c",data[a]);
					}

					fprintf(m_file, "|\n");

					line++;
					fprintf(m_file, "|");
				}

				fprintf(m_file, "%02X ",data[count]);

				//FIX TO PARSE PACKETS WITH LENGTH < OR = TO 16 BYTES.
				if (count+1 == lenght && lenght <= 16)
				{
					for (unsigned int b = countpos+1; b < 16;b++)
						fprintf(m_file, "   ");

					fprintf(m_file, "|");

					for (unsigned int a = 0; a < lenght;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(m_file, ".");
						else
							fprintf(m_file, "%c",data[a]);
					}

					for (unsigned int c = count; c < 15;c++)
						fprintf(m_file, " ");

					fprintf(m_file, "|\n");
				}

				//FIX TO PARSE THE LAST LINE OF THE PACKETS WHEN THE LENGTH IS > 16 AND ITS IN THE LAST LINE.
				if (count+1 == lenght && lenght > 16)
				{
					for (unsigned int b = countpos+1; b < 16;b++)
						fprintf(m_file, "   ");

					fprintf(m_file, "|");

					unsigned short print = 0;

					for (unsigned int a = line * 16 - 16; a < lenght;a++)
					{
						if ((data[a] < 32) || (data[a] > 126))
							fprintf(m_file, ".");
						else
							fprintf(m_file, "%c",data[a]);

						print++;
					}

					for (unsigned int c = print; c < 16;c++)
						fprintf(m_file, " ");

					fprintf(m_file, "|\n");
				}

				countpos++;
			}
		}
		fprintf(m_file, "-------------------------------------------------------------------\n\n");
		fflush(m_file);
		mutex.Release();
	}
}
