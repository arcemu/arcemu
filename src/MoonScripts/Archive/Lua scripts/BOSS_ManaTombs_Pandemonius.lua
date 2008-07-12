function Pandemonius_Void_Blast(Unit, event, miscunit, misc)
	print "Pandemonius Void Blast"
	Unit:FullCastSpellOnTarget(32325,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good Void Blast...")
end

function Pandemonius_Dark_Shell(Unit, event, miscunit, misc)
	print "Pandemonius Dark Shell"
	Unit:FullCastSpellOnTarget(32358,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Good move...For me...srry...")
end

function Pandemonius(unit, event, miscunit, misc)
	print "Pandemonius"
	unit:RegisterEvent("Pandemonius_Void_Blast",9000,0)
	unit:RegisterEvent("Pandemonius_Dark_Shell",13000,0)
end

RegisterUnitEvent(18341,1,"Pandemonius")