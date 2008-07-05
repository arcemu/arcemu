<<<<<<< .working
<<<<<<< .working
/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
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

	void Create( uint32 itemid, Player *owner );
	void LoadFromDB( Field*fields);

	bool AddItem(int8 slot, Item *item);
	bool AddItemToFreeSlot(Item *pItem, uint32 * r_slot);
	Item *GetItem(int8 slot)
	{
		if((uint8)slot < GetProto()->ContainerSlots)
			return m_Slot[slot];
		else
			return 0;
	}
	
	int8 FindFreeSlot();
	bool HasItems();
	
	void SwapItems(int8 SrcSlot,int8 DstSlot);
	Item *SafeRemoveAndRetreiveItemFromSlot(int8 slot, bool destroy); //doesnt destroy item from memory
	bool SafeFullRemoveItemFromSlot(int8 slot); //destroys item fully
   
	void SaveBagToDB(int8 slot, bool first, QueryBuffer * buf);

protected:
	Item **m_Slot;
	uint32 __fields[CONTAINER_END];
};

#endif
=======
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

	void Create( uint32 itemid, Player *owner );
	void LoadFromDB( Field*fields);

	bool AddItem(int8 slot, Item *item);
	bool AddItemToFreeSlot(Item *pItem, uint32 * r_slot);
	Item *GetItem(int8 slot)
	{
		if((uint8)slot < GetProto()->ContainerSlots)
			return m_Slot[slot];
		else
			return 0;
	}
	
	int8 FindFreeSlot();
	bool HasItems();
	
	void SwapItems(int8 SrcSlot,int8 DstSlot);
	Item *SafeRemoveAndRetreiveItemFromSlot(int8 slot, bool destroy); //doesnt destroy item from memory
	bool SafeFullRemoveItemFromSlot(int8 slot); //destroys item fully
   
	void SaveBagToDB(int8 slot, bool first, QueryBuffer * buf);

protected:
	Item **m_Slot;
	uint32 __fields[CONTAINER_END];
};

#endif
>>>>>>> .merge-right.r249
=======
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

	void Create( uint32 itemid, Player *owner );
	void LoadFromDB( Field*fields);

	bool AddItem(int8 slot, Item *item);
	bool AddItemToFreeSlot(Item *pItem, uint32 * r_slot);
	Item *GetItem(int8 slot)
	{
		if((uint8)slot < GetProto()->ContainerSlots)
			return m_Slot[slot];
		else
			return 0;
	}
	
	int8 FindFreeSlot();
	bool HasItems();
	
	void SwapItems(int8 SrcSlot,int8 DstSlot);
	Item *SafeRemoveAndRetreiveItemFromSlot(int8 slot, bool destroy); //doesnt destroy item from memory
	bool SafeFullRemoveItemFromSlot(int8 slot); //destroys item fully
   
	void SaveBagToDB(int8 slot, bool first, QueryBuffer * buf);

protected:
	Item **m_Slot;
	uint32 __fields[CONTAINER_END];
};

#endif
>>>>>>> .merge-right.r249
