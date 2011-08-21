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
 
function GetDBCSpellVar(entry, var, subindex)
   local prop = var
   if (subindex ~= nil) then prop = prop..subindex; end
   return dbcSpell:LookupEntry(entry)[prop]
end

GetLUAEngine = GetEngineName;
GetLuaEngine = GetLUAEngine;

GetLuaEngineVersion = GetEngineVersion;

function GetTaxiPath(id) return sTaxiMgr:GetTaxiPath(id); end

logcol = logcolor;

--MapID and InstanceID parameters are now irrelevant, and are replaced by this MapMgr's values.
function PerformIngameSpawn(spawntype, entry, _, x, y, z, o, facOrScale, duration, e1, e2, e3, _, save)
   if (not MapMgr) then return nil; end
   e1 = e1 or 0;
   e2 = e2 or 0;
   e3 = e3 or 0;
   local spawn
   if (spawntype == 1) then --unit
      spawn = MapMgr:GetInterface():FullSpawnCreature(entry, x, y, z, o, true, false, 0, 0, 1)
      spawn:SetEquippedItem(0, e1)
      spawn:SetEquippedItem(1, e2)
      spawn:SetEquippedItem(2, e3)
      spawn:SetFaction(facOrScale)
   elseif (spawntype == 2) then --go
      spawn = MapMgr:GetInterface():FullSpawnGameObject(entry, x, y, z, o, false, 0, 0, 1)
      spawn:SetScale(facOrScale / 100)
      spawn:PushToWorld(MapMgr)
   end
   if (duration > 0) then
      spawn:Despawn(duration, 0)
   end
   if (save) then
      spawn:SaveToDB()
   end
   return spawn
end

function EasySpawn(spawntype, entry, x, y, z, o, facOrScale, duration, e1, e2, e3, save, phase)
   phase = phase or 1
   local u = PerformIngameSpawn(spawntype, entry, nil, x, y, z, o, facOrScale, duration, e1, e2, e3, nil, save)
   u:SetPhase(phase)
   return u;
end

function SetDBCSpellVar(entry, var, subindex, value)
   local prop = var
   if (value ~= nil) then prop = prop..subindex; else value = subindex; end
   dbcSpell:LookupEntry(entry)[prop] = value
end

function SetWeather(forWhat, forWhatExtra, _type, Density)
   local sound = 0;
	if (_type == 2 or _type == 4) then                                             
      if (Density < 0.40) then
         sound = 8533;
      elseif (Density < 0.70) then
         sound = 8534;
      else
         sound = 8535;
		end
	elseif (_type == 8) then
      if (Density < 0.40) then
         sound = 8536;
      elseif (Density < 0.70) then
         sound = 8537;
      else
         sound = 8538;
      end
	elseif (_type == 16) then
      if (Density < 0.40) then
         sound = 8556;
      elseif(Density < 0.70) then
         sound = 8557;
      else
         sound = 8558;
      end
   end
   local data = LuaPacket(756, 9);
   data:WriteUInt32(_type)
   if (_type == 0 or _type == 1) then
      data:WriteFloat(0);
      data:WriteUInt32(0);
      data:WriteUInt8(0);		
   else
      data:WriteFloat(Density);
      data:WriteUInt32(sound);
      data:WriteUInt8(0);
   end

   if (forWhat == "zone") then
      SendPacketToZone(data, forWhatExtra);
   elseif (forWhat == "object") then
      forWhatExtra:SendPacket(data);
   end
end

function SendMail(_type, sender_guid, recipient_guid, subject, body, money, cod, item_guid, stationery) 
   sMailSystem:SendAutomatedMessage(_type, sender_guid, recipient_guid, subject, body, money, cod, item_guid, stationery)
end