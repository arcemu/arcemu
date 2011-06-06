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
 
local GAMEOBJECT = LCF.GOMethods
assert(GAMEOBJECT)
local function alias(LHAname, LBname)
   GAMEOBJECT[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end

function GAMEOBJECT:SetUnclickable()
	self:SetUInt32Value(LCF.GAMEOBJECT_FLAGS,0x1)
end
function GAMEOBJECT:SetClickable()
	self:SetUInt32Value(LCF.GAMEOBJECT_FLAGS,0x0)
end
function GAMEOBJECT:IsUnclickable()
	local flags = self:SetUInt32Value(LCF.GAMEOBJECT_FLAGS,0x1)
	if(bit_and(flags,0x1) ) then
		return true
	end
	return false
end
function GAMEOBJECT:IsClickable()
	local flags = self:SetUInt32Value(LCF.GAMEOBJECT_FLAGS,0x1)
	if(bit_and(flags,0x1) == 0 or bit_and(flags, 0x2) ~= 0) then
		return true
	end
	return false
end
--[[function GAMEOBJECT:Respawn()
	self:SetPosition(self:GetX(),self:GetY(),self:GetZ(),self:GetO())
end]]
function GAMEOBJECT:Close()
	self:SetByte(LCF.GAMEOBJECT_BYTES_1,0,1)
end
function GAMEOBJECT:IsOpen()
	return (self:GetByte(LCF.GAMEOBJECT_BYTES_1,0) == 0)
end
function GAMEOBJECT:IsClosed()
	return (self:GetByte(LCF.GAMEOBJECT_BYTES_1,0) == 1)
end
function GAMEOBJECT:Open()
	self:SetByte(LCF.GAMEOBJECT_BYTES_1,0,0)
end
function GAMEOBJECT:RegisterAIUpdateEvent( interval)
	self:GetScript():RegisterAIUpdateEvent(interval)
end
function GAMEOBJECT:ModifyAIUpdateEvent( nInterval)
	self:GetScript():ModifyAIUpdateEvent(nInterval)
end
function GAMEOBJECT:RemoveAIUpdateEvent()
	self:GetScript():RemoveAIUpdateEvent()
end
function GAMEOBJECT:GetName()
	return self:GetInfo().Name
end
function GAMEOBJECT:Type()
	return self:GetInfo().Type
end

function GAMEOBJECT:Activate()
   if (self:IsOpen()) then self:Close(); else self:Open(); end
end

function GAMEOBJECT:ChangeScale(scale, now)
   self:SetScale(scale)
   if (now) then
      self:Despawn(1, 1)
   end
   return true
end

function GAMEOBJECT:CreateLuaEvent(func, delay, repeats)
   --Not associated with the game object, but this is the most compatible method.
   CreateLuaEvent(func, delay, repeats)
end

function GAMEOBJECT:GetCreatureNearestCoords(x,y,z,e) return MapMgr:GetInterface():GetCreatureNearestCoords(x,y,z,e); end

function GAMEOBJECT:GetGameObjectNearestCoords(x,y,z,e) return MapMgr:GetInterface():getGameObjectNearestCoords(x,y,z,e); end

function GAMEOBJECT:GetObjectType() return "GameObject"; end

alias("IsActive", "IsClosed")

alias("IsInBack", "IsBehind")

function GAMEOBJECT:PlaySoundToSet(sound_entry)
   local data = LuaPacket(722, 4);
	data:WriteUInt32(sound_entry);
	
   self:SendMessageToSet(data, true);
end

function GAMEOBJECT:SetZoneWeather(zoneid, _type, density) return SetWeather("zone", zoneid, _type, density); end

function GAMEOBJECT:SpawnCreature(entry, x, y, z, o, fac, duration, e1, e2, e3, phase, save)
   return EasySpawn(1, entry, x, y, z, o, fac, duration, e1, e2, e3, save, phase)
end

function GAMEOBJECT:SpawnGameObject(entry, x, y, z, o, duration, scale, phase, save)
   return EasySpawn(2, entry, x, y, z, o, scale, duration, nil, nil, nil, save, phase)
end

function GAMEOBJECT:Update()
   self:Despawn(1, 1)
end

function GAMEOBJECT:CustomAnimate(aindex)
   if (aindex < 2) then
      local data = LuaPacket(179, 12);
      data:WriteGUID(self:GetGUID());
      data:WriteUInt32(aindex);
      self:SendMessageToSet(data, false, false);
      return true
   end
   return false
end

function GAMEOBJECT:GetAreaId()
   return MapMgr:GetAreaID(self:GetX(), self:GetY());
end

function GAMEOBJECT:GetLandHeight(x,y) return MapMgr:GetADTLandHeight(x,y) end