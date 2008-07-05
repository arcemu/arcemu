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

initialiseSingleton( ChannelMgr );

void WorldSession::HandleChannelJoin(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	uint32 dbc_id = 0;
	uint16 crap;		// crap = some sort of channel type?
	uint32 i;
	Channel * chn;

	recvPacket >> dbc_id >> crap;
	recvPacket >> channelname;
	recvPacket >> pass;

	if(!stricmp(channelname.c_str(), "LookingForGroup") && !sWorld.m_lfgForNonLfg)
	{
		// make sure we have lfg dungeons
		for(i = 0; i < 3; ++i)
		{
			if(_player->LfgDungeonId[i] != 0)
				break;
		}

		if(i == 3)
			return;		// don't join lfg
	}

	if( sWorld.GmClientChannel.size() && !stricmp(sWorld.GmClientChannel.c_str(), channelname.c_str()) && !GetPermissionCount())
		return;
	
	chn = channelmgr.GetCreateChannel(channelname.c_str(), _player, dbc_id);
	if(chn == NULL)
		return;

	chn->AttemptJoin(_player, pass.c_str());
	Log.Debug("ChannelJoin", "%s", channelname.c_str());
}

void WorldSession::HandleChannelLeave(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	uint32 code = 0;
	Channel * chn;

	recvPacket >> code;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn == NULL)
		return;

	chn->Part(_player);
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;

	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn != NULL)
		chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	recvPacket >> pass;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if(chn)
		chn->Password(_player, pass.c_str());
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->SetOwner(_player, plr);
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if(chn)
		chn->GetOwner(_player);
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->GiveModerator(_player, plr);
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->TakeModerator(_player, plr);
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Mute(_player, plr);
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Unmute(_player, plr);
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Invite(_player, plr);
}
void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Kick(_player, plr, false);
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Kick(_player, plr, true);
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	PlayerInfo * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayerInfoByName(newp.c_str());
	if(chn && plr)
		chn->Unban(_player, plr);
}

void WorldSession::HandleChannelAnnounce(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn)
		chn->Announce(_player);
}

void WorldSession::HandleChannelModerate(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn)
		chn->Moderate(_player);
}

void WorldSession::HandleChannelRosterQuery(WorldPacket & recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn)
		chn->List(_player);
}

void WorldSession::HandleChannelNumMembersQuery(WorldPacket & recvPacket)
{
	string channel_name;
	WorldPacket data(SMSG_CHANNEL_NUM_MEMBERS_QUERY_RESPONSE, recvPacket.size() + 4);
	Channel *chn;
	recvPacket >> channel_name;
	chn = channelmgr.GetChannel(channel_name.c_str(), _player);
	if(chn)
	{
		data << channel_name;
		data << uint8(chn->m_flags);
		data << uint32(chn->GetNumMembers());
		SendPacket(&data);
	}
}
