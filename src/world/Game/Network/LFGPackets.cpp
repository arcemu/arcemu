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
			void CLFGJoin::serialize( PacketBuffer &buffer ) const
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

			void CLFGJoin::deserialize( PacketBuffer& buffer )
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

			void SLFGUpdatePlayer::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_LFG_UPDATE_PLAYER );
				buffer << uint8( updateType );

				if( dungeons.size() > 0 )
					buffer << uint8( 1 );
				else
					buffer << uint8( 0 );
				
				if( dungeons.size() > 0 )
				{
					buffer << uint8( queued );
					buffer << uint8( unk1 );
					buffer << uint8( unk2 );
					buffer << uint8( dungeons.size() );

					for( int i = 0; i < dungeons.size(); i++ )
					{
						buffer << uint32( dungeons[ i ] );
					}

					buffer << comment;
				}
			}

			void SLFGUpdatePlayer::deserialize( PacketBuffer& buffer )
			{
				uint8 joined = 0;
				uint8 dungeonCount = 0;

				buffer >> updateType;
				buffer >> joined;

				if( joined )
				{
					buffer >> queued;
					buffer >> unk1;
					buffer >> unk2;
					buffer >> dungeonCount;

					for( int i = 0; i < dungeonCount; i++ )
					{
						uint32 dungeon;
						buffer >> dungeon;
						dungeons.push_back( dungeon );
					}

					buffer >> comment;
				}
			}
		}
	}
}

