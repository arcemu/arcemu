function Porung_Fear(Unit, event, miscunit, misc)
	print "Porung Fear"
	Unit:FullCastSpellOnTarget(30530,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Fear...Ha ha ha")
end

function Porung_Cleave(Unit, event, miscunit, misc)
	print "Porung Cleave"
	Unit:FullCastSpellOnTarget(38474,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Do you like when i cleave you...")
end

function Porung(unit, event, miscunit, misc)
	print "Porung"
	unit:RegisterEvent("Porung_Fear",10000,0)
	unit:RegisterEvent("Porung_Cleave",15000,0)

end

RegisterUnitEvent(20923,1,"Porung")