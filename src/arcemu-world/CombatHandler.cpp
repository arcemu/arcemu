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

void WorldSession::HandleAttackSwingOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	if(!guid)
	{
		// does this mean cancel combat?
		HandleAttackStopOpcode(recv_data);
		return;
	}

	// AttackSwing
	Log.Debug( "WORLD","Recvd CMSG_ATTACKSWING Message" );

	if(GetPlayer()->IsPacified() || GetPlayer()->IsStunned() || GetPlayer()->IsFeared())
		return;

//	printf("Got ATTACK SWING: %08X %08X\n", GUID_HIPART(guid), GUID_LOPART(guid));
	Unit *pEnemy = _player->GetMapMgr()->GetUnit(guid);
	//printf("Pointer: %08X\n", pEnemy);

	if(!pEnemy)
	{
		sLog.outDebug("WORLD: "I64FMT" does not exist.", guid);
		return;
	}

	if(pEnemy->isDead() || _player->isDead())		// haxors :(
		return;

	GetPlayer()->smsg_AttackStart(pEnemy);
	GetPlayer()->EventAttackStart();

	// Set PVP Flag.
	/*if(pEnemy->IsPlayer() && isHostile(_player, pEnemy))
	{
		// don't in duel.. this should be done in dealdamage anyway :S
		if( static_cast< Player* >( pEnemy )->GetTeam() != _player->GetTeam() )
			_player->SetPvPFlag();
	}*/
}

void WorldSession::HandleAttackStopOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid = GetPlayer()->GetSelection();
	Unit *pEnemy = NULL;

	if(guid)
	{
		pEnemy = _player->GetMapMgr()->GetUnit(guid);
		if(pEnemy)
		{
			GetPlayer()->EventAttackStop();
			GetPlayer()->smsg_AttackStop(pEnemy);

			/*WorldPacket data(SMSG_ATTACKSTOP, 20);
			data << _player->GetNewGUID();
			data << uint8(0);
			data << uint32(0);
			SendPacket(&data);*/
		}
	}
}

