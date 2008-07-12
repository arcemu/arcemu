

function InjuredDraenei_OnSpawn(pUnit, event)
InjuredDraeneiChance = math.random(1, 2)
if (InjuredDraeneiChance==1) then
	pUnit:SetStandState(3)
end
if (InjuredDraeneiChance==2) then
	 pUnit:SetStandState(1)
     pUnit:SetCombatCapable(0)
end
end
RegisterUnitEvent(16971, 6, "InjuredDraenei_OnSpawn")

function DraeneiSurvivor_OnSpawn(pUnit, event)
SurvivorDraeneiChance = math.random(1, 3)
if (SurvivorDraeneiChance==1) then
	pUnit:SetStandState(3)
end
if (SurvivorDraeneiChance==2) then
     pUnit:CastSpell(28630)
	 pUnit:SetStandState(1)
end
if (SurvivorDraeneiChance==3) then
     pUnit:CastSpell(28630)
     pUnit:SetCombatCapable(0)
end
end

RegisterUnitEvent(16483, 6, "DraeneiSurvivor_OnSpawn")