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
	function_to_call = 0;

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

	if(!pGroup || !_player->IsInWorld())
		return;

	if(recv_data.size() == 0)
	{
		if(pGroup->GetLeader() == _player->m_playerInfo)
		{
			WorldPacket data(CMSG_RAID_READYCHECK, 8);
			data << GetPlayer()->GetGUID();
			/* send packet to group */
			pGroup->SendPacketToAll(&data);
		}
		else
		{
			SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
		}
	}
	else
	{
		uint8 ready;
		recv_data >> ready;

		WorldPacket data(MSG_RAID_READY_CHECK_CONFIRM, 9);
		data << _player->GetGUID();
		data << ready;

		if(pGroup->GetLeader() && pGroup->GetLeader()->m_loggedInPlayer)
			pGroup->GetLeader()->m_loggedInPlayer->GetSession()->SendPacket(&data);
	}
}

