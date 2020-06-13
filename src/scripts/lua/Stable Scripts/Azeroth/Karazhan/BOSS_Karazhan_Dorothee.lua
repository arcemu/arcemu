-- TODO:Timer (3 seconds before goes into combat) ,enrage when tito is killed,check video when tito is summoned.
function Dorothee_Water_Bolt(Unit, event, miscunit, misc)
	Unit:ClearThreatList()
	Unit:FullCastSpellOnTarget(31012,Unit:GetRandomPlayer())
end

function Dorothee_Summon_Tito(Unit, event, miscunit, misc)
	print "Dorothee Summon Tito"
	Unit:FullCastSpell(31014)
	Unit:SendChatMessage(11, 0, "Don't let them hurt us, Tito! Oh, you won't, will you?")
end

-- titoenrage

function Dorothee_Fear(Unit, event, miscunit, misc)
	print "Dorothee Fear"
	Unit:FullCastSpellOnTarget(40453,Unit:GetClosestPlayer())
end
function DorotheeDeath(unit , event, miscunit, misc)
	unit:SendChatMessage(11, 0, "Oh at last, at last. I can go home.")
	unit:RemoveEvents()
end

function Dorothee(unit, event, miscunit, misc)
	tito = 0
	-- combatcap = 0
	print "Dorothee"
	--unit:SetCombatCapable(0)
	unit:SendChatMessage(11, 0, "Oh dear, we simply must find a way home! The old wizard could be our only hope! Strawman, Roar, Tinhead, will you... wait! Oh golly, look! We have visitors!")
	unit:RegisterEvent("Dorothee_Water_Bolt",8000,0)
	unit:RegisterEvent("Dorothee_Summon_Tito",10000,1)
	unit:RegisterEvent("Dorothee_Fear",15000,0)
end

RegisterUnitEvent(17535,1,"Dorothee")
RegisterUnitEvent(17535,4,"DorotheeDeath")