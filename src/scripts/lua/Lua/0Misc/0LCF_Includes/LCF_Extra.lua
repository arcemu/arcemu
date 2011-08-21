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
 
 assert(LCF)
 
LCF.zone_hooks = {}
LCF.areatrigger_hooks = {}
LCF.call_backs = {}

function LCF:HandleOnZone(event,player,zone_id)
	local zone_key = "zone "..tostring(zone_id)
	local zone_struct = self.zone_hooks[zone_key]
	if(type(zone_struct) == "table") then
		for _,v in pairs(zone_struct) do
			if(type(v) == "function") then
				--pcall(v,event,player,zone_id)
				v(event,player,zone_id)
			end
		end
	end
end
function LCF:HandleOnAreaTrigger(event,player,area_trig)
	--player:SendAreaTriggerMessage("[AREA TRIGGER] "..area_trig)
	local key = "area "..tostring(area_trig)
	local struct = self.areatrigger_hooks[key]
	if(type(struct) == "table") then
		for _,v in pairs(struct) do
			if(type(v) == "function") then
				--pcall(v,event,player,area_trig)
				v(event,player,area_trig)
			end
		end
	end
end
function LCF:RegisterZoneHook(zone_id, func)
	assert(zone_id and func,"Failed, expected valid values got ->"..tostring(zone_id).." : "..tostring(func))
	if(type(func) ~= "function") then error("Failed to register, expected valid function got->"..tostring(func)) end
	local zone_key = "zone "..tostring(zone_id)
	if(type(self.zone_hooks[zone_key]) ~= "table") then
		self.zone_hooks[zone_key] = {}
	end
	local fstr = tostring(func)
	--first, try to find it.
	for _,v in pairs(self.zone_hooks[zone_key]) do
		if(tostring(v) == fstr) then
			return
		end
	end
	table.insert(self.zone_hooks[zone_key],func)
end
function LCF:RemoveZoneHook(zone_id, func)
	local zone_key = "zone "..tostring(zone_id)
	local zone_struct = self.zone_hooks[zone_key]
	local fstr = tostring(func)
	if(type(zone_struct) == "table") then
		for k,v in pairs(zone_struct) do
			if(tostring(v) == fstr) then
				table.remove(zone_struct,k)
				break
			end
		end
	end
end
function LCF:RegisterAreaTrigger(area_trig, func)
	assert(area_trig and func,"Failed, expected valid values got ->"..tostring(area_trig).." : "..tostring(func))
	if(type(area_trig) ~= "number") then error("Failed to register, expected number value, got ->"..tostring(area_trig)) end
	if(type(func) ~= "function") then error("Failed to register, expected valid function got->"..tostring(func)) end
	local key = "area "..tostring(area_trig)
	local fstr = tostring(func)
	if(type(self.areatrigger_hooks[key]) ~= "table") then
		self.areatrigger_hooks[key] = {}
	end
	for _,v in pairs(self.areatrigger_hooks[key]) do
		if(type(v) == fstr) then
			return
		end
	end
	table.insert(self.areatrigger_hooks[key],func)
end
function LCF:RemoveAreaTrigger(area_trig,func)
	local key = "area "..tostring(area_trig)
	local struct = self.areatrigger_hooks[key]
	local fstr = tostring(func)
	if(type(struct) == "table") then
		for k,v in pairs(struct) do
			if(type(v) == fstr) then
				table.remove(struct,k)
				break;
			end
		end
	end
end
function LCF:RegisterLuaEvent(evt_typ,func,delay,repeats,...)
	if(type(func) ~= "function") then error("Invalid argument #1, expected function got ->"..type(func)) end
	if(type(delay) ~= "number") then error("Invalid argument #2, expected number got ->"..type(delay)) end
	if(type(repeats) ~= "number") then error("Invalid argument #3, expected number got->"..type(repeats)) end
	assert(type(arg) == "table","Internal error, arg is not a table.")
	if(delay <= 0) then error("Invalid argument #2, delay must be greater than 0.") end
	local struct = {evt_typ,{func,delay,delay,repeats,arg} }
	table.insert(self.call_backs,struct)
end
function LCF:RemoveLuaEvent(evt_typ,func)
	for k,v in ipairs(self.call_backs) do
		if(v[1] == evt_typ) then
			if(func ~= nil) then
				local struct = v[2]
				local fstr = tostring(func)
				if(tostring(struct[1]) == fstr) then
					table.remove(self.call_backs,k)
				end
			else
				table.remove(self.call_backs,k)
			end
		end
	end
end
function LCF:UpdateCallBacks(...)
	for k,v in pairs(self.call_backs) do
		local struct = v[2]
		struct[2] = (struct[2] - 100)
		if(struct[2] <= 0) then
			struct[1](unpack(struct[5]))
			struct[2] = struct[3]
			if(struct[4] ~= 0) then
				struct[4] = struct[4] - 1
				if(struct[4] == 0) then
					table.remove(self.call_backs,k)
				end
			end
		end
	end
end
function LCF:BindMethod(method,method_table)
	return function(...) return method(method_table,unpack(arg)) end
end
function LCF:CreateClosure(method,...)
	local args = arg
	local closure = function(...) return method(unpack(arg),unpack(args)) end
	return closure
end
RegisterServerHook(15,LCF:BindMethod(LCF.HandleOnZone,LCF))
RegisterServerHook(26,LCF:BindMethod(LCF.HandleOnAreaTrigger,LCF))
--RegisterTimedEvent("LCF:UpdateCallBacks",100,0)
CreateLuaEvent(LCF:BindMethod(LCF.UpdateCallBacks,LCF),100,0)