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

void WorldSession::HandleSplitOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 8);
	int8 DstInvSlot= 0, DstSlot= 0, SrcInvSlot= 0, SrcSlot= 0;
	int32 count= 0;

	AddItemResult result;

	recv_data >> SrcInvSlot >> SrcSlot >> DstInvSlot >> DstSlot >> count;

	/* exploit fix */
	if( count <= 0 || (SrcInvSlot <= 0 && SrcSlot < INVENTORY_SLOT_ITEM_START) || (DstInvSlot <= 0 && DstSlot < INVENTORY_SLOT_ITEM_START) )
	{
		sCheatLog.writefromsession(this, "tried to split item: SrcInvSlot %d, SrcSlot %d, DstInvSlot %d, DstSlot %d, count %l", SrcInvSlot, SrcSlot, DstInvSlot, DstSlot, count);
		return;
	}


	if( !_player->GetItemInterface()->VerifyBagSlots(SrcInvSlot, SrcSlot) )
		return;

	if( !_player->GetItemInterface()->VerifyBagSlots(DstInvSlot, DstSlot) )
		return;

	uint32 c=count;
	Item *i1 =_player->GetItemInterface()->GetInventoryItem(SrcInvSlot,SrcSlot);
	if(!i1)
		return;
	Item *i2=_player->GetItemInterface()->GetInventoryItem(DstInvSlot,DstSlot);

	uint32 itemMaxStack1 = (i1) ? ((i1->GetOwner()->ItemStackCheat) ? 0x7fffffff : i1->GetProto()->MaxCount) : 0;
	uint32 itemMaxStack2 = (i2) ? ((i2->GetOwner()->ItemStackCheat) ? 0x7fffffff : i2->GetProto()->MaxCount) : 0;
	if( (i1 && i1->wrapped_item_id) || (i2 && i2->wrapped_item_id) || (c > itemMaxStack1) )
	{
		GetPlayer()->GetItemInterface()->BuildInventoryChangeError(i1, i2, INV_ERR_ITEM_CANT_STACK);
		return;
	}

	// something already in this slot
	if( i2 )
	{
		if( i1->GetEntry() == i2->GetEntry() )
		{
			//check if player has the required stacks to avoid exploiting.
			//safe exploit check
			if(c < i1->GetStackCount())
			{
				//check if there is room on the other item.
				if( ((c + i2->GetStackCount()) <= itemMaxStack2) )
				{
					i1->ModStackCount(  -count);
					i2->ModStackCount(  c);
					i1->m_isDirty = true;
					i2->m_isDirty = true;
				}
				else
				{
					GetPlayer()->GetItemInterface()->BuildInventoryChangeError(i1, i2, INV_ERR_ITEM_CANT_STACK);
				}
			}
			else
			{
				//error cant split item
				_player->GetItemInterface()->BuildInventoryChangeError(i1, i2, INV_ERR_COULDNT_SPLIT_ITEMS);
			}
		}
		else
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(i1, i2, INV_ERR_ITEM_CANT_STACK);
		}
	}
	else
	{
		if( c < i1->GetStackCount() )
		{
			i1->ModStackCount( -count);

			i2=objmgr.CreateItem(i1->GetEntry(),_player);
			if( i2== NULL )
				return;

			i2->SetStackCount( c);
			i1->m_isDirty = true;
			i2->m_isDirty = true;

			if( DstSlot == -1 )
			{
				// Find a free slot
				SlotResult res = _player->GetItemInterface()->FindFreeInventorySlot(i2->GetProto());
				if( !res.Result )
				{
					SendNotification("Internal Error");
					return;
				}
				else
				{
					DstSlot = res.Slot;
					DstInvSlot = res.ContainerSlot;
				}
			}
			result = _player->GetItemInterface()->SafeAddItem(i2,DstInvSlot,DstSlot);
			if(!result)
			{
				sLog.outError("HandleSplit: Error while adding item to dstslot");
				i2->DeleteFromDB();
				i2->DeleteMe();
				i2 = NULL;
			}
		}
		else
		{
			_player->GetItemInterface()->BuildInventoryChangeError(i1, i2, INV_ERR_COULDNT_SPLIT_ITEMS);
		}
	}
}

void WorldSession::HandleSwapItemOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 4);

	int8 DstInvSlot= 0, DstSlot= 0, SrcInvSlot= 0, SrcSlot= 0;

	recv_data >> DstInvSlot >> DstSlot >> SrcInvSlot >> SrcSlot;

	sLog.outDetail("ITEM: swap, DstInvSlot %i DstSlot %i SrcInvSlot %i SrcSlot %i", DstInvSlot, DstSlot, SrcInvSlot, SrcSlot);

	_player->GetItemInterface()->SwapItems( DstInvSlot, DstSlot, SrcInvSlot, SrcSlot );

}

void WorldSession::HandleSwapInvItemOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 2);
	WorldPacket data;
	int8 srcslot= 0, dstslot= 0;
	int8 error= 0;

	recv_data >> dstslot >> srcslot;

	sLog.outDetail("ITEM: swap, src slot: %u dst slot: %u", (uint32)srcslot, (uint32)dstslot);

	if(dstslot == srcslot) // player trying to add item to the same slot
	{
		GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_ITEMS_CANT_BE_SWAPPED);
		return;
	}

	Item * dstitem = _player->GetItemInterface()->GetInventoryItem(dstslot);
	Item * srcitem = _player->GetItemInterface()->GetInventoryItem(srcslot);
	
	// allow weapon switching in combat
	bool skip_combat = false;
	if( srcslot < EQUIPMENT_SLOT_END || dstslot < EQUIPMENT_SLOT_END )	  // We're doing an equip swap.
	{
		if( _player->CombatStatus.IsInCombat() )
		{
			if( srcslot < EQUIPMENT_SLOT_MAINHAND || dstslot < EQUIPMENT_SLOT_MAINHAND )	// These can't be swapped
			{
				_player->GetItemInterface()->BuildInventoryChangeError(srcitem, dstitem, INV_ERR_CANT_DO_IN_COMBAT);
				return;
			}
			skip_combat= true;
		}
	}

	if( !srcitem )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( srcitem, dstitem, INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM );
		return;
	}

	if( srcslot == dstslot )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( srcitem, dstitem, INV_ERR_ITEM_DOESNT_GO_TO_SLOT );
		return;
	}
	
	if( ( error = _player->GetItemInterface()->CanEquipItemInSlot2( INVENTORY_SLOT_NOT_SET, dstslot, srcitem, skip_combat, false ) )  != 0)
	{
		if( dstslot < CURRENCYTOKEN_SLOT_END )
		{
			_player->GetItemInterface()->BuildInventoryChangeError( srcitem, dstitem, error );
			return;
		}
	}

	if( dstitem != NULL )
	{
		if((error=_player->GetItemInterface()->CanEquipItemInSlot2(INVENTORY_SLOT_NOT_SET, srcslot, dstitem, skip_combat)) != 0)
		{
			if(srcslot < CURRENCYTOKEN_SLOT_END)
			{
				data.Initialize( SMSG_INVENTORY_CHANGE_FAILURE );
				data << error;
				data << (srcitem ? srcitem->GetGUID() : uint64(0));
				data << dstitem->GetGUID();
				data << uint8(0);

				if( error == INV_ERR_YOU_MUST_REACH_LEVEL_N ) 
				{
					data << dstitem->GetProto()->RequiredLevel;
				}

				SendPacket( &data );
				return;
			}
		}
	}

	if(srcitem->IsContainer())
	{
		//source has items and dst is a backpack or bank
		if(((Container*)srcitem)->HasItems())
			if(!_player->GetItemInterface()->IsBagSlot(dstslot))
			{
				_player->GetItemInterface()->BuildInventoryChangeError(srcitem,dstitem, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
				return;
			}

		if(dstitem)
		{
			//source is a bag and dst slot is a bag inventory and has items
			if(dstitem->IsContainer())
			{
				if(((Container*)dstitem)->HasItems() && !_player->GetItemInterface()->IsBagSlot(srcslot))
				{
					_player->GetItemInterface()->BuildInventoryChangeError(srcitem,dstitem, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
					return;
				}
			}
			else
			{
				//dst item is not a bag, swap impossible
				_player->GetItemInterface()->BuildInventoryChangeError(srcitem,dstitem,INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
				return;
			}
		}

		//dst is bag inventory
		if(dstslot < INVENTORY_SLOT_BAG_END)
		{
			if(srcitem->GetProto()->Bonding==ITEM_BIND_ON_EQUIP)
				srcitem->SoulBind();
		}
	}

	// swap items
  if( _player->IsDead() ) {
  	_player->GetItemInterface()->BuildInventoryChangeError(srcitem,NULL,INV_ERR_YOU_ARE_DEAD);
	  return;
  }

  #ifdef ENABLE_ACHIEVEMENTS
  if(dstitem && srcslot < INVENTORY_SLOT_BAG_END)
  {
		_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM, dstitem->GetProto()->ItemId, 0, 0);
		if(srcslot<INVENTORY_SLOT_BAG_START) // check Superior/Epic achievement
		{
			// Achievement ID:556 description Equip an epic item in every slot with a minimum item level of 213.
			// "213" value not found in achievement or criteria entries, have to hard-code it here? :(
			// Achievement ID:557 description Equip a superior item in every slot with a minimum item level of 187.
			// "187" value not found in achievement or criteria entries, have to hard-code it here? :(
			if( (dstitem->GetProto()->Quality == ITEM_QUALITY_RARE_BLUE && dstitem->GetProto()->ItemLevel >= 187) ||
				(dstitem->GetProto()->Quality == ITEM_QUALITY_EPIC_PURPLE && dstitem->GetProto()->ItemLevel >= 213) )
				_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, srcslot, dstitem->GetProto()->Quality, 0);
		}
  }
  if(srcitem && dstslot < INVENTORY_SLOT_BAG_END)
  {
	  	_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM, srcitem->GetProto()->ItemId, 0, 0);
		if(dstslot<INVENTORY_SLOT_BAG_START) // check Superior/Epic achievement
		{
			// Achievement ID:556 description Equip an epic item in every slot with a minimum item level of 213.
			// "213" value not found in achievement or criteria entries, have to hard-code it here? :(
			// Achievement ID:557 description Equip a superior item in every slot with a minimum item level of 187.
			// "187" value not found in achievement or criteria entries, have to hard-code it here? :(
			if( (srcitem->GetProto()->Quality == ITEM_QUALITY_RARE_BLUE && srcitem->GetProto()->ItemLevel >= 187) ||
				(srcitem->GetProto()->Quality == ITEM_QUALITY_EPIC_PURPLE && srcitem->GetProto()->ItemLevel >= 213) )
				_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, dstslot, srcitem->GetProto()->Quality, 0);
		}
  }
#endif
	_player->GetItemInterface()->SwapItemSlots(srcslot, dstslot);
}

void WorldSession::HandleDestroyItemOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 2);

	int8 SrcInvSlot, SrcSlot;

	recv_data >> SrcInvSlot >> SrcSlot;

	sLog.outDetail( "ITEM: destroy, SrcInv Slot: %i Src slot: %i", SrcInvSlot, SrcSlot );
	Item *it = _player->GetItemInterface()->GetInventoryItem(SrcInvSlot,SrcSlot);

	if(it)
	{
		if(it->IsContainer())
		{
			if(((Container*)it)->HasItems())
			{
				_player->GetItemInterface()->BuildInventoryChangeError(
				it, NULL, INV_ERR_CAN_ONLY_DO_WITH_EMPTY_BAGS);
				return;
			}
		}

		if(it->GetProto()->ItemId == ITEM_ENTRY_GUILD_CHARTER)
		{
			Charter *gc = _player->m_charters[CHARTER_TYPE_GUILD];
			if(gc)
				gc->Destroy();

			_player->m_charters[CHARTER_TYPE_GUILD] = NULL;
		}

		if(it->GetProto()->ItemId == ARENA_TEAM_CHARTER_2v2)
		{
			Charter *gc = _player->m_charters[CHARTER_TYPE_ARENA_2V2];
			if(gc)
				gc->Destroy();

			_player->m_charters[CHARTER_TYPE_ARENA_2V2] = NULL;
		}

		if(it->GetProto()->ItemId == ARENA_TEAM_CHARTER_5v5)
		{
			Charter *gc = _player->m_charters[CHARTER_TYPE_ARENA_5V5];
			if(gc)
				gc->Destroy();

			_player->m_charters[CHARTER_TYPE_ARENA_5V5] = NULL;
		}

		if(it->GetProto()->ItemId == ARENA_TEAM_CHARTER_3v3)
		{
			Charter *gc = _player->m_charters[CHARTER_TYPE_ARENA_3V3];
			if(gc)
				gc->Destroy();

			_player->m_charters[CHARTER_TYPE_ARENA_3V3] = NULL;
		}

		/* Changed in 3.3.3 or 3.3.5
		uint32 mail_id = it->GetTextId();
		if(mail_id)
			sMailSystem.RemoveMessageIfDeleted(mail_id, _player);
		*/
		


		/*bool result =  _player->GetItemInterface()->SafeFullRemoveItemFromSlot(SrcInvSlot,SrcSlot);
		if(!result)
		{
			sLog.outDetail("ITEM: Destroy, SrcInv Slot: %u Src slot: %u Failed", (uint32)SrcInvSlot, (uint32)SrcSlot);
		}*/
		Item * pItem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(SrcInvSlot,SrcSlot,false);
		if(!pItem)
			return;

		if(_player->GetCurrentSpell() && _player->GetCurrentSpell()->i_caster==pItem)
		{
			_player->GetCurrentSpell()->i_caster= NULL;
			_player->GetCurrentSpell()->cancel();
		}

		pItem->DeleteFromDB();
		pItem->DeleteMe();
	}
}

void WorldSession::HandleAutoEquipItemOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 2);
	WorldPacket data;

	int8 SrcInvSlot, SrcSlot, error= 0;
	
	recv_data >> SrcInvSlot >> SrcSlot;

	sLog.outDetail("ITEM: autoequip, Inventory slot: %i Source Slot: %i", SrcInvSlot, SrcSlot); 

	Item *eitem=_player->GetItemInterface()->GetInventoryItem(SrcInvSlot,SrcSlot);

	if(!eitem) 
	{
		_player->GetItemInterface()->BuildInventoryChangeError(eitem, NULL, INV_ERR_ITEM_NOT_FOUND);
		return;
	}

    int8 Slot = _player->GetItemInterface()->GetItemSlotByType(eitem->GetProto()->InventoryType);
    if(Slot == ITEM_NO_SLOT_AVAILABLE)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(eitem,NULL,INV_ERR_ITEM_CANT_BE_EQUIPPED);
		return;
	}

	// handle equipping of 2h when we have two items equipped! :) special case.
  	if ((Slot == EQUIPMENT_SLOT_MAINHAND || Slot == EQUIPMENT_SLOT_OFFHAND) && !_player->DualWield2H)
	{
		Item *mainhandweapon = _player->GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND);
		if( mainhandweapon != NULL && mainhandweapon->GetProto()->InventoryType == INVTYPE_2HWEAPON )
		{
			if( Slot == EQUIPMENT_SLOT_OFFHAND && ( eitem->GetProto()->InventoryType == INVTYPE_WEAPON || eitem->GetProto()->InventoryType == INVTYPE_2HWEAPON ) )
			{
				Slot = EQUIPMENT_SLOT_MAINHAND;
			}
        }else{
            if( Slot == EQUIPMENT_SLOT_OFFHAND && eitem->GetProto()->InventoryType == INVTYPE_2HWEAPON )
			{
				Slot = EQUIPMENT_SLOT_MAINHAND;
			}
        }

		error = _player->GetItemInterface()->CanEquipItemInSlot(INVENTORY_SLOT_NOT_SET, Slot, eitem->GetProto(), true, true);
		if( error )
		{
			_player->GetItemInterface()->BuildInventoryChangeError(eitem,NULL, error);
			return;
		}

		if( eitem->GetProto()->InventoryType == INVTYPE_2HWEAPON )
		{
			// see if we have a weapon equipped in the offhand, if so we need to remove it
			Item *offhandweapon = _player->GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND);
			if( offhandweapon != NULL )
			{
				// we need to de-equip this
				SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot(offhandweapon->GetProto());
				if( !result.Result )
				{
					// no free slots for this item
					_player->GetItemInterface()->BuildInventoryChangeError(eitem, NULL, INV_ERR_BAG_FULL);
					return;
				}

				offhandweapon = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND, false);
				if( offhandweapon == NULL )
					return; // should never happen

				if( !_player->GetItemInterface()->SafeAddItem( offhandweapon, result.ContainerSlot, result.Slot ) && !_player->GetItemInterface()->AddItemToFreeSlot( offhandweapon ) ) // shouldn't happen either.
				{
					offhandweapon->DeleteMe();
					offhandweapon = NULL;
				}
			}
		}
		else
		{
			// can't equip a non-two-handed weapon with a two-handed weapon
			mainhandweapon = _player->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( mainhandweapon != NULL && mainhandweapon->GetProto()->InventoryType == INVTYPE_2HWEAPON )
			{
				// we need to de-equip this
				SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot(mainhandweapon->GetProto());
				if( !result.Result )
				{
					// no free slots for this item
					_player->GetItemInterface()->BuildInventoryChangeError( eitem, NULL, INV_ERR_BAG_FULL );
					return;
				}

				mainhandweapon = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND, false );
				if( mainhandweapon == NULL )
					return; // should never happen

				if( !_player->GetItemInterface()->SafeAddItem( mainhandweapon, result.ContainerSlot, result.Slot ) && !_player->GetItemInterface()->AddItemToFreeSlot( mainhandweapon ) )    // shouldn't happen either.
				{
					mainhandweapon->DeleteMe();
					mainhandweapon = NULL;
				}
			}
		}
	}
	else
	{
		error = _player->GetItemInterface()->CanEquipItemInSlot( INVENTORY_SLOT_NOT_SET, Slot, eitem->GetProto(), false, false );
		if( error  )
		{
			_player->GetItemInterface()->BuildInventoryChangeError( eitem, NULL, error );
			return;
		}
	}

	if( Slot <= INVENTORY_SLOT_BAG_END )
	{
		error = _player->GetItemInterface()->CanEquipItemInSlot( INVENTORY_SLOT_NOT_SET, Slot, eitem->GetProto(), false, false );
		if( error )
		{
			_player->GetItemInterface()->BuildInventoryChangeError( eitem, NULL, error );
			return;
		}
	}

	Item * oitem = NULL;

	if( SrcInvSlot == INVENTORY_SLOT_NOT_SET )
	{
		_player->GetItemInterface()->SwapItemSlots( SrcSlot, Slot );
	}
	else
	{
		eitem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot( SrcInvSlot, SrcSlot, false );
		oitem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot( INVENTORY_SLOT_NOT_SET, Slot, false );
		AddItemResult result;
		if( oitem != NULL )
		{
			result = _player->GetItemInterface()->SafeAddItem( oitem, SrcInvSlot, SrcSlot );
			if( !result )
			{
				sLog.outError("HandleAutoEquip: Error while adding item to SrcSlot");
				oitem->DeleteMe();
				oitem = NULL;
			}
		}
		if( eitem != NULL )
		{
			result = _player->GetItemInterface()->SafeAddItem( eitem, INVENTORY_SLOT_NOT_SET, Slot );
			if(!result)
			{
				sLog.outError("HandleAutoEquip: Error while adding item to Slot");
				eitem->DeleteMe();
				eitem = NULL;
				return;
			}
		}
		
	}

	if( eitem != NULL && eitem->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
		eitem->SoulBind();
#ifdef ENABLE_ACHIEVEMENTS
	_player->GetAchievementMgr().UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM, eitem->GetProto()->ItemId, 0, 0 );
	// Achievement ID:556 description Equip an epic item in every slot with a minimum item level of 213.
	// "213" value not found in achievement or criteria entries, have to hard-code it here? :(
	// Achievement ID:557 description Equip a superior item in every slot with a minimum item level of 187.
	// "187" value not found in achievement or criteria entries, have to hard-code it here? :(
	if( ( eitem->GetProto()->Quality == ITEM_QUALITY_RARE_BLUE && eitem->GetProto()->ItemLevel >= 187 ) ||
		( eitem->GetProto()->Quality == ITEM_QUALITY_EPIC_PURPLE && eitem->GetProto()->ItemLevel >= 213 ) )
		_player->GetAchievementMgr().UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM, Slot, eitem->GetProto()->Quality, 0 );
#endif
	//Recalculate Expertise (for Weapon specs)
	_player->CalcExpertise();
}

void WorldSession::HandleAutoEquipItemSlotOpcode( WorldPacket& recvData )
{
	CHECK_INWORLD_RETURN

	sLog.outDetail( "WORLD: Received CMSG_AUTOEQUIP_ITEM_SLOT");
	CHECK_PACKET_SIZE( recvData, 8 + 1 );
	uint64 itemguid;
	int8 destSlot;
	//int8 error = 0; // useless?
	recvData >> itemguid >> destSlot;

	int8	srcSlot		= (int8)_player->GetItemInterface()->GetInventorySlotByGuid(itemguid);
	Item	*item		= _player->GetItemInterface()->GetItemByGUID(itemguid);

	if( item == NULL )
		return;

	int8	slotType	= _player->GetItemInterface()->GetItemSlotByType(item->GetProto()->InventoryType);
	bool	hasDualWield2H	= false;

	sLog.outDebug("ITEM: AutoEquipItemSlot, ItemGUID: %u, SrcSlot: %i, DestSlot: %i, SlotType: %i", itemguid, srcSlot, destSlot, slotType);

	if( srcSlot == destSlot )
		return;

	if( _player->DualWield2H && ( slotType == EQUIPMENT_SLOT_OFFHAND || slotType == EQUIPMENT_SLOT_MAINHAND ) )
		hasDualWield2H = true;

    // Need to check if the item even goes into that slot
    // Item system is a mess too, so it needs rewrite, but hopefully this will do for now
    int8 error = _player->GetItemInterface()->CanEquipItemInSlot2( INVENTORY_SLOT_NOT_SET, destSlot, item );
    if( error ){
        _player->GetItemInterface()->BuildInventoryChangeError( item, NULL, error );
        return;
    }


	// Handle destination slot checking.
	if( destSlot == slotType || hasDualWield2H )
	{
		uint32 invType = item->GetProto()->InventoryType;
		if( invType == INVTYPE_WEAPON        || invType == INVTYPE_WEAPONMAINHAND ||
			invType == INVTYPE_WEAPONOFFHAND || invType == INVTYPE_2HWEAPON )
		{
			Item *mainHand = _player->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			Item *offHand  = _player->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND );

			if( mainHand != NULL && offHand != NULL && !_player->DualWield2H )
			{ // No DualWield2H like Titan's grip. Unequip offhand.
				SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot( offHand->GetProto() );
				if( !result.Result )
				{
					// No free slots for this item.
					_player->GetItemInterface()->BuildInventoryChangeError( offHand, NULL, INV_ERR_BAG_FULL );
					return;
				}
				mainHand = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND, false );
				_player->GetItemInterface()->AddItemToFreeSlot( offHand );
				_player->GetItemInterface()->SwapItemSlots( srcSlot, destSlot ); // Now swap Main hand with 2H weapon.
			}
			else
			{ // Swap 2H with 2H or 2H with 1H if player has DualWield2H (ex: Titans Grip).
				_player->GetItemInterface()->SwapItemSlots( srcSlot, destSlot );
			}
		}
		else if( destSlot == slotType )
		{ // If item slot types match, swap.
			_player->GetItemInterface()->SwapItemSlots( srcSlot, destSlot );
		}
		else
		{ // Item slots do not match. We get here only for players who have DualWield2H (ex: Titans Grip).
			_player->GetItemInterface()->BuildInventoryChangeError( item, NULL, INV_ERR_ITEM_DOESNT_GO_TO_SLOT );
		}
		return;
	}
	else
	{ // Item slots do not match.
		_player->GetItemInterface()->BuildInventoryChangeError( item, NULL, INV_ERR_ITEM_DOESNT_GO_TO_SLOT );
	}
}

void WorldSession::HandleItemQuerySingleOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 4);
 
	uint32 i;
	uint32 itemid= 0;
	recv_data >> itemid;

	ItemPrototype *itemProto = ItemPrototypeStorage.LookupEntry(itemid);
	if(!itemProto)
	{
		sLog.outError( "WORLD: Unknown item id 0x%.8X", itemid );
		return;
	} 

	size_t namelens;
	
	LocalizedItem * li = (language>0) ? sLocalizationMgr.GetLocalizedItem(itemid, language) : NULL;
	if(li)
		namelens = strlen(li->Name) + strlen(li->Description) + 602;
	else
		namelens = strlen(itemProto->Name1) + strlen(itemProto->Description) + 602;

	WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, namelens );
	data << itemProto->ItemId;
	data << itemProto->Class;
	data << itemProto->SubClass;
	data << itemProto->unknown_bc;
	if(li)
		data << li->Name;
	else
		data << itemProto->Name1;

	/*data << itemProto->Name2;
	data << itemProto->Name3;
	data << itemProto->Name4;*/
	data << uint8(0) << uint8(0) << uint8(0);		// name 2,3,4
	data << itemProto->DisplayInfoID;
	data << itemProto->Quality;
	data << itemProto->Flags;
	data << itemProto->Faction;
	data << itemProto->BuyPrice;
	data << itemProto->SellPrice;
	data << itemProto->InventoryType;
	data << itemProto->AllowableClass;
	data << itemProto->AllowableRace;
	data << itemProto->ItemLevel;
	data << itemProto->RequiredLevel;
	data << itemProto->RequiredSkill;
	data << itemProto->RequiredSkillRank;
	data << itemProto->RequiredSkillSubRank;
	data << itemProto->RequiredPlayerRank1;
	data << itemProto->RequiredPlayerRank2;
	data << itemProto->RequiredFaction;
	data << itemProto->RequiredFactionStanding;
	data << itemProto->Unique;
	data << itemProto->MaxCount;
	data << itemProto->ContainerSlots;
	data << itemProto->itemstatscount;
	for( i = 0; i < itemProto->itemstatscount; i++ )
	{
		data << itemProto->Stats[i].Type;
		data << itemProto->Stats[i].Value;
	}
	data << itemProto->ScalingStatsEntry;
	data << itemProto->ScalingStatsFlag;
	for(i = 0; i < 2; i++) //VLack: seen this in Aspire code, originally this went up to 5, now only to 2
	{
		data << itemProto->Damage[i].Min;
		data << itemProto->Damage[i].Max;
		data << itemProto->Damage[i].Type;
	}
	data << itemProto->Armor;
	data << itemProto->HolyRes;
	data << itemProto->FireRes;
	data << itemProto->NatureRes;
	data << itemProto->FrostRes;
	data << itemProto->ShadowRes;
	data << itemProto->ArcaneRes;
	data << itemProto->Delay;
	data << itemProto->AmmoType;
	data << itemProto->Range;
	for(i = 0; i < 5; i++) {
		data << itemProto->Spells[i].Id;
		data << itemProto->Spells[i].Trigger;
		data << itemProto->Spells[i].Charges;
		data << itemProto->Spells[i].Cooldown;
		data << itemProto->Spells[i].Category;
		data << itemProto->Spells[i].CategoryCooldown;
	}
	data << itemProto->Bonding;
	if(li)
		data << li->Description;
	else
		data << itemProto->Description;

	data << itemProto->PageId;
	data << itemProto->PageLanguage;
	data << itemProto->PageMaterial;
	data << itemProto->QuestId;
	data << itemProto->LockId;
	data << itemProto->LockMaterial;
	data << itemProto->SheathID;
	data << itemProto->RandomPropId;
	data << itemProto->RandomSuffixId;
	data << itemProto->Block;
	data << itemProto->ItemSet;
	data << itemProto->MaxDurability;
	data << itemProto->ZoneNameID;
	data << itemProto->MapID;
	data << itemProto->BagFamily;
	data << itemProto->TotemCategory;
	data << itemProto->Sockets[0].SocketColor ;
	data << itemProto->Sockets[0].Unk;
	data << itemProto->Sockets[1].SocketColor ;
	data << itemProto->Sockets[1].Unk ;
	data << itemProto->Sockets[2].SocketColor ;
	data << itemProto->Sockets[2].Unk ;
	data << itemProto->SocketBonus;
	data << itemProto->GemProperties;
	data << itemProto->DisenchantReqSkill;
	data << itemProto->ArmorDamageModifier;
	data << itemProto->ExistingDuration;								// 2.4.2 Item duration in seconds
	data << itemProto->ItemLimitCategory;
	data << itemProto->HolidayId; //MesoX: HolidayId - points to HolidayNames.dbc
	SendPacket( &data );

}

void WorldSession::HandleBuyBackOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	int32 stuff;
	Item* add ;
	AddItemResult result;
	uint8 error;

	sLog.outDetail( "WORLD: Received CMSG_BUYBACK_ITEM" );

	recv_data >> guid >> stuff;
	stuff -= 74;

	Item *it = _player->GetItemInterface()->GetBuyBack(stuff);
	if (it)
	{
		// Find free slot and break if inv full
		uint32 amount = it->GetStackCount();
		uint32 itemid = it->GetEntry();
	  
		add = _player->GetItemInterface()->FindItemLessMax(itemid,amount, false);
	 
		   uint32 FreeSlots = _player->GetItemInterface()->CalculateFreeSlots(it->GetProto());
		if ((FreeSlots == 0) && (!add))
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
			return;
		}
		
		// Check for gold
		int32 cost =_player->GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + stuff);
		if( !_player->HasGold(cost) )
		{
			WorldPacket data(SMSG_BUY_FAILED, 12);
			data << uint64(guid);
			data << uint32(itemid);
			data << uint8(2); //not enough money
			SendPacket( &data );
			return;
		}
		// Check for item uniqueness
		if ((error = _player->GetItemInterface()->CanReceiveItem(it->GetProto(), amount)) != 0)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
			return;
		}
		_player->ModGold( -cost );
		_player->GetItemInterface()->RemoveBuyBackItem(stuff);

		if (!add)
		{
			it->m_isDirty = true;			// save the item again on logout
			result = _player->GetItemInterface()->AddItemToFreeSlot(it);
			if(!result)
			{
				sLog.outError("HandleBuyBack: Error while adding item to free slot");
				it->DeleteMe();
			}
		}
		else
		{
			add->SetStackCount( add->GetStackCount() + amount);
			add->m_isDirty = true;

			// delete the item
			it->DeleteFromDB();
			it->DeleteMe();
		}

		WorldPacket data(16);
		data.Initialize( SMSG_BUY_ITEM );
		data << uint64(guid);
		data << getMSTime(); //VLack: seen is Aspire code
		data << uint32(itemid) << uint32(amount);
		SendPacket( &data );
	}
}

void WorldSession::HandleSellItemOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 17);
	sLog.outDetail( "WORLD: Received CMSG_SELL_ITEM" );

	uint64 vendorguid= 0, itemguid= 0;
	int8 amount= 0;
	//uint8 slot = INVENTORY_NO_SLOT_AVAILABLE;
	//uint8 bagslot = INVENTORY_NO_SLOT_AVAILABLE;
	//int check = 0;

	recv_data >> vendorguid;
	recv_data >> itemguid;
	recv_data >> amount;

	if(_player->IsCasting())
		_player->InterruptSpell();

	// Check if item exists
	if(!itemguid)
	{
		SendSellItem(vendorguid, itemguid, 1);
		return;
	}

	Creature *unit = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(vendorguid));
	// Check if Vendor exists
	if (unit == NULL)
	{
		SendSellItem(vendorguid, itemguid, 3);
		return;
	}

	Item* item = _player->GetItemInterface()->GetItemByGUID(itemguid);
	if(!item)
	{
		SendSellItem(vendorguid, itemguid, 1);
		return; //our player doesn't have this item
	}

	ItemPrototype *it = item->GetProto();

	if(item->IsContainer() && ((Container*)item)->HasItems())
	{
		SendSellItem(vendorguid, itemguid, 6);
		return;
	}

	// Check if item can be sold
	if (it->SellPrice == 0 || item->wrapped_item_id != 0)
	{
		SendSellItem(vendorguid, itemguid, 2);
		return;
	}
	
	uint32 stackcount = item->GetStackCount();
	uint32 quantity = 0;

	if (amount != 0)
	{
		quantity = amount;
	}
	else
	{
		quantity = stackcount; //allitems
	}

	if(quantity > stackcount) quantity = stackcount; //make sure we don't over do it
	
	uint32 price = GetSellPriceForItem(it, quantity);

	// Check they don't have more than the max gold
	if(sWorld.GoldCapEnabled)
	{
		if( (_player->GetGold() + price) > sWorld.GoldLimit )
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
			return;
		}
	}

	_player->ModGold( price );
 
	if(quantity < stackcount)
	{
		item->SetStackCount( stackcount - quantity);
		item->m_isDirty = true;
	}
	else
	{
		//removing the item from the char's inventory
		item = _player->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(itemguid, false); //again to remove item from slot
		if(item)
		{
			_player->GetItemInterface()->AddBuyBackItem(item,(it->SellPrice) * quantity);
			item->DeleteFromDB();
		}
	}

	WorldPacket data(SMSG_SELL_ITEM, 12);
	data << vendorguid << itemguid << uint8(0); 
	SendPacket( &data );

	sLog.outDetail( "WORLD: Sent SMSG_SELL_ITEM" );
}

void WorldSession::HandleBuyItemInSlotOpcode( WorldPacket & recv_data ) // drag & drop
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE( recv_data, 22 );

	sLog.outDetail( "WORLD: Received CMSG_BUY_ITEM_IN_SLOT" );

	uint64 srcguid, bagguid;
	uint32 itemid;
	int8 slot;
	uint8 amount = 0;
	uint8 error;
	int8 bagslot = INVENTORY_SLOT_NOT_SET;
	int32 vendorslot; //VLack: 3.1.2

	recv_data >> srcguid >> itemid;
	recv_data >> vendorslot; //VLack: 3.1.2 This is the slot's number on the vendor's panel, starts from 1
	recv_data >> bagguid;
	recv_data >> slot; //VLack: 3.1.2 the target slot the player selected - backpack 23-38, other bags 0-15 (Or how big is the biggest bag? 0-127?)
	recv_data >> amount;

	if(amount < 1)
		amount = 1;

	if( _player->IsCasting() )
		_player->InterruptSpell();

	Creature* unit = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(srcguid) );
	if( unit == NULL || !unit->HasItems() )
		return;

	Container* c = NULL;

	CreatureItem ci;
	unit->GetSellItemByItemId( itemid, ci );

	if( ci.itemid == 0 )
		return;

	if( ci.max_amount > 0 && ci.available_amount < amount )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( 0, 0, INV_ERR_ITEM_IS_CURRENTLY_SOLD_OUT );
		return;
	}

	ItemPrototype* it = ItemPrototypeStorage.LookupEntry( itemid );
	
	if( it == NULL)
		return;

	uint32 itemMaxStack = (_player->ItemStackCheat) ? 0x7fffffff : it->MaxCount;
	if( itemMaxStack > 0 && ci.amount*amount > itemMaxStack )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( 0, 0, INV_ERR_CANT_CARRY_MORE_OF_THIS );
		return;
	}

	uint32 count_per_stack = ci.amount * amount;

	// if slot is different than -1, check for validation, else continue for auto storing.
	if(slot != INVENTORY_SLOT_NOT_SET)
	{
		if( !(bagguid>>32) )//buy to backpack
		{
			if(slot > INVENTORY_SLOT_ITEM_END || slot < INVENTORY_SLOT_ITEM_START)
			{
				//hackers!
				_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_ITEM_DOESNT_GO_TO_SLOT);
				return;
			}
		}
		else
		{
			c = (Container*)_player->GetItemInterface()->GetItemByGUID(bagguid);
			if (!c)
				return;
			bagslot = (int8)_player->GetItemInterface()->GetBagSlotByGuid(bagguid);

			if(bagslot == INVENTORY_SLOT_NOT_SET || ((uint32)slot > c->GetProto()->ContainerSlots))
			{
				_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_ITEM_DOESNT_GO_TO_SLOT);
				return;
			}
		}
	}
	else
	{
		if((bagguid>>32))
		{
			c=(Container*)_player->GetItemInterface()->GetItemByGUID(bagguid);
			if(!c)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_ITEM_NOT_FOUND);
				return;//non empty
			}

			bagslot = (int8)_player->GetItemInterface()->GetBagSlotByGuid(bagguid);
			slot = c->FindFreeSlot();
		}
		else
			slot=_player->GetItemInterface()->FindFreeBackPackSlot();
	}

	if((error = _player->GetItemInterface()->CanReceiveItem(it, amount)) != 0)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		return;
	}

	if((error = _player->GetItemInterface()->CanAffordItem(it,amount,unit)) != 0)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		return;
	}

	if(slot==INVENTORY_SLOT_NOT_SET)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_BAG_FULL);
		return;
	}

	// ok our z and slot are set.
	Item * oldItem= NULL;
	Item * pItem= NULL;
	if(slot != INVENTORY_SLOT_NOT_SET)
		oldItem = _player->GetItemInterface()->GetInventoryItem(bagslot, slot);

	if(oldItem != NULL)
	{
		// try to add to the existing items stack
		if(oldItem->GetProto() != it)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_ITEM_DOESNT_GO_TO_SLOT);
			return;
		}

		if( (oldItem->GetStackCount() + count_per_stack) > itemMaxStack )
		{
//			sLog.outDebug( "SUPADBG can't carry #2" );
			_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_CANT_CARRY_MORE_OF_THIS);
			return;
		}

		oldItem->ModStackCount(  count_per_stack);
		oldItem->m_isDirty = true;
		pItem=oldItem;
	}
	else
	{
		// create new item
		if(slot == INVENTORY_SLOT_NOT_SET)
			slot = c->FindFreeSlot();
		
		if(slot==ITEM_NO_SLOT_AVAILABLE)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_BAG_FULL);
			return;
		}

		pItem = objmgr.CreateItem(it->ItemId, _player);
		if(pItem)
		{
			pItem->SetStackCount(  count_per_stack);
			pItem->m_isDirty = true;
//			sLog.outDebug( "SUPADBG bagslot=%u, slot=%u" , bagslot, slot );
			if(!_player->GetItemInterface()->SafeAddItem(pItem, bagslot, slot))
			{
				pItem->DeleteMe();
				return;
			}
		}
		else
			return;
	}

    _player->SendItemPushResult( false, true, false, (pItem==oldItem) ? false : true, bagslot, slot, amount*ci.amount, pItem->GetEntry(), pItem->GetItemRandomSuffixFactor(), pItem->GetItemRandomPropertyId(), pItem->GetStackCount()  );

	WorldPacket data(SMSG_BUY_ITEM, 22);
	data << uint64(srcguid);
	data << getMSTime();
	data << uint32(itemid); 
    data << uint32(amount);

	SendPacket( &data );

	sLog.outDetail( "WORLD: Sent SMSG_BUY_ITEM" );

	_player->GetItemInterface()->BuyItem(it,amount,unit);
	if(ci.max_amount)
	{
		unit->ModAvItemAmount(ci.itemid,ci.amount*amount);

		// there is probably a proper opcode for this. - burlex
		SendInventoryList(unit);
	}
}

void WorldSession::HandleBuyItemOpcode( WorldPacket & recv_data ) // right-click on item
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 14);
	sLog.outDetail( "WORLD: Received CMSG_BUY_ITEM" );

	WorldPacket data(45);
	uint64 srcguid= 0;
	uint32 itemid= 0;
	int32 slot= 0;
	uint8 amount= 0;
//	int8 playerslot = 0;
//	int8 bagslot = 0;
	Item *add = NULL;
	uint8 error = 0;
	SlotResult slotresult;
	AddItemResult result;

	recv_data >> srcguid >> itemid;
	recv_data >> slot >> amount;


	Creature *unit = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(srcguid));
	if (unit == NULL || !unit->HasItems())
		return;

    ItemExtendedCostEntry * ex = unit->GetItemExtendedCostByItemId( itemid );

	if(amount < 1)
		amount = 1;

	CreatureItem item;
	unit->GetSellItemByItemId(itemid, item);

	if(item.itemid == 0)
	{
		// vendor does not sell this item.. bitch about cheaters?
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_DONT_OWN_THAT_ITEM);
		return;
	}

	if (item.max_amount>0 && item.available_amount<amount)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_ITEM_IS_CURRENTLY_SOLD_OUT);
		return;
	}

	ItemPrototype *it = ItemPrototypeStorage.LookupEntry(itemid);
	if(!it) 
	{
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_DONT_OWN_THAT_ITEM);
		return;
	}

	uint32 itemMaxStack = (_player->ItemStackCheat) ? 0x7fffffff : it->MaxCount;
	if( itemMaxStack > 0 && amount*item.amount > itemMaxStack )
	{
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_ITEM_CANT_STACK);
		return;
	}

	if((error = _player->GetItemInterface()->CanReceiveItem(it, amount*item.amount)) != 0)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		return;
	}

	if( (error = _player->GetItemInterface()->CanAffordItem(it, amount, unit)) != 0 )
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		return;
	}

	// Find free slot and break if inv full
	add = _player->GetItemInterface()->FindItemLessMax(itemid,amount*item.amount, false);
	if (!add)
	{
		slotresult = _player->GetItemInterface()->FindFreeInventorySlot(it);
	}
	if ((!slotresult.Result) && (!add))
	{
		//Player doesn't have a free slot in his/her bag(s)
		_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_INVENTORY_FULL);
		return;
	}

	if(!add)
	{
		Item *itm = objmgr.CreateItem(item.itemid, _player);
		if(!itm)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_DONT_OWN_THAT_ITEM);
			return;
		}

		itm->m_isDirty=true;
		itm->SetStackCount(  amount*item.amount);
        
		if(slotresult.ContainerSlot == ITEM_NO_SLOT_AVAILABLE)
		{
			result = _player->GetItemInterface()->SafeAddItem(itm, INVENTORY_SLOT_NOT_SET, slotresult.Slot);
			if(!result)
			{
				itm->DeleteMe();
			}
            else{
                if( itm->IsEligibleForRefund() && ex != NULL ){
                    itm->GetOwner()->GetItemInterface()->AddRefundable( itm->GetGUID(), ex->costid );
                }
                _player->SendItemPushResult( false, true, false, true, static_cast<uint8>(INVENTORY_SLOT_NOT_SET), slotresult.Result, amount*item.amount, itm->GetEntry(), itm->GetItemRandomSuffixFactor(), itm->GetItemRandomPropertyId(), itm->GetStackCount()  );
            }
		}
		else
		{
			if(Item *bag = _player->GetItemInterface()->GetInventoryItem(slotresult.ContainerSlot))
			{
				if( !((Container*)bag)->AddItem(slotresult.Slot, itm) )
					itm->DeleteMe();
                else{
                    if( itm->IsEligibleForRefund() && ex != NULL ){
                        itm->GetOwner()->GetItemInterface()->AddRefundable( itm->GetGUID(), ex->costid );
                    }
                    _player->SendItemPushResult( false, true, false, true, slotresult.ContainerSlot, slotresult.Result, 1, itm->GetEntry(), itm->GetItemRandomSuffixFactor(), itm->GetItemRandomPropertyId(), itm->GetStackCount()  );
                }
			}
		}
	}
	else
	{
		add->ModStackCount(  amount*item.amount);
		add->m_isDirty = true;
        _player->SendItemPushResult( false, true, false, false, (uint8)_player->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()), 1, amount*item.amount , add->GetEntry(), add->GetItemRandomSuffixFactor(), add->GetItemRandomPropertyId(), add->GetStackCount()  );
	}



    _player->GetItemInterface()->BuyItem(it,amount,unit);

	data.Initialize( SMSG_BUY_ITEM );
	
    data << uint64(srcguid);
	data << getMSTime();
	data << uint32(itemid);
    data << uint32(amount*item.amount);

	SendPacket( &data );

	if(item.max_amount)
	{
		unit->ModAvItemAmount(item.itemid,item.amount*amount);

		// there is probably a proper opcode for this. - burlex
		SendInventoryList(unit);
	}
}

void WorldSession::HandleListInventoryOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 8);
	sLog.outDetail( "WORLD: Recvd CMSG_LIST_INVENTORY" );
	uint64 guid;

	recv_data >> guid;

	Creature *unit = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if (unit == NULL)
		return;

	VendorRestrictionEntry *vendor = VendorRestrictionEntryStorage.LookupEntry(unit->GetProto()->Id);

	//this is a blizzlike check
	if ( _player->GetDistanceSq( unit ) > 100 )
		return; //avoid talking to anyone by guid hacking. Like sell farmed items anytime ? Low chance hack

	if (unit->GetAIInterface())
		unit->GetAIInterface()->StopMovement(180000);

	_player->Reputation_OnTalk(unit->m_factionDBC);
	
	if (_player->CanBuyAt(vendor))
		SendInventoryList(unit);
	else
	{
		GossipMenu * pMenu;
		objmgr.CreateGossipMenuForPlayer(&pMenu,unit->GetGUID(),vendor->cannotbuyattextid,_player);
		pMenu->SendTo(_player);
	}
}

void WorldSession::SendInventoryList(Creature* unit)
{

	if( !unit->HasItems() )
	{
		sChatHandler.BlueSystemMessageToPlr( _player, "No sell template found. Report this to database's devs: %d (%s)", unit->GetEntry(), unit->GetCreatureInfo()->Name );
		sLog.outError( "'%s' discovered that a creature with entry %u (%s) has no sell template.", GetPlayer()->GetName(), unit->GetEntry(), unit->GetCreatureInfo()->Name );
		GetPlayer()->Gossip_Complete(); // cebernic: don't get a hang for the NPC
		return;
	}

	WorldPacket data(((unit->GetSellItemCount() * 28) + 9));	   // allocate

	data.SetOpcode( SMSG_LIST_INVENTORY );
	data << unit->GetGUID();
	data << uint8( 0 ); // placeholder for item count

	ItemPrototype * curItem = NULL;
	uint32 counter = 0;

	for(std::vector<CreatureItem>::iterator itr = unit->GetSellItemBegin(); itr != unit->GetSellItemEnd(); ++itr)
	{
		if(itr->itemid && (itr->max_amount == 0 || (itr->max_amount>0 && itr->available_amount >0)))
		{
			if((curItem = ItemPrototypeStorage.LookupEntry(itr->itemid)) != 0)
			{
				if(curItem->AllowableClass && !(_player->getClassMask() & curItem->AllowableClass) && !_player->GetSession()->HasGMPermissions()) // cebernic: GM looking up for everything.
					continue;
				if(curItem->AllowableRace && !(_player->getRaceMask() & curItem->AllowableRace) && !_player->GetSession()->HasGMPermissions())
					continue;

				int32 av_am = (itr->max_amount>0)?itr->available_amount:-1;
				data << (counter + 1);
				data << curItem->ItemId;
				data << curItem->DisplayInfoID;
				data << av_am;
				data << GetBuyPriceForItem(curItem, 1, _player, unit);
				data << int32(-1);			// wtf is dis?
				data << itr->amount;

				if( itr->extended_cost != NULL )
					data << itr->extended_cost->costid;
				else
					data << uint32(0);

				++counter;
				if ( counter>=MAX_CREATURE_INV_ITEMS ) break; // cebernic: in 2.4.3, client can't take more than 15 pages,it making crash for us:(
			}
		}
	}

	const_cast<uint8*>(data.contents())[8] = (uint8)counter;	// set count

	SendPacket( &data );
	sLog.outDetail( "WORLD: Sent SMSG_LIST_INVENTORY" );
}

void WorldSession::HandleAutoStoreBagItemOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 3);
	sLog.outDetail( "WORLD: Recvd CMSG_AUTO_STORE_BAG_ITEM" );
	
	//WorldPacket data;
	WorldPacket packet;
	int8 SrcInv= 0, Slot= 0, DstInv= 0;
//	Item *item= NULL;
	Item *srcitem = NULL;
	Item *dstitem= NULL;
	int8 NewSlot = 0;
	int8 error;
	AddItemResult result;

	recv_data >> SrcInv >> Slot >> DstInv;

	srcitem = _player->GetItemInterface()->GetInventoryItem(SrcInv, Slot);

	//source item exists
	if(srcitem)
	{
		//src containers cant be moved if they have items inside
		if(srcitem->IsContainer() && static_cast<Container*>(srcitem)->HasItems())
		{
			_player->GetItemInterface()->BuildInventoryChangeError(srcitem, 0, INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG);
			return;
		}
		//check for destination now before swaping.
		//destination is backpack
		if(DstInv == INVENTORY_SLOT_NOT_SET)
		{
			//check for space
			NewSlot = _player->GetItemInterface()->FindFreeBackPackSlot();
			if(NewSlot == ITEM_NO_SLOT_AVAILABLE)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(srcitem, 0, INV_ERR_BAG_FULL);
				return;
			}
			else
			{
				//free space found, remove item and add it to the destination
				srcitem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(SrcInv, Slot, false);
				if( srcitem )
				{
					result = _player->GetItemInterface()->SafeAddItem(srcitem, INVENTORY_SLOT_NOT_SET, NewSlot);
					if(!result)
					{
						sLog.outError("HandleAutoStoreBagItem: Error while adding item to newslot");
						srcitem->DeleteMe();
						return;
					}
				}
			}
		}
		else
		{
			if((error=_player->GetItemInterface()->CanEquipItemInSlot2(DstInv,  DstInv, srcitem)) != 0)
			{
				if(DstInv < CURRENCYTOKEN_SLOT_END)
				{
					_player->GetItemInterface()->BuildInventoryChangeError(srcitem,dstitem, error);
					return;
				}
			}

			//destination is a bag
			dstitem = _player->GetItemInterface()->GetInventoryItem(DstInv);
			if(dstitem)
			{
				//dstitem exists, detect if its a container
				if(dstitem->IsContainer())
				{
					NewSlot = static_cast<Container*>(dstitem)->FindFreeSlot();
					if(NewSlot == ITEM_NO_SLOT_AVAILABLE)
					{
						_player->GetItemInterface()->BuildInventoryChangeError(srcitem, 0, INV_ERR_BAG_FULL);
						return;
					}
					else
					{
						srcitem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(SrcInv, Slot, false);
						if( srcitem != NULL )
						{
							result = _player->GetItemInterface()->SafeAddItem(srcitem, DstInv, NewSlot);
							if(!result)
							{
								sLog.outError("HandleBuyItemInSlot: Error while adding item to newslot");
								srcitem->DeleteMe();
								return;
							}		
						}
					}
				}
				else
				{
					_player->GetItemInterface()->BuildInventoryChangeError(srcitem, 0,  INV_ERR_ITEM_DOESNT_GO_TO_SLOT);
					return;
				}
			}
			else
			{
				_player->GetItemInterface()->BuildInventoryChangeError(srcitem, 0, INV_ERR_ITEM_DOESNT_GO_TO_SLOT);
				return;
			}
		}
	}
	else
	{
		_player->GetItemInterface()->BuildInventoryChangeError(srcitem, 0, INV_ERR_ITEM_NOT_FOUND);
		return;
	}
}

void WorldSession::HandleReadItemOpcode(WorldPacket &recvPacket)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recvPacket, 2);
	int8 uslot= 0, slot= 0;
	recvPacket >> uslot >> slot;

	Item *item = _player->GetItemInterface()->GetInventoryItem(uslot, slot);
	sLog.outDebug("Received CMSG_READ_ITEM %d", slot);

	if(item)
	{
		// Check if it has pagetext
	   
		if(item->GetProto()->PageId)
		{
			WorldPacket data(SMSG_READ_ITEM_OK, 4);
			data << item->GetGUID();
			SendPacket(&data);
			sLog.outDebug("Sent SMSG_READ_OK %d", item->GetGUID());
		}
		else
		{
			WorldPacket data(SMSG_READ_ITEM_FAILED, 5);
			data << item->GetGUID();
			data << uint8(2);
			SendPacket(&data);
			sLog.outDebug("Sent SMSG_READ_ITEM_FAILED %d", item->GetGUID());
		}
	}
}

void WorldSession::HandleRepairItemOpcode(WorldPacket &recvPacket)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recvPacket, 17);//8+8+1

	uint64 npcguid;
	uint64 itemguid;
	Item * pItem;
	Container * pContainer;
	uint32 j, i;
	bool guildmoney;

	recvPacket >> npcguid >> itemguid >> guildmoney;

	Creature * pCreature = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(npcguid) );
	if( pCreature == NULL )
		return;

	if( !pCreature->isArmorer() )
		return;

	//this is a blizzlike check
	if( _player->GetDistanceSq( pCreature ) > 100 )
		return; //avoid talking to anyone by guid hacking. Like repair items anytime in raid ? Low chance hack
	
	if( guildmoney )
	{
		if( _player->IsInGuild() )
		{
			if( !(_player->GetGuildRankS()->iRights & GR_RIGHT_GUILD_BANK_REPAIR) )
			{
				return; //we have not permissions to do that
			}
		}
		else
			return;//can't repair with guild money if player is not in guild.
	}

	if( !itemguid ) 
	{
		int32 totalcost = 0;
		for( i = 0; i < MAX_INVENTORY_SLOT; i++ )
		{
			pItem = _player->GetItemInterface()->GetInventoryItem( static_cast<int16>( i ) );
			if( pItem != NULL )
			{
				if( pItem->IsContainer() )
				{
					pContainer = static_cast<Container*>( pItem );
					for( j = 0; j < pContainer->GetProto()->ContainerSlots; ++j )
					{
						pItem = pContainer->GetItem( static_cast<int16>( j ) );
						if( pItem != NULL )
							pItem->RepairItem( _player, guildmoney, &totalcost );
					}
				}
				else
				{
					if( i < INVENTORY_SLOT_BAG_END )
					{
						if( pItem->GetDurability() == 0 && pItem->RepairItem( _player, guildmoney, &totalcost ) )
							_player->ApplyItemMods( pItem, static_cast<int16>( i ), true );
						else
							pItem->RepairItem( _player, guildmoney, &totalcost );
					}
				}
			}
		}
		if( totalcost > 0) //we already checked if it's in guild in RepairItem()
			_player->GetGuild()->LogGuildBankActionMoney(GUILD_BANK_LOG_EVENT_REPAIR, _player->GetLowGUID(), totalcost);
	}
	else 
	{
		Item *item = _player->GetItemInterface()->GetItemByGUID(itemguid);
		if(item)
		{
			SlotResult *searchres=_player->GetItemInterface()->LastSearchResult();//this never gets null since we get a pointer to the inteface internal var
			uint32 dDurability = item->GetDurabilityMax() - item->GetDurability();

			if (dDurability)
			{
                uint32 cDurability = item->GetDurability();
				//only apply item mods if they are on char equipped
                if( item->RepairItem( _player ) && cDurability == 0 && searchres->ContainerSlot==INVALID_BACKPACK_SLOT && searchres->Slot < static_cast<int8>( INVENTORY_SLOT_BAG_END ))
                    _player->ApplyItemMods(item, searchres->Slot, true);
			}
		}
	}
	sLog.outDebug("Received CMSG_REPAIR_ITEM %d", itemguid);
}

void WorldSession::HandleBuyBankSlotOpcode(WorldPacket& recvPacket)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recvPacket, 8);

	uint64 guid;
	recvPacket >> guid;
	Creature* Banker = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );

	if(Banker == NULL || !Banker->isBanker())
	{
		WorldPacket data(SMSG_BUY_BANK_SLOT_RESULT, 4);
		data << uint32(2); // E_ERR_BANKSLOT_NOTBANKER
		SendPacket( &data );
		return;
	}

	uint32 bytes,slots;
	int32 price;

	sLog.outDebug("WORLD: CMSG_BUY_bytes_SLOT");

	bytes = GetPlayer()->GetUInt32Value(PLAYER_BYTES_2);
	slots =(uint8) (bytes >> 16);

	sLog.outDetail("PLAYER: Buy bytes bag slot, slot number = %d", slots);
	BankSlotPrice* bsp = dbcBankSlotPrices.LookupEntryForced(slots+1);
	if(bsp == NULL)
	{
		WorldPacket data(SMSG_BUY_BANK_SLOT_RESULT, 4);
		data << uint32(0); // E_ERR_BANKSLOT_FAILED_TOO_MANY
		SendPacket( &data );
		return;
	}

	price = bsp->Price;
	if( _player->HasGold(price) )
	{
	   _player->SetUInt32Value(PLAYER_BYTES_2, (bytes&0xff00ffff) | ((slots+1) << 16) );
	   _player->ModGold( -price );
#ifdef ENABLE_ACHIEVEMENTS
		_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT, 1, 0, 0);
#endif
	}
	else
	{
		WorldPacket data(SMSG_BUY_BANK_SLOT_RESULT, 4);
		data << uint32(1); // E_ERR_BANKSLOT_INSUFFICIENT_FUNDS
		SendPacket( &data );
	}
}

void WorldSession::HandleAutoBankItemOpcode(WorldPacket &recvPacket)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recvPacket, 2);
	sLog.outDebug("WORLD: CMSG_AUTO_BANK_ITEM");

	//WorldPacket data;

	SlotResult slotresult;
	int8 SrcInvSlot, SrcSlot;//, error= 0;

	recvPacket >> SrcInvSlot >> SrcSlot;

	sLog.outDetail("ITEM: Auto Bank, Inventory slot: %u Source Slot: %u", (uint32)SrcInvSlot, (uint32)SrcSlot);

	Item *eitem=_player->GetItemInterface()->GetInventoryItem(SrcInvSlot,SrcSlot);

	if(!eitem)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(eitem, NULL, INV_ERR_ITEM_NOT_FOUND);
		return;
	}

	slotresult =  _player->GetItemInterface()->FindFreeBankSlot(eitem->GetProto());

	if(!slotresult.Result)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(eitem, NULL, INV_ERR_BANK_FULL);
		return;
	}
	else
	{
		eitem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(SrcInvSlot,SrcSlot, false);
		if (eitem== NULL)
			return;

		if(!_player->GetItemInterface()->SafeAddItem(eitem, slotresult.ContainerSlot, slotresult.Slot))
		{
			sLog.outDebug("[ERROR]AutoBankItem: Error while adding item to bank bag!");
			if( !_player->GetItemInterface()->SafeAddItem(eitem, SrcInvSlot, SrcSlot) )
				eitem->DeleteMe();
		}
	}
}

void WorldSession::HandleAutoStoreBankItemOpcode(WorldPacket &recvPacket)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recvPacket, 2);
	sLog.outDebug("WORLD: CMSG_AUTOSTORE_BANK_ITEM");

	//WorldPacket data;

	int8 SrcInvSlot, SrcSlot;//, error= 0, slot=-1, specialbagslot=-1;

	recvPacket >> SrcInvSlot >> SrcSlot;

	sLog.outDetail("ITEM: AutoStore Bank Item, Inventory slot: %u Source Slot: %u", (uint32)SrcInvSlot, (uint32)SrcSlot);

	Item *eitem=_player->GetItemInterface()->GetInventoryItem(SrcInvSlot,SrcSlot);

	if(!eitem) 
	{
		_player->GetItemInterface()->BuildInventoryChangeError(eitem, NULL, INV_ERR_ITEM_NOT_FOUND);
		return;
	}

	SlotResult slotresult = _player->GetItemInterface()->FindFreeInventorySlot(eitem->GetProto());

	if(!slotresult.Result)
	{
		_player->GetItemInterface()->BuildInventoryChangeError(eitem, NULL, INV_ERR_INVENTORY_FULL);
		return;
	}
	else
	{
		eitem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(SrcInvSlot, SrcSlot, false);
		if (eitem== NULL)
			return;
		if (!_player->GetItemInterface()->AddItemToFreeSlot(eitem))
		{
			sLog.outDebug("[ERROR]AutoStoreBankItem: Error while adding item from one of the bank bags to the player bag!");
			if( !_player->GetItemInterface()->SafeAddItem(eitem, SrcInvSlot, SrcSlot) )
				eitem->DeleteMe();
		}
	}
}

void WorldSession::HandleCancelTemporaryEnchantmentOpcode(WorldPacket &recvPacket)
{
	CHECK_INWORLD_RETURN

	uint32 inventory_slot;
	recvPacket >> inventory_slot;

	Item * item = _player->GetItemInterface()->GetInventoryItem( static_cast<int16>( inventory_slot ));
	if(!item) return;

	item->RemoveAllEnchantments(true);
}

void WorldSession::HandleInsertGemOpcode(WorldPacket &recvPacket)
{
	CHECK_INWORLD_RETURN

	uint64 itemguid;
	uint64 gemguid[ 3 ];
	ItemInterface *itemi = _player->GetItemInterface();
	GemPropertyEntry * gp;
	EnchantEntry * Enchantment;
	recvPacket >> itemguid ;

	Item * TargetItem = itemi->GetItemByGUID(itemguid);
	if(!TargetItem)
		return;

	ItemPrototype *TargetProto = TargetItem->GetProto();
	int slot =itemi->GetInventorySlotByGuid(itemguid);

	bool apply = (slot>= 0 && slot <19);
	uint32 FilledSlots= 0;

	//cheat -> tried to socket same gem multiple times
	for(uint32 i = 0; i < 3; i++)
		recvPacket >> gemguid[i];
	
	if((gemguid[0] && (gemguid[0] == gemguid[1] || gemguid[0] == gemguid[2])) || (gemguid[1] && (gemguid[1] == gemguid[2]))){
		return;
	}

	bool ColorMatch = true;
	for(uint32 i = 0;i<TargetItem->GetSocketsCount();i++)
	{
		EnchantmentInstance * EI= TargetItem->GetEnchantment(SOCK_ENCHANTMENT_SLOT1 + i);
		if(EI)
		{
			FilledSlots++;
			ItemPrototype * ip = ItemPrototypeStorage.LookupEntry(EI->Enchantment->GemEntry);
			if(!ip)
				gp = NULL;
			else
				gp = dbcGemProperty.LookupEntry(ip->GemProperties);

			if(gp && !(gp->SocketMask & TargetProto->Sockets[i].SocketColor))
				ColorMatch = false;
		}

		if( gemguid[ i ] )//add or replace gem
		{
			Item * it = NULL;
			ItemPrototype * ip = NULL;
			
			// tried to put gem in socket where no socket exists (take care about prismatic sockets)
			if (!TargetProto->Sockets[i].SocketColor){
				// no prismatic socket
				if(!TargetItem->GetEnchantment(PRISMATIC_ENCHANTMENT_SLOT))
					return;
				
				// not first not-colored (not normally used) socket
				if(i != 0 && !TargetProto->Sockets[i-1].SocketColor && (i+1 >= 3 || TargetProto->Sockets[i+1].SocketColor))
					return;
				
				// ok, this is first not colored socket for item with prismatic socket
			}


			if (apply) 
			{
				it = itemi->GetItemByGUID( gemguid[ i ] );
				if( !it )
					continue;

				ip = it->GetProto();
				if( ip->Flags & ITEM_FLAG_UNIQUE_EQUIP && itemi->IsEquipped( ip->ItemId ) )
				{
					itemi->BuildInventoryChangeError( it, TargetItem, INV_ERR_CANT_CARRY_MORE_OF_THIS );
					continue;
				}
				// Skill requirement
				if( ip->RequiredSkill )
				{
					if( ip->RequiredSkillRank > _player->_GetSkillLineCurrent( ip->RequiredSkill, true ) )
					{
						itemi->BuildInventoryChangeError( it, TargetItem, INV_ERR_SKILL_ISNT_HIGH_ENOUGH );
						continue;
					}
				}
				if( ip->ItemLimitCategory )
				{
					ItemLimitCategoryEntry * ile = dbcItemLimitCategory.LookupEntryForced( ip->ItemLimitCategory );
					if( ile != NULL && itemi->GetEquippedCountByItemLimit( ip->ItemLimitCategory ) >= ile->maxAmount )
					{
						itemi->BuildInventoryChangeError(it, TargetItem, INV_ERR_ITEM_MAX_COUNT_EQUIPPED_SOCKETED);
						continue;
					}
				}
			}

			it = itemi->SafeRemoveAndRetreiveItemByGuid( gemguid[ i ],true );
			if( !it ) 
				return; //someone sending hacked packets to crash server

			gp = dbcGemProperty.LookupEntryForced(it->GetProto()->GemProperties);
			it->DeleteMe();
		
			if(!gp)
				continue;
			
			if(!(gp->SocketMask & TargetProto->Sockets[i].SocketColor))
				ColorMatch = false;

			if(!gp->EnchantmentID)//this is ok in few cases
				continue;
			//Meta gems only go in meta sockets.
			if(TargetProto->Sockets[i].SocketColor != GEM_META_SOCKET && gp->SocketMask == GEM_META_SOCKET)
				continue;
			if(EI)//replace gem
				TargetItem->RemoveEnchantment(2+i);//remove previous
			else//add gem
				FilledSlots++;

			Enchantment = dbcEnchant.LookupEntryForced(gp->EnchantmentID);
			if(Enchantment && TargetItem->GetProto()->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
				TargetItem->AddEnchantment(Enchantment, 0, true,apply,false,2+i);
		}
	}

	//Add color match bonus
	if(TargetItem->GetProto()->SocketBonus)
	{
		if(ColorMatch && (FilledSlots==TargetItem->GetSocketsCount()))
		{
			if(TargetItem->HasEnchantment(TargetItem->GetProto()->SocketBonus) > 0)
				return;

			Enchantment = dbcEnchant.LookupEntryForced(TargetItem->GetProto()->SocketBonus);
			if(Enchantment && TargetItem->GetProto()->SubClass != ITEM_SUBCLASS_WEAPON_THROWN)
			{
				uint32 Slot = TargetItem->FindFreeEnchantSlot(Enchantment,0);
				TargetItem->AddEnchantment(Enchantment, 0, true,apply,false, Slot);
			}
		}else //remove
		{
			TargetItem->RemoveSocketBonusEnchant();
		}
	}

	TargetItem->m_isDirty = true;
}

void WorldSession::HandleWrapItemOpcode( WorldPacket& recv_data )
{
	CHECK_INWORLD_RETURN

	int8 sourceitem_bagslot, sourceitem_slot;
	int8 destitem_bagslot, destitem_slot;
	uint32 source_entry;
	uint32 itemid;
	Item * src,*dst;

	recv_data >> sourceitem_bagslot >> sourceitem_slot;
	recv_data >> destitem_bagslot >> destitem_slot;

	src = _player->GetItemInterface()->GetInventoryItem( sourceitem_bagslot, sourceitem_slot );
	dst = _player->GetItemInterface()->GetInventoryItem( destitem_bagslot, destitem_slot );

	if( !src || !dst )
		return;

	if(src == dst || !(src->GetProto()->Class == 0 && src->GetProto()->SubClass == 8))
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_WRAPPED_CANT_BE_WRAPPED );
		return;
	}

	if( dst->GetStackCount() > 1 )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_STACKABLE_CANT_BE_WRAPPED );
		return;
	}

	uint32 dstItemMaxStack = (dst->GetOwner()->ItemStackCheat) ? 0x7fffffff : dst->GetProto()->MaxCount;
	if( dstItemMaxStack > 1 )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_STACKABLE_CANT_BE_WRAPPED );
		return;
	}

	if( dst->IsSoulbound() )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_BOUND_CANT_BE_WRAPPED );
		return;
	}

	if( dst->wrapped_item_id || src->wrapped_item_id )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_WRAPPED_CANT_BE_WRAPPED );
		return;
	}

	if( dst->GetProto()->Unique )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_UNIQUE_CANT_BE_WRAPPED );
		return;
	}

	if( dst->IsContainer() )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_BAGS_CANT_BE_WRAPPED );
		return;
	}

	if( dst->HasEnchantments() )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_ITEM_LOCKED );
		return;
	}
	if( destitem_bagslot == -1 && ( destitem_slot >= int8( EQUIPMENT_SLOT_START ) && destitem_slot <= int8( INVENTORY_SLOT_BAG_END ) ) )
	{
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_EQUIPPED_CANT_BE_WRAPPED );
		return;
	}

	// all checks passed ok
	source_entry = src->GetEntry();
	itemid = source_entry;
	switch( source_entry )
	{
	case 5042:
		itemid = 5043;
		break;

	case 5048:
		itemid = 5044;
		break;

	case 17303:
		itemid = 17302;
		break;

	case 17304:
		itemid = 17305;
		break;

	case 17307:
		itemid = 17308;
		break;

	case 21830:
		itemid = 21831;
		break;

	default:
		_player->GetItemInterface()->BuildInventoryChangeError( src, dst, INV_ERR_WRAPPED_CANT_BE_WRAPPED );
		return;
		break;
	}

	dst->SetProto( src->GetProto() );

	if( src->GetStackCount() <= 1 )
	{
		// destroy the source item
		_player->GetItemInterface()->SafeFullRemoveItemByGuid( src->GetGUID() );
	}
	else
	{
		// reduce stack count by one
		src->ModStackCount(  -1 );
		src->m_isDirty = true;
	}

	// change the dest item's entry
	dst->wrapped_item_id = dst->GetEntry();
	dst->SetEntry(  itemid );

	// set the giftwrapper fields
    dst->SetGiftCreatorGUID( _player->GetGUID() );
	dst->SetDurability( 0 );
	dst->SetDurabilityMax( 0 );
    dst->Wrap();

	// save it
	dst->m_isDirty = true;
	dst->SaveToDB( destitem_bagslot, destitem_slot, false, NULL );
}

void WorldSession::HandleItemRefundInfoOpcode( WorldPacket& recvPacket ){
	CHECK_INWORLD_RETURN

    sLog.outDebug("Recieved CMSG_ITEMREFUNDINFO.");

    //////////////////////////////////////////////////////////////////////////////////////////
    //  As of 3.2.0a the client sends this packet to request refund info on an item
    //
    //	{CLIENT} Packet: (0x04B3) UNKNOWN PacketSize = 8 TimeStamp = 265984125
    //	E6 EE 09 18 02 00 00 42 
    //
    //  
    //
    //  Structure:
    //  uint64 GUID
    //////////////////////////////////////////////////////////////////////////////////////////

    uint64 GUID;
    recvPacket >> GUID;

    this->SendRefundInfo( GUID );

}

void WorldSession::HandleItemRefundRequestOpcode( WorldPacket& recvPacket ){
	CHECK_INWORLD_RETURN

    sLog.outDebug("Recieved CMSG_ITEMREFUNDREQUEST.");

    //////////////////////////////////////////////////////////////////////////////////////////
    //  As of 3.2.0a the client sends this packet to initiate refund of an item
    //
    //	{CLIENT} Packet: (0x04B4) UNKNOWN PacketSize = 8 TimeStamp = 266021296
    //	E6 EE 09 18 02 00 00 42 
    //
    //  
    //
    //  Structure:
    //  uint64 GUID
    //////////////////////////////////////////////////////////////////////////////////////////

    uint64 GUID;
    uint32 error = 1;
    Item* itm = NULL;
    std::pair< time_t, uint32 > RefundEntry;
    ItemExtendedCostEntry *ex = NULL;
    ItemPrototype *proto = NULL;
    
    
    recvPacket >> GUID;

    itm = _player->GetItemInterface()->GetItemByGUID( GUID );

    if( itm != NULL ){
        if( itm->IsEligibleForRefund() ){
            RefundEntry.first = 0;
            RefundEntry.second = 0;
            
            RefundEntry = _player->GetItemInterface()->LookupRefundable( GUID );

			// If the item is refundable we look up the extendedcost
            if( RefundEntry.first != 0 && RefundEntry.second != 0 ){
                uint32 *played = _player->GetPlayedtime();

                if( played[1] < ( RefundEntry.first + 60*60*2 ) )
                    ex = dbcItemExtendedCost.LookupEntry( RefundEntry.second );
            }

            if( ex != NULL )
			{
                proto = itm->GetProto();

////////////////////////////////// We remove the refunded item and refund the cost //////////////////////////////////
                    
                for( int i = 0; i < 5; ++i ){
                    _player->GetItemInterface()->AddItemById( ex->item[i], ex->count[i], 0 );
                }

                _player->GetItemInterface()->AddItemById( 43308, ex->honor, 0 );   // honor points
                _player->GetItemInterface()->AddItemById( 43307, ex->arena, 0 );  // arena points
                _player->ModGold( proto->BuyPrice );

                _player->GetItemInterface()->RemoveItemAmtByGuid( GUID, 1 );

                _player->GetItemInterface()->RemoveRefundable( GUID );

				// we were successful!
				error = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  As of 3.1.3 the client sends this packet to provide refunded cost info to the client
    //
    //	{SERVER} Packet: (0x04B5) UNKNOWN PacketSize = 64 TimeStamp = 266021531
    //	E6 EE 09 18 02 00 00 42 00 00 00 00 00 00 00 00 4B 25 00 00 00 00 00 00 50 50 00 00 0A 00 00 00 00
    //  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
    //
    //
    //  Structure (on success):
    //  uint64 GUID
    //  uint32 errorcode
    //  
    //
    //  Structure (on failure):
    //  uint64 GUID
    //  uint32 price (in copper)
    //  uint32 honor
    //  uint32 arena
    //  uint32 item1
    //  uint32 item1cnt
    //  uint32 item2
    //  uint32 item2cnt
    //  uint32 item3
    //  uint32 item3cnt
    //  uint32 item4
    //  uint32 item4cnt
    //  uint32 item5
    //  uint32 item5cnt
    //  
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        WorldPacket packet( SMSG_ITEMREFUNDREQUEST, 60 );
        packet << uint64( GUID );
        packet << uint32( error );

        if( error == 0 ){
            packet << uint32( proto->BuyPrice );
            packet << uint32( ex->honor );
            packet << uint32( ex->arena );
            
            for( int i = 0; i < 5; ++i ){
                packet << uint32( ex->item[i] );
                packet << uint32( ex->count[i] );
            }

        }

        SendPacket( &packet );

        sLog.outDebug("Sent SMSG_ITEMREFUNDREQUEST.");
}