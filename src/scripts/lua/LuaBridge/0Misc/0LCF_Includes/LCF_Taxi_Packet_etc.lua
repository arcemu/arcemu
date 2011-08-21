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
local function alias(LHAname, LBname)
   LCF.PacketMethods[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end
 
-----TAXI-----
function LCF.TaxiMethods:QuickAddPathNode(map, x, y, z, idx)
   idx = idx or self:GetNodeCount();
   local n = TaxiPathNode(map, x, y, z)
   self:AddPathNode(idx, n)
end

function LCF.TaxiMethods:GetId() return self:GetID(); end

function LCF.TaxiMethods:GetObjectType() return "TaxiPath"; end

-----PACKET-----
alias("GetOpcode", "opcode")

alias("GetSize", "size")

function LCF.PacketMethods:WriteUnitWoWGuid(unit) self:writeWoWGuid(unit:GetNewGuid()); end

function LCF.PacketMethods:WriteUnitGuid(unit) self:WriteGUID(unit:GetGUID()); end

function LCF.PacketMethods:GetObjectType() return "Packet"; end

-----QRESULT------
function LCF.QResultMethods:GetColumnCount() return self:GetFieldCount(); end