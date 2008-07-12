function Dorothee_Water_Bolt(Unit, event, miscunit, misc)
	print "Dorothee Water Bolt"
	Unit:FullCastSpellOnTarget(31012,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "You like water...")
end

function Dorothee_Summon_Tito(Unit, event, miscunit, misc)
	print "Dorothee Summon Tito"
	Unit:FullCastSpell(31014)
	Unit:SendChatMessage(11, 0, "Come to help me Tito...")
end

function Dorothee_Fear(Unit, event, miscunit, misc)
	print "Dorothee Fear"
	Unit:FullCastSpellOnTarget(40453,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Fear for all of you...")
end
function Dorothee(unit, event, miscunit, misc)
	print "Dorothee"
	unit:RegisterEvent("Dorothee_Water_Bolt",8000,0)
	unit:RegisterEvent("Dorothee_Summon_Tito",10000,1)
	unit:RegisterEvent("Dorothee_Fear",15000,0)
end

RegisterUnitEvent(17535,1,"Dorothee")