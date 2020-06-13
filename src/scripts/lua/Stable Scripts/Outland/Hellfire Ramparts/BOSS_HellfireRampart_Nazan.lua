function Nazan_Fireball(Unit, event, miscunit, misc)
	print "Nazan Fireball"
	Unit:FullCastSpellOnTarget(37111,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ha..Ho...A good fire ball...")
end

function Nazan_Cone_Fire(Unit, event, miscunit, misc)
	print "Nazan Cone Fire"
	Unit:FullCastSpellOnTarget(30926,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Cone of fire...")
end

function Nazan(unit, event, miscunit, misc)
	print "Nazan"
	unit:RegisterEvent("Nazan_Fireball",5000,0)
	unit:RegisterEvent("Nazan_Cone_Fire",11000,0)
end

RegisterUnitEvent(17536,1,"Nazan")