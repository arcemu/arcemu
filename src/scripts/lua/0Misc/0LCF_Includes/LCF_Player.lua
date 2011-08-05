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

local oldGetInventoryItem = getregistry("Player").GetInventoryItem;
getregistry("Player").GetInventoryItem = function(self, contslot, slot)
   if (slot == nil) then 
      return oldGetInventoryItem(self, -1, contslot);
   else 
      return oldGetInventoryItem(self, contslot, slot);
   end
end

function PLAYER:GetInventoryItemById(id)
   local s = self:GetItemInterface():GetInventorySlotByID(id)
   return self:GetItemInterface():GetInventoryItem(-1, s)
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

function PLAYER:GetUnitSelection() 
   local sel = self:GetSelection();
   if (sel == nil) then return nil; end
   return TO_UNIT(MapMgr:GetObject(sel)):Actual();
end

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
   local killscheck = self:GetUInt32Value(LCF.PLAYER_FIELD_LIFETIME_HONORBALE_KILLS)
   if (check == "add") then self:SetUInt32Value(LCF.PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, killscheck+kills);
   elseif (check == "del") then self:SetUInt32Value(LCF.PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, killscheck-kills);
   elseif (check == "set") then self:SetUInt32Value(LCF.PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, kills);
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

function PLAYER:QuickStartTaxi(taxipath, mountid) self:TaxiStart(taxipath, mountid, 0); end
function PLAYER:StartTaxi(taxipath, mountid) self:TaxiStart(taxipath, mountid, 0); end

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
   data:WriteUInt32(0); data:WriteString(message); data:WriteUInt8(0);
   self:SendPacket(data);
end

alias("RemovePvPFlag", "RemovePVPFlag")

function PLAYER:AddItem(id, amt)
   return self:GetItemInterface():AddItemByID(id, amt, 0);
end

function PLAYER:AddArenaPoints(points)
   if (points < 0) then return; end
   self.m_arenaPoints = self.m_arenaPoints + points;
   self:RecalculateHonor();
end

function PLAYER:RemoveArenaPoints(points)
   local newPoints = self.m_arenaPoints - points;
   if (newPoints >= 0) then
      self.m_arenaPoints = newPoints;
      self:RecalculateHonor();
   end
end

function PLAYER:AddGroupMember(plr, subgrp)
   subgrp = subgrp or -1
   if (self:GetGroup()) then
      self:GetGroup():AddMember(plr:GetPlayerInfo(), subgrp)
   end
end

function PLAYER:AddGuildMember(id, rank)
   rank = rank or -1;
   local guild = objmgr:GetGuild(id)
   if (guild) then
      guild:AddGuildMember(self:GetPlayerInfo(), nil, rank)
   end
end

function PLAYER:AddLifetimeKills(points)
   if (points > 0) then
      self.m_killsLifetime = self.m_killsLifetime + points
   end
end

function PLAYER:AdvanceQuestObjective(questid, objective)
   local qle = self:GetQuestLogForEntry(questid)
   if (qle ~= nil) then
      qle:SetMobCount(objective, qle:GetMobCount(objective) + 1)
      qle:SendUpdateAddKill()
      if (qle:CanBeFinished()) then
         qle:SendQuestComplete()
      end
      
      qle:UpdatePlayerFields()
   end
end

function PLAYER:ChangeGuildMaster(newMaster)
   local guild = self:GetGuild()
   if (guild) then
      guild:ChangeGuildMaster(newMaster:GetPlayerInfo(), self:GetSession())
   end
end

function PLAYER:DemoteGuildMember(target)
   local guild = self:GetGuild()
   if (guild) then
      guild:DemoteGuildMember(target:GetPlayerInfo(), self:GetSession())
   end
end

function PLAYER:DisbandGuild(guildID)
   guildID = guildID or -1
   local guild
   if (guildID >= 0) then
      guild = objmgr:GetGuild(guildID)
   else
      guild = self:GetGuild()
   end
   if (guild) then
      guild:Disband()
   end
end

function PLAYER:ExpandToRaid()
   local group = self:GetGroup()
   if (group) then
      group:ExpandToRaid()
   end
end

function PLAYER:GetGroupLeader()
   local group = self:GetGroup()
   if (group) then
      return group:GetLeader().loggedInPlayer
   end
   return nil
end

function PLAYER:GetGroupType()
   local group = self:GetGroup()
   if (group) then
      return group:GetGroupType()
   end
   return nil
end

function PLAYER:GetGuildMemberCount()
   local guild = self:GetGuild()
   if (guild) then
      return guild:GetNumMembers()
   end
   return nil
end

function PLAYER:GetGuildMotd(guildID)
   guildID = guildID or -1
   local guild
   if (guildID >= 0) then
      guild = objmgr:GetGuild(guildID)
   else
      guild = self:GetGuild()
   end
   if (guild) then
      return guild:GetMOTD()
   end
   return nil
end

function PLAYER:GetGuildName()
   local guild = self:GetGuild()
   if (guild) then
      return guild:GetGuildName()
   end
   return nil
end

function PLAYER:GuildBankDepositMoney(amount)
   local guild = self:GetGuild()
   if (guild) then
      guild:DepositMoney(self:GetSession(), amount)
   end
end

function PLAYER:GuildBankWithdrawMoney(amount)
   local guild = self:GetGuild()
   if (guild) then
      guild:WithdrawMoney(self:GetSession(), amount)
   end
end

function PLAYER:PromoteGuildMember(target)
   local guild = self:GetGuild()
   if (guild) then
      guild:PromoteGuildMember(target:GetPlayerInfo(), self:GetSession())
   end
end

function PLAYER:RemoveGuildMember(target)
   local guild = self:GetGuild()
   if (guild) then
      guild:RemoveGuildMember(target:GetPlayerInfo(), self:GetSession())
   end
end

function PLAYER:SendGuildChatMessage(message, isOfficerChat)
   local guild = self:GetGuild()
   if (guild) then
      guild:RemoveGuildMember(target:GetPlayerInfo(), self:GetSession())
      if (isOfficerChat) then
         guild:OfficerChat(message, self:GetSession(), 0)
      else
         guild:GuildChat(message, self:GetSession(), 0)
      end
   end
end

function PLAYER:SendGuildInvite(target, sendErrors)
   if (sendErrors == nil) then sendErrors = true; end
   local GUILD_CREATE_S = 0
   local GUILD_INVITE_S = 1
   
   --error codes that may be returned
   --note: there is no longer a faction check in this, you can do it yourself if necessary.
   local GUILD_PLAYER_NOT_IN_GUILD = 9 --(sender not in guild)
   local ALREADY_IN_GUILD = 3 --(target already in guild)
   local ALREADY_INVITED_TO_GUILD = 5 --(target already invited to guild)
   
   local GUILD_U_HAVE_INVITED = 0
   local SMSG_GUILD_INVITE = 131
   local inviteeName = self:GetName()
   local guild = self:GetGuild()
   
   if (not guild) then
      if (sendErrors) then Guild.SendGuildCommandResult(self:GetSession(), GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD); end
      return GUILD_PLAYER_NOT_IN_GUILD;
   elseif (target:GetGuildId()) then
      if (sendErrors) then Guild.SendGuildCommandResult(self:GetSession(), GUILD_INVITE_S, target:GetName(), ALREADY_IN_GUILD); end
      return ALREADY_IN_GUILD;
   elseif (target:GetGuildInvitersGuid() ~= 0) then
      if (sendErrors) then Guild.SendGuildCommandResult(self:GetSession(), GUILD_INVITE_S, target:GetName(), ALREADY_INVITED_TO_GUILD); end
      return ALREADY_INVITED_TO_GUILD;
   else
      Guild.SendGuildCommandResult(self:GetSession(), GUILD_INVITE_S, inviteeName, GUILD_U_HAVE_INVITED);
      local data = LuaPacket(SMSG_GUILD_INVITE, 100);
      data:WriteString(inviteeName)
      data:WriteString(guild:GetGuildName());
      target:SendPacket(data);
      target:SetGuildInvitersGuid(self:GetLowGUID());
      return true;
   end
end

function PLAYER:SendGuildLog()
   local guild = self:GetGuild()
   if (guild) then
      guild:SendGuildLog(self:GetSession())
   end
end

function PLAYER:SendPacketToGuild(packet, guildID)
   guildID = guildID or -1
   local guild
   if (guildID >= 0) then
      guild = objmgr:GetGuild(guildID)
   else
      guild = self:GetGuild()
   end
   if (guild) then
      guild:SendPacket(packet)
   end
end

function PLAYER:SendPacketToGroup(packet)
   local group = self:GetGroup()
   if (group) then
      group:SendPacketToAll(packet)
   end
end

function PLAYER:SetGroupLeader(newLeader, bSilent)
   local group = self:GetGroup()
   if (group) then
      group:SetGroupLeader(newLeader, bSilent)
   end
end

function PLAYER:SetGuildInformation(gi)
   local guild = self:GetGuild()
   if (guild) then
      guild:SetGuildInformation(gi, self:GetSession())
   end
end

function PLAYER:SetGuildMotd(gi)
   local guild = self:GetGuild()
   if (guild) then
      guild:SetMOTD(gi, self:GetSession())
   end
end

function PLAYER:SetOfficerNote(target, note)
   local guild = self:GetGuild()
   if (guild) then
      guild:SetOfficerNote(target:GetPlayerInfo(), note, self:GetSession())
   end
end