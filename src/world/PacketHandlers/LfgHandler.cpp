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

void WorldSession::HandleLFGPlayerInfo(WorldPacket & recvPacket)
{
	LOG_DEBUG( "Received LFG player info request." );

	PacketBuffer response( SMSG_LFG_PLAYER_INFO, 5 );
	response << uint8( 0 );
	response << uint32( 0 );
	SendPacket( &response );

	LOG_DEBUG( "Sent LFG player info response." );
}

void WorldSession::HandleLFGPartyInfo(WorldPacket & recvPacket)
{
	LOG_DEBUG( "Received LFG party info request." );

	PacketBuffer response( SMSG_LFG_PARTY_INFO, 1 );
	response << uint8( 0 );
	SendPacket( &response );

	LOG_DEBUG( "Sent LFG party info response." );
}

void WorldSession::HandleLFGJoin(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	Arcemu::GamePackets::LFG::CLFGJoin packet;
	packet.deserialize( recvPacket );

	LOG_DEBUG( "Received LFG join request." );

	PacketBuffer buffer;
	Arcemu::GamePackets::LFG::SLFGJoinResult response;
	response.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_INTERNAL_ERROR;
	response.state = 0;
	response.serialize( buffer );
	SendPacket( &buffer );

	LOG_DEBUG( "Sent LFG join result" );
}

void WorldSession::HandleLFGLeave(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	LOG_DEBUG( "Received LFG leave request." );
}

void WorldSession::HandleEnableAutoAddMembers(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleDisableAutoAddMembers(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleMsgLookingForGroup(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleSetLookingForGroup(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleSetLookingForMore(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleLfgClear(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleMeetingStoneInfo(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN
}

void WorldSession::HandleLfgInviteAccept(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN;
}
