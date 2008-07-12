function Tito_Annoying_Yipping(Unit, event, miscunit, misc)
	print "Tito Annoying Yipping"
	Unit:FullCastSpellOnTarget(31015,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Ho. your are unable to attack or cast spells...")
end

function Tito(unit, event, miscunit, misc)
	print "Tito"
	unit:RegisterEvent("Tito_Annoying_Yipping",10000,0)
end

RegisterUnitEvent(17548,1,"Tito")