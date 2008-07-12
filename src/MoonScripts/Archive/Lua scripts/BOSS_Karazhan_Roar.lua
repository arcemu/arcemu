function Roar_Frightened_Scream(Unit, event, miscunit, misc)
	print "Roar Frightened Scream"
	Unit:FullCastSpellOnTarget(31013,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Scream boys...")
end

function Roar(unit, event, miscunit, misc)
	print "Roar"
	unit:RegisterEvent("Roar_Frightened_Scream",10000,0)
end

RegisterUnitEvent(17546,1,"Roar")