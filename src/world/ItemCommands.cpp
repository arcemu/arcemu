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

#include "StdAfx.h"

RepairItemCommand::RepairItemCommand(Player *player, uint64 itemGUID)
{
	this->player = player;
	this->itemGUID = itemGUID;
}

void RepairItemCommand::execute()
{
	Item* item = player->GetItemInterface()->GetItemByGUID(itemGUID);
	if( item == NULL )
		return;
	
	SlotResult* searchres = player->GetItemInterface()->LastSearchResult(); //this never gets null since we get a pointer to the inteface internal var
	uint32 dDurability = item->GetDurabilityMax() - item->GetDurability();

	if(dDurability)
	{
		uint32 cDurability = item->GetDurability();
		if( item->RepairItem(player) )
		{
			//only apply item mods if they are on char equipped
			if(cDurability == 0 && searchres->ContainerSlot == static_cast<int8>(INVALID_BACKPACK_SLOT) && searchres->Slot < static_cast<int8>(INVENTORY_SLOT_BAG_END))
				player->ApplyItemMods(item, searchres->Slot, true);
		}
	}
}

RepairAllItemsCommand::RepairAllItemsCommand( Player *player, bool useGuildMoney )
{
	this->player = player;
	this->useGuildMoney = useGuildMoney;
}

void RepairAllItemsCommand::execute()
{
	int32 totalcost = 0;
	for(int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item *item = player->GetItemInterface()->GetInventoryItem(static_cast<int16>(i));
		if(item != NULL)
		{
			if(item->IsContainer())
			{
				Container *container = TO< Container* >(item);
				for(uint32 j = 0; j < container->GetProto()->ContainerSlots; ++j)
				{
					Item *containerItem = container->GetItem(static_cast<int16>(j));
					if(containerItem != NULL)
						containerItem->RepairItem(player, useGuildMoney, &totalcost);
				}
			}
			else
			{
				uint32 cDurability = item->GetDurability();
				if(item->RepairItem(player, useGuildMoney, &totalcost))
				{
					if( i < INVENTORY_SLOT_BAG_END && cDurability == 0 )
						player->ApplyItemMods(item, static_cast<int16>(i), true);
				}
			}
		}
	}
	if(totalcost > 0)  //we already checked if it's in guild in RepairItem()
		player->GetGuild()->LogGuildBankActionMoney(GUILD_BANK_LOG_EVENT_REPAIR, player->GetLowGUID(), totalcost);
}
