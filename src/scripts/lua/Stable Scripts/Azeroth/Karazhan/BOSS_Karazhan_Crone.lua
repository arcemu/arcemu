function Crone_Summon_Cyclone(Unit, event, miscunit, misc)
	print "Crone Summon Cyclone"
	Unit:FullCastSpellOnTarget(38337,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "You cannot move...")
end

function Crone_Chain_Lightning(Unit, event, miscunit, misc)
	print "Crone Chain Lightning"
	Unit:FullCastSpellOnTarget(32337,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Chain lightning...")
end

function Crone(unit, event, miscunit, misc)
	print "Crone"
	unit:RegisterEvent("Crone_Summon_Cyclone",10000,0)
	unit:RegisterEvent("Crone_Chain_Lightning",17000,0)

end

RegisterUnitEvent(18168,1,"Crone")