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

void WorldSession::HandleConvertGroupToRaidOpcode(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	// This is just soooo easy now   
	Group *pGroup = _player->GetGroup();
	if(!pGroup) return;

	if ( pGroup->GetLeader() != _player->m_playerInfo )   //access denied
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	pGroup->ExpandToRaid();
	SendPartyCommandResult(_player, 0, "", ERR_PARTY_NO_ERROR);
}

void WorldSession::HandleGroupChangeSubGroup(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	std::string name;
	uint8 subGroup;

	recv_data >> name;
	recv_data >> subGroup;

	PlayerInfo * inf = objmgr.GetPlayerInfoByName(name.c_str());
	if(inf == NULL || inf->m_Group == NULL || inf->m_Group != _player->m_playerInfo->m_Group)
		return;

	_player->GetGroup()->MovePlayer(inf, subGroup);
}

void WorldSession::HandleGroupAssistantLeader(WorldPacket & recv_data)
{
	uint64 guid;
	uint8 on;

	if(!_player->IsInWorld())
		return;

	if(_player->GetGroup() == NULL)
		return;

	if ( _player->GetGroup()->GetLeader() != _player->m_playerInfo )   //access denied
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	recv_data >> guid >> on;
	if(on == 0)
        _player->GetGroup()->SetAssistantLeader(NULL);
	else
	{
		PlayerInfo * np = objmgr.GetPlayerInfo((uint32)guid);
		if(np==NULL)
			_player->GetGroup()->SetAssistantLeader(NULL);
		else
		{
			if(_player->GetGroup()->HasMember(np))
				_player->GetGroup()->SetAssistantLeader(np);
		}
	}
}

void WorldSession::HandleGroupPromote(WorldPacket& recv_data)
{
	uint8 promotetype, on;
	uint64 guid;

	if(!_player->IsInWorld())
		return;

	if(_player->GetGroup() == NULL)
		return;

	if ( _player->GetGroup()->GetLeader() != _player->m_playerInfo )   //access denied
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	recv_data >> promotetype >> on;
	recv_data >> guid;

	void(Group::*function_to_call)(PlayerInfo*);

	if(promotetype == 0)
		function_to_call = &Group::SetMainTank;
	else if(promotetype==1)
		function_to_call = &Group::SetMainAssist;

	if(on == 0)
		(_player->GetGroup()->*function_to_call)(NULL);
	else
	{
		PlayerInfo * np = objmgr.GetPlayerInfo((uint32)guid);
		if(np==NULL)
			(_player->GetGroup()->*function_to_call)(NULL);
		else
		{
			if(_player->GetGroup()->HasMember(np))
				(_player->GetGroup()->*function_to_call)(np);
		}
	}
}

void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket & recv_data)
{  
	//		  SMSG_RAID_INSTANCE_INFO			 = 716,  //(0x2CC)	
	//sInstanceSavingManager.BuildRaidSavedInstancesForPlayer(_player);
	sInstanceMgr.BuildRaidSavedInstancesForPlayer(_player);
}

void WorldSession::HandleReadyCheckOpcode(WorldPacket& recv_data)
{
	Group * pGroup  = _player->GetGroup();
	WorldPacket data(CMSG_RAID_READYCHECK, 20);
	uint8 ready;

	if(!pGroup || ! _player->IsInWorld())
		return;

	if(recv_data.size() == 0)
	{
		if(pGroup->GetLeader() == _player->m_playerInfo)
		{
			/* send packet to group */
			pGroup->SendPacketToAllButOne(&data, _player);
		}
		else
		{
			SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
		}
	}
	else
	{
		if(_player->m_playerInfo != pGroup->GetLeader())
		{
			recv_data >> ready;
			data << _player->GetGUID();
			data << ready;
			if(pGroup->GetLeader() && pGroup->GetLeader()->m_loggedInPlayer)
				pGroup->GetLeader()->m_loggedInPlayer->GetSession()->SendPacket(&data);
		}
	}
}

