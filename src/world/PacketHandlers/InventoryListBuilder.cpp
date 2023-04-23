/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

void InventoryListBuilder::build( WorldPacket &data )
{
	PlayerItemPrototypeFilter filter( _player );

	Vendor *vendor = creature->getVendor();
	const std::vector< VendorItem > &vendorItems = vendor->getVendorItems();
		
	data.Initialize( SMSG_LIST_INVENTORY );
	data.reserve( ( vendorItems.size() * 28) + 9 );

	data << uint64( creature->GetGUID() );
	data << uint8( 0 );   // item count

	uint8 count = 0;	

	std::vector< VendorItem >::const_iterator itr = vendorItems.begin();		
	while( itr != vendorItems.end() )
	{
		if( ( itr->itemid != 0 ) && ( itr->max_amount == 0 || ( itr->max_amount > 0 && itr->available_amount > 0 ) ) )
		{
			const ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itr->itemid);
			if( proto != NULL )
			{
				if( !filter.filter( proto ) )
				{
					++itr;
					continue;
				}
					
				uint32 av_am = ( itr->max_amount > 0 ) ? itr->available_amount : 0xFFFFFFFF;
				uint32 price = 0;

				if( ( itr->extended_cost == NULL ) || proto->HasFlag2( ITEM_FLAG2_EXT_COST_REQUIRES_GOLD ) )
					price = GetBuyPriceForItem( proto, 1, _player, creature );
					
				data << uint32( count + 1 );    // client starts counting from 1
				data << uint32( proto->ItemId );
				data << uint32( proto->DisplayInfoID );
				data << uint32( av_am );
				data << uint32( price );
				data << uint32( proto->MaxDurability );
				data << uint32( itr->amount );
					
				if( itr->extended_cost != NULL )
					data << uint32( itr->extended_cost->costid );
				else
					data << uint32( 0 );
					
				++count;

				if ( count >= MAX_CREATURE_INV_ITEMS )
					break;
			}
		}

		++itr;
	}

	const_cast< uint8* >( data.contents() )[ 8 ] = count;
}
