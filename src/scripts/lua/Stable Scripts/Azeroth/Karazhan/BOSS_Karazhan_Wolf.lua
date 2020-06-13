function Wolf_Burning_Terrifying_Howl(Unit, event, miscunit, misc)
	print "Wolf Burning Terrifying Howl"
	Unit:FullCastSpellOnTarget(8715,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, you are terrify guys...")
end

function Wolf_Wide_Swipe(Unit, event, miscunit, misc)
	print "Wolf_Wide_Swipe"
	Unit:FullCastSpellOnTarget(6749,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A gooooood swipe...")
end

function Wolf_Riding_Hood(Unit, event, miscunit, misc)
	print "Wolf Riding Hood"
	Unit:FullCastSpellOnTarget(30756,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "HO... The little red riding hood...Miam...Miam...")
end

function Wolf(unit, event, miscunit, misc)
	print "Wolf"
	unit:RegisterEvent("Wolf_Burning_Terrifying_Howl",25000,0)
	unit:RegisterEvent("Wolf_Wide_Swipe",27000,0)
	unit:RegisterEvent("Wolf_Riding_Hood",35000,0)
end

RegisterUnitEvent(17521,1,"Wolf")