function Soccothrates_Immolation(Unit, event, miscunit, misc)
	print "Soccothrates Immolation"
	Unit:FullCastSpellOnTarget(36051,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You will suffer...")
end

function Soccothrates_Shock(Unit, event, miscunit, misc)
	print "Soccothrates Shock"
	Unit:FullCastSpellOnTarget(22423,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's the lightning crush you...")
end

function Soccothrates_Charge(Unit, event, miscunit, misc)
	print "Soccothrates Charge"
	Unit:FullCastSpellOnTarget(36058,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will send you over the wall...")
end

function Soccothrates(unit, event, miscunit, misc)
	print "Soccothrates"
	unit:RegisterEvent("Soccothrates_Immolation",8000,0)
	unit:RegisterEvent("Soccothrates_Shock",11000,0)
	unit:RegisterEvent("Soccothrates_Charge",21000,0)
end

RegisterUnitEvent(20886,1,"Soccothrates")