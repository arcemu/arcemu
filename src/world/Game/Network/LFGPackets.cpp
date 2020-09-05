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
					buffer << uint16( dungeons[ i ].dungeon );
					buffer << uint8( dungeons[ i ].unk1 );
					buffer << uint8( dungeons[ i ].unk2 );
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
				
				for( uint8 i = 0; i < count; i++ )
				{
					LFGDungeon d;
					buffer >> d.dungeon;
					buffer >> d.unk1;
					buffer >> d.unk2;
					
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

					for( size_t i = 0; i < dungeons.size(); i++ )
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

			void SLFGUpdateParty::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_LFG_UPDATE_PARTY );

				buffer << uint8( updateType );

				if( dungeons.size() > 0 )
					buffer << uint8( 1 );
				else
					buffer << uint8( 0 );

				if( dungeons.size() > 0 )
				{
					buffer << uint8( joined );
					buffer << uint8( queued );
					buffer << uint8( unk1 );
					buffer << uint8( unk2 );

					for( int i = 0; i < 3; i++ )
					{
						buffer << uint8( 0 );
					}

					buffer << uint8( dungeons.size() );

					for( size_t i = 0; i < dungeons.size(); i++ )
					{
						buffer << uint32( dungeons[ i ] );
					}

					buffer << comment;
				}
			}

			void SLFGUpdateParty::deserialize( PacketBuffer& buffer )
			{
				uint8 hasDungeons;
				uint8 dungeonsCount;
				uint8 something;

				buffer >> updateType;
				buffer >> hasDungeons;				
				
				if( hasDungeons )
				{
					buffer >> joined;
					buffer >> queued;
					buffer >> unk1;
					buffer >> unk2;

					for( int i = 0; i < 3; i++ )
						buffer >> something;

					buffer >> dungeonsCount;
					
					for( int i = 0; i < dungeonsCount; i++ )
					{
						uint32 dungeon;
						buffer >> dungeon;
						dungeons.push_back( dungeon );
					}

					buffer >> comment;
				}
			}

			void SLFGProposalUpdate::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_LFG_PROPOSAL_UPDATE );
				buffer << uint32( dungeonId );
				buffer << uint8( proposalState );
				buffer << uint32( proposalId );
				buffer << uint32( encountersFinishedMask );
				buffer << uint8( silent );
				buffer << uint8( entries.size() );

				for( size_t i = 0; i < entries.size(); i++ )
				{
					const LFGProposalEntry &entry = entries[ i ];
					buffer << uint32( entry.roleMask );
					buffer << uint8( entry.isCurrentPlayer );
					buffer << uint8( entry.inDungeon );
					buffer << uint8( entry.inSameGroup );
					buffer << uint8( entry.hasAnswered );
					buffer << uint8( entry.hasAccepted );
				}
			}

			void SLFGProposalUpdate::deserialize( PacketBuffer& buffer )
			{
				buffer >> dungeonId;
				buffer >> proposalState;
				buffer >> proposalId;
				buffer >> encountersFinishedMask;
				buffer >> silent;

				uint8 entryCount;
				buffer >> entryCount;

				for( uint8 i = 0; i < entryCount; i++ )
				{
					LFGProposalEntry entry;
					buffer >> entry.roleMask;
					buffer >> entry.isCurrentPlayer;
					buffer >> entry.inDungeon;
					buffer >> entry.inSameGroup;
					buffer >> entry.hasAnswered;
					buffer >> entry.hasAccepted;

					entries.push_back( entry );
				}
			}

			void CLFGProposalResult::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( CMSG_LFG_PROPOSAL_RESULT );
				buffer << uint32( proposalId );
				buffer << uint8( result );
			}

			void CLFGProposalResult::deserialize( PacketBuffer& buffer )
			{
				buffer >> proposalId;
				buffer >> result;
			}

			void CLFGSetComment::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( CMSG_LFG_SET_COMMENT );
				buffer << comment;
			}

			void CLFGSetComment::deserialize( PacketBuffer& buffer )
			{
				buffer >> comment;
			}

			void CLFGTeleport::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( CMSG_LFG_TELEPORT );
				buffer << uint8( out );
			}

			void CLFGTeleport::deserialize( PacketBuffer& buffer )
			{
				buffer >> out;
			}

			void SLFGTeleportDenied::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_LFG_TELEPORT_DENIED );
				buffer << uint32( reason );
			}

			void SLFGTeleportDenied::deserialize( PacketBuffer &buffer )
			{
				buffer >> reason;
			}

			void SLFGQueueStatus::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_LFG_QUEUE_STATUS );
				buffer << uint32( dungeon );
				buffer << int32( avgWaitTime );
				buffer << int32( waitTime );
				buffer << int32( waitTimeTank );
				buffer << int32( waitTimeHealer );
				buffer << int32( waitTimeDps );
				buffer << uint8( tanksNeeded );
				buffer << uint8( healersNeeded );
				buffer << uint8( dpsNeeded );
				buffer << uint32( queueTime );
			}

			void SLFGQueueStatus::deserialize( PacketBuffer &buffer )
			{
				buffer >> dungeon;
				buffer >> waitTime;
				buffer >> avgWaitTime;
				buffer >> waitTimeTank;
				buffer >> waitTimeHealer;
				buffer >> waitTimeDps;
				buffer >> tanksNeeded;
				buffer >> healersNeeded;
				buffer >> dpsNeeded;
				buffer >> queueTime;
			}
		}
	}
}

