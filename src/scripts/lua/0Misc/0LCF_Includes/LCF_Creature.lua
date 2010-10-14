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
 
local CREATURE = LCF.CreatureMethods
assert(CREATURE)
--[[CREATURE.spell_events = {}
CREATURE.CAST_FLAG_ON_SELF =  0x0
CREATURE.CAST_FLAG_ON_TARGET = 0x1
CREATURE.CAST_FLAG_DEST = 0x2
CREATURE.CAST_FLAG_ALL_PLAYERS = 0x3
CREATURE.CAST_FLAG_FRIENDLY_UNITS = 0x4
CREATURE.CAST_FLAG_ENEMY_UNITS = 0x5
CREATURE.CAST_ATTRIBUTE_INSTANT = 0x8 -- or 1000B]]

function CREATURE:SetUnselectable()
	if(self:HasFlag(LCF.UNIT_FIELD_FLAGS, LCF.UNIT_FLAG_NOT_SELECTABLE) == false) then
		self:SetUInt32Value(LCF.UNIT_FIELD_FLAGS,bit_or(self:GetUInt32Value(LCF.UNIT_FIELD_FLAGS),LCF.UNIT_FLAG_NOT_SELECTABLE))
	end
end
function CREATURE:SetSelectable()
	if(self:HasFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_SELECTABLE)) then
		self:RemoveFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_SELECTABLE)
	end
end
function CREATURE:SetUnattackable()
	if(self:HasFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_ATTACKABLE_9) == false) then
		self:SetUInt32Value(LCF.UNIT_FIELD_FLAGS,bit_or(self:GetUInt32Value(LCF.UNIT_FIELD_FLAGS),LCF.UNIT_FLAG_NOT_ATTACKABLE_9))
	end
end
function CREATURE:SetAttackable()
	if(self:HasFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_ATTACKABLE_9) ) then
		self:RemoveFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_ATTACKABLE_9)
	end
	if(self:HasFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_ATTACKABLE_2) ) then
		self:RemoveFlag(LCF.UNIT_FIELD_FLAGS,LCF.UNIT_FLAG_NOT_ATTACKABLE_2)
	end
end
function CREATURE:SetChannelTarget(target)
	local guid = nil
	if(target) then	
		guid = target:GetGUID()
	end
	self:SetUInt64Value(LCF.UNIT_FIELD_CHANNEL_OBJECT,guid)
end
function CREATURE:GetChannelTarget()
	local guid = self:GetUInt64Value(LCF.UNIT_FIELD_CHANNEL_OBJECT)
	if(guid) then
		return self:GetObject(guid)
	end
	return nil
end
function CREATURE:SpawnLocalCreature(crc_entry,faction,duration)
	local x,y,z,o = self:GetLocation()
	return self:SpawnCreature(crc_entry,x,y,z,o,faction,duration)
end
function CREATURE:SpawnLocalGameObject(go_entry,duration)
	local x,y,z,o = self:GetLocation()
	return self:SpawnGameObject(go_entry,x,y,z,o,duration)
end
function CREATURE:MonsterYell(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_YELL,LCF.LANG_UNIVERSAL,msg)
end
function CREATURE:MonsterSay(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_SAY,LCF.LANG_UNIVERSAL,msg)
end
function CREATURE:MonsterEmote(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_EMOTE,LCF.LANG_UNIVERSAL,msg)
end
function CREATURE:MonsterWhisper(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_WHISPER,LCF.LANG_UNIVERSAL,msg)
end
function CREATURE:MonsterYellToPlayer(plr,msg)
	self:SendChatMessageToPlayer(LCF.CHAT_MSG_MONSTER_YELL,LCF.LANG_UNIVERSAL,msg,plr)
end
function CREATURE:MonsterSayToPlayer(plr,msg)
	self:SendChatMessageToPlayer(LCF.CHAT_MSG_MONSTER_SAY,LCF.LANG_UNIVERSAL,msg,plr)
end
function CREATURE:MonsterWhisperToPlayer(plr,msg)
	self:SendChatMessageToPlayer(LCF.CHAT_MSG_MONSTER_WHISPER,LCF.LANG_UNIVERSAL,msg,plr)
end
function CREATURE:BossRaidEmote(msg)
	self:SendChatMessage(LCF.CHAT_MSG_RAID_BOSS_EMOTE,LCF.LANG_UNIVERSAL,msg)
end
function CREATURE:MoveToUnit(target)
	local x,y,z = target:GetLocation()
	self:MoveTo(x,y,z,self:GetO())
end
function CREATURE:GetSpawnLocation()
	local x,y,z,o = self:GetSpawnX(),self:GetSpawnY(),self:GetSpawnZ(),self:GetSpawnO()
	return x,y,z,o
end
function CREATURE:RegisterLuaEvent(func,delay,repeats,...)
	self:RegisterEvent(LCF:CreateClosure(func,...),delay,repeats)
end
function CREATURE:IsCasting()
	return self:GetCurrentSpell() ~= nil
end
function CREATURE:SetCreator(creator)
	local guid = nil
	if(creator) then
		guid = creator:GetGUID()
	end
	self:SetUInt64Value(LCF.UNIT_FIELD_CREATEDBY,guid)
	self:SetUInt64Value(LCF.UNIT_FIELD_SUMMONEDBY,guid)
end
function CREATURE:GetCreator()
	local creator_guid = self:GetUInt64Value(LCF.UNIT_FIELD_CREATEDBY)
	if(creator_guid == nil) then
		creator_guid = self:GetUInt64Value(LCF.UNIT_FIELD_SUMMONEDBY)
	end
	return self:GetObject(creator_guid)
end
function CREATURE:GetLocalCreature(entry)
	local x,y,z = self:GetLocation()
	local crc = self:GetCreatureNearestCoords(x,y,z,entry)
	return crc
end
function CREATURE:GetLocalGameObject(entry)
	local x,y,z = self:GetLocation()
	local go = self:GetGameObjectNearestCoords(x,y,z,entry)
	return go
end