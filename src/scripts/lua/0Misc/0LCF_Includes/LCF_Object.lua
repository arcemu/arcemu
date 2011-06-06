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

alias = function(_func, ...)
	for _,label in ipairs(arg) do
		OBJ[label] = _func
	end
end

local stub;
function stub(self)
	local players = self:inrangeplayers()
	for k,v in ipairs(players) do
		players[k] = TO_PLAYER(v)
	end
	return players
end
alias(stub, "GetInRangePlayers", "getInRangePlayers", "getinrangeplayers")

function stub(self)
	local allies = self:GetInRangeSameFactions()
	for k,v in ipairs(allies) do
		allies[k] = TO_UNIT(v):Actual();
	end
	return allies
end
alias(stub, "GetInRangeAllies", "getInRangeAllies", "getinrangeallies", "GetInRangeFriends", "getInRangeFriends", "getinrangefriends", "GetAllies", "getAllies", "getallies")

function stub(self)
	local hostiles = self:GetInRangeOpposingFactions()
	for k,v in ipairs(hostiles) do
		hostiles[k] = TO_UNIT(v):Actual();
	end
	return hostiles
end
alias(stub, "GetInRangeEnemies", "getInRangeEnemies", "getinrangeenemies", "GetInRangeHostiles", "getInRangeHostiles", "getinrangehostiles", "GetEnemies", "getEnemies", "getenemies", "GetHostiles", "getHostiles", "gethostiles")

function stub(self)
	local allies = self:GetInRangeSameFactions()
	return TO_UNIT( allies[ math.random(#allies) ] ):Actual();
end
alias(stub, "GetRandomAlly", "getRandomAlly", "getrandomally", "GetRandomFriend", "getRandomFriend", "getrandomfriend")

function stub(self)
	local hostiles = self:GetInRangeOpposingFactions()
	return TO_UNIT( hostiles[ math.random( #hostiles) ] ):Actual();
end
alias(stub, "GetRandomEnemy", "getRandomEnemy", "getrandomenemy", "GetRandomHostile", "getRandomHostile", "getrandomhostile")

function stub(self)
	local allies = self:GetInRangeSameFactions()
	local distance = 9999.99;
	
	local suitable_ally, current_distance;
	for _, unit in ipairs(allies) do
		current_distance = self:CalcDistanceToObject(unit)
		if(current_distance <= distance) then
			distance = current_distance
			suitable_ally = unit
		end
	end
	return TO_UNIT(suitable_ally):Actual();
end
alias(stub, "GetClosestAlly", "getClosestAlly", "getclosestally", "GetClosestFriend", "getClosestFriend", "getclosestfriend")

function stub(self)
	local hostiles = self:GetInRangeSameFactions()
	local distance = 9999.99
	
	local suitable_hostile, current_distance;
	for _, unit in ipairs(hostiles) do
		current_distance = self:CalcDistanceToObject(unit)
		if(current_distance <= distance) then
			distance = current_distance
			suitable_hostile = unit
		end
	end
	return TO_UNIT(suitable_hostile):Actual();
end
alias(stub, "GetClosestEnemy", "getClosestEnemy", "getclosestenemy", "GetClosestHostile", "getClosestHostile", "getclosesthostile")

function stub(self)
	local players = self:inrangeplayers()
	local distance = 9999.99
	
	local suitable_player, current_distance;
	for _, plr in ipairs(players) do
		current_distance = self:CalcDistanceToObject(plr)
		if(current_distance <= distance) then
			distance = current_distance
			suitable_player = plr
		end
	end
	return TO_PLAYER(suitable_player)
end
alias(stub, "GetClosestPlayer", "getClosestPlayer", "getclosestplayer", "GetNearestPlayer", "getNearestPlayer", "getnearestplayer")

function stub(self, flag)
	local players = self:GetInRangePlayers()
	local short_distance, long_distance = 8, 30
	local aux = {}
	if(0 == flag or nil == flag) then
		return players[ math.random(#players) ]
	elseif(1 == flag) then
		for _, plr in ipairs(players) do
			if( self:CalcDistanceToObject(plr) <= short_distance ) then
				table.insert(aux, plr)
			end
		end
		return aux[ math.random( #aux) ]
	elseif( 3 == flag) then
		for _, plr in ipairs(players) do
			if( self:CalcDistanceToObject(plr) >= long_distance ) then
				table.insert(aux, plr)
			end
		end
		return aux [ math.random(#aux) ]
	elseif(2 == flag) then
		local aux_dist
		for _, plr in ipairs(players) do
			aux_dist = self:CalcDistanceToObject(plr)
			if(aux_dist > short_distance and aux_dist < long_distance ) then
				table.insert( aux, plr)
			end
		end
		return aux [ math.random(#aux) ]
	elseif(4 == flag) then
		for _, plr in ipairs(players) do
			if( plr:getPowerType() == LCF.POWER_TYPE_MANA ) then
				table.insert( aux, plr)
			end
		end
		return aux [ math.random(#aux) ]
	elseif( 5 == flag) then
		for _, plr in ipairs(players) do
			if( plr:getPowerType() == LCF.POWER_TYPE_RAGE ) then
				table.insert( aux, plr)
			end
		end
		return aux [ math.random(#aux) ]
	elseif( 6 == flag) then
		for _, plr in ipairs(players) do
			if( plr:getPowerType() == LCF.POWER_TYPE_ENERGY ) then
				table.insert( aux, plr)
			end
		end
		return aux [ math.random(#aux) ]
	elseif( 7 == flag and self:IsCreature() ) then
		local mosthated = self:getAI():getMostHated()
		for _, plr in ipairs(players) do
			if( tostring(plr) ~= tostring(mosthated) ) then
				table.insert( aux, plr)
			end
		end
		return aux [ math.random(#aux) ]
	else
		assert( false, "GetRandomPlayer(flag), flag argument was invalid. Acceptable ranges are 0-7")
	end
	return nil
end

alias(stub, "GetRandomPlayer", "getRandomPlayer", "getrandomplayer")

function stub(self)
	local objs = self:GetInRangeObjects()
	local aux = {}
	for _,v in ipairs(objs) do
		if( v:IsCreature() ) then
			table.insert( aux, TO_CREATURE(v) )
		end
	end
	return aux
end
alias(stub, "GetInRangeCreatures", "getInRangeCreatures", "getinrangecreatures", "GetCreatures", "getCreatures", "getcreatures")

function stub(self)
	local objs = self:GetInRangeObjects()
	local aux = {}
	for _,v in ipairs(objs) do
		if( v:IsCreature() ) then
			table.insert( aux, TO_CREATURE(v) )
		elseif ( v:IsPlayer() ) then
			table.insert( aux, TO_PLAYER(v) )
		end
	end
	return aux
end
alias(stub, "GetInRangeUnits", "getInRangeUnits", "getinrangeunits")

function stub(self)
	local units = self:GetInRangeUnits()
	local distance = 9999.99
	
	local suitable_unit, current_distance;
	for _, un in ipairs(units) do
		current_distance = self:CalcDistanceToObject(un)
		if(current_distance <= distance) then
			distance = current_distance
			suitable_unit = un
		end
	end
	return TO_PLAYER(suitable_unit)
end
alias(stub, "GetClosestUnit", "getClosestUnit", "getclosestunit", "GetNearestUnit", "getNearestUnit", "getnearestunit")

function stub(self)
	local objs = self:getInRangeSameFactions()
	local aux = {}
	for _,v in ipairs(objs) do
		if( v:IsCreature() ) then
			table.insert( aux, TO_CREATURE(v) )
		end
	end
	return aux
end
alias(stub, "GetFriendlyCreatures", "getFriendlyCreatures", "getfriendlycreatures", "GetAlliedCreatures", "getAlliedCreatures", "getalliedcreatures", "GetInRangeFriends")

function stub(self)
	local objs = self:getInRangeOpposingFactions()
	local aux = {}
	for _,v in ipairs(objs) do
		if( v:IsCreature() ) then
			table.insert( aux, TO_CREATURE(v) )
		end
	end
	return aux
end
alias(stub, "GetHostileCreatures", "getHostileCreatures", "gethostilecreatures", "GetEnemyCreatures", "getEnemyCreatures", "getenemycreatures", "GetInRangeEnemies")

function stub(self)
	local objs = self:GetInRangeObjects()
	local aux = {}
	for _,v in ipairs(objs) do
		if( v:IsGameObject() ) then
			table.insert( aux, TO_GAMEOBJECT(v) )
		end
	end
	return aux
end
alias(stub, "GetInRangeGameObjects", "getInRangeGameObjects", "getinrangegameobjects", "GetGameObjects", "getGameObjects", "getgameobjects")

function OBJ:GetInRangeGameObjectsCount()
   return #(self:GetInRangeGameObjects())
end

function OBJ:GetInRangePlayersCount()
   return #(self:GetInRangePlayers())
end

function OBJ:GetInRangeCreaturesCount()
   return #(self:GetInRangeCreatures())
end