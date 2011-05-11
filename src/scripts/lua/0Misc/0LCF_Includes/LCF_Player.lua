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

local PLAYER = LCF.PlayerMethods
assert(PLAYER)
local function alias(LHAname, LBname)
   PLAYER[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end

function PLAYER:GetStrength()
	return self:GetUInt32Value(LCF.UNIT_FIELD_STAT0)
end
function PLAYER:GetAgility()
	return self:GetUInt32Value(LCF.UNIT_FIELD_STAT1)
end
function PLAYER:GetStamina()
	return self:GetUInt32Value(LCF.UNIT_FIELD_STAT2)
end
function PLAYER:GetIntellect()
	return self:GetUInt32Value(LCF.UNIT_FIELD_STAT3)
end
function PLAYER:GetSpirit()
	return self:GetUInt32Value(LCF.UNIT_FIELD_STAT4)
end
function PLAYER:IsAlliance()
	return self:GetTeam() == 0
end
function PLAYER:IsHorde()
	return self:GetTeam() ~= 0
end 
function PLAYER:IsHeroic()
	local dung = self:GetDungeonDifficulty()
	if(self:IsInDungeon() ) then
		return dung == 1
	elseif(self:IsInRaid() ) then
		return (dung == 3 or dung == 4)
	end
	return false
end
function PLAYER:SpawnLocalCreature(entry,faction,phase, duration)
	local x,y,z,o = self:GetLocation()
	local crc = MapMgr:GetInterface():SpawnCreature(entry, x,y,z,o, true, true, 0, 0, phase)
	if(faction > 0) then
		crc:SetFaction(faction)
	end
	if(duration) then
		crc:Despawn(duration, 0)
	end
end
function PLAYER:SpawnLocalGameObject(go_entry, phase, duration)
	local x,y,z,o = self:GetLocation()
	local go = MapMgr:GetInterface():SpawnGameObject(entry, x, y, z, o, true, 0, 0, phase)
	if(duration) then
		go:Despawn(duration ,0)
	end
end
function PLAYER:MoveToUnit(target)
	local x,y,z = target:GetLocation()
	self:MoveTo(x,y,z,self:GetO())
end

function PLAYER:IsCasting()
	return player:GetCurrentSpell() ~= nil
end
function PLAYER:GetLocalCreature(entry)
	local x,y,z = self:GetLocation()
	local crc = MapMgr:GetInterface():GetCreatureNearestCoords(x,y,z,entry)
	return crc
end
function PLAYER:GetLocalGameObject(entry)
	local x,y,z = self:GetLocation()
	local go = MapMgr:GetInterface():GetGameObjectNearestCoords(x,y,z,entry)
	return go
end

function PLAYER:AddSkill(skill,cur,_max)
   self:AddSkillLine(skill, cur, _max)
   self:UpdateMaxSkillLineCounts()
end

function PLAYER:CanUseCommand(char)
   return (string.find(self:GetSession():GetPermissions(), char) ~= nil)
end

alias("ClearAllCooldowns", "ResetAllCooldowns")

alias("DealGoldCost", "TakeGold")

alias("DealGoldMerit", "GiveGold")

alias("FlagFFA", "SetFFAPvPFlag")

alias("FlagPvP", "SetPVPFlag")

function PLAYER:GetAccountName() return self:GetSession():GetAccountName(); end

alias("GetArenaPoints", "GetArenaCurrency")

alias("GetCoinage", "GetGold")

alias("GetCurrentSkill", "GetSkillLineCurrent")

function PLAYER:GetCurrentSpellId()
   if (self:GetCurrentSpell() == nil) then return nil; end
   return self:GetCurrentSpell().proto.Id;
end

function PLAYER:GetEquippedItemBySlot(slot) return self:GetItemInterface():GetInventorySlotByID(slot); end

function PLAYER:GetGmRank() return self:GetSession():GetPermissions(); end

alias("GetGuildId", "GetGuildID")

function PLAYER:GetInventoryItem(...) return self:GetItemInterface():GetInventoryItem(...); end

function PLAYER:GetInventoryItemById(id)
   local s = self:GetItemInterface():GetInventorySlotById(id)
   return self:GetItemInterface():GetInventoryItem(s)
end

function PLAYER:GetItemCount(...) return self:GetItemInterface():GetItemCount(...); end

alias("GetMaxSkill", "GetSkillLineMax")

function PLAYER:GetPlayerClass()
   local c = self:getClass()
   if (c == 1) then
      return "Warrior";
   elseif (c == 2) then
      return "Paladin";
   elseif (c == 3) then
      return "Hunter";
   elseif (c == 4) then
      return "Rogue";
   elseif (c == 5) then
      return "Priest";
   elseif (c == 6) then
      return "Death Knight";
   elseif (c == 7) then
      return "Shaman";
   elseif (c == 8) then
      return "Mage";
   elseif (c == 9) then
      return "Warlock";
   elseif (c == 11) then
      return "Druid";
   else
      return "Unknown";
   end
end

alias("GetPlayerLevel", "getLevel")

function PLAYER:GetPlayerMovementFlags() return self:GetSession():GetMovementInfo().flags; end

alias("GetPlayerRace", "getRace")

function PLAYER:GetUnitSelection() return MapMgr:GetObject(self:GetSelection()); end

alias("GetTaxi", "GetTaxiPath")

alias("GetTotalHonor", "GetHonorCurrency")

function PLAYER:HasItem(id) return (self:GetItemInterface():GetItemCount(id, false) > 0); end

alias("HasSkill", "HasSkillLine")

alias("IsFFAFlagged", "IsFFAPvPFlagged")

function PLAYER:IsGm() return self:GetSession():IsGM(); end

alias("IsGroupedWith", "IsGroupMember")

alias("IsOnTaxi", "GetTaxiState")

alias("IsPlayerAttacking", "IsAttacking")

alias("IsPvPFlagged", "IsPVPFlagged")

alias("KickPlayer", "Kick")

alias("LearnSpell", "addSpell")

--disgusting function but alas, it must be done.
function PLAYER:LifeTimeKills(kills, check)
   local killscheck = self:GetUInt32Value(1228)
   if (check == "add") then self:SetUInt32Value(1228, killscheck+kills);
   elseif (check == "del") then self:SetUInt32Value(1228, killscheck-kills);
   elseif (check == "set") then self:SetUInt32Value(1228, kills);
   else return killscheck; end
end

alias("PlaySoundToPlayer", "PlaySound")

alias("PlayerSendChatMessage", "SendChatMessage")

function PLAYER:RemoveItem(...) return self:GetItemInterface():removeItemAmount(...); end

alias("RemoveSkill", "RemoveSkillLine")

alias("ResetAllTalents", "ResetTalents")

alias("SendPacketToPlayer", "SendPacket")

function PLAYER:QuickSetKnownTitle(id) return self:SetKnownTitle(id, true); end

alias("SetPlayerAtWar", "SetAtWar")

alias("SetPlayerLevel", "setLevel")

function PLAYER:SetPlayerLock(on)
   if (on) then
      --todo: implement this
      --self.m_pacified = true
      self:SetFlag(LCF.UNIT_FIELD_FLAGS, bit_or(LCF.UNIT_FLAG_PACIFIED,LCF.UNIT_FLAG_SILENCED))
      local p = LuaPacket(345, 9)
      p:WriteWoWGuid(self:GetNewGuid()); p:WriteUInt8(0);
      self:SendPacket(p)
   else
      --todo: implement this
      --self.m_pacified = false
      self:RemoveFlag(LCF.UNIT_FIELD_FLAGS, bit_or(LCF.UNIT_FLAG_PACIFIED,LCF.UNIT_FLAG_SILENCED))
      p:WriteWoWGuid(self:GetNewGuid()); p:WriteUInt8(1);
      self:SendPacket(p)
   end
end

function PLAYER:SetPlayerWeather(_type, density) SetWeather("object", self, _type, density); end

function PLAYER:QuickStartTaxi(taxipath, mountid) self:StartTaxi(taxipath, mountid, 0); end

alias("UnlearnSpell", "removeSpell")

function PLAYER:UnsetKnownTitle(id) return self:SetKnownTitle(id, false); end

function PLAYER:GetObjectType() return "Player"; end

alias("SavePlayer", "SaveToDB")

function PLAYER:SendBroadcastMessage(message) 
   local packet = sChatHandler:FillSystemMessageData(message)
   self:SendPacket(packet)
end

function PLAYER:SendAreaTriggerMessage(message) 
   local data = LuaPacket(696, 6 + string.len(message))
   data:WriteUInt32(0); data:WriteString(msg); data:WriteUInt8(0);
   self:SendPacket(data);
end

alias("RemovePvPFlag", "RemovePVPFlag")