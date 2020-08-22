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
