


function EclipsionSpawn(pUnit, Event)
	
	x = pUnit:GetX();
	y = pUnit:GetY();
	z = pUnit:GetZ();
	o = pUnit:GetO();
	
	pUnit:SpawnCreature(21627, x, y, z, o, 23000);
	
end

function EclipsionDespawn(pUnit, Event)

	pUnit:Despawn(1000, 0);
	
end

RegisterUnitEvent(19806, 1, "EclipsionSpawn");
RegisterUnitEvent(22018, 1, "EclipsionSpawn");
RegisterUnitEvent(21627, 2, "EclipsionDespawn");

function DoomWalker_Earthquake(unit)
unit:CastSpell(32686)
unit:SendChatMessage(12, 0, "Magnitude set. Release.")
unit:PlaySoundToSet (11345)
end

function DoomWalker_overrun(unit)
unit:CastSpell(32637)
unit:SendChatMessage(12, 0, "Trajectory Locked.")
unit:PlaySoundToSet (11347)
end

function DoomWalker_ChainLightning(unit)
local plr = unit:GetClosestPlayer()
if (plr ~= nil) then
unit:CastSpellOnTarget(33665, plr) 
unit:PlaySoundToSet (11346)
end
end

function DoomWalker_SunderArmor(unit)
local plr = unit:GetClosestPlayer()
if (plr ~= nil) then
unit:FullCastSpellOnTarget(30901, plr) 
end
end

function DoomWalker_OnEnterCombat(unit)
unit:SendChatMessage(12, 0, "Do not proceed. You will be eliminated.")
unit:PlaySoundToSet (11344)
unit:RegisterEvent("DoomWalker_ChainLightning",23000, 0)
unit:RegisterEvent("DoomWalker_SunderArmor",10000, 0)
unit:RegisterEvent("DoomWalker_Earthquake",50000, 0)
unit:RegisterEvent("DoomWalker_overrun",120000, 0) 
end

function DoomWalker_OnLeaveCombat(unit)
unit:RemoveEvents()
end

function DoomWalker_KilledTarget(unit)
unit:SendChatMessage(12, 0, "Target Exterminated.")
unit:PlaySoundToSet (11351)
unit:RemoveEvents()
end

function DoomWalker_OnDied(unit)
unit:SendChatMessage(12, 0, "System failure in five....four....")
unit:PlaySoundToSet (11352)
unit:RemoveEvents()
end



RegisterUnitEvent(17711, 1, "DoomWalker_OnEnterCombat")
RegisterUnitEvent(17711, 2, "DoomWalker_OnLeaveCombat")
RegisterUnitEvent(17711, 3, "DoomWalker_OnKilledTarget")
RegisterUnitEvent(17711, 4, "DoomWalker_OnDied") 