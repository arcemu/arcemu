--[[
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
 ]]

assert( include("LCF.lua") )
local UNIT = LCF.UnitMethods
assert(UNIT)
local function alias(LHAname, LBname)
   UNIT[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end

function UNIT:FullCastSpell( id)
	self:CastSpell(self, id, false)
end
function UNIT:FullCastSpellOnTarget( target, id)
	self:CastSpell( target, id, false)
end
function UNIT:CastSpell(id)
	self:CastSpell(self, id, true)
end
function UNIT:CastSpellOnTarget(target, id)
	self:CastSpell(target, id, true)
end
function UNIT:SetChannelTarget(target)
	local guid = nil
	if(target) then	
		guid = target:GetGUID()
	end
	self:SetUInt64Value(LCF.UNIT_FIELD_CHANNEL_OBJECT,guid)
end
function UNIT:SetChannelSpell( spell)
	self:SetUInt32Value(LCF.UNIT_CHANNEL_SPELL, spell)
end

function UNIT:GetChannelTarget()
	local guid = self:GetUInt64Value(LCF.UNIT_FIELD_CHANNEL_OBJECT)
	print( tostring(guid) )
	if(guid) then
		return MapMgr:GetObject(guid)
	end
	return nil
end
function UNIT:GetChannelSpell()
	return self:GetUInt32Value( LCF.UNIT_CHANNEL_SPELL )
end
function UNIT:ChannelSpell( spell, target)
	self:SetChannelTarget( target)
	self:SetChannelSpell( spell)
end
function UNIT:StopChannel()
	self:SetChannelTarget( nil)
	self:SetChannelSpell( 0 )
end
function UNIT:SetCreatedBy(creator)
	local guid = 0
	if(creator) then
		guid = creator:GetGUID()
	end
	self:SetUInt64Value(LCF.UNIT_FIELD_CREATEDBY, guid)
end
function UNIT:SetSummonedBy( summoner)
	local guid = 0
	if(summoner) then
		guid = summoner:GetGUID()
	end
	self:SetUInt64Value( LCF.UNIT_FIELD_SUMMONEDBY, summoner:GetGUID() )
end
function UNIT:GetCreatedBy()
	return self:GetObject( self:GetUInt64Value(LCF.UNIT_FIELD_CREATEDBY) )
end
function UNIT:GetSummonedBy()
	return self:GetObject( self:GetUInt64Value(LCF.UNIT_FIELD_SUMMONEDBY) )
end

function UNIT:CastSpellAoFID(x,y,z,id)
   self:CastSpellAoF(x, y, z, dbcSpell.LookupEntry(id), true)
end

alias("CastSpellAoE", "CastSpellAoF")

function UNIT:QuickChannel(id, tar)
   self:CastSpell(tar, id, true)
   self:ChannelSpell(id, tar)
end

alias("DeMorph", "Demorph")

alias("EventChat", "SendChatMessage")

function UNIT:FullCastSpellAoF(x,y,z,id) self:CastSpellAoF(x, y, z, dbcSpell.LookupEntry(id), false); end

alias("FullCastSpellAoE", "FullCastSpellAoF")

function UNIT:GetCreatureNearestCoords(x,y,z,e) return MapMgr:GetInterface():GetCreatureNearestCoords(x,y,z,e); end

function UNIT:GetGameObjectNearestCoords(x,y,z,e) return MapMgr:GetInterface():getGameObjectNearestCoords(x,y,z,e); end

alias("GetDisplay", "GetDisplayId")

alias("GetGender", "getGender")

alias("GetLevel", "getLevel")

function UNIT:GetMana() return self:GetPower(0); end

function UNIT:GetMaxMana() return self:GetMaxPower(0); end

alias("GetNativeDisplay", "GetNativeDisplayId")

alias("IsAlive", "isAlive")

function UNIT:IsInCombat() return self.CombatStatus:IsInCombat(); end

alias("SetAttackTimer", "setAttackTimer")

alias("SetGender", "setGender")

alias("SetLevel", "setLevel")

function UNIT:SetMana(value) self:SetPower(0, value); end

function UNIT:SetMaxMana(value) self:SetMaxPower(0, value); end

alias("SetModel", "SetDisplayId")

function UNIT:SetNPCFlags(value) self:SetUInt32Value(LCF.UNIT_NPC_FLAGS, value); end

function UNIT:SetZoneWeather(zoneid, _type, density) return SetWeather("zone", zoneid, _type, density); end

function UNIT:SpawnCreature(entry, x, y, z, o, fac, duration, e1, e2, e3, phase, save)
   return EasySpawn(1, entry, x, y, z, o, fac, duration, e1, e2, e3, save, phase)
end

function UNIT:SpawnGameObject(entry, x, y, z, o, duration, scale, phase, save)
   return EasySpawn(2, entry, x, y, z, o, scale, duration, nil, nil, nil, save, phase)
end

function UNIT:CreateGuardian(entry, duration, angle, lvl)
   return self:create_guardian(entry, duration, angle, lvl, 0)
end

alias("Emote", "EventAddEmote")

local oldSendChatMessage = getregistry("Unit").SendChatMessage;
getregistry("Unit").SendChatMessage = function(self, _type, lang, msg, delay)
   delay = delay or 0;
   oldSendChatMessage(self, _type, lang, msg, delay)
end

local oldGetPower = getregistry("Unit").GetPower;
getregistry("Unit").GetPower = function(self, powtype)
   powtype = powtype or self:GetPowerType();
   oldGetPower(self, powtype)
end

function UNIT:GetPowerPct(idx) return (self:GetPower(idx) / self:GetMaxPower(idx)) * 100; end

local oldGetMaxPower = getregistry("Unit").GetMaxPower;
getregistry("Unit").GetMaxPower = function(self, powtype)
   powtype = powtype or self:GetPowerType();
   oldGetMaxPower(self, powtype)
end

local oldSetPower = getregistry("Unit").SetPower;
getregistry("Unit").SetPower = function(self, powtype)
   powtype = powtype or self:GetPowerType();
   oldSetPower(self, powtype)
end

function UNIT:SetPowerPct(pct, _type)
   self:SetPower(_type, (pct / 100) * self:GetMaxPower(_type));
end

local oldSetMaxPower = getregistry("Unit").SetMaxPower;
getregistry("Unit").SetMaxPower = function(self, powtype)
   powtype = powtype or self:GetPowerType();
   oldSetMaxPower(self, powtype)
end

function UNIT:GetAreaId()
   return MapMgr:GetAreaID(self:GetX(), self:GetY());
end

function UNIT:Actual()
   if (self:IsCreature()) then 
      return TO_CREATURE(self);
   else
      return TO_PLAYER(self);
   end
end

alias("AddAuraObject", "AddAura")

function UNIT:AddAuraByID(spellid, duration, temp)
   local aura = Aura(dbcSpell:LookupEntry(spellid), duration, self, self, temp)
   self:AddAura(aura)
   return true
end

--------------------------------- RE-DEFINED 'alias' here------------------------------
alias = function(_func, ...)
	for _,label in ipairs(arg) do
		UNIT[label] = _func
	end
end

alias( function(self, id, ...) for _,target in ipairs(arg) do self:CastSpell(target, id, false) end end, "FullCastSpellOnTargets", "fullCastSpellOnTargets", "fullcastspellontargets")
alias( function(self, id, ...) for _,target in ipairs(arg) do self:CastSpell(target, id, true) end end, "CastSpellOnTargets", "castSpellOnTargets", "castspellontargets")
