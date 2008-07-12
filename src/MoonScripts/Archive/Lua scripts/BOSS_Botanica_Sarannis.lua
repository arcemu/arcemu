function Sarannis_Arcane_Resonance(Unit, event, miscunit, misc)
	print "Sarannis Arcane Resonance"
	Unit:FullCastSpellOnTarget(34794,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's Rock...")
end

function Sarannis_Arcane_Devastation(Unit, event, miscunit, misc)
	print "Sarannis Arcane Devastation"
	Unit:FullCastSpellOnTarget(34799,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function Sarannis(unit, event, miscunit, misc)
	print "Sarannis"
	unit:RegisterEvent("Sarannis_Arcane_Resonance",10000,0)
	unit:RegisterEvent("Sarannis_Arcane_Devastation",21000,0)
end

RegisterUnitEvent(17976,1,"Sarannis")