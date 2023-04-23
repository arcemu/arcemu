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

#ifndef ARCEMU_VENDOR_H
#define ARCEMU_VENDOR_H

/// A single item in the Vendor NPC's items list
struct VendorItem
{
	uint32 itemid;
	uint32 amount; //!!!!! stack amount.
	uint32 available_amount;
	uint32 max_amount;
	uint32 incrtime;
	ItemExtendedCostEntry* extended_cost;

	VendorItem()
	{
		itemid = 0;
		amount = 0;
		available_amount = 0;
		max_amount = 0;
		incrtime = 0;
		extended_cost = NULL;
	}

	/// Substract the specified amount from the number of available items
	void takeAmount( uint32 value )
	{
		available_amount -= value;
	}

	/// Restock the item: restore the avaible item count to maximum
	void restock()
	{
		available_amount = max_amount;
	}
};

/// Component that provides vendor functionality for NPCs
class SERVER_DECL Vendor
{
public:
	Vendor( std::vector< VendorItem > *items );
	~Vendor();

	/// Tells if the vendor has the specified item
	bool hasItem( uint32 id );

	/// Retrieves data of the specified item
	void GetSellItemByItemId( uint32 itemid, VendorItem &ci );

	/// Returns the item extended cost of the specified item
	ItemExtendedCostEntry* GetItemExtendedCostByItemId( uint32 itemid );

	/// Adds a new item to the vendor
	void addItem( uint32 itemid, uint32 amount, ItemExtendedCostEntry* ec );

	//// Removes the specified item from the vendor
	void removeItem( uint32 id );

	/// Removes all items from the vendor
	void removeAllItems();

	/// Takes the specified amount from the specified limited item
	void takeItemAmount( uint32 itemid, uint32 value );

	/// Restocks items that are past their restock time
	void restockItems();

	/// Sets the available amount for the specified item to maximum
	void restockItem( uint32 id );

	/// Returns the item list of this vendor
	const std::vector< VendorItem >& getVendorItems() const { return *items; }

private:
	/// The vendor's item list
	std::vector< VendorItem >* items;

	/// Map that contains the restock times of limited vendor items
	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > itemRestockTimes;
};

#endif
