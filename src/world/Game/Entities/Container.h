/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef _CONTAINER_H
#define _CONTAINER_H

class SERVER_DECL Container : public Item
{
	public:
		friend class WorldSession;
		Container(uint32 high, uint32 low);
		~Container();

		void Create(uint32 itemid, Player* owner);
		void LoadFromDB(Field* fields);

		bool AddItem(int16 slot, Item* item);
		bool AddItemToFreeSlot(Item* pItem, uint32* r_slot);
		Item* GetItem(int16 slot)
		{
			if(slot >= 0 && (uint16)slot < GetProto()->ContainerSlots)
				return m_Slot[slot];
			else
				return 0;
		}

		int8 FindFreeSlot();
		bool HasItems();

		void SwapItems(int8 SrcSlot, int8 DstSlot);
		Item* SafeRemoveAndRetreiveItemFromSlot(int16 slot, bool destroy); //doesn't destroy item from memory
		bool SafeFullRemoveItemFromSlot(int16 slot); //destroys item fully

		void SaveBagToDB(int8 slot, bool first, QueryBuffer* buf);
		void SetNumSlots(uint32 num) { SetUInt32Value(CONTAINER_FIELD_NUM_SLOTS, num); }
		uint32 GetNumSlots() { return GetUInt32Value(CONTAINER_FIELD_NUM_SLOTS); }
		void SetSlot(uint16 slot, uint64 guid) { SetUInt64Value(CONTAINER_FIELD_SLOT_1 + (slot * 2), guid); }
		uint64 GetSlot(uint16 slot) { return GetUInt64Value(CONTAINER_FIELD_SLOT_1 + (slot * 2)); }

	protected:
		Item** m_Slot;
		uint32 __fields[CONTAINER_END];
};

#endif
