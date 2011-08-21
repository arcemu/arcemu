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

local GAMEOBJECT = LCF.GOMethods
assert(GAMEOBJECT)

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
function GAMEOBJECT:Respawn()
	self:SetPosition(self:GetX(),self:GetY(),self:GetZ(),self:GetO())
end
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
function GAMEOBJECT:RegisterLuaEvent(func,delay,repeats,...)
	self:RegisterEvent(LCF:CreateClosure(func,self,unpack(arg)),delay,repeats)
end
function GAMEOBJECT:SetCreator(creator)
	local guid = creator:GetGUID()
	self:SetUInt64Value(LCF.OBJECT_FIELD_CREATED_BY,guid)
end
function GAMEOBJECT:GetLocalCreature(entry)
	local x,y,z = self:GetLocation()
	local crc = self:GetCreatureNearestCoords(x,y,z,entry)
	return crc
end
function GAMEOBJECT:GetLocalGameObject(entry)
	local x,y,z = self:GetLocation()
	local go = self:GetGameObjectNearestCoords(x,y,z,entry)
	return go
end
function GAMEOBJECT:SpawnLocalCreature(entry,faction,duration)
	local x,y,z,o = self:GetLocation()
	self:SpawnCreature(entry,x,y,z,o,faction,duration)
end
function GAMEOBJECT:SpawnLocalGameObject(entry,duration)
	local x,y,z,o = self:GetLocation()
	self:SpawnGameObject(entry,x,y,z,o,duration)
end
function GAMEOBJECT:GetCreator()
	local creator_guid = self:GetUInt64Value(LCF.UNIT_FIELD_CREATEDBY)
	if(creator_guid == nil) then
		creator_guid = self:GetUInt64Value(LCF.UNIT_FIELD_SUMMONEDBY)
	end
	return self:GetObject(creator_guid)
end