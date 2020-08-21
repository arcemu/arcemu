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

namespace Arcemu
{
	namespace GamePackets
	{
		namespace LFG
		{
			void CLFGAutoJoin::serialize( PacketBuffer &buffer ) const
			{
				buffer << uint32( roles );
				buffer << uint8( unk1 );
				buffer << uint8( unk2 );
				buffer << uint8( count );

				for( int i = 0; i < count; i++ )
				{
					buffer << uint8( dungeons[ i ].dungeon );
					buffer << uint8( dungeons[ i ].unk1 );
					buffer << uint8( dungeons[ i ].unk2 );
					buffer << uint8( dungeons[ i ].unk3 );
				}

				buffer << unk3;
				buffer << unk4;
				buffer << unk5;
				buffer << unk6;
				buffer << comment;
			}

			void CLFGAutoJoin::deserialize( PacketBuffer& buffer )
			{
				buffer >> roles;
				buffer >> unk1;
				buffer >> unk2;
				buffer >> count;
				
				for( int i = 0; i < count; i++ )
				{
					LFGDungeon d;
					buffer >> d.dungeon;
					buffer >> d.unk1;
					buffer >> d.unk2;
					buffer >> d.unk3;
					
					dungeons.push_back( d );
				}
				
				buffer >> unk3;
				buffer >> unk4;
				buffer >> unk5;
				buffer >> unk6;
				buffer >> comment;
			}

			void SLFGJoinResult::serialize( PacketBuffer &buffer ) const
			{
				buffer.SetOpcode( SMSG_LFG_JOIN_RESULT );
				buffer << uint32( result );
				buffer << uint32( state );
			}
			
			void SLFGJoinResult::deserialize( PacketBuffer& buffer )
			{
				buffer >> result;
				buffer >> state;
			}
		}
	}
}

