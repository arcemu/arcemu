/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"

Container::Container(uint32 high,uint32 low) : Item()
{
	m_objectTypeId = TYPEID_CONTAINER;
	m_valuesCount = CONTAINER_END;
	m_uint32Values = __fields;
	memset(m_uint32Values, 0,(CONTAINER_END)*sizeof(uint32));
	m_updateMask.SetCount(CONTAINER_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_CONTAINER|TYPE_ITEM|TYPE_OBJECT);
	SetUInt32Value( OBJECT_FIELD_GUID,low);
	SetUInt32Value( OBJECT_FIELD_GUID+1,high);
	m_wowGuid.Init(GetGUID());

	SetFloatValue( OBJECT_FIELD_SCALE_X, 1 );//always 1


	m_Slot = NULL;
	random_suffix=random_prop=0;
}

Container::~Container( )
{
   for(uint32 i = 0; i < m_itemProto->ContainerSlots; i++)
	{
		if(m_Slot[i] && m_Slot[i]->GetOwner() == m_owner)
		{
			m_Slot[i]->DeleteMe();
		}
	}

	delete [] m_Slot;
}
void Container::LoadFromDB( Field*fields )
{

	uint32 itemid=fields[2].GetUInt32();
	m_itemProto = ItemPrototypeStorage.LookupEntry( itemid );

	ASSERT(m_itemProto);
	SetUInt32Value( OBJECT_FIELD_ENTRY, itemid );
	

	SetUInt32Value( ITEM_FIELD_CREATOR, fields[5].GetUInt32() );
	SetUInt32Value( ITEM_FIELD_STACK_COUNT, 1);
	
	SetUInt32Value( ITEM_FIELD_FLAGS, fields[8].GetUInt32());
	SetUInt32Value( ITEM_FIELD_RANDOM_PROPERTIES_ID, fields[9].GetUInt32());

	SetUInt32Value( ITEM_FIELD_MAXDURABILITY, m_itemProto->MaxDurability);
	SetUInt32Value( ITEM_FIELD_DURABILITY, fields[12].GetUInt32());
  

	SetUInt32Value( CONTAINER_FIELD_NUM_SLOTS, m_itemProto->ContainerSlots);

	m_Slot = new Item*[m_itemProto->ContainerSlots];
	memset(m_Slot, 0, sizeof(Item*)*(m_itemProto->ContainerSlots));

}

void Container::Create( uint32 itemid, Player *owner )
{

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemid );
	ASSERT(m_itemProto);

	SetUInt32Value( OBJECT_FIELD_ENTRY, itemid );

	// TODO: this shouldnt get NULL form containers in mail fix me
	if( owner != NULL )
	{
		SetUInt64Value( ITEM_FIELD_OWNER, owner->GetGUID() );
		SetUInt64Value( ITEM_FIELD_CONTAINED, owner->GetGUID() );
	}
	SetUInt32Value( ITEM_FIELD_STACK_COUNT, 1 );
	SetUInt32Value( CONTAINER_FIELD_NUM_SLOTS, m_itemProto->ContainerSlots);

	m_Slot = new Item*[m_itemProto->ContainerSlots];
	memset(m_Slot, 0, sizeof(Item*)*(m_itemProto->ContainerSlots));

	m_owner = owner;
}


int8 Container::FindFreeSlot()
{
	int8 TotalSlots = static_cast<int8>(GetUInt32Value( CONTAINER_FIELD_NUM_SLOTS ));
	for (int8 i=0; i < TotalSlots; i++)
	{
		if(!m_Slot[i]) 
		{ 
			return i; 
		}
	}
	sLog.outDebug( "Container::FindFreeSlot: no slot available" );
	return ITEM_NO_SLOT_AVAILABLE;
}

bool Container::HasItems()
{
	int8 TotalSlots = static_cast<int8>(GetUInt32Value( CONTAINER_FIELD_NUM_SLOTS ));
	for (int8 i=0; i < TotalSlots; i++)
	{
		if(m_Slot[i]) 
		{ 
			return true; 
		}
	}
	return false;
}

bool Container::AddItem(int8 slot, Item *item)
{
	if((uint32)slot > m_itemProto->ContainerSlots)
		return false;

	//ASSERT(m_Slot[slot] == NULL);
	if(m_Slot[slot] != NULL)
	{
		//sLog.outString("Bad container item %u slot %d", item->GetGUID(), slot);
		return false;
	}

	if (!m_owner)
		return false;

	m_Slot[slot] = item;
	item->m_isDirty = true;

	item->SetUInt64Value(ITEM_FIELD_CONTAINED, GetGUID());
	item->SetOwner(m_owner);

	if (item->GetProto()->Bonding == ITEM_BIND_ON_PICKUP) 
		item->SoulBind();

	SetUInt64Value(CONTAINER_FIELD_SLOT_1  + (slot*2), item->GetGUID());

	//new version to fix bag issues
	if(m_owner->IsInWorld() && !item->IsInWorld())
	{
		//item->AddToWorld();
		item->PushToWorld(m_owner->GetMapMgr());

		ByteBuffer buf(2500);
		uint32 count = item->BuildCreateUpdateBlockForPlayer(&buf, m_owner);
		m_owner->PushCreationData(&buf, count);
	}
	return true;
}

void Container::SwapItems(int8 SrcSlot, int8 DstSlot)
{
	Item *temp;
	if( SrcSlot < 0 || SrcSlot >= (int8)m_itemProto->ContainerSlots )
		return;
	
	if( DstSlot < 0 || DstSlot >= (int8)m_itemProto->ContainerSlots )
		return;
	
	if(m_Slot[DstSlot] &&  m_Slot[SrcSlot]&&m_Slot[DstSlot]->GetEntry()==m_Slot[SrcSlot]->GetEntry() && m_Slot[SrcSlot]->wrapped_item_id == 0 && m_Slot[DstSlot]->wrapped_item_id == 0 && m_Slot[DstSlot]->GetProto()->MaxCount>1)
	{
		uint32 total=m_Slot[SrcSlot]->GetUInt32Value(ITEM_FIELD_STACK_COUNT)+m_Slot[DstSlot]->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
		m_Slot[DstSlot]->m_isDirty = m_Slot[SrcSlot]->m_isDirty = true;
		if(total<=m_Slot[DstSlot]->GetProto()->MaxCount)
		{
			m_Slot[DstSlot]->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT,m_Slot[SrcSlot]->GetUInt32Value(ITEM_FIELD_STACK_COUNT));
			SafeFullRemoveItemFromSlot(SrcSlot);
			return;
		}
		else
		{
			if(m_Slot[DstSlot]->GetUInt32Value(ITEM_FIELD_STACK_COUNT) == m_Slot[DstSlot]->GetProto()->MaxCount)
			{

			}
			else
			{
				int32 delta=m_Slot[DstSlot]->GetProto()->MaxCount-m_Slot[DstSlot]->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
				m_Slot[DstSlot]->SetUInt32Value(ITEM_FIELD_STACK_COUNT,m_Slot[DstSlot]->GetProto()->MaxCount);
				m_Slot[SrcSlot]->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT,-delta);
				return;
			}
		}
	}
   
	temp = m_Slot[SrcSlot];
	m_Slot[SrcSlot] = m_Slot[DstSlot];
	m_Slot[DstSlot] = temp;

	if( m_Slot[DstSlot])
	{
		SetUInt64Value(CONTAINER_FIELD_SLOT_1  + (DstSlot*2),  m_Slot[DstSlot]->GetGUID()  );
		m_Slot[DstSlot]->m_isDirty = true;
	}
	else
	{
		SetUInt64Value(CONTAINER_FIELD_SLOT_1  + (DstSlot*2), 0 );
	}

	if( m_Slot[SrcSlot])
	{
		SetUInt64Value(CONTAINER_FIELD_SLOT_1  + (SrcSlot*2), m_Slot[SrcSlot]->GetGUID() );
		m_Slot[SrcSlot]->m_isDirty = true;
	}
	else
	{
		SetUInt64Value(CONTAINER_FIELD_SLOT_1  + (SrcSlot*2), 0 );
	}
}

Item *Container::SafeRemoveAndRetreiveItemFromSlot(int8 slot, bool destroy)
{
	ASSERT((uint32)slot < GetProto()->ContainerSlots);

	Item *pItem = m_Slot[slot];

	if (pItem == NULL || pItem==this) return NULL;
	m_Slot[slot] = NULL;

	if( pItem->GetOwner() == m_owner )
	{
		SetUInt64Value(CONTAINER_FIELD_SLOT_1  + slot*2, 0 );
		pItem->SetUInt64Value(ITEM_FIELD_CONTAINED, 0);

		if(destroy)
		{
			if(pItem->IsInWorld())
			{
				pItem->RemoveFromWorld();
			}
			pItem->DeleteFromDB();
		}
	}
	else
		pItem = NULL;

	return pItem;
}

bool Container::SafeFullRemoveItemFromSlot(int8 slot)
{
	ASSERT((uint32)slot < GetProto()->ContainerSlots);

	Item *pItem = m_Slot[slot];

	if (pItem == NULL ||pItem==this) return false;
	m_Slot[slot] = NULL;

	SetUInt64Value(CONTAINER_FIELD_SLOT_1  + slot*2, 0 );
	pItem->SetUInt64Value(ITEM_FIELD_CONTAINED, 0);

	if(pItem->IsInWorld())
	{
		pItem->RemoveFromWorld();
	}
	pItem->DeleteFromDB();
	pItem->DeleteMe();

	return true;
}

bool Container::AddItemToFreeSlot(Item *pItem, uint32 * r_slot)
{
	uint32 slot;
	for(slot = 0; slot < GetProto()->ContainerSlots; slot++)
	{
		if(!m_Slot[slot])
		{
			m_Slot[slot] = pItem;
			pItem->m_isDirty = true;

			pItem->SetUInt64Value(ITEM_FIELD_CONTAINED, GetGUID());
			pItem->SetOwner(m_owner);

			SetUInt64Value(CONTAINER_FIELD_SLOT_1  + (slot*2), pItem->GetGUID());

			if(m_owner->IsInWorld() && !pItem->IsInWorld())
			{
				pItem->PushToWorld(m_owner->GetMapMgr());
				ByteBuffer buf(2500);
				uint32 count = pItem->BuildCreateUpdateBlockForPlayer( &buf, m_owner );
				m_owner->PushCreationData(&buf, count);
			}
			if(r_slot)
				*r_slot = slot;
			return true;
		}
	}
	return false;
}


void Container::SaveBagToDB(int8 slot, bool first, QueryBuffer * buf)
{
	((Item*)this)->SaveToDB(INVENTORY_SLOT_NOT_SET, slot, first, buf);

	for(uint32 i = 0; i < m_itemProto->ContainerSlots; i++)
	{
		if (m_Slot[i] && !((m_Slot[i]->GetProto()->Flags)& 2) )
		{
			m_Slot[i]->SaveToDB(slot, i, first, buf);
		}
	}
}


