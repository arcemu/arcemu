local mod = getfenv(1)
assert(mod)
module(mod._NAME..".TALON_KING_IKISS",package.seeall)
local self = getfenv(1)

function OnSpawn(unit)
	local chest = unit:GetGameObjectNearestCoords(80.069473,287.675293,26.629349,187372)
	if(chest) then
		chest:SetUnclickable()
	end
end
function OnCombat(unit,_,mTarget)
	self[tostring(unit)] = {
		volley = math.random(5,10),
		explosion = math.random(20,30),
		blink = math.random(20,30),
		poly = math.random(15,30),
		shield = math.random(30,45),
		slow = math.random(15,30),
		isHeroic = (mTarget:IsPlayer() and mTarget:IsHeroic() )
	}
	local say_text = math.random(1,3)
	if(say_text == 1) then
		unit:MonsterYell("Mmmm you make war on Ikiss?")
		unit:PlaySoundToSet(10554)
	elseif(say_text == 2) then
		unit:MonsterYell("Ikiss cut you pretty slice you yes!")
		unit:PlaySoundToSet(10555)
	else
		unit:MonsterYell("No escape for- for you!")
		unit:PlaySoundToSet(10556)
	end
	unit:RegisterAIUpdateEvent(1000)
end
function OnWipe(unit)
	unit:RemoveAIUpdateEvent()
	self[tostring(unit)] = nil
end
function OnKill(unit)
	local say = math.random()
	if(say) then
		unit:MonsterYell("Mmmmmmmmmm!")
		unit:PlaySoundToSet(10558)
	else
		unit:MonsterYell("You die! Stay away from trinkets!")
		unit:PlaySoundToSet(10559)
	end
end
function OnDeath(unit)
	unit:MonsterYell("Ikiss will not... die!")
	unit:PlaySoundToSet(10560)
	local chest = unit:GetGameObjectNearestCoords(80.069473,287.675293,26.629349,187372)
	if(chest) then
		chest:SetClickable()
	end
end
function AIUpdate(unit)
	if(unit:IsCasting() ) then return end
	if(unit:GetNextTarget() == nil) then
		unit:WipeThreatList()
		return
	end
	local vars = self[tostring(unit)]
	vars.volley = vars.volley -1
	vars.explosion = vars.explosion - 1
	vars.blink = vars.blink - 1
	vars.poly = vars.poly - 1
	vars.shield = vars.shield - 1
	vars.slow = vars.slow - 1
	if(vars.explosion <= 0) then
		if(vars.isHeroic) then
			unit:FullCastSpell(40425)
		else
			unit:FullCastSpell(38197)
		end
		vars.explosion = math.random(20,30)
	elseif(vars.volley <= 0) then
		if(vars.isHeroic) then
			unit:FullCastSpell(40424)
		else
			unit:FullCastSpell(35059)
		end
		vars.volley = math.random(5,10)
	elseif(vars.blink <= 0) then
		unit:FullCastSpell(38194)
		vars.explosion = 1
		vars.blink = math.random(30,45)
	elseif(vars.shield <= 0) then
		vars.shield = math.random(30,45)
		unit:FullCastSpell(38151)
	elseif(vars.slow <=0 and vars.isHeroic) then
		local target = unit:GetRandomEnemy()
		unit:FullCastSpellOnTarget(35032,target)
		vars.slow = math.random(15,30)
	elseif(vars.poly <=0) then
		local target = unit:GetRandomEnemy()
		if(vars.isHeroic) then
			unit:FullCastSpellOnTarget(43309,target)
		else
			unit:FullCastSpellOnTarget(38245,target)
		end
		vars.poly = math.random(10,15)
	end
end
RegisterUnitEvent(18473,1,OnCombat)
RegisterUnitEvent(18473,2,OnWipe)
RegisterUnitEvent(18473,3,OnKill)
RegisterUnitEvent(18473,4,OnDeath)
RegisterUnitEvent(18473,21,AIUpdate)
	