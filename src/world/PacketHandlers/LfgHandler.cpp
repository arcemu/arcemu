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

void WorldSession::HandleLFGJoin(WorldPacket & recvPacket)
{
	CHECK_INWORLD_RETURN

	Arcemu::GamePackets::LFG::CLFGJoin packet;
	packet.deserialize( recvPacket );

	LOG_DEBUG( "Received LFG join request." );

	// If experimental LFG is not enabled, just send an internal LFG error message
	if( !Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		PacketBuffer buffer;
		Arcemu::GamePackets::LFG::SLFGJoinResult response;
		response.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_INTERNAL_ERROR;
		response.state = 0;
		response.serialize( buffer );
		SendPacket( &buffer );
		
		LOG_DEBUG( "Sent LFG join result" );

		return;
	}
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
