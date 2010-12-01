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

// Last edited by:	$Author$
// revision:		$Rev$
// date:			$Date$


#include "StdAfx.h"
#include "Item.h"
#include "Container.h"
#include "ItemPrototype.h"
#include "Player.h"
//
//-------------------------------------------------------------------//
ItemInterface::ItemInterface( Player *pPlayer ) :
m_EquipmentSets( pPlayer->GetLowGUID() )
{
	m_pOwner = pPlayer;
	memset(m_pItems, 0, sizeof(Item*)*MAX_INVENTORY_SLOT);
	memset(m_pBuyBack, 0, sizeof(Item*)*MAX_BUYBACK_SLOT);
    m_refundableitems.clear();
		
}

//-------------------------------------------------------------------//
ItemInterface::~ItemInterface()
{
	for(int i = 0; i < MAX_INVENTORY_SLOT; i++)
	{
		if( m_pItems[i] != NULL && m_pItems[i]->GetOwner() == m_pOwner )
		{
			m_pItems[i]->DeleteMe();
		}
	}
    this->m_refundableitems.clear();
}

//-------------------------------------------------------------------// 100%
uint32 ItemInterface::m_CreateForPlayer(ByteBuffer *data)
{
	Arcemu::Util::ARCEMU_ASSERT(   m_pOwner != NULL);
	uint32 count = 0;

	for(int i = 0; i < MAX_INVENTORY_SLOT; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->IsContainer())
			{
				count += ((Container*)(m_pItems[i]))->BuildCreateUpdateBlockForPlayer(data, m_pOwner);

				for(uint32 e= 0; e < m_pItems[i]->GetProto()->ContainerSlots; e++)
				{
					Item *pItem = ((Container*)(m_pItems[i]))->GetItem(static_cast<int16>( e ));
					if(pItem)
					{
						if(pItem->IsContainer())
						{
							count += ((Container*)(pItem))->BuildCreateUpdateBlockForPlayer( data, m_pOwner );
						}
						else
						{
							count += pItem->BuildCreateUpdateBlockForPlayer( data, m_pOwner );
						}
					}
				}
			}
			else
			{
				count += m_pItems[i]->BuildCreateUpdateBlockForPlayer(data, m_pOwner);
			}
		}
	}
	return count;
}

//-------------------------------------------------------------------// 100%
void ItemInterface::m_DestroyForPlayer()
{
	Arcemu::Util::ARCEMU_ASSERT(   m_pOwner != NULL);

	for(int i = 0; i < MAX_INVENTORY_SLOT; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->IsContainer())
			{
				for(uint32 e= 0; e < m_pItems[i]->GetProto()->ContainerSlots; e++)
				{
					Item *pItem = ((Container*)(m_pItems[i]))->GetItem( static_cast<int16>( e ));
					if(pItem)
					{
						if(pItem->IsContainer())
						{
							m_pOwner->SendDestroyObject( pItem->GetGUID() );
						}
						else
						{
							m_pOwner->SendDestroyObject( pItem->GetGUID() );
						}
					}
				}
				m_pOwner->SendDestroyObject( m_pItems[ i ]->GetGUID() );
			}
			else
			{
				m_pOwner->SendDestroyObject( m_pItems[ i ]->GetGUID() );
			}
		}
	}
}

//-------------------------------------------------------------------//
//Description: Creates and adds a item that can be manipulated after		  
//-------------------------------------------------------------------//
Item *ItemInterface::SafeAddItem(uint32 ItemId, int8 ContainerSlot, int16 slot)
{
	Item *pItem;
	ItemPrototype *pProto = ItemPrototypeStorage.LookupEntry(ItemId);
	if(!pProto) { return NULL; }

	if(pProto->InventoryType == INVTYPE_BAG)
	{
		pItem = TO_ITEM(new Container(HIGHGUID_TYPE_CONTAINER,objmgr.GenerateLowGuid(HIGHGUID_TYPE_CONTAINER)));
		static_cast<Container*>(pItem)->Create( ItemId, m_pOwner);
		if(m_AddItem(pItem, ContainerSlot, slot))
		{
			return pItem;
		}
		else
		{
			pItem->DeleteMe();
			return NULL;
		}
	}
	else
	{
		pItem = new Item;
		pItem->Init(HIGHGUID_TYPE_ITEM,objmgr.GenerateLowGuid(HIGHGUID_TYPE_ITEM));
		pItem->Create( ItemId, m_pOwner);
		if(m_AddItem(pItem, ContainerSlot, slot))
		{
			return pItem;
		}
		else
		{
			delete pItem;
			return NULL;
		}
	}
}

//-------------------------------------------------------------------//
//Description: Creates and adds a item that can be manipulated after		  
//-------------------------------------------------------------------//
AddItemResult ItemInterface::SafeAddItem(Item *pItem, int8 ContainerSlot, int16 slot)
{
	return m_AddItem( pItem, ContainerSlot, slot );
}

//-------------------------------------------------------------------//
//Description: adds items to player inventory, this includes all types of slots.		  
//-------------------------------------------------------------------//
AddItemResult ItemInterface::m_AddItem(Item *item, int8 ContainerSlot, int16 slot)
{
	Arcemu::Util::ARCEMU_ASSERT(    slot < MAX_INVENTORY_SLOT );
	Arcemu::Util::ARCEMU_ASSERT(    ContainerSlot < MAX_INVENTORY_SLOT );
	if( item == NULL || !item->GetProto() || slot < 0)
		return ADD_ITEM_RESULT_ERROR;

	item->m_isDirty = true;

	// double checking
	uint32 i, j, k;
	Item * tempitem;
	for(i = 0; i < MAX_INVENTORY_SLOT; ++i)
	{
		tempitem = m_pItems[i];
		if( tempitem != NULL )
		{
			if( tempitem == item )
			{
#ifdef WIN32
				//OutputCrashLogLine("item duplication, callstack:");
				//printf("item duplication, callstack: ");
				//CStackWalker ws;
				//ws.ShowCallstack();
#endif
				return ADD_ITEM_RESULT_DUPLICATED;
			}

			if( tempitem->IsContainer() )
			{
				k = tempitem->GetProto()->ContainerSlots;
				for(j = 0; j < k; ++j)
				{
					if( static_cast<Container*>(tempitem)->GetItem( static_cast<int16>( j ) ) == item )
					{
#ifdef WIN32
						//OutputCrashLogLine("item duplication in container, callstack:");
						//printf("item duplication in container, callstack: ");
						//CStackWalker ws;
						//ws.ShowCallstack();
#endif
						return ADD_ITEM_RESULT_DUPLICATED;
					}
				}
			}
		}
	}

	//case 1, item is from backpack container
	if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
	{
		//Arcemu::Util::ARCEMU_ASSERT(   m_pItems[slot] == NULL);
		if(GetInventoryItem(slot) != NULL /*|| (slot == EQUIPMENT_SLOT_OFFHAND && !m_pOwner->HasSkillLine(118))*/)
		{
			//sLog.outError("bugged inventory: %u %u", m_pOwner->GetName(), item->GetGUID());
			SlotResult result = this->FindFreeInventorySlot(item->GetProto());
				
			// send message to player
			sChatHandler.BlueSystemMessageToPlr(m_pOwner, "A duplicated item, `%s` was found in your inventory. We've attempted to add it to a free slot in your inventory, if there is none this will fail. It will be attempted again the next time you log on.",
				item->GetProto()->Name1);
			if(result.Result == true)
			{
				// Found a new slot for that item.
				slot = result.Slot;
				ContainerSlot = result.ContainerSlot;
			}
			else
			{
				// don't leak memory!
				/*if(item->IsContainer())
					delete ((Container*)item);
				else
					delete item;*/

				return ADD_ITEM_RESULT_ERROR;
			}
		}

		if(!GetInventoryItem(slot)) //slot is free, add item.
		{
			item->SetOwner( m_pOwner );
			item->SetContainerGUID( m_pOwner->GetGUID() );
			m_pItems[(int)slot] = item;

			if(item->GetProto()->Bonding == ITEM_BIND_ON_PICKUP)
			{
				if(item->GetProto()->Flags & ITEM_FLAG_ACCOUNTBOUND) // don't "Soulbind" account-bound items
					item->AccountBind();
				else
					item->SoulBind();
			}

			if( m_pOwner->IsInWorld() && !item->IsInWorld())
			{
				item->PushToWorld(m_pOwner->GetMapMgr());
				ByteBuffer buf(2500);
				uint32 count = item->BuildCreateUpdateBlockForPlayer( &buf, m_pOwner );
				m_pOwner->PushCreationData(&buf, count);
			}
			m_pOwner->SetInventorySlot(slot, item->GetGUID());
		}
		else
		{
			return ADD_ITEM_RESULT_ERROR;
		}
	}
	else //case 2: item is from a bag container
	{
		if( GetInventoryItem(ContainerSlot) && GetInventoryItem(ContainerSlot)->IsContainer() &&
			slot < (int32)GetInventoryItem(ContainerSlot)->GetProto()->ContainerSlots) //container exists
		{
			bool result = static_cast<Container*>(m_pItems[(int)ContainerSlot])->AddItem(slot, item);
			if( !result )
			{
				return ADD_ITEM_RESULT_ERROR;
			}
		}
		else
		{
			item->DeleteFromDB(); //wpe dupefix ..we don't want it reappearing on the next relog now do we?
			return ADD_ITEM_RESULT_ERROR;
		}
	}

	if ( slot < EQUIPMENT_SLOT_END && ContainerSlot == INVENTORY_SLOT_NOT_SET )
	{
		int VisibleBase = GetOwner()->GetVisibleBase(slot);
		if( VisibleBase > PLAYER_VISIBLE_ITEM_19_ENTRYID )
		{
			sLog.outDebug("Slot warning: slot: %d", slot);
		}
		else
		{
			m_pOwner->SetUInt32Value( VisibleBase, item->GetEntry()  );
            m_pOwner->SetUInt32Value( VisibleBase + 1, item->GetEnchantmentId( 0 ) );
		}
	}

	if( m_pOwner->IsInWorld() && slot < INVENTORY_SLOT_BAG_END && ContainerSlot == INVENTORY_SLOT_NOT_SET )
	{
		m_pOwner->ApplyItemMods( item, slot, true );
	}

	if(slot >= CURRENCYTOKEN_SLOT_START && slot < CURRENCYTOKEN_SLOT_END)
	{
		m_pOwner->UpdateKnownCurrencies(item->GetEntry(), true);
	}

	if( ContainerSlot == INVENTORY_SLOT_NOT_SET && slot == EQUIPMENT_SLOT_OFFHAND && item->GetProto()->Class == ITEM_CLASS_WEAPON ){
		m_pOwner->SetDualWield( true );

		/////////////////////////////////////////// Titan's grip stuff ////////////////////////////////////////////////////////////

		uint32 subclass = item->GetProto()->SubClass;
		if( subclass == ITEM_SUBCLASS_WEAPON_TWOHAND_AXE 
			|| subclass == ITEM_SUBCLASS_WEAPON_TWOHAND_MACE 
			|| subclass == ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD ){
				
				m_pOwner->CastSpell( m_pOwner, 49152, true );

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}

#ifdef ENABLE_ACHIEVEMENTS
	m_pOwner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM, item->GetEntry(), 1, 0);
#endif
////////////////////////////////////////////////////// existingduration stuff ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if( item->GetProto()->ExistingDuration != 0 ){
        if( item->GetItemExpireTime() == 0 ){
            item->SetItemExpireTime( UNIXTIME + item->GetProto()->ExistingDuration );
            item->SetDuration( item->GetProto()->ExistingDuration );
            sEventMgr.AddEvent( item, &Item::EventRemoveItem, EVENT_REMOVE_ITEM, item->GetProto()->ExistingDuration * 1000 , 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
        }else{
            item->SetDuration( static_cast< uint32 >( item->GetItemExpireTime() - UNIXTIME ) );
            sEventMgr.AddEvent( item, &Item::EventRemoveItem, EVENT_REMOVE_ITEM,  ( item->GetItemExpireTime() - UNIXTIME ) * 1000 , 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
        }
        
        // if we are already in the world we will send the durationupdate now, so we can see the remaining duration in the client
        // otherwise we will send the updates in Player::Onpushtoworld anyways
        if( m_pOwner->IsInWorld() )
            sEventMgr.AddEvent( item, &Item::SendDurationUpdate, EVENT_SEND_PACKET_TO_PLAYER_AFTER_LOGIN, 0, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
    }

	return ADD_ITEM_RESULT_OK;
}

//-------------------------------------------------------------------//
//Description: Checks if the slot is a Bag slot
//-------------------------------------------------------------------//
bool ItemInterface::IsBagSlot(int16 slot)
{
	if((slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END) || (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END))
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------//
//Description: removes the item safely and returns it back for usage
//-------------------------------------------------------------------//
Item *ItemInterface::SafeRemoveAndRetreiveItemFromSlot(int8 ContainerSlot, int16 slot, bool destroy)
{
	Arcemu::Util::ARCEMU_ASSERT(   slot < MAX_INVENTORY_SLOT);
	Arcemu::Util::ARCEMU_ASSERT(   ContainerSlot < MAX_INVENTORY_SLOT);
	Item *pItem = NULL;

	if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
	{
		pItem = GetInventoryItem(ContainerSlot,slot);

		if (pItem == NULL) { return NULL; }

		if(pItem->GetProto()->ContainerSlots > 0 && pItem->GetTypeId() == TYPEID_CONTAINER && ((Container*)pItem)->HasItems())
		{
			/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
			pItem->DeleteFromDB();
			return false;
		}

		m_pItems[(int)slot] = NULL;
		if(pItem->GetOwner() == m_pOwner)
		{
			pItem->m_isDirty = true;

			m_pOwner->SetInventorySlot(slot, 0 );

			if ( slot < EQUIPMENT_SLOT_END )
			{
				m_pOwner->ApplyItemMods( pItem, slot, false );
				int VisibleBase = GetOwner()->GetVisibleBase(slot);
				for (int i = VisibleBase; i < VisibleBase + PLAYER_VISIBLE_ITEM_LENGTH; ++i)
				{
					m_pOwner->SetUInt32Value(i, 0);
				}
			}
			else if ( slot < INVENTORY_SLOT_BAG_END )
				m_pOwner->ApplyItemMods( pItem, slot, false );

			if(slot == EQUIPMENT_SLOT_OFFHAND)
				m_pOwner->SetDualWield( false );

			if(destroy)
			{
				if (pItem->IsInWorld())
				{
					pItem->RemoveFromWorld();
				}
				pItem->DeleteFromDB();
			}
		}
		else
			pItem = NULL;
	}
	else
	{
		Item *pContainer = GetInventoryItem(ContainerSlot);
		if(pContainer && pContainer->IsContainer())
		{
			pItem = static_cast<Container*>(pContainer)->SafeRemoveAndRetreiveItemFromSlot(slot, destroy);
		}
	}

	return pItem;
}

//-------------------------------------------------------------------//
//Description: removes the item safely by guid and returns it back for usage, supports full inventory
//-------------------------------------------------------------------//
Item *ItemInterface::SafeRemoveAndRetreiveItemByGuid(uint64 guid, bool destroy)
{
	int16 i = 0;

	for(i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
		else
		{
			if(item && item->IsContainer())
			{
				for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
					if (item2 && item2->GetGUID() == guid)
					{
						return ((Container*)item)->SafeRemoveAndRetreiveItemFromSlot(static_cast<int16>( j ), destroy);
					}
				}
			}
		}
	}

	for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
		else
		{
			if(item && item->IsContainer())
			{
				for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
					if (item2 && item2->GetGUID() == guid)
					{
						return ((Container*)item)->SafeRemoveAndRetreiveItemFromSlot(static_cast<int16>( j ), destroy);
					}
				}
			}
		}
	}

	return NULL;
}

//-------------------------------------------------------------------//
//Description: completely removes item from player
//Result: true if item removal was succefull
//-------------------------------------------------------------------//
bool ItemInterface::SafeFullRemoveItemFromSlot(int8 ContainerSlot, int16 slot)
{
	Arcemu::Util::ARCEMU_ASSERT(   slot < MAX_INVENTORY_SLOT);
	Arcemu::Util::ARCEMU_ASSERT(   ContainerSlot < MAX_INVENTORY_SLOT);

	if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
	{
		Item *pItem = GetInventoryItem(slot);

		if (pItem == NULL) { return false; }

		if(pItem->GetProto()->ContainerSlots > 0 && pItem->GetTypeId() == TYPEID_CONTAINER && ((Container*)pItem)->HasItems())
		{
			/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
			pItem->DeleteFromDB();
			return false;
		}

		m_pItems[(int)slot] = NULL;
		// hacky crashfix
		if( pItem->GetOwner() == m_pOwner )
		{
			pItem->m_isDirty = true;

			m_pOwner->SetInventorySlot(slot, 0 );

			if ( slot < EQUIPMENT_SLOT_END )
			{
				m_pOwner->ApplyItemMods(pItem, slot, false );
				int VisibleBase = GetOwner()->GetVisibleBase(slot);
				for (int i = VisibleBase; i < VisibleBase + PLAYER_VISIBLE_ITEM_LENGTH; ++i)
				{
					m_pOwner->SetUInt32Value(i, 0);
				}
			}
			else if( slot < INVENTORY_SLOT_BAG_END )
				m_pOwner->ApplyItemMods(pItem, slot, false ); //watch containers that give attackspeed and stuff ;)

			if(slot == EQUIPMENT_SLOT_OFFHAND)
				m_pOwner->SetDualWield( false );

			if (pItem->IsInWorld())
			{
				pItem->RemoveFromWorld();
			}

			pItem->DeleteFromDB();
			
            //delete pItem;
            // We make it a garbage item, so when it's used for a spell, it gets deleted in the next Player update
            // otherwise we get a nice crash
            m_pOwner->AddGarbageItem( pItem );
		}
	}
	else
	{
		Item *pContainer = GetInventoryItem(ContainerSlot);
		if(pContainer && pContainer->IsContainer())
		{
			static_cast<Container*>(pContainer)->SafeFullRemoveItemFromSlot(slot);
		}
	}
	return true;
}

//-------------------------------------------------------------------//
//Description: removes the item safely by guid, supports full inventory
//-------------------------------------------------------------------//
bool ItemInterface::SafeFullRemoveItemByGuid(uint64 guid)
{
	int16 i = 0;

	for(i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
		else
		{
			if(item && item->IsContainer())
			{
				for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
					if (item2 && item2->GetGUID() == guid)
					{
						return ((Container*)item)->SafeFullRemoveItemFromSlot(static_cast<int16>( j ));
					}
				}
			}
		}
	}

	for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
		else
		{
			if(item && item->IsContainer())
			{
				for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
					if (item2 && item2->GetGUID() == guid)
					{
						return ((Container*)item)->SafeFullRemoveItemFromSlot(static_cast<int16>( j ));
					}
				}
			}
		}
	}
	return false;
}

//-------------------------------------------------------------------//
//Description: Gets a item from Inventory
//-------------------------------------------------------------------//
Item *ItemInterface::GetInventoryItem(int16 slot)
{
	if(slot < 0 || slot > MAX_INVENTORY_SLOT)
		return NULL;

	return m_pItems[(int)slot];
}

//-------------------------------------------------------------------//
//Description: Gets a Item from inventory or container
//-------------------------------------------------------------------//
Item *ItemInterface::GetInventoryItem( int8 ContainerSlot, int16 slot )
{
	if( ContainerSlot <= INVENTORY_SLOT_NOT_SET )
	{
		if( slot < 0 || slot > MAX_INVENTORY_SLOT )
			return NULL;

		return m_pItems[ (int)slot ];
	}
	else
	{
		if( IsBagSlot( ContainerSlot ) )
		{
			if( m_pItems[ (int)ContainerSlot ] )
			{
				return static_cast< Container* >( m_pItems[ (int)ContainerSlot ] )->GetItem( slot );
			}
		}
	}
	return NULL;
}

//-------------------------------------------------------------------//
//Description: checks for stacks that didn't reached max capacity
//-------------------------------------------------------------------//
Item* ItemInterface::FindItemLessMax(uint32 itemid, uint32 cnt, bool IncBank)
{
	uint32 i;
	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			uint32 itemMaxStack = (item->GetOwner()->ItemStackCheat) ? 0x7fffffff : item->GetProto()->MaxCount;
			if((item->GetEntry() == itemid && item->wrapped_item_id== 0) && (itemMaxStack >= (item->GetStackCount() + cnt)))
			{
				return item; 
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if(item && item->IsContainer())
		{
			  for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
					if (item2)
					{
						uint32 itemMaxStack = (item2->GetOwner()->ItemStackCheat) ? 0x7fffffff : item2->GetProto()->MaxCount;
						if( (item2->GetProto()->ItemId == itemid && item2->wrapped_item_id== 0) && (itemMaxStack >= (item2->GetStackCount() + cnt)) )
						{
							return item2;
						}
					}
				}
			
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			uint32 itemMaxStack = (item->GetOwner()->ItemStackCheat) ? 0x7fffffff : item->GetProto()->MaxCount;
			if((item->GetEntry() == itemid && item->wrapped_item_id== 0) && (itemMaxStack >= (item->GetStackCount() + cnt)))
			{
				return item; 
			}
		}
	}

	if(IncBank)
	{
		for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
		{
			Item *item = GetInventoryItem(static_cast<int16>( i ));
			if (item)
			{
				uint32 itemMaxStack = (item->GetOwner()->ItemStackCheat) ? 0x7fffffff : item->GetProto()->MaxCount;
				if((item->GetEntry() == itemid && item->wrapped_item_id== 0) && (itemMaxStack >= (item->GetStackCount() + cnt)))
				{
					return item; 
				}
			}
		}

		for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
		{
			Item *item = GetInventoryItem(static_cast<int16>( i ));
			if(item && item->IsContainer())
			{

					for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
					{
						Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
						if (item2)
						{
							uint32 itemMaxStack = (item2->GetOwner()->ItemStackCheat) ? 0x7fffffff : item2->GetProto()->MaxCount;
							if( (item2->GetProto()->ItemId == itemid && item2->wrapped_item_id== 0) && (itemMaxStack >= (item2->GetStackCount() + cnt)) )
							{
								return item2;
							}
						}
					}
				
			}
		}
	}

	return NULL;
}

//-------------------------------------------------------------------//
//Description: finds item ammount on inventory, banks not included
//-------------------------------------------------------------------//
uint32 ItemInterface::GetItemCount(uint32 itemid, bool IncBank)
{
	uint32 cnt = 0;
	uint32 i;
	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));

		if (item)
		{
			if(item->GetEntry() == itemid && item->wrapped_item_id== 0)
			{
				cnt += item->GetStackCount() ? item->GetStackCount() : 1; 
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if(item && item->IsContainer())
		{
				for (uint32 j = 0; j < item->GetProto()->ContainerSlots;j++)
				{
					Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
					if (item2)
					{
						if (item2->GetEntry() == itemid && item->wrapped_item_id== 0)
						{
							cnt += item2->GetStackCount() ? item2->GetStackCount() : 1; 
						}
					}
				}
			
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));

		if (item)
		{
			if(item->GetProto()->ItemId == itemid && item->wrapped_item_id== 0)
			{
				cnt += item->GetStackCount() ? item->GetStackCount() : 1;
			}
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));

		if (item)
		{
			if(item->GetProto()->ItemId == itemid && item->wrapped_item_id== 0)
			{
				cnt += item->GetStackCount() ? item->GetStackCount() : 1;
			}
		}
	}

	if(IncBank)
	{
		for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; i++)
		{
			Item *item = GetInventoryItem(static_cast<int16>( i ));
			if (item)
			{
				if(item->GetProto()->ItemId == itemid && item->wrapped_item_id== 0)
				{
					cnt += item->GetStackCount() ? item->GetStackCount() : 1;
				}
			}
		}

		for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
		{
			Item *item = GetInventoryItem(static_cast<int16>( i ));
			if(item)
			{
				if(item->IsContainer())
				{
					for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
					{
						Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
						if (item2)
						{
							if(item2->GetProto()->ItemId == itemid && item->wrapped_item_id== 0)
							{
								cnt += item2->GetStackCount() ? item2->GetStackCount() : 1;
							}
						}
					}
				}
			}
		}
	}
	return cnt;
}

//-------------------------------------------------------------------//
//Description: Removes a ammount of items from inventory
//-------------------------------------------------------------------//
uint32 ItemInterface::RemoveItemAmt(uint32 id, uint32 amt)
{
	//this code returns shit return value is fucked
	if (GetItemCount(id) < amt)
	{
		return 0;
	}
	uint32 i;

	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			if(item->GetEntry() == id && item->wrapped_item_id== 0)
			{
				if(item->GetProto()->ContainerSlots > 0 && item->IsContainer() && ((Container*)item)->HasItems())
				{
					/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
					item->DeleteFromDB();
					continue;
				}

				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
				}
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if(item && item->IsContainer())
		{
			for (uint32 j = 0; j < item->GetProto()->ContainerSlots;j++)
			{
				Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
				if (item2)
				{
					if (item2->GetProto()->ItemId == id && item->wrapped_item_id== 0)
					{
						if (item2->GetStackCount() > amt)
						{
							item2->SetStackCount( item2->GetStackCount() - amt);
							item2->m_isDirty = true;
							return amt;
						}
						else if (item2->GetStackCount()== amt)
						{
							bool result = SafeFullRemoveItemFromSlot(static_cast<int8>( i ), static_cast<int16>( j ));
							if(result)
							{
								return amt;
							}
							else
							{
								return 0;
							}
						}
						else
						{
							amt -= item2->GetStackCount();
							SafeFullRemoveItemFromSlot(static_cast<int8>( i ), static_cast<int16>( j ));
					  
						}
					}
				}
			}
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id== 0)
			{
				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
				}
			}
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id== 0)
			{
				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
				}
			}
		}
	}
	return 0;
}

uint32 ItemInterface::RemoveItemAmt_ProtectPointer(uint32 id, uint32 amt, Item** pointer)
{
	//this code returns shit return value is fucked
	if (GetItemCount(id) < amt)
	{
		return 0;
	}
	uint32 i;

	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem( static_cast<int16>( i ));
		if (item)
		{
			if(item->GetEntry() == id && item->wrapped_item_id== 0)
			{
				if(item->GetProto()->ContainerSlots > 0 && item->IsContainer() && ((Container*)item)->HasItems())
				{
					/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
					item->DeleteFromDB();
					continue;
				}

				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));

					// bool result = true;

					/*
					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
					*/

					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));

					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
				}
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if(item && item->IsContainer())
		{
			for (uint32 j = 0; j < item->GetProto()->ContainerSlots;j++)
			{
				Item *item2 = ((Container*)item)->GetItem(static_cast<int16>( j ));
				if (item2)
				{
					if (item2->GetProto()->ItemId == id && item->wrapped_item_id== 0)
					{
						if (item2->GetStackCount() > amt)
						{
							item2->SetStackCount( item2->GetStackCount() - amt);
							item2->m_isDirty = true;
							return amt;
						}
						else if (item2->GetStackCount()== amt)
						{
							bool result = SafeFullRemoveItemFromSlot(static_cast<int8>( i ), static_cast<int16>( j ));
							if( pointer != NULL && *pointer != NULL && *pointer == item2 )
								*pointer = NULL;

							if(result)
							{
								return amt;
							}
							else
							{
								return 0;
							}
						}
						else
						{
							amt -= item2->GetStackCount();
							SafeFullRemoveItemFromSlot(static_cast<int8>( i ), static_cast<int16>( j ));

							if( pointer != NULL && *pointer != NULL && *pointer == item2 )
								*pointer = NULL;
						}
					}
				}
			}
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id== 0)
			{
				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;

					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));

					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
				}
			}
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(static_cast<int16>( i ));
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id== 0)
			{
				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;

					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));

					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
				}
			}
		}
	}
	return 0;
}

// Removes desired amount of items by guid
uint32 ItemInterface::RemoveItemAmtByGuid(uint64 guid, uint32 amt)
{
	int16 i;

	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetGUID() == guid && item->wrapped_item_id== 0)
			{
				if(item->GetProto()->ContainerSlots > 0 && item->IsContainer() && ((Container*)item)->HasItems())
				{
					/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
					item->DeleteFromDB();
					continue;
				}

				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					return amt;
				}
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if(item && item->IsContainer())
		{
			for (uint32 j = 0; j < item->GetProto()->ContainerSlots;j++)
			{
				Item *item2 = ((Container*)item)->GetItem( static_cast<int16>( j ));
				if (item2)
				{
					if (item2->GetGUID() == guid && item->wrapped_item_id== 0)
					{
						if (item2->GetStackCount() > amt)
						{
							item2->SetStackCount( item2->GetStackCount() - amt);
							item2->m_isDirty = true;
							return amt;
						}
						else if (item2->GetStackCount()== amt)
						{
							bool result = SafeFullRemoveItemFromSlot(static_cast<int8>( i ), static_cast<int16>( j ));
							if(result)
							{
								return amt;
							}
							else
							{
								return 0;
							}
						}
						else
						{
							amt -= item2->GetStackCount();
							SafeFullRemoveItemFromSlot(static_cast<int8>( i ), static_cast<int16>( j ));
							return amt;
						}
					}
				}
			}
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetGUID() == guid && item->wrapped_item_id== 0)
			{
				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					return amt;
				}
			}
		}
	}

	for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetGUID() == guid && item->wrapped_item_id== 0)
			{
				if (item->GetStackCount() > amt)
				{
					item->SetStackCount( item->GetStackCount() - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetStackCount()== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetStackCount();
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					return amt;
				}
			}
		}
	}
	return 0;
}

void ItemInterface::RemoveAllConjured()
{
	for(uint32 x = INVENTORY_SLOT_BAG_START; x < INVENTORY_SLOT_ITEM_END; ++x)
	{
		if (m_pItems[x]!= NULL)
		{
			if(IsBagSlot(static_cast<int16>( x )) && m_pItems[x]->IsContainer())
			{
				Container * bag = static_cast<Container*>( m_pItems[x] );
 
				for(uint32 i = 0; i < bag->GetProto()->ContainerSlots; i++)
				{
					if (bag->GetItem(static_cast<int16>( i )) != NULL && bag->GetItem(static_cast<int16>( i ))->GetProto()->Flags & 2)
						bag->SafeFullRemoveItemFromSlot(static_cast<int16>( i ));
				}
			}
			else
			{
				if(m_pItems[x]->GetProto()->Flags & 2 )
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, static_cast<int16>( x ));
			}
		}
	}
}


//-------------------------------------------------------------------//
//Description: Gets slot number by itemid, banks not included
//-------------------------------------------------------------------//
int16 ItemInterface::GetInventorySlotById(uint32 ID)
{
	for(uint16 i= 0;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetProto()->ItemId == ID)
			{
				return i;
			}
		}
	}

	for(uint16 i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetProto()->ItemId == ID)
			{
				return i;
			}
		}
	}

	for(uint16 i=CURRENCYTOKEN_SLOT_START; i<CURRENCYTOKEN_SLOT_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetProto()->ItemId == ID)
			{
				return i;
			}
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

//-------------------------------------------------------------------//
//Description: Gets slot number by item guid, banks not included
//-------------------------------------------------------------------//
int16 ItemInterface::GetInventorySlotByGuid(uint64 guid)
{
	for(uint16 i=EQUIPMENT_SLOT_START ;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint16 i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint16 i=CURRENCYTOKEN_SLOT_START; i<CURRENCYTOKEN_SLOT_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	return ITEM_NO_SLOT_AVAILABLE; //was changed from 0 cuz 0 is the slot for head
}

int16 ItemInterface::GetBagSlotByGuid(uint64 guid)
{
	for(uint16 i=EQUIPMENT_SLOT_START ;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint16 i=INVENTORY_KEYRING_START; i<CURRENCYTOKEN_SLOT_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint16 i=INVENTORY_SLOT_BAG_START; i<INVENTORY_SLOT_BAG_END; ++i)
	{
		if(m_pItems[i]&&m_pItems[i]->GetTypeId()==TYPEID_CONTAINER)
		{
			for(uint32 j = 0; j < m_pItems[i]->GetProto()->ContainerSlots; ++j)
			{
				Item * inneritem = ( static_cast<Container*>( m_pItems[i] ))->GetItem(static_cast<int16>( j ));
				if(inneritem && inneritem->GetGUID()==guid)
					return i;
			}
		}
	}

	return ITEM_NO_SLOT_AVAILABLE; //was changed from 0 cuz 0 is the slot for head
}


//-------------------------------------------------------------------//
//Description: Adds a Item to a free slot
//-------------------------------------------------------------------//
AddItemResult ItemInterface::AddItemToFreeSlot(Item *item)
{
	if( item == NULL )
		return ADD_ITEM_RESULT_ERROR;

	if( item->GetProto() == NULL )
		return ADD_ITEM_RESULT_ERROR;

	uint32 i = 0;
	bool result2;
	AddItemResult result3;
	Player* p = m_pOwner;
	uint32 itemMaxStack = item->GetProto()->MaxCount;

	//detect special bag item
	if( item->GetProto()->BagFamily )
	{
		if( item->GetProto()->BagFamily & ITEM_TYPE_KEYRING || item->GetProto()->Class == ITEM_CLASS_KEY )
		{
			for(i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++ )
			{
				if(m_pItems[i] == NULL)
				{
					result3 = SafeAddItem( item, INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ) );
					if( result3 )
					{
						result.ContainerSlot = INVENTORY_SLOT_NOT_SET;
						result.Slot = static_cast<int8>( i );
						result.Result = true;
						return ADD_ITEM_RESULT_OK;
					}
				}
			}
		}
		else if( item->GetProto()->BagFamily & ITEM_TYPE_CURRENCY )
		{
			for( i=CURRENCYTOKEN_SLOT_START; i<CURRENCYTOKEN_SLOT_END; i++ )
			{
				if( m_pItems[i] )
					itemMaxStack = (p->ItemStackCheat) ? 0x7fffffff : m_pItems[i]->GetProto()->MaxCount;

				if( m_pItems[i] == NULL )
				{
					result3 = SafeAddItem( item, INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ) );
					if( result3 ) 
					{
						result.ContainerSlot = INVENTORY_SLOT_NOT_SET;
						result.Slot = static_cast<int8>( i );
						result.Result = true;
						p->UpdateKnownCurrencies(m_pItems[i]->GetEntry(), true);
						return ADD_ITEM_RESULT_OK;
					}
				}
				else if( m_pItems[i]->GetProto()->ItemId == item->GetProto()->ItemId && itemMaxStack > 1 &&
						m_pItems[i]->GetStackCount() < itemMaxStack  &&
						m_pItems[i]->GetStackCount() + item->GetStackCount() <= itemMaxStack )
					{
						m_pItems[i]->SetStackCount(  m_pItems[i]->GetStackCount() + item->GetStackCount() );
						result.Slot = static_cast<int8>( i );
						result.Result=true;
						p->UpdateKnownCurrencies(m_pItems[i]->GetEntry(), true);
						return ADD_ITEM_RESULT_OK;
					}
			}
		}
		else
		{
			for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
			{
				if(m_pItems[i])
				{
					if (m_pItems[i]->GetProto()->BagFamily & item->GetProto()->BagFamily)
					{
						if(m_pItems[i]->IsContainer())
						{
							uint32 r_slot;
							result2 = static_cast<Container*>(m_pItems[i])->AddItemToFreeSlot(item, &r_slot);
							if (result2) {
								result.ContainerSlot = static_cast<int8>( i );
								result.Slot = static_cast<int8>( r_slot );
								result.Result = true;
								return ADD_ITEM_RESULT_OK;
							}
						}
					}
				}
			}
		}
	}

	//INVENTORY
	for(i=INVENTORY_SLOT_ITEM_START;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if( m_pItems[i] )
			itemMaxStack = (p->ItemStackCheat) ? 0x7fffffff : m_pItems[i]->GetProto()->MaxCount;
		if(m_pItems[i] == NULL)
		{
			result3 = SafeAddItem(item, INVENTORY_SLOT_NOT_SET, static_cast<int16>( i ));
			if(result3) {
				result.ContainerSlot = INVENTORY_SLOT_NOT_SET;
				result.Slot = static_cast<int8>( i );
				result.Result = true;
				return ADD_ITEM_RESULT_OK;
			}
		}
		else if(m_pItems[i]->GetProto()->ItemId == item->GetProto()->ItemId &&
				itemMaxStack > 1 &&
				m_pItems[i]->GetStackCount() < itemMaxStack  &&
				m_pItems[i]->GetStackCount() + item->GetStackCount() <= itemMaxStack )
			{
				m_pItems[i]->SetStackCount( m_pItems[i]->GetStackCount() + item->GetStackCount() );
				result.Slot = static_cast<int8>( i );
				result.Result=true;
				return ADD_ITEM_RESULT_OK;
			}
	}

	//INVENTORY BAGS
	for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
	{
		if(m_pItems[i] != NULL && m_pItems[i]->GetProto()->BagFamily == 0 && m_pItems[i]->IsContainer()) //special bags ignored
		{
			for (uint32 j = 0; j < m_pItems[i]->GetProto()->ContainerSlots;j++)
			{
				Item *item2 = ( static_cast<Container*>( m_pItems[i] ))->GetItem(static_cast<int16>( j ));
				if( item2 )
					itemMaxStack = (p->ItemStackCheat) ? 0x7fffffff : item2->GetProto()->MaxCount;
				if( item2 == NULL )
				{
					result3 = SafeAddItem(item, static_cast<int8>( i ), static_cast<int16>( j ));
					if(result3) {
						result.ContainerSlot = static_cast<int8>( i );
						result.Slot = static_cast<int8>( j );
						result.Result = true;
						return ADD_ITEM_RESULT_OK;
					}
				}
				else if(item2->GetProto()->ItemId == item->GetProto()->ItemId &&
					itemMaxStack > 1 &&
					item2->GetStackCount() < itemMaxStack &&
					item2->GetStackCount() + item->GetStackCount() <= itemMaxStack )
				{
					item2->SetStackCount( item2->GetStackCount() + item->GetStackCount() );
					result.Slot = static_cast<int8>( i );
					result.Result=true;
					return ADD_ITEM_RESULT_OK;
				}
			}
		}
	}
	return ADD_ITEM_RESULT_ERROR;
}

//-------------------------------------------------------------------//
//Description: Calculates inventory free slots, bag inventory slots not included
//-------------------------------------------------------------------//
uint32 ItemInterface::CalculateFreeSlots(ItemPrototype *proto)
{
	uint32 count = 0;
	uint32 i;

	if(proto)
	{
		if(proto->BagFamily)
		{
			if(proto->BagFamily & ITEM_TYPE_KEYRING || proto->Class == ITEM_CLASS_KEY)
			{
				for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
				{
					if(m_pItems[i] == NULL)
					{
						count++;
					}
				}
			}
			else if(proto->BagFamily & ITEM_TYPE_CURRENCY )
			{
				for(i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++)
				{
					if(m_pItems[i] == NULL)
					{
						count++;
					}
				}
			}
			else
			{
				for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END;i++)
				{
					if(m_pItems[i] && m_pItems[i]->IsContainer())
					{
						if (m_pItems[i]->GetProto()->BagFamily & proto->BagFamily)
						{
							int8 slot = ((Container*)m_pItems[i])->FindFreeSlot();
							if(slot != ITEM_NO_SLOT_AVAILABLE) 
							{
								count++;
							}
						}
					}
				}
			}
		}
	}

	for(i=INVENTORY_SLOT_ITEM_START;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i] == NULL)
		{
			count++;
		}
	}

	for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
	{
		if(m_pItems[i] != NULL )
		{
			if(m_pItems[i]->IsContainer() && !m_pItems[i]->GetProto()->BagFamily)
			{

				for (uint32 j = 0; j < m_pItems[i]->GetProto()->ContainerSlots;j++)
				{
					Item *item2 = ( static_cast<Container*>( m_pItems[i] ))->GetItem( static_cast<int16>( j ));
					if (item2 == NULL)
					{
						count++;
					}
				}
			}
		}
	}
	return count;
}

//-------------------------------------------------------------------//
//Description: finds a free slot on the backpack
//-------------------------------------------------------------------//
int8 ItemInterface::FindFreeBackPackSlot()
{
	//search for backpack slots
	for(int8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if(!item) 
		{ 
			return i; 
		}
	}

	return ITEM_NO_SLOT_AVAILABLE; //no slots available
}

uint8 ItemInterface::FindFreeBackPackSlotMax()
{
	//search for backpack slots
	uint8 slotfree = 0;
	for(int8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = GetInventoryItem(i);
		if(!item) slotfree++;
	}
	return slotfree;
}


//-------------------------------------------------------------------//
//Description: converts bank bags slot ids into player bank byte slots(0-5)
//-------------------------------------------------------------------//
int8 ItemInterface::GetInternalBankSlotFromPlayer(int8 islot)
{
	switch(islot)
	{
	case BANK_SLOT_BAG_1:
		{
			return 1;
		}
	case BANK_SLOT_BAG_2:
		{
			return 2;
		}
	case BANK_SLOT_BAG_3:
		{
			return 3;
		}
	case BANK_SLOT_BAG_4:
		{
			return 4;
		}
	case BANK_SLOT_BAG_5:
		{
			return 5;
		}
	case BANK_SLOT_BAG_6:
		{
			return 6;
		}
	case BANK_SLOT_BAG_7:
		{
			return 7;
		}
	default:
		return 8;
	}
}

//-------------------------------------------------------------------//
//Description: checks if the item can be equipped on a specific slot
//             this will check unique-equipped gems as well
//-------------------------------------------------------------------//
int8 ItemInterface::CanEquipItemInSlot2( int8 DstInvSlot, int8 slot, Item* item, bool ignore_combat /* = false */, bool skip_2h_check /* = false */ )
{
	ItemPrototype *proto = item->GetProto();

	if( int8 ret = CanEquipItemInSlot(DstInvSlot, slot, proto, ignore_combat, skip_2h_check) )
		return ret;

	if( ( slot < INVENTORY_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET ) || ( slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET ) )
	{
		for( uint32 count = 0; count < item->GetSocketsCount(); count++ )
		{
			EnchantmentInstance *ei = item->GetEnchantment( SOCK_ENCHANTMENT_SLOT1 + count );
			if (ei 
				&& ei->Enchantment->GemEntry //huh ? Gem without entry ?
				)
			{
				ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(ei->Enchantment->GemEntry);

				if (
					ip //maybe gem got removed from db due to update ?
					)
				{
                    if(
                        ip->Flags & ITEM_FLAG_UNIQUE_EQUIP 
                    &&  IsEquipped( ip->ItemId )
                    )
                    {
                       return INV_ERR_CANT_CARRY_MORE_OF_THIS;
                    }

                    if( ip->ItemLimitCategory > 0 )
                    {
                       uint32 LimitId = ip->ItemLimitCategory;
                       ItemLimitCategoryEntry * ile = dbcItemLimitCategory.LookupEntryForced( LimitId );
                       if( ile )
                       {
                          uint32 gemCount = 0;
                          if(       ( ile->equippedFlag & ILFLAG_EQUIP_ONLY  && slot < EQUIPMENT_SLOT_END ) 
                             ||   ( !(ile->equippedFlag & ILFLAG_EQUIP_ONLY) && slot > EQUIPMENT_SLOT_END ) )
                             gemCount = item->CountGemsWithLimitId( ile->Id );

                          uint32 gCount = GetEquippedCountByItemLimit( ile->Id );
                          if( ( gCount + gemCount ) > ile->maxAmount )
                             return INV_ERR_CANT_CARRY_MORE_OF_THIS;
                       }
					}
				}
			}
		}
	}

	return 0;
}

//-------------------------------------------------------------------//
//Description: checks if the item can be equipped on a specific slot
//-------------------------------------------------------------------//
int8 ItemInterface::CanEquipItemInSlot( int8 DstInvSlot, int8 slot, ItemPrototype* proto, bool ignore_combat /* = false */, bool skip_2h_check /* = false */ )
{
	if ( proto == NULL ) return INV_ERR_ITEMS_CANT_BE_SWAPPED;

	uint32 type = proto->InventoryType;

	if(slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END && DstInvSlot == -1)
		if(proto->ContainerSlots == 0)
			return INV_ERR_ITEMS_CANT_BE_SWAPPED;

	if((slot < INVENTORY_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET) || (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET))
	{
		if (!ignore_combat && m_pOwner->CombatStatus.IsInCombat() && (slot < EQUIPMENT_SLOT_MAINHAND || slot > EQUIPMENT_SLOT_RANGED))
			return INV_ERR_CANT_DO_IN_COMBAT;

		if( IsEquipped(proto->ItemId) && (proto->Unique || proto->Flags & ITEM_FLAG_UNIQUE_EQUIP ))
			return INV_ERR_CANT_CARRY_MORE_OF_THIS;

		// Check to see if we have the correct race
		if(!(proto->AllowableRace& m_pOwner->getRaceMask()))
			return INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM;

		// Check to see if we have the correct class
		if(!(proto->AllowableClass & m_pOwner->getClassMask()))
			return INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM2;

		// Check to see if we have the reqs for that reputation
		if(proto->RequiredFaction)
		{
			Standing current_standing = Player::GetReputationRankFromStanding(m_pOwner->GetStanding(proto->RequiredFaction));
			if(current_standing < (Standing)proto->RequiredFactionStanding)	   // Not enough rep rankage..
				return INV_ERR_ITEM_REPUTATION_NOT_ENOUGH;
		}

		// Check to see if we have the correct level.
		if( proto->RequiredLevel > m_pOwner->getLevel() )
			return INV_ERR_YOU_MUST_REACH_LEVEL_N;

		if(proto->Class == 4)
		{
			uint32 bogus_subclass = 0;
			uint32 playerlevel = 0;
			/* scaling heirloom items */
			if(proto->ScalingStatsEntry != 0 && proto->ScalingStatsFlag != 0)
			{
				playerlevel = m_pOwner->getLevel();

				if (playerlevel < 40 && proto->SubClass >= 3)
					bogus_subclass = proto->SubClass - 1; /* mail items become leather, plate items become mail below lvl40 */
				else bogus_subclass = proto->SubClass;
			}
			else
				bogus_subclass = proto->SubClass;

			if(!(m_pOwner->GetArmorProficiency()&(((uint32)(1))<<bogus_subclass)))
				return INV_ERR_NO_REQUIRED_PROFICIENCY;

		}
		else if(proto->Class == 2)
		{
			if(!(m_pOwner->GetWeaponProficiency()&(((uint32)(1))<<proto->SubClass)))
				return INV_ERR_NO_REQUIRED_PROFICIENCY;
		}

		if(proto->RequiredSkill)
			if (proto->RequiredSkillRank > m_pOwner->_GetSkillLineCurrent(proto->RequiredSkill,true))
				return INV_ERR_SKILL_ISNT_HIGH_ENOUGH;

		if(proto->RequiredSkillSubRank)
			if (!m_pOwner->HasSpell(proto->RequiredSkillSubRank))
				return INV_ERR_NO_REQUIRED_PROFICIENCY;

				// You are dead !
		if(m_pOwner->getDeathState() != ALIVE)
			return INV_ERR_YOU_ARE_DEAD;
	}

	switch(uint8(slot))//CURRENCYTOKEN_SLOT_ are over 128
	{
	case EQUIPMENT_SLOT_HEAD:
		{
			if(type == INVTYPE_HEAD)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_NECK:
		{
			if(type == INVTYPE_NECK)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_SHOULDERS:
		{
			if(type == INVTYPE_SHOULDERS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_BODY:
		{
			if(type == INVTYPE_BODY)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_CHEST:
		{
			if(type == INVTYPE_CHEST || type == INVTYPE_ROBE)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_WAIST:
		{
			if(type == INVTYPE_WAIST)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_LEGS:
		{
			if(type == INVTYPE_LEGS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_FEET:
		{
			if(type == INVTYPE_FEET)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_WRISTS:
		{
			if(type == INVTYPE_WRISTS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_HANDS:
		{
			if(type == INVTYPE_HANDS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_FINGER1:
	case EQUIPMENT_SLOT_FINGER2:
		{
			if(type == INVTYPE_FINGER)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_TRINKET1:
	case EQUIPMENT_SLOT_TRINKET2:
		{
			if(type == INVTYPE_TRINKET)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_BACK:
		{
			if(type == INVTYPE_CLOAK)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_MAINHAND:
		{
			if( type == INVTYPE_WEAPON || type == INVTYPE_WEAPONMAINHAND ||
				( type == INVTYPE_2HWEAPON && ( !GetInventoryItem(EQUIPMENT_SLOT_OFFHAND ) || skip_2h_check || m_pOwner->DualWield2H ) ) )
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_OFFHAND:
		{
			if( ( type == INVTYPE_2HWEAPON || type == INVTYPE_SHIELD ) && m_pOwner->DualWield2H )
			{
				return 0;
			}

			if(type == INVTYPE_WEAPON || type == INVTYPE_WEAPONOFFHAND)
			{
				Item* mainweapon = GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if(mainweapon) //item exists
				{
					if(mainweapon->GetProto()->InventoryType != INVTYPE_2HWEAPON)
					{
						if(m_pOwner->_HasSkillLine(SKILL_DUAL_WIELD))
							return 0;
						else
							return INV_ERR_CANT_DUAL_WIELD;
					}
					else
					{
						if( !skip_2h_check )
							return INV_ERR_CANT_EQUIP_WITH_TWOHANDED;
						else
							return 0;
					}
				}
				else
				{
					if(m_pOwner->_HasSkillLine(SKILL_DUAL_WIELD))
						return 0;
					else
						return INV_ERR_CANT_DUAL_WIELD;
				}
			}
			else if(type == INVTYPE_SHIELD || type == INVTYPE_HOLDABLE)
			{
				Item* mainweapon = GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if(mainweapon) //item exists
				{
					if( mainweapon->GetProto()->InventoryType != INVTYPE_2HWEAPON )
					{
						return 0;
					}
					else
					{
						if(!skip_2h_check)
							return INV_ERR_CANT_EQUIP_WITH_TWOHANDED;
						else
							return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_RANGED:
		{
			if(type == INVTYPE_RANGED || type == INVTYPE_THROWN || type == INVTYPE_RANGEDRIGHT || type == INVTYPE_RELIC)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;//6;
		}
	case EQUIPMENT_SLOT_TABARD:
		{
			if(type == INVTYPE_TABARD)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT; // 6;
		}
	case BANK_SLOT_BAG_1:
	case BANK_SLOT_BAG_2:
	case BANK_SLOT_BAG_3:
	case BANK_SLOT_BAG_4:
	case BANK_SLOT_BAG_5:
	case BANK_SLOT_BAG_6:
	case BANK_SLOT_BAG_7:
		{
			int32 bytes,slots;
			int8 islot;

			if(!GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot))
			{
				//check if player got that slot.
				bytes = m_pOwner->GetUInt32Value(PLAYER_BYTES_2);
				slots =(uint8) (bytes >> 16);
				islot = GetInternalBankSlotFromPlayer(slot);
				if(slots < islot)
				{
					return INV_ERR_MUST_PURCHASE_THAT_BAG_SLOT;
				}

				//in case bank slot exists, check if player can put the item there
				if(type == INVTYPE_BAG)
				{
					return 0;
				}
				else
				{
					return INV_ERR_NOT_A_BAG;
				}
			}
			else
			{
				if(GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
				{
					if((IsBagSlot(slot) && DstInvSlot == INVENTORY_SLOT_NOT_SET))
					{
						if(proto->InventoryType == INVTYPE_BAG )
						{
							return 0;
						}
					}

					if(proto->BagFamily & GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
					{
						return 0;
					}
					else
					{
						return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
					}
				}
				else
				{
					return 0;
				}
			}
		}
	case INVENTORY_SLOT_BAG_1:	
	case INVENTORY_SLOT_BAG_2:
	case INVENTORY_SLOT_BAG_3:
	case INVENTORY_SLOT_BAG_4:
		{
			//this chunk of code will limit you to equip only 1 Ammo Bag. Later i found out that this is not blizzlike so i will remove it when it's blizzlike
			//we are trying to equip an Ammo Bag
			if(proto->Class==ITEM_CLASS_QUIVER)
			{
				//check if we already have an AB equipped
				FindAmmoBag();
				//we do have ammo bag but we are not swapping them then we send error
				if(result.Slot!=ITEM_NO_SLOT_AVAILABLE && result.Slot != slot)
				{
					return INV_ERR_CAN_EQUIP_ONLY1_AMMOPOUCH;
				}
			}
			if(GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot))
			{
				if(GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
				{
					if((IsBagSlot(slot) && DstInvSlot == INVENTORY_SLOT_NOT_SET))
					{
						if(proto->InventoryType == INVTYPE_BAG )
						{
							return 0;
						}
					}
					
					if(proto->BagFamily & GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
					{
						return 0;
					}
					else
					{
						return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if(type == INVTYPE_BAG)
				{
					return 0;
				}
				else
				{
					return INV_ERR_NOT_A_BAG;
				}
			}
		}
	case INVENTORY_SLOT_ITEM_1:
	case INVENTORY_SLOT_ITEM_2:
	case INVENTORY_SLOT_ITEM_3:
	case INVENTORY_SLOT_ITEM_4:
	case INVENTORY_SLOT_ITEM_5:
	case INVENTORY_SLOT_ITEM_6:
	case INVENTORY_SLOT_ITEM_7:
	case INVENTORY_SLOT_ITEM_8:
	case INVENTORY_SLOT_ITEM_9:
	case INVENTORY_SLOT_ITEM_10:
	case INVENTORY_SLOT_ITEM_11:
	case INVENTORY_SLOT_ITEM_12:
	case INVENTORY_SLOT_ITEM_13:
	case INVENTORY_SLOT_ITEM_14:
	case INVENTORY_SLOT_ITEM_15:
	case INVENTORY_SLOT_ITEM_16:
		{
			return 0;
		}
	case INVENTORY_KEYRING_1:
	case INVENTORY_KEYRING_2:
	case INVENTORY_KEYRING_3:
	case INVENTORY_KEYRING_4:
	case INVENTORY_KEYRING_5:
	case INVENTORY_KEYRING_6:
	case INVENTORY_KEYRING_7:
	case INVENTORY_KEYRING_8:
	case INVENTORY_KEYRING_9:
	case INVENTORY_KEYRING_10:
	case INVENTORY_KEYRING_11:
	case INVENTORY_KEYRING_12:
	case INVENTORY_KEYRING_13:
	case INVENTORY_KEYRING_14:
	case INVENTORY_KEYRING_15:
	case INVENTORY_KEYRING_16:
	case INVENTORY_KEYRING_17:
	case INVENTORY_KEYRING_18:
	case INVENTORY_KEYRING_19:
	case INVENTORY_KEYRING_20:
	case INVENTORY_KEYRING_21:
	case INVENTORY_KEYRING_22:
	case INVENTORY_KEYRING_23:
	case INVENTORY_KEYRING_24:
	case INVENTORY_KEYRING_25:
	case INVENTORY_KEYRING_26:
	case INVENTORY_KEYRING_27:
	case INVENTORY_KEYRING_28:
	case INVENTORY_KEYRING_29:
	case INVENTORY_KEYRING_30:
	case INVENTORY_KEYRING_31:
	case INVENTORY_KEYRING_32:
		{
			if(proto->BagFamily & ITEM_TYPE_KEYRING || proto->Class == ITEM_CLASS_KEY)
			{
					return 0;
			}
			else
			{
					return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
			}
		}
	case CURRENCYTOKEN_SLOT_1:
	case CURRENCYTOKEN_SLOT_2:	
	case CURRENCYTOKEN_SLOT_3:	
	case CURRENCYTOKEN_SLOT_4:		
	case CURRENCYTOKEN_SLOT_5:	
	case CURRENCYTOKEN_SLOT_6:		
	case CURRENCYTOKEN_SLOT_7:		
	case CURRENCYTOKEN_SLOT_8:		
	case CURRENCYTOKEN_SLOT_9:		
	case CURRENCYTOKEN_SLOT_10:		
	case CURRENCYTOKEN_SLOT_11:		
	case CURRENCYTOKEN_SLOT_12:		
	case CURRENCYTOKEN_SLOT_13:		
	case CURRENCYTOKEN_SLOT_14:		
	case CURRENCYTOKEN_SLOT_15:		
	case CURRENCYTOKEN_SLOT_16:		
	case CURRENCYTOKEN_SLOT_17:	
	case CURRENCYTOKEN_SLOT_18:	
	case CURRENCYTOKEN_SLOT_19:	
	case CURRENCYTOKEN_SLOT_20:	
	case CURRENCYTOKEN_SLOT_21:		
	case CURRENCYTOKEN_SLOT_22:	
	case CURRENCYTOKEN_SLOT_23:		
	case CURRENCYTOKEN_SLOT_24:		
	case CURRENCYTOKEN_SLOT_25:		
	case CURRENCYTOKEN_SLOT_26:		
	case CURRENCYTOKEN_SLOT_27:		
	case CURRENCYTOKEN_SLOT_28:		
	case CURRENCYTOKEN_SLOT_29:		
	case CURRENCYTOKEN_SLOT_30:		
	case CURRENCYTOKEN_SLOT_31:		
	case CURRENCYTOKEN_SLOT_32:		
		{
			if( proto->BagFamily & ITEM_TYPE_CURRENCY )
			{
					return 0;
			}
			else
			{
					return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
			}
		}
	default:
		return 0;
	}
}

//-------------------------------------------------------------------//
//Description: Checks if player can receive the item
//-------------------------------------------------------------------//
int8 ItemInterface::CanReceiveItem(ItemPrototype * item, uint32 amount)
{
	if(!item)
	{
		return 0;
	}

	if(item->Unique)
	{
		uint32 count = GetItemCount(item->ItemId, true);
		if(count == item->Unique || ((count + amount) > item->Unique))
		{
			return INV_ERR_CANT_CARRY_MORE_OF_THIS;
		}
	}
	if( item->ItemLimitCategory > 0 )
	{
		ItemLimitCategoryEntry * ile = dbcItemLimitCategory.LookupEntryForced( item->ItemLimitCategory );
		if( ile && !(ile->equippedFlag & ILFLAG_EQUIP_ONLY) )
		{
			uint32 count = GetItemCountByLimitId( ile->Id, false );
			if( count >= ile->maxAmount || ( ( count + amount) > ile->maxAmount ) )
				return INV_ERR_CANT_CARRY_MORE_OF_THIS;
		}
	}
	return 0;
}

void ItemInterface::BuyItem(ItemPrototype *item, uint32 total_amount, Creature * pVendor)
{
	if(item->BuyPrice)
	{
		uint32 itemprice = GetBuyPriceForItem(item, total_amount, m_pOwner, pVendor);
		if( !m_pOwner->HasGold(itemprice) )
			m_pOwner->SetGold( 0 );
		else
			m_pOwner->ModGold( -(int32)itemprice );
	}
	ItemExtendedCostEntry * ex = pVendor->GetItemExtendedCostByItemId( item->ItemId );
	if( ex != NULL )
	{
		for(int i = 0;i<5;i++)
		{
			if(ex->item[i])
				m_pOwner->GetItemInterface()->RemoveItemAmt( ex->item[i], total_amount * ex->count[i] );
		}

		if(m_pOwner->GetHonorCurrency() >= (ex->honor*total_amount))
		{
			m_pOwner->ModHonorCurrency(-int32((ex->honor*total_amount)));
			m_pOwner->m_honorPoints -=int32(ex->honor*total_amount);
		}
		if(m_pOwner->GetArenaCurrency() >= (ex->arena*total_amount))
		{
			m_pOwner->ModArenaCurrency(-int32(ex->arena*total_amount));
			m_pOwner->m_arenaPoints -=int32(ex->arena*total_amount);
		}        
	}
}

enum CanAffordItem
{
	CAN_AFFORD_ITEM_ERROR_NOT_FOUND				= 0,
	CAN_AFFORD_ITEM_ERROR_SOLD_OUT				= 1,
	CAN_AFFORD_ITEM_ERROR_NOT_ENOUGH_MONEY		= 2,
	CAN_AFFORD_ITEM_ERROR_DOESNT_LIKE_YOU		= 4,
	CAN_AFFORD_ITEM_ERROR_TOO_FAR_AWAY			= 5,
	CAN_AFFORD_ITEM_ERROR_CANT_CARRY_ANY_MORE	= 8,
	CAN_AFFORD_ITEM_ERROR_NOT_REQUIRED_RANK		= 11,
	CAN_AFFORD_ITEM_ERROR_REPUTATION			= 12,
};

int8 ItemInterface::CanAffordItem(ItemPrototype * item,uint32 amount, Creature * pVendor)
{
	ItemExtendedCostEntry * ex = pVendor->GetItemExtendedCostByItemId( item->ItemId );
	if( ex != NULL )
	{
		for(int i = 0;i<5;i++)
		{
			if(ex->item[i])
			{
				if(m_pOwner->GetItemInterface()->GetItemCount(ex->item[i], false) < (ex->count[i]*amount))
					return INV_ERR_ITEM_NOT_FOUND;
			}
		}

		if(m_pOwner->GetHonorCurrency() < (ex->honor*amount))
			return INV_ERR_NOT_ENOUGH_HONOR_POINTS;
		if(m_pOwner->GetArenaCurrency() < (ex->arena*amount))
			return INV_ERR_NOT_ENOUGH_ARENA_POINTS;
 		if(m_pOwner->GetMaxPersonalRating() < ex->personalrating)
			return CAN_AFFORD_ITEM_ERROR_NOT_REQUIRED_RANK;
	}

	if(item->BuyPrice)
	{
		int32 price = GetBuyPriceForItem(item, amount, m_pOwner, pVendor) * amount;
		if( !m_pOwner->HasGold(price) )
		{
			return CAN_AFFORD_ITEM_ERROR_NOT_ENOUGH_MONEY;
		}
	}
	if(item->RequiredFaction)
	{
		FactionDBC *factdbc = dbcFaction.LookupEntryForced(item->RequiredFaction);
		if(!factdbc || factdbc->RepListId < 0)
			return (int8)NULL;
		
		if( m_pOwner->GetReputationRankFromStanding( m_pOwner->GetStanding( item->RequiredFaction )) < (int32)item->RequiredFactionStanding )
		{
			return CAN_AFFORD_ITEM_ERROR_REPUTATION;
		}
	}
	return 0;
}

//-------------------------------------------------------------------//
//Description: Gets the Item slot by item type
//-------------------------------------------------------------------//
int8 ItemInterface::GetItemSlotByType(uint32 type)
{
	switch(type)
	{
	case INVTYPE_NON_EQUIP:
		return ITEM_NO_SLOT_AVAILABLE; 
	case INVTYPE_HEAD:
		{
			return EQUIPMENT_SLOT_HEAD;
		}
	case INVTYPE_NECK:
		{
			return EQUIPMENT_SLOT_NECK;
		}
	case INVTYPE_SHOULDERS:
		{
			return EQUIPMENT_SLOT_SHOULDERS;
		}
	case INVTYPE_BODY:
		{
			return EQUIPMENT_SLOT_BODY;
		}
	case INVTYPE_CHEST:
		{
			return EQUIPMENT_SLOT_CHEST;
		}
	case INVTYPE_ROBE: // ???
		{
			return EQUIPMENT_SLOT_CHEST;
		}
	case INVTYPE_WAIST:
		{
			return EQUIPMENT_SLOT_WAIST;
		}
	case INVTYPE_LEGS:
		{
			return EQUIPMENT_SLOT_LEGS;
		}
	case INVTYPE_FEET:
		{
			return EQUIPMENT_SLOT_FEET;
		}
	case INVTYPE_WRISTS:
		{
			return EQUIPMENT_SLOT_WRISTS;
		}
	case INVTYPE_HANDS:
		{
			return EQUIPMENT_SLOT_HANDS;
		}
	case INVTYPE_FINGER:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_FINGER1))
				return EQUIPMENT_SLOT_FINGER1;
			else if (!GetInventoryItem(EQUIPMENT_SLOT_FINGER2))
				return EQUIPMENT_SLOT_FINGER2;
			else
				return EQUIPMENT_SLOT_FINGER1; //auto equips always in finger 1
		}
	case INVTYPE_TRINKET:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_TRINKET1))
				return EQUIPMENT_SLOT_TRINKET1;
			else if (!GetInventoryItem(EQUIPMENT_SLOT_TRINKET2))
				return EQUIPMENT_SLOT_TRINKET2;
			else
				return EQUIPMENT_SLOT_TRINKET1; //auto equips always on trinket 1
		}
	case INVTYPE_CLOAK:
		{
			return EQUIPMENT_SLOT_BACK;
		}
	case INVTYPE_WEAPON:
		{
			if( !GetInventoryItem( EQUIPMENT_SLOT_MAINHAND ) /* || !m_pOwner->DualWield2H */ )
				return EQUIPMENT_SLOT_MAINHAND;
			else if(!GetInventoryItem(EQUIPMENT_SLOT_OFFHAND))
				return EQUIPMENT_SLOT_OFFHAND;
			else
				return EQUIPMENT_SLOT_MAINHAND;
		}
	case INVTYPE_SHIELD:
		{
			return EQUIPMENT_SLOT_OFFHAND;
		}
	case INVTYPE_RANGED:
		{
			return EQUIPMENT_SLOT_RANGED;
		}
	case INVTYPE_2HWEAPON:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_MAINHAND) )
				return EQUIPMENT_SLOT_MAINHAND;
			else if(!GetInventoryItem(EQUIPMENT_SLOT_OFFHAND))
				return EQUIPMENT_SLOT_OFFHAND;
			else
				return EQUIPMENT_SLOT_MAINHAND;
		}
	case INVTYPE_TABARD:
		{
			return EQUIPMENT_SLOT_TABARD;
		}
	case INVTYPE_WEAPONMAINHAND:
		{
			return EQUIPMENT_SLOT_MAINHAND;
		}
	case INVTYPE_WEAPONOFFHAND:
		{
			return EQUIPMENT_SLOT_OFFHAND;
		}
	case INVTYPE_HOLDABLE:
		{
			return EQUIPMENT_SLOT_OFFHAND;
		}
	case INVTYPE_THROWN:
		return EQUIPMENT_SLOT_RANGED; // ?
	case INVTYPE_RANGEDRIGHT:
		return EQUIPMENT_SLOT_RANGED; // ?
	case INVTYPE_RELIC:
		return EQUIPMENT_SLOT_RANGED;
	case INVTYPE_BAG:
		{
			for (int8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
			{
				if (!GetInventoryItem(i))
					return i;
			}
			return ITEM_NO_SLOT_AVAILABLE; //bags are not supposed to be auto-equipped when slots are not free
		}
	default:
		return ITEM_NO_SLOT_AVAILABLE;
	}
}

//-------------------------------------------------------------------//
//Description: Gets a Item by guid
//-------------------------------------------------------------------//
Item* ItemInterface::GetItemByGUID(uint64 Guid)
{
	uint32 i;

	//EQUIPMENT
	for( i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++ )
	{
		if( m_pItems[i] != 0 )
		{
			if( m_pItems[i]->GetGUID() == Guid )
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;//not a containerslot. In 1.8 client marked wrong slot like this
				result.Slot = static_cast<int8>( i );
				return m_pItems[i];
			}
		}
	}

	//INVENTORY BAGS
	for( i = INVENTORY_SLOT_BAG_START; i <INVENTORY_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL && m_pItems[i]->IsContainer() )
		{
			if( m_pItems[i]->GetGUID() == Guid ) 
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = static_cast<int8>( i );
				return m_pItems[i]; 
			}

			for( uint32 j = 0; j < m_pItems[i]->GetProto()->ContainerSlots; j++ )
			{
				Item *item2 = ( static_cast<Container*>( m_pItems[i] ))->GetItem( static_cast<int16>( j ));
				if( item2 )
				{
					if( item2->GetGUID() == Guid )
					{
						result.ContainerSlot = static_cast<int8>( i );
						result.Slot = static_cast<int8>( j );
						return item2;
					}
				}
			}
		}
	}

	//INVENTORY
	for( i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++ )
	{
		if(m_pItems[i] != 0)
		{
			if(m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = static_cast<int8>( i );
				return m_pItems[i];
			}
		}
	}

	//Keyring
	for(i=INVENTORY_KEYRING_START;i<INVENTORY_KEYRING_END;i++)
	{
		if(m_pItems[i] != 0)
		{
			if(m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = static_cast<int8>( i );
				return m_pItems[i];
			}
		}
	}

	//Currency
	for(i=CURRENCYTOKEN_SLOT_START;i<CURRENCYTOKEN_SLOT_END;i++)
	{
		if(m_pItems[i] != 0)
		{
			if(m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = static_cast<int8>( i );
				return m_pItems[i];
			}
		}
	}
	return NULL;
}

//-------------------------------------------------------------------//
//Description: Inventory Error report
//-------------------------------------------------------------------//
void ItemInterface::BuildInventoryChangeError(Item *SrcItem, Item *DstItem, uint8 Error)
{
	WorldPacket data( SMSG_INVENTORY_CHANGE_FAILURE, 22);

	data << uint8( Error );

	if( SrcItem != NULL )
		data << uint64( SrcItem->GetGUID() );
	else
		data << uint64( 0 );

	if( DstItem != NULL )
		data << uint64( DstItem->GetGUID() );
	else
		data << uint64( 0 );

	data << uint8( 0 );

	if( Error == INV_ERR_YOU_MUST_REACH_LEVEL_N ) 
	{
		if(SrcItem)
		{
			data << uint32( SrcItem->GetProto()->RequiredLevel );
		}
	}

	m_pOwner->SendPacket( &data );
}

void ItemInterface::EmptyBuyBack()
{
	 for (uint32 j = 0;j < MAX_BUYBACK_SLOT;j++)
	 {
		 if (m_pBuyBack[j] != NULL)
		 {
			 m_pOwner->SendDestroyObject( m_pBuyBack[ j ]->GetGUID() );
			 m_pBuyBack[j]->DeleteFromDB();

			 if(m_pBuyBack[j]->IsContainer())
			 {
				if (static_cast<Container*>(m_pBuyBack[j])->IsInWorld())
					static_cast<Container*>(m_pBuyBack[j])->RemoveFromWorld();
				
				delete static_cast<Container*>(m_pBuyBack[j]);
			 }
			 else
			 {
				if (m_pBuyBack[j]->IsInWorld())
					m_pBuyBack[j]->RemoveFromWorld();
				delete m_pBuyBack[j];
				m_pBuyBack[j] = NULL;
			 }

			 m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*j),0);
			 m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + j,0);
			 m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j,0);
			 m_pBuyBack[j] = NULL;
		 }
		 else
			 break;
	 }
}

void ItemInterface::AddBuyBackItem(Item *it,uint32 price)
{
	int i;
	if ((m_pBuyBack[MAX_BUYBACK_SLOT-1] != NULL) && (m_pOwner->GetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (MAX_BUYBACK_SLOT-1)*2) != 0))
	{
		if(m_pBuyBack[0] != NULL)
		{		   
			m_pOwner->SendDestroyObject( m_pBuyBack[ 0 ]->GetGUID() );
			 m_pBuyBack[0]->DeleteFromDB();

			 if(m_pBuyBack[0]->IsContainer())
			 {
				if (static_cast<Container*>(m_pBuyBack[0])->IsInWorld())
					static_cast<Container*>(m_pBuyBack[0])->RemoveFromWorld();
				
				delete static_cast<Container*>(m_pBuyBack[0]);
			 }
			 else
			 {
				if (m_pBuyBack[0]->IsInWorld())
					m_pBuyBack[0]->RemoveFromWorld();
				delete m_pBuyBack[0];
			 }

			m_pBuyBack[0] = NULL;
		}

		for (int j = 0;j < MAX_BUYBACK_SLOT-1;j++)
		{
			//SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*j),buyback[j+1]->GetGUID());
			m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*j),m_pOwner->GetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ((j+1)*2) ) );
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + j,m_pOwner->GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + j+1));
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j,m_pOwner->GetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j+1));
			m_pBuyBack[j] = m_pBuyBack[j+1];
		}
		m_pBuyBack[MAX_BUYBACK_SLOT-1] = it;

		m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*(MAX_BUYBACK_SLOT-1)),m_pBuyBack[MAX_BUYBACK_SLOT-1]->GetGUID());
		m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + MAX_BUYBACK_SLOT-1,price);
		m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + MAX_BUYBACK_SLOT-1,(uint32)UNIXTIME);
		return;
	}

	for(i= 0; i <= (MAX_BUYBACK_SLOT - 1)*2;i+=2) //at least 1 slot is empty
	{
		if((m_pOwner->GetUInt32Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + i) == 0) || (m_pBuyBack[i/2] == NULL))
		{
			sLog.outDetail("setting buybackslot %u",i/2);
			m_pBuyBack[i >> 1] = it;

			m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + i,m_pBuyBack[i >> 1]->GetGUID());
			//SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + i,it->GetGUID());
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + (i >> 1),price);
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + (i >> 1),(uint32)UNIXTIME);
			return;
		}
	}
}

void ItemInterface::RemoveBuyBackItem(uint32 index)
{
	int32 j = 0;
	for( j = index;j < MAX_BUYBACK_SLOT-1; j++ )
	{
		if (m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( j * 2 ) ) != 0 )
		{
			m_pOwner->SetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( 2 * j ), m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( ( j + 1 ) * 2 ) ) );
			m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j+1));
			m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j + 1 ) );
			
			if( m_pBuyBack[j+1] != NULL && ( m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( ( j + 1 ) * 2 ) ) != 0 ) )
			{
				m_pBuyBack[j] = m_pBuyBack[j+1];
			}
			else
			{
				m_pBuyBack[j] = NULL;

				sLog.outDetail( "nulling %u", j );
			}
		}
		else
			return;
	}
	j = 11;
	m_pOwner->SetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( 2 * j ), m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( ( j + 1 ) * 2 ) ) );
	m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j + 1 ) );
	m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j + 1 ) );
	if( m_pBuyBack[MAX_BUYBACK_SLOT-1] )
	{
		m_pBuyBack[MAX_BUYBACK_SLOT-1] = NULL;
	}
	
}

//-------------------------------------------------------------------//
//Description: swap inventory slots
//-------------------------------------------------------------------//
void ItemInterface::SwapItemSlots(int8 srcslot, int8 dstslot)
{
	// srcslot and dstslot are int... NULL might not be an int depending on arch where it is compiled
	if( srcslot >= MAX_INVENTORY_SLOT || srcslot < 0 )
		return;

	if( dstslot >= MAX_INVENTORY_SLOT || dstslot < 0 )
		return;

	Item* SrcItem = GetInventoryItem( srcslot );
	Item* DstItem = GetInventoryItem( dstslot );

	sLog.outDebug( "ItemInterface::SwapItemSlots(%u, %u);" , srcslot , dstslot );
	//Item * temp = GetInventoryItem( srcslot );
	//if( temp )
	//	sLog.outDebug( "Source item: %s (inventoryType=%u, realslot=%u);" , temp->GetProto()->Name1 , temp->GetProto()->InventoryType , GetItemSlotByType( temp->GetProto()->InventoryType ) );
	//	temp = GetInventoryItem( dstslot );
	//if( temp )
	//	sLog.outDebug( "Destination: Item: %s (inventoryType=%u, realslot=%u);" , temp->GetProto()->Name1 , temp->GetProto()->InventoryType , GetItemSlotByType( temp->GetProto()->InventoryType ) );
	//else
	//	sLog.outDebug( "Destination: Empty" );

	// don't stack equipped items (even with ItemStackCheat), just swap them
	uint32 srcItemMaxStack, dstItemMaxStack;
	if( SrcItem != NULL )
	{
		if( srcslot < INVENTORY_SLOT_BAG_END || !(SrcItem->GetOwner()->ItemStackCheat) )
		{
			srcItemMaxStack = SrcItem->GetProto()->MaxCount;
		}
		else
		{
			srcItemMaxStack = 0x7fffffff;
		}
	}
	else
	{
		srcItemMaxStack = 0;
	}
	if( DstItem != NULL )
	{
		if( dstslot < INVENTORY_SLOT_BAG_END || !(DstItem->GetOwner()->ItemStackCheat) )
		{
			dstItemMaxStack = DstItem->GetProto()->MaxCount;
		}
		else
		{
			dstItemMaxStack = 0x7fffffff;
		}
	}
	else
	{
		dstItemMaxStack = 0;
	}
	
	if( SrcItem != NULL && DstItem != NULL && SrcItem->GetEntry()==DstItem->GetEntry() && srcItemMaxStack > 1 && SrcItem->wrapped_item_id == 0 && DstItem->wrapped_item_id == 0 )
	{
		uint32 total = SrcItem->GetStackCount() + DstItem->GetStackCount();
		if( total <= dstItemMaxStack )
		{
			DstItem->ModStackCount(  SrcItem->GetStackCount() );
			SafeFullRemoveItemFromSlot( INVENTORY_SLOT_NOT_SET, srcslot );
			DstItem->m_isDirty = true;
			return;
		}
		else
		{
			if( DstItem->GetStackCount() == dstItemMaxStack )
			{

			}
			else
			{
				int32 delta = dstItemMaxStack - DstItem->GetStackCount();
				DstItem->SetStackCount(  dstItemMaxStack );
				SrcItem->ModStackCount(  -delta );
				SrcItem->m_isDirty = true;
				DstItem->m_isDirty = true;
				return;
			}
		}
	}

	//src item was equipped previously
	if( srcslot < INVENTORY_SLOT_BAG_END ) 
	{
		if( m_pItems[(int)srcslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)srcslot], srcslot, false );
	}

	//dst item was equipped previously
	if( dstslot < INVENTORY_SLOT_BAG_END ) 
	{
		if( m_pItems[(int)dstslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)dstslot], dstslot, false );
	}

	//sLog.outDebug( "Putting items into slots..." );



	m_pItems[(int)dstslot] = SrcItem;

	// Moving a bag with items to a empty bagslot
	if ( DstItem == NULL && SrcItem != NULL && SrcItem->IsContainer() )
	{
		Item *tSrcItem = NULL;

		for ( uint32 Slot = 0; Slot < SrcItem->GetProto()->ContainerSlots; Slot++ )
		{
			tSrcItem = ( static_cast<Container*>( (m_pItems[(int)srcslot]) ))->GetItem( static_cast<int16>( Slot ));

			m_pOwner->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(srcslot, static_cast<int16>( Slot ), false);

			if ( tSrcItem != NULL )
			{
				m_pOwner->GetItemInterface()->SafeAddItem(tSrcItem, dstslot, static_cast<int16>( Slot ));
			}
		}
	}

	m_pItems[(int)srcslot] = DstItem;

	// swapping 2 bags filled with items
	if ( DstItem != NULL && SrcItem != NULL && SrcItem->IsContainer() && DstItem->IsContainer() )
	{
		Item *tDstItem = NULL;
		Item *tSrcItem = NULL;
		uint32 TotalSlots = 0;

		// Determine the max amount of slots to swap
		if ( SrcItem->GetProto()->ContainerSlots > DstItem->GetProto()->ContainerSlots )
			TotalSlots = SrcItem->GetProto()->ContainerSlots;
		else
			TotalSlots = DstItem->GetProto()->ContainerSlots;

		// swap items in the bags
		for( uint32 Slot = 0; Slot < TotalSlots; Slot++ )
		{
			tSrcItem = (static_cast<Container*>( (m_pItems[(int)srcslot]) ))->GetItem( static_cast<int16>( Slot ));
			tDstItem = (static_cast<Container*>( (m_pItems[(int)dstslot]) ))->GetItem( static_cast<int16>( Slot ));

			if ( tSrcItem != NULL )
				m_pOwner->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(srcslot, static_cast<int16>( Slot ), false);
			if ( tDstItem != NULL )
				m_pOwner->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(dstslot, static_cast<int16>( Slot ), false);

			if ( tSrcItem != NULL )
				(static_cast<Container*>( DstItem ))->AddItem(static_cast<int16>( Slot ), tSrcItem);
			if ( tDstItem != NULL )
				(static_cast<Container*>(SrcItem))->AddItem(static_cast<int16>( Slot ), tDstItem);
		}
	}

	if( DstItem != NULL )
		DstItem->m_isDirty = true;
	if( SrcItem != NULL )
		SrcItem->m_isDirty = true;

	if( m_pItems[(int)dstslot] != NULL )
	{
		//sLog.outDebug( "(SrcItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now %u" , dstslot * 2 , m_pItems[(int)dstslot]->GetGUID() );
		m_pOwner->SetInventorySlot(dstslot,  m_pItems[(int)dstslot]->GetGUID() );
	}
	else
	{
		//sLog.outDebug( "(SrcItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now 0" , dstslot * 2 );
		m_pOwner->SetInventorySlot(dstslot, 0 );
	}

	if( m_pItems[(int)srcslot] != NULL )
	{
		//sLog.outDebug( "(DstItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now %u" , dstslot * 2 , m_pItems[(int)srcslot]->GetGUID() );
		m_pOwner->SetInventorySlot(srcslot, m_pItems[(int)srcslot]->GetGUID() );
	}
	else
	{
		//sLog.outDebug( "(DstItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now 0" , dstslot * 2 );
		m_pOwner->SetInventorySlot(srcslot, 0 );
	}

	if( srcslot < INVENTORY_SLOT_BAG_END )	// source item is equipped
	{
		if( m_pItems[(int)srcslot] ) // dstitem goes into here.
		{
			// Bags aren't considered "visible".
			if( srcslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (srcslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, m_pItems[(int)srcslot]->GetEntry() );
                m_pOwner->SetUInt32Value( VisibleBase + 1, m_pItems[(int)srcslot]->GetEnchantmentId( 0 ) );
			}

			// handle bind on equip
			if( m_pItems[(int)srcslot]->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
				m_pItems[(int)srcslot]->SoulBind();
		} 
		else 
		{
			// Bags aren't considered "visible".
			if( srcslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (srcslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 1, 0 );
/*				m_pOwner->SetUInt32Value( VisibleBase + 2, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 3, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 4, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 5, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 6, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 7, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 8, 0 );*/
			}
		}
	}  

	if( dstslot < INVENTORY_SLOT_BAG_END )   // source item is inside inventory
	{
		if( m_pItems[(int)dstslot] != NULL ) // srcitem goes into here.
		{	
			// Bags aren't considered "visible".
			if( dstslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (dstslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, m_pItems[(int)dstslot]->GetEntry() );
                m_pOwner->SetUInt32Value( VisibleBase + 1, m_pItems[(int)dstslot]->GetEnchantmentId( 0 ) );
			}

			// handle bind on equip
			if( m_pItems[(int)dstslot]->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
				m_pItems[(int)dstslot]->SoulBind();

		}
		else
		{

			// bags aren't considered visible
			if( dstslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (dstslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 1, 0 );
/*				m_pOwner->SetUInt32Value( VisibleBase + 2, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 3, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 4, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 5, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 6, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 7, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 8, 0 );*/
			}
		}
	}

	// handle dual wield
	if( dstslot == EQUIPMENT_SLOT_OFFHAND || srcslot == EQUIPMENT_SLOT_OFFHAND )
	{
		if( m_pItems[EQUIPMENT_SLOT_OFFHAND] != NULL && m_pItems[EQUIPMENT_SLOT_OFFHAND]->GetProto()->Class == ITEM_CLASS_WEAPON ){
			m_pOwner->SetDualWield( true );

		/////////////////////////////////////////// Titan's grip stuff ////////////////////////////////////////////////////////////

		uint32 subclass = m_pItems[EQUIPMENT_SLOT_OFFHAND]->GetProto()->SubClass;
		if( subclass == ITEM_SUBCLASS_WEAPON_TWOHAND_AXE 
			|| subclass == ITEM_SUBCLASS_WEAPON_TWOHAND_MACE 
			|| subclass == ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD ){
				
				m_pOwner->CastSpell( m_pOwner, 49152, true );
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}else
			m_pOwner->SetDualWield( false );
	}

	//src item is equipped now
	if( srcslot < INVENTORY_SLOT_BAG_END ) 
 	{
		if( m_pItems[(int)srcslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)srcslot], srcslot, true );
		else if( srcslot == EQUIPMENT_SLOT_MAINHAND || srcslot == EQUIPMENT_SLOT_OFFHAND )
			m_pOwner->CalcDamage();
 	}

	//dst item is equipped now
	if( dstslot < INVENTORY_SLOT_BAG_END ) 
	{
		if( m_pItems[(int)dstslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)dstslot], dstslot, true );
		else if( dstslot == EQUIPMENT_SLOT_MAINHAND || dstslot == EQUIPMENT_SLOT_OFFHAND )
			m_pOwner->CalcDamage();
	}

	//Recalculate Expertise (for Weapon specs)
	m_pOwner->CalcExpertise();
}

//-------------------------------------------------------------------//
//Description: Item Loading
//-------------------------------------------------------------------//
void ItemInterface::mLoadItemsFromDatabase(QueryResult * result)
{
	int8 containerslot, slot;
	Item* item;
	ItemPrototype* proto;

	if( result )
	{
		do
		{
			Field* fields = result->Fetch();

            containerslot = fields[13].GetInt8();
			slot = fields[14].GetInt8();
			proto = ItemPrototypeStorage.LookupEntry(fields[2].GetUInt32());

			if( proto != NULL )
			{
				if( proto->InventoryType == INVTYPE_BAG )
				{
					item = new Container( HIGHGUID_TYPE_CONTAINER, fields[1].GetUInt32() );
					static_cast< Container* >( item )->LoadFromDB( fields );

				}
				else
				{
					item = new Item;
					item->Init( HIGHGUID_TYPE_ITEM, fields[1].GetUInt32() );
					item->LoadFromDB( fields, m_pOwner, false);

				}

                // if we encounter an item that expired, we remove it from db
                if( item->GetItemExpireTime() > 0 && UNIXTIME > item->GetItemExpireTime() ){
                    item->DeleteFromDB();
                    item->DeleteMe();
                    continue;
                }
                
				if( SafeAddItem( item, containerslot, slot ) )
				    item->m_isDirty = false;
				else
				{
					delete item;
					item = NULL;
				}
			}
		}
		while( result->NextRow() );
	}
}

//-------------------------------------------------------------------//
//Description: Item saving
//-------------------------------------------------------------------//
void ItemInterface::mSaveItemsToDatabase(bool first, QueryBuffer * buf)
{
	int16 x;

	for( x = EQUIPMENT_SLOT_START; x < CURRENCYTOKEN_SLOT_END; ++x )
	{
		if( GetInventoryItem( x ) != NULL )
		{
			if( IsBagSlot( x ) && GetInventoryItem( x )->IsContainer() )
			{
				static_cast< Container* >( GetInventoryItem( x ) )->SaveBagToDB( static_cast<int8>( x ), first, buf );
			}
			else
			{
				GetInventoryItem( x )->SaveToDB( INVENTORY_SLOT_NOT_SET, static_cast<int8>( x ), first, buf );
			}
		}
	}
}

AddItemResult ItemInterface::AddItemToFreeBankSlot(Item *item)
{
	//special items first
	for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL )
		{
			if( m_pItems[i]->GetProto()->BagFamily & item->GetProto()->BagFamily )
			{
				if( m_pItems[i]->IsContainer() )
				{
					bool result = static_cast< Container* >( m_pItems[i] )->AddItemToFreeSlot( item, NULL );
					if( result )
						return ADD_ITEM_RESULT_OK;
				}
			}
		}
	}

	for( int16 i= BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++ )
	{
		if( m_pItems[i] == NULL )
		{
			return SafeAddItem( item, INVENTORY_SLOT_NOT_SET, i );
		}
	}

	for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL && m_pItems[i]->GetProto()->BagFamily == 0 && m_pItems[i]->IsContainer() ) //special bags ignored
		{
			for( uint32 j = 0; j < m_pItems[i]->GetProto()->ContainerSlots; j++ )
			{
				Item *item2 = static_cast< Container* >( m_pItems[i] )->GetItem( static_cast<int16>( j ));
				if( item2 == NULL )
				{
					return SafeAddItem( item, static_cast<int8>( i ), static_cast<int16>( j ) );
				}
			}
		}
	}
	return ADD_ITEM_RESULT_ERROR;
}

int8 ItemInterface::FindSpecialBag(Item *item)
{
	for( uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL )
		{
			if( m_pItems[i]->GetProto()->BagFamily & item->GetProto()->BagFamily )
			{
				return i;
			}
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

int8 ItemInterface::FindFreeKeyringSlot()
{
	for( uint8 i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++ )
	{
		if( m_pItems[i] == NULL )
		{
			return i;
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

int16 ItemInterface::FindFreeCurrencySlot()
{
	for( uint16 i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++ )
	{
		if( m_pItems[i] == NULL )
		{
			return i;
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

SlotResult ItemInterface::FindFreeInventorySlot(ItemPrototype *proto)
{
	//special item
	//special slots will be ignored of item is not set
	if( proto != NULL )
	{
		//sLog.outDebug( "ItemInterface::FindFreeInventorySlot called for item %s" , proto->Name1 );
		if( proto->BagFamily)
		{
			if( proto->BagFamily & ITEM_TYPE_KEYRING || proto->Class == ITEM_CLASS_KEY )
			{
				for(uint32 i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++ )
				{
					if( m_pItems[i] == NULL )
					{
						result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
						result.Slot = static_cast<int8>( i );
						result.Result = true;
						return result;
					}
				}
			}
			else if( proto->BagFamily & ITEM_TYPE_CURRENCY )
			{
				for(uint32 i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++ )
				{
					if( m_pItems[i] == NULL )
					{
						result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
						result.Slot = static_cast<int8>( i );
						result.Result = true;
						return result;
					}
				}
			}
			else
			{
				for( uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
				{
					if( m_pItems[i] != NULL && m_pItems[i]->IsContainer() )
					{
						if( m_pItems[i]->GetProto()->BagFamily & proto->BagFamily )
						{
							int32 slot = static_cast< Container* >( m_pItems[i] )->FindFreeSlot();
							if( slot != ITEM_NO_SLOT_AVAILABLE ) 
							{
								result.ContainerSlot = static_cast<int8>( i );
								result.Slot = static_cast<int8>( slot );
								result.Result = true;
								return result;
							}
						}
					}
				}
			}
		}
	}

	//backpack
	for( uint32 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++ )
	{
		Item *item = GetInventoryItem( static_cast<int16>( i ) );
		if( item == NULL ) 
		{
			result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
			result.Slot = static_cast<int8>( i );
			result.Result = true;
			return result;
		}
	}

	//bags
	for( uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
	{
		Item* item = GetInventoryItem(static_cast<int16>( i ));
		if( item != NULL )
		{
			if( item->IsContainer() && !item->GetProto()->BagFamily )
			{
				int32 slot = static_cast< Container* >( m_pItems[i] )->FindFreeSlot();
				if( slot != ITEM_NO_SLOT_AVAILABLE ) 
				{ 
					result.ContainerSlot = static_cast<int8>( i );
					result.Slot = static_cast<int8>( slot );
					result.Result = true;
					return result;
				}
			}
		}
	}

	result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
	result.Slot = ITEM_NO_SLOT_AVAILABLE;
	result.Result = false;

	return result;
}

SlotResult ItemInterface::FindFreeBankSlot(ItemPrototype *proto)
{
	//special item
	//special slots will be ignored of item is not set
	if( proto != NULL )
	{
		if( proto->BagFamily )
		{
			for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
			{
				if( m_pItems[i] != NULL && m_pItems[i]->IsContainer() )
				{
					if( m_pItems[i]->GetProto()->BagFamily & proto->BagFamily )
					{
						int32 slot = static_cast< Container* >( m_pItems[i] )->FindFreeSlot();
						if( slot != ITEM_NO_SLOT_AVAILABLE ) 
						{
							result.ContainerSlot = static_cast<int8>( i );
							result.Slot = static_cast<int8>( slot );
							result.Result = true;
							return result;
						}
					}
				}
			}
		}
	}

	//backpack
	for( uint32 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++ )
	{
		Item* item = GetInventoryItem( static_cast<int16>( i ) );
		if( item == NULL ) 
		{
			result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
			result.Slot = static_cast<int8>( i );
			result.Result = true;
			return result;
		}
	}

	//bags
	for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
	{
		Item* item = GetInventoryItem( static_cast<int16>( i ));
		if( item != NULL )
		{
			if( item->IsContainer() && !item->GetProto()->BagFamily )
			{
				int32 slot = static_cast< Container* >( m_pItems[i] )->FindFreeSlot();
				if( slot != ITEM_NO_SLOT_AVAILABLE) 
				{ 
					result.ContainerSlot = static_cast<int8>( i );
					result.Slot = static_cast<int8>( slot );
					result.Result = true;
					return result;
				}
			}
		}
	}

	result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
	result.Slot = ITEM_NO_SLOT_AVAILABLE;
	result.Result = false;

	return result;
}

SlotResult ItemInterface::FindAmmoBag()
{
	for( uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
		if( m_pItems[i] != NULL && m_pItems[i]->IsAmmoBag())
		{
			result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
			result.Slot = static_cast<int8>( i );
			result.Result = true;
			return result;
		}

	result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
	result.Slot = ITEM_NO_SLOT_AVAILABLE;
	result.Result = false;

	return result;
}

void ItemInterface::ReduceItemDurability()
{
	uint32 f = RandomUInt(100);
	if( f <= 10 ) //10% chance to loose 1 dur from a random valid item.
	{
		int32 slot = RandomUInt( EQUIPMENT_SLOT_END );
		Item* pItem = GetInventoryItem( INVENTORY_SLOT_NOT_SET, static_cast<int16>( slot ) );
		if( pItem != NULL )
		{
            if( pItem->GetDurability() && pItem->GetDurabilityMax() )
			{
                pItem->SetDurability( pItem->GetDurabilityMax() - 1  );
				pItem->m_isDirty = true;
				//check final durability
                if( !pItem->GetDurability() ) //no dur left
				{
					m_pOwner->ApplyItemMods( pItem, static_cast<int16>( slot ), false, true );
                    
				}
			}
		}
	}
}

bool ItemInterface::IsEquipped(uint32 itemid)
{
	for( uint32 x = EQUIPMENT_SLOT_START; x < EQUIPMENT_SLOT_END; ++x )
	{
		Item * it = m_pItems[x];

		if( it != NULL )
		{
			if( it->GetProto()->ItemId == itemid )
 				return true;

			// check gems as well
			for( uint32 count= 0; count<it->GetSocketsCount(); count++ )
			{
				EnchantmentInstance *ei = it->GetEnchantment(SOCK_ENCHANTMENT_SLOT1 + count);
				
				if (ei && ei->Enchantment)
				{
					ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(ei->Enchantment->GemEntry);
					
					if (ip && ip->ItemId == itemid)
						return true;
				}
			}
		}
	}
	return false;
}

void ItemInterface::CheckAreaItems()
{
	for( uint32 x = EQUIPMENT_SLOT_START; x < INVENTORY_SLOT_ITEM_END; ++x )
	{
		if( m_pItems[x] != NULL )
		{
			if( IsBagSlot(static_cast<int16>( x )) && m_pItems[x]->IsContainer() )
			{
				Container* bag = (Container*)m_pItems[x];
 
				for( uint32 i = 0; i < bag->GetProto()->ContainerSlots; i++ )
				{
					if( bag->GetItem(static_cast<int16>( i )) != NULL && bag->GetItem(static_cast<int16>( i ))->GetProto()->MapID && bag->GetItem(static_cast<int16>( i ))->GetProto()->MapID != GetOwner()->GetMapId() )
						bag->SafeFullRemoveItemFromSlot( static_cast<int16>( i ) );
				}
			}
			else
			{
				if( m_pItems[x]->GetProto()->MapID && m_pItems[x]->GetProto()->MapID != GetOwner()->GetMapId() )
					SafeFullRemoveItemFromSlot( INVENTORY_SLOT_NOT_SET, static_cast<int16>( x ) );
			}
		}
	}
}

uint32 ItemInterface::GetEquippedCountByItemLimit(uint32 LimitId)
{
	uint32 count = 0;
	for( uint32 x = EQUIPMENT_SLOT_START; x < EQUIPMENT_SLOT_END; ++x )
	{
		Item* it = m_pItems[x];

		if( it != NULL )
		{
			for( uint32 socketcount = 0; socketcount < it->GetSocketsCount(); socketcount++ )
			{
				EnchantmentInstance *ei = it->GetEnchantment( SOCK_ENCHANTMENT_SLOT1 + socketcount );
				if (ei && ei->Enchantment)
				{
					ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(ei->Enchantment->GemEntry);
					if( ip && ip->ItemLimitCategory == LimitId )
						count++;
				}
			}
		}
	}
	return count;
}

uint32 ItemInterface::GetItemCountByLimitId(uint32 LimitId, bool IncBank)
{
	uint32 cnt = 0;
	uint32 i = 0;
	for( i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++ )
	{
		Item * item = GetInventoryItem( static_cast<int16>( i ));
		if( item != NULL )
		{
			if( item->GetProto()->ItemLimitCategory == LimitId 
				&& item->wrapped_item_id == 0 )
			{
				cnt += item->GetStackCount() ? item->GetStackCount() : 1; 
			}
		}
	}

	for( i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
	{
		Item * item = GetInventoryItem( static_cast<int16>( i ));
		if( item && item->IsContainer() )
		{
			for( uint32 j = 0; j < item->GetProto()->ContainerSlots; j++ )
			{
				Item * item2 = (static_cast<Container*>( item ))->GetItem(static_cast<int16>( j ));
				if( item2 != NULL )
				{
					if( item2->GetProto()->ItemLimitCategory == LimitId 
						&& item2->wrapped_item_id == 0 )
					{
						cnt += item2->GetStackCount() ? item2->GetStackCount() : 1; 
					}
				}
			}
		}
	}

	for( i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++ )
	{
		Item * item = GetInventoryItem(static_cast<int16>( i ));
		if( item != NULL )
		{
			if( item->GetProto()->ItemLimitCategory == LimitId 
				&& item->wrapped_item_id == 0 )
			{
				cnt += item->GetStackCount() ? item->GetStackCount() : 1;
			}
		}
	}

	for( i = CURRENCYTOKEN_SLOT_START; i < CURRENCYTOKEN_SLOT_END; i++ )
	{
		Item * item = GetInventoryItem(static_cast<int16>( i ));
		if( item != NULL )
		{
			if( item->GetProto()->ItemLimitCategory == LimitId 
				&& item->wrapped_item_id == 0 )
			{
				cnt += item->GetStackCount() ? item->GetStackCount() : 1;
			}
		}
	}

	if( IncBank )
	{
		for( i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; i++ )
		{
			Item * item = GetInventoryItem(static_cast<int16>( i ));
			if( item != NULL )
			{
				if( item->GetProto()->ItemLimitCategory == LimitId  
					&& item->wrapped_item_id == 0 )
				{
					cnt += item->GetStackCount() ? item->GetStackCount() : 1;
				}
			}
		}

		for( i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
		{
			Item * item = GetInventoryItem(static_cast<int16>( i ));
			if( item != NULL )
			{
				if( item->IsContainer() )
				{
					for (uint32 j = 0; j < item->GetProto()->ContainerSlots; j++)
					{
						Item * item2 = (static_cast<Container*>( item ))->GetItem(static_cast<int16>( j ));
						if( item2 != NULL )
						{
							if( item2->GetProto()->ItemLimitCategory == LimitId 
								&& item2->wrapped_item_id == 0 )
							{
								cnt += item2->GetStackCount() ? item2->GetStackCount() : 1;
							}
						}
					}
				}
			}
		}
	}
	cnt += GetEquippedCountByItemLimit( LimitId );

	return cnt;
}

///////////////////////////////////////////////////////////////////////////////////
//
// Look for items with limited duration and send the remaining time to the client
//
///////////////////////////////////////////////////////////////////////////////////
void ItemInterface::HandleItemDurations(){
    
    for( uint32 i = EQUIPMENT_SLOT_START; i <= CURRENCYTOKEN_SLOT_END; ++i ){
        Item *item1 = this->GetInventoryItem( static_cast< int16 >( i ) );
        Item *realitem = NULL;

        if( item1 != NULL && item1->IsContainer() ){
            
            for( uint32 j = 0; j < item1->GetProto()->ContainerSlots; ++j ){
                Item *item2 = static_cast< Container* >( item1 )->GetItem( static_cast< int16 >( j ));
                
                if( item2 != NULL && item2->GetProto()->ExistingDuration > 0 )
                    realitem = item2;
            }

        }else{
            if( item1 != NULL )
                realitem = item1;
        }

        if( realitem != NULL )
            sEventMgr.AddEvent( realitem, &Item::SendDurationUpdate, EVENT_SEND_PACKET_TO_PLAYER_AFTER_LOGIN, 0, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
    }
}

//////////////////////////////////////////////////////////////////////////
//
// Inserts a new entry into the RefundableMap
// 
// This should be called when purchasing the item
//////////////////////////////////////////////////////////////////////////
void ItemInterface::AddRefundable( uint64 GUID,  uint32 extendedcost ){
    std::pair< time_t, uint32 > RefundableEntry;
    std::pair< uint64, std::pair< time_t, uint32 > > insertpair;
    
    Item *item = this->GetItemByGUID( GUID );

    if( item == NULL )
        return;
	
	uint32 *played = this->GetOwner()->GetPlayedtime();

    RefundableEntry.first = played[1];               // time of purchase in playedtime
    RefundableEntry.second = extendedcost;          // extendedcost

    insertpair.first = GUID;
    insertpair.second = RefundableEntry;

    this->m_refundableitems.insert( insertpair );

    sEventMgr.AddEvent( item, &Item::RemoveFromRefundableMap, EVENT_REMOVE_ITEM_FROM_REFUNDABLE_MAP, ( UNIXTIME + 60*60*2 ), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void ItemInterface::AddRefundable( uint64 GUID, uint32 extendedcost, time_t buytime ){
    std::pair< time_t, uint32 > RefundableEntry;
    std::pair< uint64, std::pair< time_t, uint32 > > insertpair;
    
    Item *item = this->GetItemByGUID( GUID );

    if( item == NULL )
        return;

    RefundableEntry.first = buytime;               // time of purchase in playedtime
    RefundableEntry.second = extendedcost;      // extendedcost

    insertpair.first = GUID;
    insertpair.second = RefundableEntry;

    this->m_refundableitems.insert( insertpair );

    sEventMgr.AddEvent( item, &Item::RemoveFromRefundableMap, EVENT_REMOVE_ITEM_FROM_REFUNDABLE_MAP, buytime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void ItemInterface::AddRefundable( Item *item, uint32 extendedcost, time_t buytime ){
    std::pair< time_t, uint32 > RefundableEntry;
    std::pair< uint64, std::pair< time_t, uint32 > > insertpair;
    
    if( item == NULL )
        return;

    RefundableEntry.first = buytime;      // time of purchase in playedtime
    RefundableEntry.second = extendedcost; // extendedcost

    insertpair.first = item->GetGUID();
    insertpair.second = RefundableEntry;

    this->m_refundableitems.insert( insertpair );

    sEventMgr.AddEvent( item, &Item::RemoveFromRefundableMap, EVENT_REMOVE_ITEM_FROM_REFUNDABLE_MAP, ( buytime + 60*60*2 ), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}


//////////////////////////////////////////////////////////////////////////
//
//  Removes an entry from the RefundableMap
//
//////////////////////////////////////////////////////////////////////////
void ItemInterface::RemoveRefundable(uint64 GUID){

    this->m_refundableitems.erase( GUID );
}


//////////////////////////////////////////////////////////////////////////
//
//  Looks up an item in the RefundableMap, and returns the data
//
//////////////////////////////////////////////////////////////////////////
std::pair< time_t, uint32 > ItemInterface::LookupRefundable(uint64 GUID){
    std::pair< time_t, uint32 > RefundableEntry;
    RefundableMap::iterator itr;

    RefundableEntry.first = 0;   // time of purchase in playedtime
    RefundableEntry.second = 0;  // extendedcost

    itr = this->m_refundableitems.find( GUID );
    if( itr != this->m_refundableitems.end() ){
        RefundableEntry.first = itr->second.first;
        RefundableEntry.second = itr->second.second;
    }

    return RefundableEntry;
}

bool ItemInterface::AddItemById( uint32 itemid, uint32 count, int32 randomprop )
{
	if( count == 0 )
		return false;

    Player *chr = GetOwner();

    // checking if the iteminterface has owner, impossible to not have one
    if( chr == NULL )
        return false;

	ItemPrototype* it = ItemPrototypeStorage.LookupEntry(itemid);
	if(it == NULL )
		return false;

	uint32 maxStack = chr->ItemStackCheat ? 0x7fffffff : it->MaxCount;
	uint32 toadd;
	bool freeslots = true;
	
	while( count > 0 && freeslots )
	{
		if( count < maxStack )
		{
			// find existing item with free stack
			Item* free_stack_item = FindItemLessMax( itemid, count, false );
			if( free_stack_item != NULL )
			{
				// increase stack by new amount
				free_stack_item->ModStackCount(  count );
				free_stack_item->m_isDirty = true;
				return true;
			}
		}
		
		// create new item
		Item *item = objmgr.CreateItem( itemid, chr );
		if( item == NULL )
			return false;

		if( it->Bonding == ITEM_BIND_ON_PICKUP )
		{
			if( it->Flags & ITEM_FLAG_ACCOUNTBOUND ) // don't "Soulbind" account-bound items
				item->AccountBind();
			else
				item->SoulBind();
		}

		// Let's try to autogenerate randomprop / randomsuffix
		if( randomprop == 0 ){

			if( ( it->RandomPropId != 0 ) && ( it->RandomSuffixId != 0 ) ){
				sLog.outError("Item %u ( %s ) has both RandomPropId and RandomSuffixId.", itemid, it->Name1 );
			}

			if( it->RandomPropId != 0 ){
				RandomProps *rp = lootmgr.GetRandomProperties( it );

				if( rp != NULL ){
					randomprop = rp->ID;
				}else{
					sLog.outError( "Item %u ( %s ) has unknown RandomPropId %u", itemid, it->Name1, it->RandomPropId );
				}
			}

			if( it->RandomSuffixId != 0 ){
				ItemRandomSuffixEntry *rs = lootmgr.GetRandomSuffix( it );

				if( rs != NULL ){
					randomprop = -1 * rs->id;
				}else{
					sLog.outError( "Item %u ( %s ) has unknown RandomSuffixId %u", itemid, it->Name1, it->RandomSuffixId );
				}
			}
		}
		
		if( randomprop != 0 )
		{
			if( randomprop < 0 )
				item->SetRandomSuffix( -randomprop );
			else
				item->SetItemRandomPropertyId( randomprop );

			item->ApplyRandomProperties( false );
		}

		toadd = count > maxStack ? maxStack : count;

		item->SetStackCount(  toadd );
		if( AddItemToFreeSlot( item ) )
		{
			SlotResult *lr = LastSearchResult();
            
            chr->SendItemPushResult( false, true, false, true, lr->ContainerSlot, lr->Slot, toadd , item->GetEntry(), item->GetItemRandomSuffixFactor(), item->GetItemRandomPropertyId(), item->GetStackCount()   );
#ifdef ENABLE_ACHIEVEMENTS
            chr->GetAchievementMgr().UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, itemid, 1, 0 );
#endif
			count -= toadd;
		}
		else
		{
			freeslots = false;
            chr->GetSession()->SendNotification("No free slots were found in your inventory!");
			item->DeleteMe();
		}
	}
	return true;
}


bool ItemInterface::SwapItems( int8 DstInvSlot, int8 DstSlot, int8 SrcInvSlot, int8 SrcSlot )
{
	Item *SrcItem = NULL;
	Item *DstItem = NULL;
	bool adderror = false;
	int8 error;

	if( DstInvSlot == SrcSlot && SrcInvSlot == -1 ) // player trying to add self container to self container slots
	{
		BuildInventoryChangeError( NULL, NULL, INV_ERR_ITEMS_CANT_BE_SWAPPED );
		return false;
	}

	if( ( DstInvSlot <= 0 && DstSlot < 0 ) || DstInvSlot < -1 )
		return false;
	
	if( ( SrcInvSlot <= 0 && SrcSlot < 0 ) || SrcInvSlot < -1 )
		return false;

	SrcItem = GetInventoryItem( SrcInvSlot,SrcSlot );
	if( !SrcItem )
		return false;

	DstItem = GetInventoryItem( DstInvSlot,DstSlot );

	if( DstItem )
	{   //check if it will go to equipment slot
		if( SrcInvSlot == INVENTORY_SLOT_NOT_SET )//not bag
		{
			if( DstItem->IsContainer() )
			{
				if(((Container*)DstItem)->HasItems())
				{
					if(SrcSlot < INVENTORY_SLOT_BAG_START || SrcSlot >= INVENTORY_SLOT_BAG_END || SrcSlot < BANK_SLOT_BAG_START || SrcSlot >= BANK_SLOT_BAG_END)
					{
						BuildInventoryChangeError(SrcItem, DstItem, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
						return false;
					}
				}
			}

			if(SrcSlot <  CURRENCYTOKEN_SLOT_END)
			{
				if((error = CanEquipItemInSlot2(SrcInvSlot, SrcSlot, DstItem)) != 0)
				{
					BuildInventoryChangeError(SrcItem, DstItem, error);
					return false;
				}
			}
		}
		else
		{
			if(DstItem->IsContainer())
			{
				if(((Container*)DstItem)->HasItems())
				{
					BuildInventoryChangeError(SrcItem, DstItem, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
					return false;
				}
			}

			if(( error = CanEquipItemInSlot2(SrcInvSlot, SrcInvSlot, DstItem)) != 0)
			{
				BuildInventoryChangeError(SrcItem, DstItem, error);
				return false;
			}
		}
	}

	if(SrcItem)
	{   //check if it will go to equipment slot
		if(DstInvSlot==INVENTORY_SLOT_NOT_SET)//not bag
		{
			if(SrcItem->IsContainer())
			{
				if(((Container*)SrcItem)->HasItems())
				{
					if(DstSlot < INVENTORY_SLOT_BAG_START || DstSlot >= INVENTORY_SLOT_BAG_END || DstSlot < BANK_SLOT_BAG_START || DstSlot >= BANK_SLOT_BAG_END)
					{
						BuildInventoryChangeError(SrcItem, DstItem, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
						return false;
					}
				}
			}

			if(DstSlot <  CURRENCYTOKEN_SLOT_END)
			{
				if(( error = CanEquipItemInSlot2(DstInvSlot, DstSlot, SrcItem)) != 0)
				{
					BuildInventoryChangeError(SrcItem, DstItem, error);
					return false;
				}
			} 
		}
		else
		{
			if(SrcItem->IsContainer())
			{
				if(((Container*)SrcItem)->HasItems())
				{
					BuildInventoryChangeError(SrcItem, DstItem, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
					return false;
				}
			}

			if(( error = CanEquipItemInSlot2(DstInvSlot, DstInvSlot, SrcItem)) != 0)
			{
				BuildInventoryChangeError(SrcItem, DstItem, error);
				return false;
			}
		}
	}

#ifdef ENABLE_ACHIEVEMENTS
	if( SrcItem && DstSlot < INVENTORY_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET ) //equip - bags can be soulbound too
	{
		if( SrcItem->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
			SrcItem->SoulBind();

		m_pOwner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM, SrcItem->GetProto()->ItemId, 0, 0);

		if(DstSlot<INVENTORY_SLOT_BAG_START) // check Superior/Epic achievement
		{
			// Achievement ID:556 description Equip an epic item in every slot with a minimum item level of 213.
			// "213" value not found in achievement or criteria entries, have to hard-code it here? :(
			// Achievement ID:557 description Equip a superior item in every slot with a minimum item level of 187.
			// "187" value not found in achievement or criteria entries, have to hard-code it here? :(
			if( (SrcItem->GetProto()->Quality == ITEM_QUALITY_RARE_BLUE && SrcItem->GetProto()->ItemLevel >= 187) ||
				(SrcItem->GetProto()->Quality == ITEM_QUALITY_EPIC_PURPLE && SrcItem->GetProto()->ItemLevel >= 213) )
				m_pOwner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, DstSlot, SrcItem->GetProto()->Quality, 0);
		}
	}

	if( DstItem && SrcSlot < INVENTORY_SLOT_BAG_END && SrcInvSlot == INVENTORY_SLOT_NOT_SET ) //equip - make sure to soulbind items swapped from equip slot to bag slot
	{
		if( DstItem->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
			DstItem->SoulBind();
		m_pOwner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM, DstItem->GetProto()->ItemId, 0, 0);
		if(SrcSlot<INVENTORY_SLOT_BAG_START) // check Superior/Epic achievement
		{
			if( (DstItem->GetProto()->Quality == ITEM_QUALITY_RARE_BLUE && DstItem->GetProto()->ItemLevel >= 187) ||
				(DstItem->GetProto()->Quality == ITEM_QUALITY_EPIC_PURPLE && DstItem->GetProto()->ItemLevel >= 213) )
				m_pOwner->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, SrcSlot, DstItem->GetProto()->Quality, 0);
		}
	}
#endif

	if( SrcInvSlot == DstInvSlot )//in 1 bag
	{
		if( SrcInvSlot == INVENTORY_SLOT_NOT_SET ) //in backpack
		{
			SwapItemSlots( SrcSlot, DstSlot );
		}
		else//in bag
		{
			static_cast< Container* >( GetInventoryItem( SrcInvSlot ) )->SwapItems( SrcSlot, DstSlot );
		}
	}
	else
	{
		//Check for stacking
		uint32 srcItemMaxStack = (SrcItem) ? ((SrcItem->GetOwner()->ItemStackCheat) ? 0x7fffffff : SrcItem->GetProto()->MaxCount) : 0;
		uint32 dstItemMaxStack = (DstItem) ? ((DstItem->GetOwner()->ItemStackCheat) ? 0x7fffffff : DstItem->GetProto()->MaxCount) : 0;
		if(DstItem && SrcItem && SrcItem->GetEntry()==DstItem->GetEntry() && srcItemMaxStack>1 && SrcItem->wrapped_item_id == 0 && DstItem->wrapped_item_id == 0)
		{
			uint32 total=SrcItem->GetStackCount()+DstItem->GetStackCount();
			if( total <= dstItemMaxStack )
			{
				DstItem->ModStackCount( SrcItem->GetStackCount());
				DstItem->m_isDirty = true;
				bool result = SafeFullRemoveItemFromSlot(SrcInvSlot,SrcSlot);
				if(!result)
				{
					BuildInventoryChangeError(SrcItem, DstItem, INV_ERR_ITEM_CANT_STACK);
				}
				return false;
			}
			else
			{
				if( DstItem->GetStackCount() == dstItemMaxStack )
				{

				}
				else
				{
					int32 delta = dstItemMaxStack - DstItem->GetStackCount();
					DstItem->SetStackCount( dstItemMaxStack);
					SrcItem->ModStackCount( -delta);
					SrcItem->m_isDirty = true;
					DstItem->m_isDirty = true;
					return false;
				}
			}
		}

		if(SrcItem)
			SrcItem = SafeRemoveAndRetreiveItemFromSlot(SrcInvSlot,SrcSlot, false);

		if(DstItem)
			DstItem = SafeRemoveAndRetreiveItemFromSlot(DstInvSlot,DstSlot, false);

		if(SrcItem)
		{
			AddItemResult result = SafeAddItem(SrcItem,DstInvSlot,DstSlot);
			if(!result)
			{
				sLog.outError("HandleSwapItem: Error while adding item to dstslot");
				SrcItem->DeleteFromDB();
				SrcItem->DeleteMe();
				SrcItem = NULL;
				adderror = true;
			}
		}

		if(DstItem)
		{
			AddItemResult result = SafeAddItem(DstItem,SrcInvSlot,SrcSlot);
			if(!result)
			{
				sLog.outError("HandleSwapItem: Error while adding item to srcslot");
				DstItem->DeleteFromDB();
				DstItem->DeleteMe();
				DstItem = NULL;
				adderror = true;
			}
		}
	}

	//Recalculate Expertise (for Weapon specs)
	m_pOwner->CalcExpertise();

	if( adderror )
		return false;
	else
		return true;
}
