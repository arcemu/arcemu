--[[
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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
 ]]
 
assert( include("LCF.lua") )
LCF.MapMgrMethods = {}
LCF.MapScriptInterfaceMethods = {}

local MAP = LCF.MapMgrMethods
local INTERFACE = LCF.MapScriptInterfaceMethods

setmetatable( getregistry("MapMgr"), MAP )
setmetatable( getregistry("MapScriptInterface"), INTERFACE)

local function malias(_func, ...)
	for _,label in ipairs(arg) do
		MAP[label] = _func
	end
end
local function ialias(_func, ...)
	for _,label in ipairs(arg) do
		INTERFACE[label] = _func
	end
end

local function stub(self, entry, x, y, z, o, phase )
	return self:FullSpawnCreature(entry, x, y, z, o, true, false, 0, 0, phase )
end
ialias(stub, "SpawnCreature", "spawnCreature", "spawncreature" )

local function stub(self, entry, x, y, z, o, phase )
	return self:FullSpawnGameObject(entry, x, y, z, o, true, 0, 0, phase )
end
ialias(stub, "SpawnGameObject", "spawnGameObject", "spawngameobject")