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
		namespace Account
		{
			SAccountDataTimes::SAccountDataTimes()
			{
			}

			void SAccountDataTimes::serialize( PacketBuffer &buffer ) const
			{
				buffer.Initialize( SMSG_ACCOUNT_DATA_TIMES );
				buffer << uint32( time );
				buffer << uint8( unk1 );
				buffer << uint32( mask );
				
				for( uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i )
				{
					if( mask & ( 1 << i ) )
					{
						buffer << uint32( times[ i ] );
					}
				}
			}

			void SAccountDataTimes::deserialize( PacketBuffer & buffer )
			{
				buffer >> time;
				buffer >> unk1;
				buffer >> mask;
				
				for( uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i )
				{
					if( mask & ( 1 << i ) )
					{
						buffer >> times[ i ];
					}
					else
					{
						times[ i ] = 0;
					}
				}
			}
		}
	}
}

