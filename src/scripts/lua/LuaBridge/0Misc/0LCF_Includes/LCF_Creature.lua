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
 
local CREATURE = LCF.CreatureMethods
assert(CREATURE)
local function alias(LHAname, LBname)
   CREATURE[LHAname] = function(self, ...)
      return self[LBname](self, ...);
   end
end
local function aialias(LHAname, LBname)
   CREATURE[LHAname] = function(self, ...)
      return self:GetAIInterface()[LBname](self:GetAIInterface(), ...);
   end
end

-- Some shorcuts to getting the ai interface.
CREATURE.AI = function(self) return self:GetAIInterface() end
CREATURE.ai = CREATURE.AI

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
function CREATURE:MonsterYell(msg) 
	self:SendChatMessage( LCF.CHAT_MSG_MONSTER_YELL,  LCF.LANG_UNIVERSAL, msg, 0)
end
function CREATURE:MonsterSay(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_SAY,LCF.LANG_UNIVERSAL,msg, 0)
end
function CREATURE:MonsterEmote(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_EMOTE,LCF.LANG_UNIVERSAL,msg, 0)
end
function CREATURE:MonsterWhisper(msg)
	self:SendChatMessage(LCF.CHAT_MSG_MONSTER_WHISPER,LCF.LANG_UNIVERSAL,msg, 0)
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
	self:SendChatMessage(LCF.CHAT_MSG_RAID_BOSS_EMOTE,LCF.LANG_UNIVERSAL,msg, 0)
end
function CREATURE:MoveToUnit(target)
	local x,y,z = target:GetLocation()
	self:MoveTo(x,y,z,self:GetO())
end

function CREATURE:RegisterAIUpdateEvent( interval)
	self:GetScript():RegisterAIUpdateEvent(interval)
end
function CREATURE:ModifyAIUpdateEvent( nInterval)
	self:GetScript():ModifyAIUpdateEvent(nInterval)
end
function CREATURE:RemoveAIUpdateEvent()
	self:GetScript():RemoveAIUpdateEvent()
end

CREATURE.GetName = function(self) return self:GetCreatureInfo().Name end
CREATURE.Name = CREATURE.GetName
CREATURE.name = CREATURE.Name

function CREATURE:Type()
	return self:GetCreatureInfo().Type
end

function CREATURE:WipeThreatList()
	self:GetAIInterface():WipeHateList()
end
function CREATURE:WipeHateList() self:GetAIInterface():WipeHateList() end

function CREATURE:DisableCombat( bewl)
	self:GetAIInterface().disable_combat = bewl
end
function CREATURE:DisableMelee( bewl)
	self:GetAIInterface().disable_melee = bewl
end
function CREATURE:DisableSpells( bewl)
	self:GetAIInterface().disable_spell = bewl
end
function CREATURE:DisableTargeting( bewl)
	self:GetAIInterface().disable_targeting = bewl
end
function CREATURE:DisableRanged( bewl)
	self:GetAIInterface().m_canRangedAttack = bewl
end

function CREATURE:SetCombatCapable( bewl)
	self:GetAIInterface().disable_combat = not bewl
end
function CREATURE:SetCombatMeleeCapable( bewl)
	self:GetAIInterface().disable_melee = not bewl
end
function CREATURE:SetCombatSpellCapable( bewl)
	self:GetAIInterface().disable_spell = not bewl
end
function CREATURE:SetCombatTargetingCapable( bewl)
	self:GetAIInterface().disable_targeting = not bewl
end
function CREATURE:SetCombatRangedCapable( bewl)
	self:GetAIInterface().m_canRangedAttack = not bewl
end

CREATURE.DisableRespawn = function (self, bewl) self:GetAIInterface().m_noRespawn = bewl end
CREATURE.NoRespawn = CREATURE.DisableRespawn

function CREATURE:CallForHelpHp( hp)
	self:GetAIInterface().m_CallForHelpHealth = hp
end
function CREATURE:CanCallForHelp( bewl)
	self:GetAIInterface().m_canCallForHelp = bewl
end
function CREATURE:RemoveThreat(target)
	self:GetAIInterface():RemoveThreatByPtr( target)
end
function CREATURE:WipeCurrentTarget()
	local target = self:GetAIInterface():getNextTarget()
	if( target ~= nil) then
		self:GetAIInterface():RemoveThreatByPtr(target)
	end
	self:GetAIInterface():setNextTarget( nil)
end
aialias("GetNextTarget", "getNextTarget")
function CREATURE:SetNextTarget(target) self:GetAIInterface():setNextTarget( target) end

CREATURE.GetMostHated = function(self) return self:GetAIInterface():GetMostHated() end
CREATURE.GetMainTank = CREATURE.GetMostHated

CREATURE.GetSecondHated = function(self) return self:GetAIInterface():GetSecondHated() end
CREATURE.GetAddTank = CREATURE.GetSecondHated

function CREATURE:MoveTo(...) self:GetAIInterface():MoveTo(...) end

function CREATURE:EventCastSpell(target, spell, delay, repeats)
	assert( target and spell)
	local function dospell(self, target, spell)
		self:FullCastSpellOnTarget(spell, target)
	end
	self:RegisterEvent( dospell, delay, repeats, target, spell)
end

function CREATURE:SetEquippedItem(slot, id)
   self:SetUInt32Value(LCF.UNIT_VIRTUAL_ITEM_SLOT_ID+slot, id)
end

function CREATURE:Attack(target)
   self:GetAIInterface():WipeTargetList()
   self:GetAIInterface():taunt(target, true)
   return true
end

aialias("ChangeTarget", "setNextTarget")

aialias("ClearThreatList", "ClearHateList")

function CREATURE:CreateWaypoint(x,y,z,o,waittime,flags,model)
   model = model or 0
   local way = Waypoint()
   way.x = x; way.y = y; way.z = z; way.o = o; way.waittime = waittime; way.flags = flags;
   way.backwardskinid = model; way.forwardskinid = model;
   self:GetAIInterface():addWayPoint(way);
   return way;
end

alias("CreateCustomWaypoint", "CreateWaypoint")



function CREATURE:CreatureHasQuest(id) return self:HasQuest(id, 0xFFFFFFFF); end

aialias("DeleteWaypoint", "deleteWaypoint")

aialias("DeleteWaypoints", "deleteWaypoints")

function CREATURE:EventCastSpell(tar, id, delay, repeats)
   self:RegisterEvent(function() self:CastSpell(tar, id, true); end, delay, repeats);
end

aialias("GetAIState", "getAIState")

aialias("GetAITargetsCount", "GetAITargetsCount")

aialias("GetCurrentWaypoint", "getCurrentWaypoint")

aialias("GetMoveType", "getMoveType")

aialias("GetSoulLinkedWith", "getSoullinkedWith")

aialias("GetTauntedBy", "getTauntedBy")

aialias("GetThreat", "getThreatByPtr")

aialias("GetUnitToFollow", "getUnitToFollow")

aialias("IsFlying", "IsFlying")

aialias("ModThreat", "modThreatByPtr")

function CREATURE:ModifyFlySpeed(value) self.flyspeed = value; end

function CREATURE:ModifyRunSpeed(value) self.runspeed = value; end

function CREATURE:ModifyWalkSpeed(value) self.walkspeed = value; end

function CREATURE:MoveToWaypoint(id)
   self:GetAIInterface():setMoveType(3);
   self:GetAIInterface():setWaypointToMove(id)
end

aialias("RemoveThreatByPtr", "RemoveThreatByPtr")

function CREATURE:ReturnToSpawnPoint()
   local x,y,z,o = self:GetSpawnLocation()
   self:MoveTo(x,y,z,o)
end

aialias("SetAIState", "SetAIState")

aialias("SetMoveRunFlag", "setMoveRunFlag")

aialias("SetMovementType", "setMoveType")

aialias("SetOutOfCombatRange", "setOutOfCombatRange")

aialias("SetPetOwner", "SetPetOwner")

aialias("SetSoulLinkedWith", "SetSoullinkedWith")

aialias("SetTauntedBy", "taunt")

function CREATURE:SetUnitToFollow(unit, dist)
   self:GetAIInterface():SetUnitToFollow(unit)
   self:GetAIInterface():SetFollowDistance(dist)
end

aialias("StopMovement", "StopMovement")

aialias("WipeThreatList", "WipeHateList")

function CREATURE:GetObjectType() return "Unit"; end

function CREATURE:AddAssistTargets(friend)
   if (isFriendly(self, friend)) then
      self:GetAIInterface():addAssistTargets(friend)
   end
end

aialias("AttackReaction", "AttackReaction")

function CREATURE:AggroWithInRangeFriends()
   if (not self.CombatStatus:IsInCombat()) then return; end
   
   local pTarget = self:GetAIInterface():getNextTarget();
   if (not pTarget) then return; end
   
   for k,v in pairs(self:getInRangeSameFactions()) do
      if (not v:IsDead() and v:IsCreature()) then
         if (self:CalcDistanceToObject(v) < 10) then
            v:GetAIInterface():setNextTarget(pTarget)
            v:GetAIInterface():AttackReaction(pTarget, 1, 0)
         end
      end
   end
end