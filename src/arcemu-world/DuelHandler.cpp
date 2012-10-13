/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

void WorldSession::HandleDuelAccepted(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->DuelingWith == NULL)
		return;

	if(_player->m_duelState != DUEL_STATE_FINISHED)
		return;

	if(_player->m_duelCountdownTimer > 0)
		return;

	_player->m_duelStatus = DUEL_STATUS_INBOUNDS;
	_player->DuelingWith->m_duelStatus = DUEL_STATUS_INBOUNDS;

	_player->m_duelState = DUEL_STATE_STARTED;
	_player->DuelingWith->m_duelState = DUEL_STATE_STARTED;

	WorldPacket data(SMSG_DUEL_COUNTDOWN, 4);
	data << uint32(3000);

	SendPacket(&data);
	_player->DuelingWith->m_session->SendPacket(&data);

	_player->m_duelCountdownTimer = 3000;

	sEventMgr.AddEvent(_player, &Player::DuelCountdown, EVENT_PLAYER_DUEL_COUNTDOWN, 1000, 3, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void WorldSession::HandleDuelCancelled(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->DuelingWith ==  NULL)
		return;

	if(_player->m_duelState == DUEL_STATE_STARTED)
	{
		_player->DuelingWith->EndDuel(DUEL_WINNER_KNOCKOUT);
		return;
	}

	WorldPacket data(SMSG_DUEL_COMPLETE, 1);
	data << uint8(1);

	SendPacket(&data);
	_player->DuelingWith->m_session->SendPacket(&data);

	GameObject* arbiter = _player->GetMapMgr() ? _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(_player->GetDuelArbiter())) : NULL;
	if(arbiter != NULL)
	{
		arbiter->RemoveFromWorld(true);
		delete arbiter;
	}

	_player->DuelingWith->SetDuelArbiter(0);
	_player->DuelingWith->SetDuelTeam(0);
	_player->DuelingWith->m_duelState = DUEL_STATE_FINISHED;
	_player->DuelingWith->m_duelCountdownTimer = 0;
	_player->DuelingWith->DuelingWith = NULL;

	_player->SetDuelArbiter(0);
	_player->SetDuelTeam(0);
	_player->m_duelState = DUEL_STATE_FINISHED;
	_player->m_duelCountdownTimer = 0;
	_player->DuelingWith = NULL;

	for(uint32 x = MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; x++)
	{
		if(_player->m_auras[x])
		{
			_player->m_auras[x]->Remove();
		}
	}
	std::list<Pet*> summons = _player->GetSummons();
	for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
	{
		if((*itr)->isAlive())
		{
			(*itr)->SetPetAction(PET_ACTION_STAY);
			(*itr)->SetPetAction(PET_ACTION_FOLLOW);
		}
	}
}
