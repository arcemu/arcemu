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
local SPELL = LCF.SpellMethods
local AURA = LCF.AuraMethods
assert(SPELL)
assert(AURA)
local function alias(LHAname, LBname)
   SPELL[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end
local function aliaslower(LHAname)
   SPELL[LHAname] = function(self, ...)
      return self[string.lower(LHAname)](self, ...);
   end
end

aliaslower("Cancel")

aliaslower("Cast")

aliaslower("Finish")

function SPELL:GetCaster()
   if (self.i_caster) then return self.i_caster;
   elseif (self.g_caster) then return self.g_caster;
   elseif (self.p_caster) then return self.p_caster;
   elseif (self.u_caster) then return self.u_caster;
   else return self.m_caster; end
end

function SPELL:GetEntry() return self:getproto().Id; end

alias("GetSpellState", "getstate")

alias("GetSpellType", "gettype")

function SPELL:GetVar(var, subindex)
   local prop = var
   if (subindex ~= nil) then prop = prop..subindex; end
   return self:getproto()[prop]
end

aliaslower("IsAspect")

aliaslower("IsDuelSpell")

aliaslower("IsInvisibilitySpell")

aliaslower("IsSeal")

aliaslower("IsStealthSpell")

function SPELL:SetVar(var, subindex, value)
   local prop = var
   if (value ~= nil) then prop = prop..subindex; else value = subindex; end
   self:getproto()[prop] = value
end

function SPELL:GetObjectType() return "Spell"; end

function AURA:GetSpellId() return self:GetSpellID(); end

function AURA:GetVar(var, subindex)
   local prop = var
   if (subindex ~= nil) then prop = prop..subindex; end
   return self:getproto()[prop]
end

function AURA:SetVar(var, subindex, value)
   local prop = var
   if (value ~= nil) then prop = prop..subindex; else value = subindex; end
   self:getproto()[prop] = value
end

function AURA:GetObjectType() return "Aura"; end