/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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
    SetLowGUID( low );
	SetHighGUID( high );
	m_wowGuid.Init(GetGUID());

	SetScale(  1 );//always 1


	m_Slot = NULL;
	random_suffix=random_prop= 0;
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

	Arcemu::Util::ARCEMU_ASSERT(   m_itemProto  != NULL );
	SetEntry(  itemid );
	

	SetCreatorGUID( fields[5].GetUInt32() );
	SetStackCount(  1);
	
	SetUInt32Value( ITEM_FIELD_FLAGS, fields[8].GetUInt32());
	SetItemRandomPropertyId( fields[9].GetUInt32());

	SetDurabilityMax( m_itemProto->MaxDurability );
	SetDurability( fields[12].GetUInt32() );
  

	SetNumSlots(m_itemProto->ContainerSlots);

	m_Slot = new Item*[m_itemProto->ContainerSlots];
	memset(m_Slot, 0, sizeof(Item*)*(m_itemProto->ContainerSlots));

}

void Container::Create( uint32 itemid, Player *owner )
{

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemid );
	Arcemu::Util::ARCEMU_ASSERT(   m_itemProto != NULL );

	SetEntry(  itemid );

	// TODO: this shouldn't get NULL form containers in mail fix me
	if( owner != NULL )
	{
        SetOwnerGUID( 0 );
        SetContainerGUID( owner->GetGUID() );
	}
	SetStackCount(  1 );
	SetNumSlots(m_itemProto->ContainerSlots);

	m_Slot = new Item*[m_itemProto->ContainerSlots];
	memset(m_Slot, 0, sizeof(Item*)*(m_itemProto->ContainerSlots));

	m_owner = owner;
}


int8 Container::FindFreeSlot()
{
	int8 TotalSlots = static_cast<int8>(GetNumSlots());
	for (int8 i= 0; i < TotalSlots; i++)
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
	int8 TotalSlots = static_cast<int8>(GetNumSlots());
	for (int8 i= 0; i < TotalSlots; i++)
	{
		if(m_Slot[i]) 
		{ 
			return true; 
		}
	}
	return false;
}

bool Container::AddItem(int16 slot, Item *item)
{
	if (slot < 0 || (uint32)slot >= GetProto()->ContainerSlots)
		return false;

	//Arcemu::Util::ARCEMU_ASSERT(   m_Slot[slot] == NULL);
	if(m_Slot[slot] != NULL)
	{
		//sLog.outString("Bad container item %u slot %d", item->GetGUID(), slot);
		return false;
	}

	if (!m_owner)
		return false;

	m_Slot[slot] = item;
	item->m_isDirty = true;

	
    item->SetContainerGUID( GetGUID() );
	item->SetOwner(m_owner);

	if(item->GetProto()->Bonding == ITEM_BIND_ON_PICKUP)
	{
		if(item->GetProto()->Flags & ITEM_FLAG_ACCOUNTBOUND) // don't "Soulbind" account-bound items
			item->AccountBind();
		else
			item->SoulBind();
	}

	SetSlot(slot, item->GetGUID());

	//new version to fix bag issues
	if(m_owner->IsInWorld() && !item->IsInWorld())
	{
		//item->AddToWorld();
		item->PushToWorld(m_owner->GetMapMgr());

		ByteBuffer buf(3000);
		uint32 count = item->BuildCreateUpdateBlockForPlayer(&buf, m_owner);
		m_owner->PushCreationData(&buf, count);
	}
#ifdef ENABLE_ACHIEVEMENTS
	m_owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM, item->GetProto()->ItemId, item->GetStackCount(), 0);
#endif
	return true;
}

void Container::SwapItems(int8 SrcSlot, int8 DstSlot)
{
	Item *temp;
	if( SrcSlot < 0 || SrcSlot >= (int8)m_itemProto->ContainerSlots )
		return;
	
	if( DstSlot < 0 || DstSlot >= (int8)m_itemProto->ContainerSlots )
		return;

	uint32 destMaxCount = (m_owner->ItemStackCheat) ? 0x7fffffff : ((m_Slot[DstSlot]) ? m_Slot[DstSlot]->GetProto()->MaxCount : 0);
	if( m_Slot[DstSlot] &&  m_Slot[SrcSlot]&&m_Slot[DstSlot]->GetEntry() == m_Slot[SrcSlot]->GetEntry() && m_Slot[SrcSlot]->wrapped_item_id == 0 && m_Slot[DstSlot]->wrapped_item_id == 0 && destMaxCount > 1 )
	{
		uint32 total=m_Slot[SrcSlot]->GetStackCount()+m_Slot[DstSlot]->GetStackCount();
		m_Slot[DstSlot]->m_isDirty = m_Slot[SrcSlot]->m_isDirty = true;
		if( total <= destMaxCount )
		{
			m_Slot[DstSlot]->ModStackCount( m_Slot[SrcSlot]->GetStackCount());
			SafeFullRemoveItemFromSlot(SrcSlot);
			return;
		}
		else
		{
			if( m_Slot[DstSlot]->GetStackCount() == destMaxCount )
			{

			}
			else
			{
				int32 delta = destMaxCount - m_Slot[DstSlot]->GetStackCount();
				m_Slot[DstSlot]->SetStackCount( destMaxCount);
				m_Slot[SrcSlot]->ModStackCount( -delta);
				return;
			}
		}
	}

	temp = m_Slot[SrcSlot];
	m_Slot[SrcSlot] = m_Slot[DstSlot];
	m_Slot[DstSlot] = temp;

	if( m_Slot[DstSlot])
	{
		SetSlot(DstSlot, m_Slot[DstSlot]->GetGUID());
		m_Slot[DstSlot]->m_isDirty = true;
	}
	else
	{
		SetSlot(DstSlot, 0);
	}

	if( m_Slot[SrcSlot])
	{
		SetSlot(SrcSlot, m_Slot[SrcSlot]->GetGUID());
		m_Slot[SrcSlot]->m_isDirty = true;
	}
	else
	{
		SetSlot(SrcSlot, 0 );
	}
}

Item *Container::SafeRemoveAndRetreiveItemFromSlot(int16 slot, bool destroy)
{
	if (slot < 0 || (uint32)slot >= GetProto()->ContainerSlots)
		return false;

	Item *pItem = m_Slot[slot];

	if (pItem == NULL || pItem==this) return NULL;
	m_Slot[slot] = NULL;

	if( pItem->GetOwner() == m_owner )
	{
		SetSlot(slot, 0);
        pItem->SetContainerGUID( 0 );

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

bool Container::SafeFullRemoveItemFromSlot(int16 slot)
{
	if (slot < 0 || (uint32)slot >= GetProto()->ContainerSlots)
		return false;

	Item *pItem = m_Slot[slot];

	if (pItem == NULL ||pItem==this) return false;
	m_Slot[slot] = NULL;

	SetSlot(slot, 0);
	pItem->SetContainerGUID( 0 );

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

            pItem->SetContainerGUID( GetGUID() );
			pItem->SetOwner(m_owner);

			SetSlot(uint16(slot), pItem->GetGUID());

			if(m_owner->IsInWorld() && !pItem->IsInWorld())
			{
				pItem->PushToWorld(m_owner->GetMapMgr());
				ByteBuffer buf(2500);
				uint32 count = pItem->BuildCreateUpdateBlockForPlayer( &buf, m_owner );
				m_owner->PushCreationData(&buf, count);
			}
			if(r_slot)
				*r_slot = slot;
#ifdef ENABLE_ACHIEVEMENTS
			m_owner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM, pItem->GetProto()->ItemId, pItem->GetStackCount(), 0);
#endif
			return true;
		}
	}
	return false;
}


void Container::SaveBagToDB(int8 slot, bool first, QueryBuffer * buf)
{
	SaveToDB(INVENTORY_SLOT_NOT_SET, slot, first, buf);

	for(uint32 i = 0; i < m_itemProto->ContainerSlots; i++)
	{
		if (m_Slot[i] && !((m_Slot[i]->GetProto()->Flags)& 2) )
		{
			m_Slot[i]->SaveToDB(slot, static_cast<int8>( i ), first, buf);
		}
	}
}


