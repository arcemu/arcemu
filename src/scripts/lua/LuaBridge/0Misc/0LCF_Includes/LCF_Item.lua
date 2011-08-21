--[[
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
 *]]

 assert( include("LCF.lua") )
local ITEM = LCF.ItemMethods
assert(ITEM)
local function alias(LHAname, LBname)
   ITEM[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end

function ITEM:GetBuyPrice() return self:GetProto().BuyPrice; end

alias("GetEntryId", "GetEntry")

function ITEM:GetEquippedSlot()
   local o = self:GetOwner()
   if (not o or not o:GetItemInterface()) then return 0; end
   return (o:GetItemInterface():GetInventorySlotByID(self:GetEntry()))
end

function ITEM:GetItemLevel() return self:GetProto().ItemLevel; end

alias("GetMaxDurability", "GetDurabilityMax")

function ITEM:GetName() return self:GetProto().Name1; end

function ITEM:GetRequiredLevel() return self:GetProto().RequiredLevel; end

function ITEM:GetSellPrice() return self:GetProto().SellPrice; end

alias("Remove", "RemoveFromWorld")

alias("RepairItem", "SetDurabilityToMax")

function ITEM:GetObjectType() return "Item"; end

--[[
function ITEM:AddEnchantmentByID(uEntry, uDuration, bRemoveAtLogout)
   self:AddEnchantment(dbcEnchant.LookupEntry(uEntry), uDuration, false, true, bRemoveAtLogout, 0, 0)
end
]]

--returns the number of items with id 'itemid' in the container
function ITEM:GetContainerItemCount(itemid)
   if (not self:IsContainer()) then return 0; end
   local pCont = TO_CONTAINER(self);
	local TotalSlots = pCont:GetNumSlots();
   local cnt = 0;
	for i=0, TotalSlots do
		local item = pCont:GetItem(i);
		if (item) then
			if (item:GetEntry() == itemid) then
				cnt = cnt + item:GetStackCount(); 
			end
		end
	end
	return cnt;
end

local oldGetItemLink = getregistry("Item").GetItemLink;
getregistry("Item").GetItemLink = function(self, lang)
   lang = lang or LCF.LANG_UNIVERSAL;
   oldGetItemLink(self, lang)
end

function ITEM:GetSpellId(idx)
   return self:GetProto():GetItemSpell(idx).Id;
end

function ITEM:GetSpellTrigger(idx)
   return self:GetProto():GetItemSpell(idx).Trigger;
end