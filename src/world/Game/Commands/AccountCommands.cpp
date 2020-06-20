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

void SendAccountDataTimesCommand::execute()
{
	Arcemu::GamePackets::Account::SAccountDataTimes packet;
	packet.time = uint32( UNIXTIME );
	packet.unk1 = uint8( 1 );
	packet.mask = uint32( mask );
	memset( &packet.times, 0, sizeof( packet.times ) );
	
	PacketBuffer buffer;
	packet.serialize( buffer );
	session->SendPacket( &buffer );
}
