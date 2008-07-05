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

void WorldSession::HandleDuelAccepted(WorldPacket & recv_data)
{
	if( !_player->IsInWorld() )
		return;

	if( _player->DuelingWith == NULL )
		return;

	if( _player->m_duelState != DUEL_STATE_FINISHED )
		return;
	
	if( _player->m_duelCountdownTimer > 0 )
		return;

	_player->m_duelStatus = DUEL_STATUS_INBOUNDS;
	_player->DuelingWith->m_duelStatus = DUEL_STATUS_INBOUNDS;

	_player->m_duelState = DUEL_STATE_STARTED;
	_player->DuelingWith->m_duelState = DUEL_STATE_STARTED;

	WorldPacket data( SMSG_DUEL_COUNTDOWN, 4 );
	data << uint32( 3000 );

	SendPacket( &data );
	_player->DuelingWith->m_session->SendPacket( &data );

	_player->m_duelCountdownTimer = 3000;

	sEventMgr.AddEvent(_player, &Player::DuelCountdown, EVENT_PLAYER_DUEL_COUNTDOWN, 1000, 3,0);
}

void WorldSession::HandleDuelCancelled(WorldPacket & recv_data)
{
	if( _player->DuelingWith ==  NULL )
		return;

	if( _player->m_duelState == DUEL_STATE_STARTED )
	{
		_player->DuelingWith->EndDuel( DUEL_WINNER_KNOCKOUT );
		return;
	}

	WorldPacket data( SMSG_DUEL_COMPLETE, 1 );
	data << uint8( 1 );

	SendPacket( &data );
	_player->DuelingWith->m_session->SendPacket( &data );

	GameObject* arbiter = _player->GetMapMgr() ? _player->GetMapMgr()->GetGameObject( _player->GetUInt32Value( PLAYER_DUEL_ARBITER ) ) : NULL;
	if( arbiter != NULL )
	{
		arbiter->RemoveFromWorld( true );
		delete arbiter;
 	}

	_player->DuelingWith->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
	_player->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );

	_player->DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
	_player->SetUInt32Value( PLAYER_DUEL_TEAM, 0);

	_player->DuelingWith->m_duelState = DUEL_STATE_FINISHED;
	_player->m_duelState = DUEL_STATE_FINISHED;

	_player->DuelingWith->m_duelCountdownTimer = 0;
	_player->m_duelCountdownTimer = 0;

	_player->DuelingWith->DuelingWith = NULL;
	_player->DuelingWith = NULL;

}
