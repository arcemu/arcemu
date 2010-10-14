/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ITEMFUNCTIONS_H
#define ITEMFUNCTIONS_H

#include "StdAfx.h"

namespace lua_engine
{
	void bindItemMethods(luabridge::module &  m)
	{
#define BIND(name) .method(#name, &ItemInterface::name)
#define BINDE(name, functor) .method(name, functor)
		m	.class_<ItemInterface>("ItemInterface")
			BIND(GetOwner)
			BIND(IsBagSlot)
			BINDE("GetInventoryItem", (Item *(ItemInterface::*)(int8,int16) )&ItemInterface::GetInventoryItem)
			BINDE("GetInventorySlotByID", &ItemInterface::GetInventorySlotById)
			BINDE("SafeAddItem", (Item*(ItemInterface::*)(uint32, int8, int16) )&ItemInterface::SafeAddItem)
			BINDE("RemoveItemFromSlot", &ItemInterface::SafeFullRemoveItemFromSlot)
			BIND(AddItemToFreeSlot)
			BIND(AddItemToFreeBankSlot)
			BIND(GetItemCount)
			BIND(RemoveItemAmt)
			BIND(RemoveAllConjured)
			BIND(BuyItem)
			BIND(FindFreeBackPackSlot)
			BIND(FindFreeBackPackSlotMax)
			BIND(FindFreeKeyringSlot)
			BIND(IsEquipped)
			BIND(CanReceiveItem)
			BIND(CanAffordItem);
#undef BIND
#undef BINDE
#define BIND(name) .method(#name, &Item::name)
#define BINDE(name, functor) .method(name, functor)
		m	.subclass<Item,Object>("Item")
			BIND(GetProto)
			BIND(GetOwner)
			BIND(SetOwner)
			BIND(SetStackCount)
			BIND(GetStackCount)
			BIND(ModStackCount)
			BIND(GetDuration)
			BIND(SetDuration)
			BIND(GetCharges)
			BIND(SetCharges)
			BIND(ModCharges)
			BIND(SoulBind)
			BIND(IsSoulbound)
			BIND(AccountBind)
			BIND(IsAccountbound)
			BIND(MakeConjured)
			BIND(IsConjured)
			BIND(Lock)
			BIND(UnLock)
			BIND(Wrap)
			BIND(UnWrap)
			BIND(ClearFlags)

			BINDE("GetItemRandomPropertyID", &Item::GetItemRandomPropertyId)
			BIND(GetItemRandomSuffixFactor)
			BINDE("SetItemRandomPropertyID", &Item::SetItemRandomPropertyId)
			BIND(SetItemRandomSuffixFactor)
			BIND(SetRandomSuffix)
			BIND(SetDurability)
			BIND(SetDurabilityMax)
			BIND(GetDurability)
			BIND(GetDurabilityMax)
			BIND(SetDurabilityToMax)
			BINDE("GetEnchantmentID", &Item::GetEnchantmentId)
			BINDE("SetEnchantmentID", &Item::SetEnchantmentId)
			BIND(GetEnchantmentDuration)
			BIND(SetEnchantmentDuration)
			BIND(GetEnchantmentCharges)
			BIND(SetEnchantmentCharges)

			BIND(IsContainer)
			BIND(GetChargesLeft)

			BIND(AddEnchantment)
			BIND(RemoveEnchantment)
			BIND(HasEnchantment)
			BIND(HasEnchantmentOnSlot)
			BIND(ModifyEnchantmentTime)
			BIND(RemoveAllEnchantments)
			BIND(IsAmmoBag)
			BIND(RemoveFromWorld)
			BIND(HasEnchantments);
#undef BIND
#undef BINDE

#define BIND(name) .property_ro(#name, &ItemPrototype::name)
		m	.class_<ItemPrototype>("ItemPrototype")
			BIND(ItemId)
			BIND(Name1)
			BIND(DisplayInfoID)
			BIND(Flags)
			BIND(Faction)
			BIND(BuyPrice)
			BIND(SellPrice)
			BIND(ItemLevel)
			BIND(RequiredLevel)
			BIND(RequiredSkill)
			BIND(RequiredFaction)
			BIND(RequiredFactionStanding)
			BIND(Unique)
			BIND(MaxCount)
			BIND(Delay)
			BIND(RandomPropId)
			BIND(RandomSuffixId)
			BIND(TotemCategory)
			BIND(QuestId)
			BIND(Description);
#undef BIND
	}
}
#endif