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
 
 

local PLAYER = LCF.PlayerMethods
assert(PLAYER)

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
function PLAYER:SpawnLocalCreature(crc_entry,faction,duration)
	local x,y,z,o = self:GetLocation()
	return self:SpawnCreature(crc_entry,x,y,z,o,faction,duration)
end
function PLAYER:SpawnLocalGameObject(go_entry,duration)
	local x,y,z,o = self:GetLocation()
	return self:SpawnGameObject(go_entry,x,y,z,o,duration)
end
function PLAYER:MoveToUnit(target)
	local x,y,z = target:GetLocation()
	self:MoveTo(x,y,z,self:GetO())
end
function PLAYER:RegisterLuaEvent(func,delay,repeats,...)
	self:CreateLuaEvent(LCF:CreateClosure(func,self,unpack(arg)),delay,repeats)
end
function PLAYER:RemoveLuaEvent(func)
	LCF:RemoveLuaEvent(tostring(self),func)
end
function PLAYER:IsCasting()
	return player:GetCurrentSpell() ~= nil
end
function PLAYER:GetLocalCreature(entry)
	local x,y,z = self:GetLocation()
	local crc = self:GetCreatureNearestCoords(x,y,z,entry)
	return crc
end
function PLAYER:GetLocalGameObject(entry)
	local x,y,z = self:GetLocation()
	local go = self:GetGameObjectNearestCoords(x,y,z,entry)
	return go
end