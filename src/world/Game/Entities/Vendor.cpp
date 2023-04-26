/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2023 Ascent Team <http://www.ascentemu.com/>
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

Vendor::Vendor( std::vector< VendorItem > *items )
{
	this->items = items;
}

Vendor::~Vendor()
{
	itemRestockTimes.clear();
	items = NULL;
}

bool Vendor::hasItem( uint32 id )
{
	for( std::vector< VendorItem >::iterator itr = items->begin(); itr != items->end(); ++itr )
	{
		if( itr->itemid == id )
			return true;
	}
	
	return false;
}

void Vendor::GetSellItemByItemId( uint32 itemid, VendorItem &ci )
{
	for( std::vector< VendorItem >::iterator itr = items->begin(); itr != items->end(); ++itr )
	{
		if( itr->itemid == itemid )
		{
			ci = (*itr);
			return;
		}
	}
}

ItemExtendedCostEntry* Vendor::GetItemExtendedCostByItemId( uint32 itemid )
{
	for( std::vector< VendorItem >::iterator itr = items->begin(); itr != items->end(); ++itr )
	{
		if( itr->itemid == itemid )
			return itr->extended_cost;
	}
	
	return NULL;
}

void Vendor::addItem( uint32 itemid, uint32 amount, ItemExtendedCostEntry* ec )
{
	VendorItem ci;
	ci.amount = amount;
	ci.itemid = itemid;
	ci.available_amount = 0;
	ci.max_amount = 0;
	ci.incrtime = 0;
	ci.extended_cost = ec;

	items->push_back( ci );
}

void Vendor::removeItem( uint32 id )
{
	for( std::vector< VendorItem >::iterator itr = items->begin(); itr != items->end(); ++itr )
	{
		if( itr->itemid == id )
		{
			items->erase( itr );
			break;
		}
	}

	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::iterator itr = itemRestockTimes.find( id );
	if( itr != itemRestockTimes.end() )
	{
		itemRestockTimes.erase( itr );
	}
}

void Vendor::removeAllItems()
{
	items->clear();
	itemRestockTimes.clear();
}

void Vendor::takeItemAmount( uint32 itemid, uint32 value )
{
	for( std::vector< VendorItem >::iterator itr = items->begin(); itr != items->end(); ++itr )
	{
		if( itr->itemid == itemid )
		{
			itr->takeAmount( value );
			
			HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::iterator itr2 = itemRestockTimes.find( itemid );
			if( itr2 == itemRestockTimes.end() )
			{
				itemRestockTimes.insert( std::pair< uint32, uint32 >( itemid, static_cast< uint32 >( UNIXTIME + ( itr->incrtime / 1000 ) ) ) );
			}
			break;
		}
	}
}

void Vendor::restockItems()
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::iterator itr = itemRestockTimes.begin();
	while( itr != itemRestockTimes.end() )
	{
		uint32 itemId = itr->first;
		uint32 restockTime = itr->second;

		++itr;

		if( UNIXTIME >= restockTime )
		{
			restockItem( itemId );
		}
	}
}

void Vendor::restockItem( uint32 id )
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::iterator eraseItr;

	for( std::vector< VendorItem >::iterator itr = items->begin(); itr != items->end(); ++itr )
	{
		if( itr->itemid == id )
		{
			itr->restock();

			eraseItr = itemRestockTimes.find( id );
			if( eraseItr != itemRestockTimes.end() )
			{
				itemRestockTimes.erase( eraseItr );
			}

			return;
		}
	}
}

