function Stalker_Charge(Unit, event, miscunit, misc)
	print "Stalker Charge"
	Unit:FullCastSpellOnTarget(31715,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "CHARGE...")
end

function Stalker_Light(Unit, event, miscunit, misc)
	print "Stalker Light"
	Unit:FullCastSpellOnTarget(31330,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will kill you...")
end

function Stalker_Levitate(Unit, event, miscunit, misc)
	print "Stalker Levitate"
	Unit:FullCastSpellOnTarget(31704,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "You want to fly...")
end

function Stalker(unit, event, miscunit, misc)
	print "Stalker"
	unit:RegisterEvent("Stalker_Charge",7000,0)
	unit:RegisterEvent("Stalker_Light",8000,0)
	unit:RegisterEvent("Stalker_Levitate",11000,0)
end

RegisterUnitEvent(17882,1,"Stalker")