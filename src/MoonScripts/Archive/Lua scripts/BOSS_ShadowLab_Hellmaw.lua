function Hellmaw_Terrifying_Howl(Unit, event, miscunit, misc)
	print "Hellmaw Terrifying Howl"
	Unit:FullCastSpellOnTarget(30752,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You like afraid...")
end

function Hellmaw_Corrosive_Acide(Unit, event, miscunit, misc)
	print "Hellmaw Corrosive Acide"
	Unit:FullCastSpellOnTarget(23314,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Some acid for diner...")
end

function Hellmaw(unit, event, miscunit, misc)
	print "Hellmaw"
	unit:RegisterEvent("Hellmaw_Terrifying_Howl",20000,0)
	unit:RegisterEvent("Hellmaw_Corrosive_Acide",23000,0)
end

RegisterUnitEvent(18731,1,"Hellmaw")