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

#include "LUAEngine.h"

namespace lua_engine
{
	void bindItemMethods(luabridge::module &  m)
	{
#define BIND(name) .method(#name, &ItemInterface::name)
#define BINDE(name, functor) .method(name, functor)
		m	.class_<ItemInterface>("ItemInterface")
			.method( &ItemInterface::GetOwner, "GetOwner", "getOwner", "getowner", NULL)
			.method( &ItemInterface::IsBagSlot, "IsBagSlot", "isBagSlot", "isbagslot", NULL)
			.method( (Item *(ItemInterface::*)(int8,int16) )&ItemInterface::GetInventoryItem, "GetInventoryItem", "getInventoryItem", "getinventoryitem", NULL)
			.method (&ItemInterface::GetInventorySlotById, "GetInventorySlotByID", "getInventorySlotByID", "getinventoryslotbyid", NULL)
			.method( (Item*(ItemInterface::*)(uint32, int8, int16) )&ItemInterface::SafeAddItem, "SafeAddItem","addItem", "additem", NULL)
			.method( &ItemInterface::SafeFullRemoveItemFromSlot, "RemoveItemFromSlot", "removeItemFromSlot", "removeitemfromslot", NULL)
			.method( &ItemInterface::AddItemToFreeSlot, "AddItemToFreeSlot", "addItemToFreeSlot", "additemtofreeslot", NULL)
			.method( &ItemInterface::AddItemToFreeBankSlot, "AddItemToFreeBankSlot", "addItemToFreeBankSlot", "additemtofreebankslot", NULL)
			.method( &ItemInterface::GetItemCount, "GetItemCount", "getItemCount", "getitemcount", NULL)
			.method( &ItemInterface::RemoveItemAmt, "RemoveItemAmnt", "removeItemAmount", "removeitemamount", NULL)
			.method( &ItemInterface::RemoveAllConjured, "removeAllConjured", "removeallconjured", NULL)
			.method( &ItemInterface::BuyItem, "BuyItem", "buyItem", "buyitem", NULL)
			.method( &ItemInterface::FindFreeBackPackSlot, "FindFreeBackPackSlot", "findFreeBackPackSlot", "findfreebackpackslot", NULL)
			.method( &ItemInterface::FindFreeBackPackSlotMax, "FindFreeBackPackSlotMax", "findFreeBackPackSlotMax", "findfreebackpackslotmax", NULL)
			//BIND(FindAmmoBag)
			.method( &ItemInterface::FindFreeKeyringSlot, "findFreeKeyringSlot", "FindFreeKeyRingSlot", "findfreekeyringslot", NULL)
			.method( &ItemInterface::IsEquipped, "isEquipped", "IsEquipped", "isequipped", NULL)
			.method( &ItemInterface::CanReceiveItem, "CanReceiveItem", "canReceiveItem", "canreceiveitem", NULL)
			.method( &ItemInterface::CanAffordItem, "CanAffordItem", "canAffordItem", "canafforditem", NULL)
			.method( &ItemInterface::AddItemById, "AddItemByID", "addItemByID", "additembyid", NULL);
#undef BIND
#undef BINDE
#define BIND(name) .method(#name, &Item::name)
#define BINDE(name, functor) .method(name, functor)
		m	.subclass<Item, Object>("Item")
			.method (&Item::GetProto, "GetProto", "getProto", "getproto", NULL)
			.method(&Item::GetOwner, "getOwner", "GetOwner", "getowner", NULL)
			.method(&Item::SetOwner, "SetOwner", "setOwner", "setowner", NULL)
			.method(&Item::SetStackCount, "SetStackCount", "setStackCount", "setstackcount", NULL)
			.method(&Item::GetStackCount, "getStackCount", "GetStackCount", "getstackcount", NULL)
			.method(&Item::ModStackCount, "ModStackCount", "modStackCount", "modstackcount", NULL)
			.method(&Item::GetDuration, "GetDuration", "getDuration", "getduration", NULL)
			.method(&Item::SetDuration, "SetDuration", "setDuration", "setduration", NULL)
			.method(&Item::GetCharges, "GetCharges", "getCharges", "getcharges", NULL)
			.method(&Item::SetCharges, "SetCharges", "setCharges", "setcharges", NULL)
			.method(&Item::ModCharges, "ModCharges", "modCharges", "modcharges", NULL)
			.method(&Item::SoulBind, "SoulBind", "soulBind", "soulbind", NULL)
			.method(&Item::IsSoulbound, "IsSoulbound", "IsSoulBound", "isSoulBound", "issoulbound", NULL)
			.method(&Item::AccountBind, "AccountBind", "accountBind", "accountbind", NULL)
			.method(&Item::IsAccountbound, "IsAccountbound", "IsAccountBound", "isAccountBound", "isaccountbound", NULL)
			.method(&Item::MakeConjured, "MakeConjured", "makeConjured", "makeconjured", NULL)
			.method(&Item::IsConjured, "IsConjured", "isConjured", "isconjured", NULL)
			.method(&Item::Lock, "Lock", "lock", NULL)
			.method(&Item::UnLock, "UnLock", "unlock", "Unlock", NULL)
			.method(&Item::Wrap, "Wrap", "wrap", NULL)
			.method(&Item::UnWrap, "UnWrap", "Unwrap", "unwrap", NULL)
			.method(&Item::ClearFlags, "ClearFlags", "clearFlags", "clearflags", NULL)

			.method( &Item::GetItemRandomPropertyId, "GetItemRandomPropertyID", "getItemRandomPropertyID", "getitemrandompropertyid", NULL)
			.method( &Item::GetItemRandomSuffixFactor, "GetItemRandomSuffixFactor", "getItemRandomSuffixFactor", "getitemrandomsuffixfactor", NULL)
			.method( &Item::SetItemRandomPropertyId, "SetItemRandomPropertyId", "setItemRandomPropertyId", "setitemrandompropertyid", NULL)
			.method( &Item::SetItemRandomSuffixFactor, "SetItemRandomSuffixFactor", "setItemRandomSuffixFactor", "setitemrandomsuffixfactor", NULL)
			.method( &Item::SetRandomSuffix, "SetRandomSuffix", "setRandomSuffix", "setrandomsuffix", NULL)
			.method( &Item::SetDurability, "SetDurability", "setDurability", "setdurability", NULL)
			.method( &Item::SetDurabilityMax, "SetDurabilityMax", "setDurabilityMax", "setdurabilitymax", NULL)
			.method( &Item::GetDurability, "GetDurability", "getDurability", "getdurability", NULL)
			.method( &Item::GetDurabilityMax, "GetDurabilityMax", "getDurabilityMax", "getdurabilitymax", NULL)
			.method( &Item::SetDurabilityToMax, "SetDurabilityToMax", "setDurabilityToMax", "setdurabilitytomax", NULL)
			.method( &Item::GetEnchantmentId, "GetEnchantmentID", "getEnchantmentID", "getenchantmentid", NULL)
			.method( &Item::SetEnchantmentId, "SetEnchantmentID", "setEnchantmentID", "setenchantmentid", NULL)
			.method( &Item::GetEnchantmentDuration, "GetEnchantmentDuration", "getEnchantmentDuration", "getenchantmentduration", NULL)
			.method( &Item::SetEnchantmentDuration, "SetEnchantmentDuration", "setEnchantmentDuration", "setenchantmentduration", NULL)
			.method( &Item::GetEnchantmentCharges, "GetEnchantmentCharges", "getEnchantmentCharges", "getenchantmentcharges", NULL)
			.method( &Item::SetEnchantmentCharges, "SetEnchantmentCharges", "setEnchantmentCharges", "setenchantmentcharges", NULL)

			.method( &Item::IsContainer, "IsContainer", "isContainer", "iscontainer", NULL)
			.method( &Item::GetChargesLeft, "GetChargesLeft", "getChargesLeft", "getchargesleft", NULL)

			.method( &Item::AddEnchantment, "AddEnchantment", "addEnchantment", "addenchantment", NULL)
			.method( &Item::RemoveEnchantment, "RemoveEnchantment", "removeEnchantment", "removeenchantment", NULL)
			.method( &Item::HasEnchantment, "HasEnchantment", "hasEnchantment", "hasenchantment", NULL)
			.method( &Item::HasEnchantmentOnSlot, "HasEnchantmentOnSlot", "hasEnchantmentOnSlot", "hasenchantmentonslot", NULL)
			.method( &Item::ModifyEnchantmentTime, "ModifyEnchantmentTime", "modifyEnchantmentTime", "modifyenchantmenttime", NULL)
			.method( &Item::RemoveAllEnchantments, "RemoveAllEnchantments", "removeAllEnchantments", "removeallenchantments", NULL)
			.method( &Item::IsAmmoBag, "IsAmmoBag", "isAmmoBag", "isammobag", NULL)
			.method( &Item::RemoveFromWorld, "RemoveFromWorld", "removeFromWorld", "removefromworld", NULL)
			.method( &Item::HasEnchantments, "HasEnchantments", "hasEnchantments", "hasenchantments", NULL)
			.method( &Item::SaveToDB, "SaveToDB", "saveToDB", "savetodb",  NULL);
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