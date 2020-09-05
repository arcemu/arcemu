/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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
#include "LFGPacketHandlers.h"

DEFINE_PACKET_HANDLER_METHOD( LFGProposalResultPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	Arcemu::GamePackets::LFG::CLFGProposalResult result;
	result.deserialize( recv_data );

	LOG_DEBUG( "Received proposal result" );

	/// If experimental LFG is not enabled, don't do anything
	if( !Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		return;
	}

	sLfgMgr.updateProposal( result.proposalId, _player->GetLowGUID(), result.result );
}

DEFINE_PACKET_HANDLER_METHOD( LFGSetCommentHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	Arcemu::GamePackets::LFG::CLFGSetComment packet;
	packet.deserialize( recv_data );

	_player->Lfgcomment = packet.comment;

	LOG_DEBUG( "Received set comment message" );
}

DEFINE_PACKET_HANDLER_METHOD( LFGPlayerInfoHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	LOG_DEBUG( "Received LFG player info request." );

	PacketBuffer response( SMSG_LFG_PLAYER_INFO, 5 );
	response << uint8( 0 );
	response << uint32( 0 );
	_player->SendPacket( &response );

	LOG_DEBUG( "Sent (empty) LFG player info response." );
}

DEFINE_PACKET_HANDLER_METHOD( LFGPartyInfoHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	LOG_DEBUG( "Received LFG party info request." );

	PacketBuffer response( SMSG_LFG_PARTY_INFO, 1 );
	response << uint8( 0 );
	_player->SendPacket( &response );

	LOG_DEBUG( "Sent (empty) LFG party info response." );
}

DEFINE_PACKET_HANDLER_METHOD( LFGJoinHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	Arcemu::GamePackets::LFG::CLFGJoin packet;
	packet.deserialize( recv_data );

	if( packet.dungeons.size() == 0 )
	{
		LOG_DEBUG( "Received LFG join request without dungeons." );
		return;
	}

	LOG_DEBUG( "Received LFG join request. Roles: %u Dungeon1: %u Type1: %u", packet.roles, packet.dungeons[ 0 ].dungeon, packet.dungeons[ 0 ].unk2 );

	// If experimental LFG is not enabled, just send an internal LFG error message
	if( !Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		PacketBuffer buffer;
		Arcemu::GamePackets::LFG::SLFGJoinResult response;
		response.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_INTERNAL_ERROR;
		response.state = 0;
		response.serialize( buffer );
		_player->SendPacket( &buffer );
		
		LOG_DEBUG( "Sent LFG join result" );

		return;
	}

	std::vector< uint32 > dungeons;
	std::vector< Arcemu::GamePackets::LFG::CLFGJoin::LFGDungeon >::const_iterator itr = packet.dungeons.begin();
	while( itr != packet.dungeons.end() )
	{
		dungeons.push_back( itr->dungeon );
		++itr;
	}

	sLfgMgr.addPlayer( _player->GetLowGUID(), packet.roles, dungeons );
}

DEFINE_PACKET_HANDLER_METHOD( LFGLeaveHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	LOG_DEBUG( "Received LFG leave request." );

	/// If experimental LFG is not enabled, don't do anything
	if( !Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		return;
	}

	sLfgMgr.removePlayer( _player->GetLowGUID() );
}

DEFINE_PACKET_HANDLER_METHOD( LFGTeleportHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	Arcemu::GamePackets::LFG::CLFGTeleport packet;
	packet.deserialize( recv_data );

	LOG_DEBUG( "Received LFG teleport request." );


	/// If experimental LFG is not enabled, don't do anything
	if( !Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		return;
	}

	if( packet.out == 1 )
		sLfgMgr.teleportPlayer( _player->GetLowGUID(), true );
	else
	if( packet.out == 0 )
		sLfgMgr.teleportPlayer( _player->GetLowGUID(), false );
}

