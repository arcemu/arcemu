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

void WorldSession::HandleSetLookingForGroupComment(WorldPacket& recvPacket)
{
	std::string comment;
		
	recvPacket >> comment;
	
	GetPlayer()->Lfgcomment = comment;	
}

void WorldSession::HandleEnableAutoJoin(WorldPacket& recvPacket)
{
	uint32 i;

	// make sure they're not queued in any invalid cases
	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			if(LfgDungeonTypes[_player->LfgDungeonId[i]] != LFG_INSTANCE && LfgDungeonTypes[_player->LfgDungeonId[i]] != LFG_HEROIC_DUNGEON)
			{
				return;
			}
		}
	}

	// enable autojoin, join any groups if possible.
	_player->m_Autojoin = true;
	
	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			_player->SendMeetingStoneQueue(_player->LfgDungeonId[i], 1);
			sLfgMgr.UpdateLfgQueue(_player->LfgDungeonId[i]);
		}
	}
}

void WorldSession::HandleDisableAutoJoin(WorldPacket& recvPacket)
{
	uint32 i;
	_player->m_Autojoin = false;

	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			if(LfgDungeonTypes[_player->LfgDungeonId[i]] == LFG_INSTANCE || LfgDungeonTypes[_player->LfgDungeonId[i]] == LFG_HEROIC_DUNGEON)
				_player->SendMeetingStoneQueue(_player->LfgDungeonId[i], 0);
		}
	}
}

void WorldSession::HandleEnableAutoAddMembers(WorldPacket& recvPacket)
{
	uint32 i;
	_player->m_AutoAddMem = true;

	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			sLfgMgr.UpdateLfgQueue(_player->LfgDungeonId[i]);
		}
	}
}

void WorldSession::HandleDisableAutoAddMembers(WorldPacket& recvPacket)
{
	_player->m_AutoAddMem = false;	
}

void WorldSession::HandleMsgLookingForGroup(WorldPacket& recvPacket)
{
	/* this is looking for more */
	uint32 LfgType,LfgDungeonId,unk1;
	recvPacket >> LfgType >> LfgDungeonId >> unk1;
	
	if(LfgDungeonId > MAX_DUNGEONS)
		return;

	if(LfgDungeonId)
		sLfgMgr.SendLfgList(_player, LfgDungeonId);
}

void WorldSession::HandleSetLookingForGroup(WorldPacket& recvPacket)
{
	uint32 LfgQueueId;
	uint16 LfgDungeonId;
	uint8 LfgType,unk1;
	uint32 i;
	
	recvPacket >> LfgQueueId >> LfgDungeonId >> unk1 >> LfgType;
	
	if(LfgDungeonId >= MAX_DUNGEONS || LfgQueueId >= MAX_LFG_QUEUE_ID || LfgType != LfgDungeonTypes[LfgDungeonId])		// last one is for cheaters
		return;
	
	if(_player->LfgDungeonId[LfgQueueId] != 0)
		sLfgMgr.RemovePlayerFromLfgQueue(_player, _player->LfgDungeonId[LfgQueueId]);
	
	_player->LfgDungeonId[LfgQueueId]=LfgDungeonId;
	_player->LfgType[LfgQueueId]=LfgType;

	if(LfgDungeonId)
	{
		sLfgMgr.SetPlayerInLFGqueue(_player, LfgDungeonId);

		if(LfgType == LFG_HEROIC_DUNGEON || LfgType == LFG_INSTANCE)
		{
			sLfgMgr.UpdateLfgQueue(LfgDungeonId);
			if(_player->m_Autojoin)
				_player->SendMeetingStoneQueue(LfgDungeonId, 1);
		}
	}
	else
	{
		for(i = 0; i < 3; ++i)
		{
			if(_player->LfgDungeonId[i] != 0)
				break;
		}

		if( i == 3 )
			_player->PartLFGChannel();
	}
}

void WorldSession::HandleSetLookingForMore(WorldPacket& recvPacket)
{
	uint16 LfgDungeonId;
	uint8 LfgType,unk1;

	recvPacket >> LfgDungeonId >> unk1 >> LfgType;
	if( LfgDungeonId >= MAX_DUNGEONS )
		return;

	// remove from an existing queue
	if(LfgDungeonId != _player->LfmDungeonId && _player->LfmDungeonId)
		sLfgMgr.RemovePlayerFromLfmList(_player, _player->LfmDungeonId);

	_player->LfmDungeonId = LfgDungeonId;
	_player->LfmType = LfgType;

	if(LfgDungeonId)
		sLfgMgr.SetPlayerInLfmList(_player, LfgDungeonId);
}

void WorldSession::HandleLfgClear(WorldPacket & recvPacket)
{
	sLfgMgr.RemovePlayerFromLfgQueues(_player);
}

void WorldSession::HandleLfgInviteAccept(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
	
	_player->PartLFGChannel();
	if(_player->m_lfgMatch == NULL && _player->m_lfgInviterGuid == 0)
	{
		if(_player->m_lfgMatch == NULL)
			OutPacket(SMSG_LFG_MATCHMAKING_AUTOJOIN_FAILED_NO_PLAYER);		// Matched Player(s) have gone offline.
		else
			OutPacket(SMSG_LFG_MATCHMAKING_AUTOJOIN_FAILED);				// Group no longer available.

		return;
	}

	if( _player->m_lfgMatch != NULL )
	{
		// move into accepted players
		_player->m_lfgMatch->lock.Acquire();
		_player->m_lfgMatch->PendingPlayers.erase(_player);

		if( !_player->GetGroup() )
		{
			_player->m_lfgMatch->AcceptedPlayers.insert(_player);

			if(!_player->m_lfgMatch->PendingPlayers.size())
			{
				// all players have accepted
				Group * pGroup = new Group(true);
				for(set<Player*>::iterator itr = _player->m_lfgMatch->AcceptedPlayers.begin(); itr != _player->m_lfgMatch->AcceptedPlayers.end(); ++itr)
					pGroup->AddMember((*itr)->m_playerInfo);

				_player->m_lfgMatch->pGroup = pGroup;
			}
		}
		_player->m_lfgMatch->lock.Release();
	}
	else
	{
		Player * pPlayer = objmgr.GetPlayer(_player->m_lfgInviterGuid);
		if( pPlayer == NULL )
		{
			OutPacket(SMSG_LFG_MATCHMAKING_AUTOJOIN_FAILED_NO_PLAYER);			// Matched Player(s) have gone offline.
			return;
		}

		if( pPlayer->GetGroup() == NULL || pPlayer->GetGroup()->IsFull() || pPlayer->GetGroup()->GetLeader() != pPlayer->m_playerInfo )
		{
			OutPacket(SMSG_LFG_MATCHMAKING_AUTOJOIN_FAILED);
			return;
		}

		pPlayer->GetGroup()->AddMember(_player->m_playerInfo);
	}
	_player->m_lfgInviterGuid = 0;
	_player->m_lfgMatch = NULL;
}
