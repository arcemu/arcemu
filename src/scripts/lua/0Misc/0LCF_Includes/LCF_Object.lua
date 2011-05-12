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
 
 local OBJ = LCF.ObjectMethods
 assert(OBJ)
 local function alias(LHAname, LBname)
   OBJ[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end
 
 function OBJ:GetX() return self:GetPositionX() end
 function OBJ:GetY() return self:GetPositionY() end
 function OBJ:GetZ() return self:GetPositionZ() end
 
 OBJ.GetMapId = function() return MapMgr:GetMapID() end
 OBJ.GetMapID = OBJ.GetMapId
 OBJ.MapID = OBJ.GetMapId
 
 OBJ.GetLocation = function(self) return self:GetPositionX(), self:GetPositionY(), self:GetPositionZ(), self:GetO() end
 OBJ.Location = OBJ.GetLocation
 OBJ.location = OBJ.Location
 OBJ.getLocation = OBJ.Location
 OBJ.getlocation = OBJ.Location
 
 
function OBJ:GetSpawnLocation()
	return self:GetSpawnX(),self:GetSpawnY(),self:GetSpawnZ(),self:GetSpawnO()
end

function OBJ:GPS()
	return string.format("Map %u Instance %u x: %f y: %f z: %f facing: %f", MapMgr:GetMapID(), MapMgr:GetInstanceID(), self:GetPositionX(), self:GetPositionY(), self:GetPositionZ(), self:GetO() )
end

function OBJ:CalcDistance(target) return self:CalcDistanceToObject( target) end

function OBJ:findLocalCreature(entry)
	local x,y,z = self:GetLocation()
	return MapMgr:GetInterface():GetCreatureNearestCoords(x,y,z,entry)
end
function OBJ:findLocalGameObject(entry)
	local x,y,z = self:GetLocation()
	local go = MapMgr:GetInterface():GetGameObjectNearestCoords(x,y,z,entry)
	return go
end
function OBJ:SpawnLocalCreature(entry,faction,phase, duration)
	local x,y,z,o = self:GetLocation()
	local crc = MapMgr:GetInterface():SpawnCreature(entry, x,y,z,o, true, true, 0, 0, phase)
	if(faction > 0) then
		crc:SetFaction(faction)
	end
	if(duration) then
		crc:Despawn(duration, 0)
	end
end
function OBJ:SpawnLocalGameObject(entry,duration)
	local x,y,z,o = self:GetLocation()
	local go = MapMgr:GetInterface():SpawnGameObject(entry, x, y, z, o, true, 0, 0, phase)
	if(duration) then
		go:Despawn(duration ,0)
	end
end

function OBJ:RemoveFlag(index, oldflag)
   self:SetUInt32Value(index, bit_and(self:GetUInt32Value(index), bit_not(oldFlag)));
end

alias("GetByteValue", "GetByte")

alias("SetByteValue", "SetByte")

alias("CalcRadAngle", "calcRadAngle")

alias("CalcAngle", "calcAngle")

alias("CalcToDistance", "CalcDistanceToLocation")

alias("GetDistanceYards", "CalcDistanceToObject")

function OBJ:GetInstanceID() return MapMgr:GetInstanceID(); end

function OBJ:GetObject(guid) return MapMgr:GetObject(guid); end

function OBJ:GetUnitByGUID(guid) return MapMgr:GetObject(guid); end

function OBJ:GetUnitBySqlId(id) return MapMgr:GetSqlIdCreature(id); end

alias("GetZoneId", "GetZoneID")

function OBJ:IsInDungeon() return (MapMgr:GetMapInfo().type == 4); end

function OBJ:IsInRaid() return (MapMgr:GetMapInfo().type == 1); end

alias("ModUInt32Value", "ModSignedUInt32Value")

function OBJ:PhaseAdd(p) self:Phase(1, p); end

function OBJ:PhaseDelete(p) self:Phase(2, p); end

function OBJ:PhaseSet(p) self:Phase(0, p); end

alias("AddToPhase", "PhaseAdd")

alias("DeletePhase", "PhaseDelete")

alias("SetPhase", "PhaseSet")

function OBJ:IsInPhase(p) return (bit_and(self:GetPhase(), p) > 0); end
 