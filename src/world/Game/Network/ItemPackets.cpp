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
		namespace Items
		{
			void CItemRefundInfo::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( CMSG_ITEMREFUNDINFO );
				buffer << uint64( guid );
			}

			void CItemRefundInfo::deserialize( PacketBuffer &buffer )
			{
				buffer >> guid;
			}

			
			void SItemRefundInfo::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_ITEMREFUNDINFO );
				buffer << uint64( guid );
				buffer << uint32( buyprice );
				buffer << uint32( honor );
				buffer << uint32( arena );

				for( uint32 i = 0; i < MAX_ITEM_COUNT; i++ )
				{
					buffer << uint32( item[ i ] );
					buffer << uint32( item_count[ i ] );
				}

				buffer << uint32( unk1 );
				buffer << uint32( remaining_time );
			}

			void SItemRefundInfo::deserialize( PacketBuffer &buffer )
			{
				buffer >> guid;
				buffer >> buyprice;
				buffer >> honor;
				buffer >> arena;

				for( uint32 i = 0; i < MAX_ITEM_COUNT; i++ )
				{
					buffer >> item[ i ];
					buffer >> item_count[ i ];
				}

				buffer >> unk1;
				buffer >> remaining_time;
			}

			void CItemRefundRequest::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( CMSG_ITEMREFUNDREQUEST );
				buffer << guid;
			}

			void CItemRefundRequest::deserialize( PacketBuffer &buffer )
			{
				buffer >> guid;
			}

			void SItemRefundRequest::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_ITEMREFUNDREQUEST );
				buffer << uint64( guid );
				buffer << uint32( error );
				
				if( error == 0 )
				{
					buffer << uint32( buyprice );
					buffer << uint32( honor );
					buffer << uint32( arena );
					
					for( int i = 0; i < MAX_ITEM_COUNT; i++ )
					{
						buffer << uint32( item[i] );
						buffer << uint32( item_count[i] );
					}
				}
			}

			void SItemRefundRequest::deserialize( PacketBuffer &buffer )
			{
				buffer >> guid;
				buffer >> error;

				if( error == 0 )
				{
					buffer >> buyprice;
					buffer >> honor;
					buffer >> arena;

					for( int i = 0; i < MAX_ITEM_COUNT; i++ )
					{
						buffer >> item[ i ];
						buffer >> item_count[ i ];
					}
				}
			}

			void SSellItem::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_SELL_ITEM );
				buffer << uint64( vendorGuid );
				buffer << uint64( itemGuid );
				buffer << uint8( result );
			}

			void SSellItem::deserialize( PacketBuffer &buffer )
			{
				buffer >> vendorGuid;
				buffer >> itemGuid;
				buffer >> result;
			}
		}
	}
}
