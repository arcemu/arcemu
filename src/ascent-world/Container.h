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
