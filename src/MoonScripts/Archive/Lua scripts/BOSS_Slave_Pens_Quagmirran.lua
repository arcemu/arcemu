function Quagmirran_Acid_Gayser(Unit, event, miscunit, misc)
	print "Quagmirran Acid Gayser"
	Unit:FullCastSpellOnTarget(38739,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "BAM...")
end

function Quagmirran_Poison_Bolt(Unit, event, miscunit, misc)
	print "Quagmirran Poison Bolt"
	Unit:FullCastSpellOnTarget(38655,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Some Poison...")
end

function Quagmirran(unit, event, miscunit, misc)
	print "Quagmirran"
	unit:RegisterEvent("Quagmirran_Acid_Gayser",8000,0)
	unit:RegisterEvent("Quagmirran_Poison_Bolt",11000,0)
end

RegisterUnitEvent(17942,1,"Quagmirran")