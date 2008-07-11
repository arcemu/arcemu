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

void WorldSession::HandleEnableMicrophoneOpcode(WorldPacket & recv_data)
{
	uint8 enabled, unk;
	recv_data >> enabled;
	recv_data >> unk;

	if(!enabled)
	{
		/*
		{SERVER} Packet: (0x039F) UNKNOWN PacketSize = 16
		|------------------------------------------------|----------------|
		|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
		|------------------------------------------------|----------------|
		|F3 0D 13 01 00 00 00 00 4A C5 00 00 00 00 D1 E1 |........J.......|
		-------------------------------------------------------------------
		uint64 player_guid
		uint64 channel_id
		*/
	}
	else
	{
		/*
		{SERVER} Packet: (0x03D8) UNKNOWN PacketSize = 24
		|------------------------------------------------|----------------|
		|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
		|------------------------------------------------|----------------|
		|4B C5 00 00 00 00 D1 E1 00 6D 6F 6F 63 6F 77 00 |K........moocow.|
		|F3 0D 13 01 00 00 00 00                         |........        |
		-------------------------------------------------------------------
		uint8 channel_id?
		uint8 channel_type (00=custom channel, 03=party, 04=raid?)
		string channel_name (not applicable to party/raid)
		uint64 player_guid
		*/
	}
}

void WorldSession::HandleChannelVoiceQueryOpcode(WorldPacket & recv_data)
{
	string name;
	recv_data >> name;

	// custom channel
	Channel * chn = channelmgr.GetChannel(name.c_str(), _player);
	if(chn == NULL || chn->voice_enabled == false || chn->m_general)
		return;

	WorldPacket data(SMSG_CHANNEL_NOTIFY_AVAILABLE_VOICE_SESSION, 17+chn->m_name.size());
	data << uint32(0x00002e57);
	data << uint32(0xe0e10000);
	data << uint8(0);		// 00=custom,03=party,04=raid
	data << chn->m_name;
	data << _player->GetGUID();
	SendPacket(&data);
}

void WorldSession::HandleVoiceChatQueryOpcode(WorldPacket & recv_data)
{
	if(!sVoiceChatHandler.CanUseVoiceChat())
		return;

	uint32 type;
	string name;
	recv_data >> type >> name;
	if(type == 0)
	{
		// custom channel
		Channel * chn = channelmgr.GetChannel(name.c_str(), _player);
		if(chn == NULL)
			return;

		if(chn->m_general || !chn->voice_enabled)
			return;

		chn->JoinVoiceChannel(_player);
	}
}



/************************************************************************/
/* Singleton Stuff                                                      */
/************************************************************************/
initialiseSingleton(VoiceChatHandler);

VoiceChatHandler::VoiceChatHandler()
{
	request_high=1;
	next_connect = 0;
	m_client = 0;
	port =0;
	ip = 0;
	enabled=false;
}

void VoiceChatHandler::OnRead(WorldPacket* pck)
{
	uint32 request_id;
	uint8 error;
	uint16 channel_id = 0;

	switch(pck->GetOpcode())
	{
	case VOICECHAT_SMSG_PONG:
		{
			printf("!! VOICECHAT PONGZ!\n");
			m_client->last_pong = UNIXTIME;
		}break;
	case VOICECHAT_SMSG_CHANNEL_CREATED:
		{
			*pck >> request_id;
			*pck >> error;

			Log.Debug("VoiceChatHandler", "Request ID %u, error %u", request_id, (int)error);
			for(vector<VoiceChatChannelRequest>::iterator itr = m_requests.begin(); itr != m_requests.end(); ++itr)
			{
				if(itr->id == request_id)
				{
					// zero = error, wehn we pass it to OnChannelCreated
					if( error == 0 )
						*pck >> channel_id;

					VoiceChannel * chn = new VoiceChannel;
					chn->channelId = channel_id;
					chn->type = itr->groupid ? 3 : 0;
					chn->team = itr->team;

					if( itr->groupid == 0 )
					{
						Channel * chan = channelmgr.GetChannel(itr->channel_name.c_str(), itr->team);
						if(chan != NULL)
						{
							chn->miscPointer = chan;
							m_voiceChannels.insert(make_pair((uint32)channel_id, chn));
							chan->VoiceChannelCreated(channel_id);
						}
						else
							delete chn;
					}
					else
					{
						Group * grp = objmgr.GetGroupById( itr->groupid );
						if( grp != NULL )
						{
							chn->miscPointer = grp;
							m_voiceChannels.insert(make_pair((uint32)channel_id, chn));
							grp->VoiceChannelCreated(channel_id);
						}
						else
							delete chn;
					}

					m_requests.erase(itr);
					break;
				}
			}
		}break;
	}
}

void VoiceChatHandler::SocketDisconnected()
{
	m_lock.Acquire();

	Log.Debug("VoiceChatHandler", "SocketDisconnected");
	m_client = NULL;
	m_requests.clear();

	WorldPacket data(SMSG_VOICE_SYSTEM_STATUS, 2);
	data << uint8(2);
	data << uint8(0);
	sWorld.SendGlobalMessage(&data, NULL);
	next_connect = UNIXTIME + 5;

	sWorld.SendWorldText("Channel/Party voice services are now offline.");

	// notify channels
	for(HM_NAMESPACE::hash_map<uint32, VoiceChannel*>::iterator itr = m_voiceChannels.begin(); itr != m_voiceChannels.end(); ++itr)
	{
		if( itr->second->type == 3 )		// party
			((Group*)itr->second->miscPointer)->VoiceSessionDropped();

		delete itr->second;
	}
	m_voiceChannels.clear();

	m_lock.Release();
}

bool VoiceChatHandler::CanUseVoiceChat()
{
	return (enabled && m_client != NULL);
}

void VoiceChatHandler::CreateVoiceChannel(Channel * chn)
{
	if(m_client == NULL)
		return;

	Log.Debug("VoiceChatHandler", "CreateVoiceChannel %s", chn->m_name.c_str());
	VoiceChatChannelRequest req;

	m_lock.Acquire();

	req.id = request_high++;
	req.channel_name = chn->m_name;
	req.team = chn->m_team;
	req.groupid = 0;
	m_requests.push_back(req);

	WorldPacket data(VOICECHAT_CMSG_CREATE_CHANNEL, 5);
	data << (uint8)0;
	data << req.id;
	m_client->SendPacket(&data);
	m_lock.Release();
}

void VoiceChatHandler::CreateGroupChannel(Group * pGroup)
{
	if( m_client == NULL )
		return;

	Log.Debug("VoiceChatHandler", "CreateGroupChannel for group %u", pGroup->GetID());
	ByteBuffer buf(50);
	VoiceChatChannelRequest req;

	m_lock.Acquire();

	req.id = request_high++;
	req.groupid = pGroup->GetID();
	req.team = 0;

	m_requests.push_back( req );

	WorldPacket data(VOICECHAT_CMSG_CREATE_CHANNEL, 5);
	data << (uint8)3;
	data << req.id;
	m_client->SendPacket(&data);

	m_lock.Release();
}

void VoiceChatHandler::DestroyGroupChannel(Group * pGroup)
{
	if( pGroup->m_voiceChannelId <= 0 )
		return;

	m_lock.Acquire();
	HM_NAMESPACE::hash_map<uint32, VoiceChannel*>::iterator itr = m_voiceChannels.find(pGroup->m_voiceChannelId);
	if( itr != m_voiceChannels.end() )
	{
		// cleanup channel structure
		m_voiceChannels.erase( itr );
	}

	if( m_client == NULL )
	{
		m_lock.Release();
		return;
	}

	WorldPacket data(VOICECHAT_CMSG_DELETE_CHANNEL, 5);
	data << (uint8)3;
	data << (uint16)pGroup->m_voiceChannelId;
	m_client->SendPacket(&data);

	m_lock.Release();
}

void VoiceChatHandler::ActivateChannelSlot(uint16 channel_id, uint8 slot_id)
{
	if( m_client == NULL )
		return;

	Log.Debug("VoiceChatHandler", "Channel %u activate slot %u", (int)channel_id, (int)slot_id);

	m_lock.Acquire();

	WorldPacket data(VOICECHAT_CMSG_ADD_MEMBER, 5);
	data << channel_id;
	data << slot_id;
	m_client->SendPacket(&data);

	m_lock.Release();
}

void VoiceChatHandler::DeactivateChannelSlot(uint16 channel_id, uint8 slot_id)
{
	if( m_client == NULL )
		return;

	Log.Debug("VoiceChatHandler", "Channel %u deactivate slot %u", (int)channel_id, (int)slot_id);

	m_lock.Acquire();

	WorldPacket data(VOICECHAT_CMSG_REMOVE_MEMBER, 5);
	data << channel_id;
	data << slot_id;
	m_client->SendPacket(&data);

	m_lock.Release();
}


void VoiceChatHandler::DestroyVoiceChannel(Channel * chn)
{
	Log.Debug("VoiceChatHandler", "DestroyVoiceChannel %s", chn->m_name.c_str());

	if(chn->i_voice_channel_id != (uint16)-1 && m_client)
	{
		ByteBuffer buf(15);
		buf << uint32(VOICECHAT_CMSG_DELETE_CHANNEL);
		buf << uint32(chn->i_voice_channel_id);
		m_client->Send(buf.contents(), 8);
	}

	chn->VoiceDied();
}

void VoiceChatHandler::Startup()
{
	ip_s = Config.MainConfig.GetStringDefault("VoiceChat", "ServerIP", "127.0.0.1");
	port = Config.MainConfig.GetIntDefault("VoiceChat", "ServerPort", 3727);
	enabled = Config.MainConfig.GetBoolDefault("VoiceChat", "Enabled", false);
	if(!enabled)
		return;

	ip = inet_addr(ip_s.c_str());
	next_connect = 0;
	Update();
}

void VoiceChatHandler::Update()
{
	if(!enabled)
		return;

	if( m_client == NULL )
	{
		if(UNIXTIME > next_connect)
		{
			Log.Notice("VoiceChatHandler", "Attempting to connect to voicechat server %s:%u", ip_s.c_str(), port);
			VoiceChatClientSocket * s = ConnectTCPSocket<VoiceChatClientSocket>(ip_s.c_str(), port);
			if(s != NULL)
			{
				// connected!
				m_client = s;
				Log.Notice("VoiceChatHandler", "Connected to %s:%u.", ip_s.c_str(), port);
				WorldPacket data(SMSG_VOICE_SYSTEM_STATUS, 2);
				data << uint8(2) << uint8(1);
				sWorld.SendGlobalMessage(&data, NULL);

				objmgr.GroupVoiceReconnected();
				sWorld.SendWorldText("Voice services are back online.");
			}
			else
			{
				Log.Notice("VoiceChatHandler", "Could not connect. Will try again later.");
				m_client = NULL;
				next_connect = UNIXTIME + 10;
			}
		}
	}
	else
	{
		if( UNIXTIME >= m_client->next_ping )
		{
			m_client->next_ping = UNIXTIME + 15;
			WorldPacket data(VOICECHAT_CMSG_PING, 4);
			data << uint32(0);
			m_client->SendPacket(&data);
		}
		// because the above might kill m_client
		if( m_client != NULL && (UNIXTIME - m_client->last_pong) > (15 * 3) )
		{
			// ping timeout
			printf("ping timeout on voice socket\n");
			m_client->Disconnect();
		}
	}
}

#else			// VOICE_CHAT


void WorldSession::HandleEnableMicrophoneOpcode(WorldPacket & recv_data)
{

}

void WorldSession::HandleChannelVoiceQueryOpcode(WorldPacket & recv_data)
{

}

void WorldSession::HandleVoiceChatQueryOpcode(WorldPacket & recv_data)
{

}

#endif			// VOICE_CHAT
