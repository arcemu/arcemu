local mod = getfenv(1)
assert(mod)
module(mod._NAME..".ANZU_THE_RAVEN_GOD",package.seeall)
local self = getfenv(1)

WorldDBQuery("DELETE FROM ai_agents WHERE entry = 23035;")

function OnSpawn(unit)
	local claw = unit:GetLocalGameObject(185554)
	if(claw) then
		claw:RemoveFromWorld()
	end
	unit:MonsterYell("No! How can this be?!")
	unit:MonsterYell("Pain will be the price for your insolence! You cannot stop me from claiming the Emerald Dream as my own!")
end
function OnCombat(unit,_,mTarget)
	self[tostring(unit)] = {
		spell_bomb = math.random(5,15),
		cyclone = math.random(2,10),
		screech = math.random(15,30),
		m_banished = false,
		banish_phase = 1,
		banish_timer = nil,
		crows = {}
	}
	unit:RegisterAIUpdateEvent(1000)
end
function OnWipe(unit,_,mTarget)
	unit:RemoveAIUpdateEvent()
	unit:RemoveLuaEvents()
	for _,v in pairs(self[tostring(unit)].crows) do
		v:Despawn(1000,0)
	end
	self[tostring(unit)] = nil
end
function AIUpdate(unit)
	if(unit:IsCasting() ) then return end
	if(unit:GetNextTarget() == nil) then
		unit:WipeThreatList()
		return
	end
	local vars = self[tostring(unit)]
	--Seems casted only when he's going banishment.
	--vars.spell_bomb = vars.spell_bomb - 1
	vars.cyclone = vars.cyclone - 1
	vars.screech = vars.screech - 1
	if(vars.screech <= 0) then
		unit:FullCastSpell(40184)
		vars.screech = math.random(20,30)
	elseif(vars.cyclone <= 0) then
		local target = unit:GetRandomEnemy()
		--we use getrandom enemy to allow this boss to fight another npc if it's ever put in that situation
		--[[while(tostring(target) == tostring(unit:GetMainTank()) ) do
			target = unit:GetRandomEnemy()
		end]]
		unit:FullCastSpellOnTarget(40321,target)
		vars.cyclone = math.random(15,20)
	elseif(vars.spell_bomb ~= nil and vars.spell_bomb <= 0) then
		local target = unit:GetRandomPlayer(0)
		unit:FullCastSpellOnTarget(40303,target)
		vars.spell_bomb = nil
		local say = math.random(0,1)
		if(say) then
			unit:MonsterWhisperToPlayer(target,"Yes...cast your precious little spells, ak-a-ak!")
		else
			unit:MonsterWhisperToPlayer(target,"Your magics shall be your undoing... ak-a-ak..")
		end
	else
		local hp = unit:GetHealthPct()
		if( (hp <= 66 and vars.banish_phase ==1) or (hp <= 33 and vars.banish_phase == 2) ) then
			print("Banish event")
			BanishEvent(unit,1)
			vars.banish_phase = vars.banish_phase +1
		end
	end
	if(vars.m_banished) then
		vars.banish_timer = vars.banish_timer -1 
		if(vars.banish_timer <= 0 or #vars.crows == 0) then
			vars.m_banished = false
			unit:DisableMelee(false)
			unit:SetAttackable()
			unit:SetSelectable()
			unit:RemoveAura(32567)
		end
	end
end
function BanishEvent(unit,phase)
	if(phase == 1) then
		unit:MonsterYell("Awaken my children and assist your master!")
		unit:RegisterLuaEvent(BanishEvent,5000,1,2)
		unit:DisableMelee(true)
	elseif(phase == 2) then
		local vars = self[tostring(unit)]
		unit:FullCastSpell(32567)
		unit:SetUnattackable()
		unit:SetUnselectable()
		vars.m_banished = true
		vars.banish_timer = 60
		vars.spell_bomb = 0
		unit:RegisterLuaEvent(BanishEvent,3000,1)
	else
		local vars = self[tostring(unit)]
		for i = 1,math.random(10,16) do
			table.insert(vars.crows,unit:SpawnCreature(23132,unit:GetX(),unit:GetY(),unit:GetZ()+15,unit:GetO(),0,0))
		end
	end
end
function SummonRavenGod(_,pSpell)
	-- to do: Simulate the cool entrance effects of anzu xD
	local caster = pSpell:GetCaster()
	local claw = caster:GetLocalGameObject(185554)
	if(claw) then
		claw:SpawnLocalCreature(23035,0,0)
	end
end
RegisterDummySpell(40098,SummonRavenGod)
RegisterUnitEvent(23035,18,OnSpawn)
RegisterUnitEvent(23035,1,OnCombat)
RegisterUnitEvent(23035,2,OnWipe)
RegisterUnitEvent(23035,21,AIUpdate)

function BroodOnSpawn(unit)
	unit:SetFlying()
	unit:ModifyFlySpeed(15)
	local angle = 0
	local radius = math.random(20)
	local wp = 1
	for i = 1,8 do
		unit:CreateCustomWaypoint(wp,unit:GetX()+math.cos(math.rad(angle))*radius,unit:GetY()+math.sin(math.rad(angle))*radius,unit:GetZ(),unit:GetO(),0,0x300)
		angle = angle + 45
		wp = wp + 1
	end
	--unit:SetAIState(10)
	unit:SetMovementType(1) -- random wp
	unit:RegisterEvent(BroodLandAndAttack,10000,1)
end
function BroodLandAndAttack(unit)
	unit:DeleteWaypoints()
	local target = unit:GetRandomEnemy()
	unit:AttackReaction(target,1,0)
	unit:MoveToUnit(target)
end
RegisterUnitEvent(23132,18,BroodOnSpawn)