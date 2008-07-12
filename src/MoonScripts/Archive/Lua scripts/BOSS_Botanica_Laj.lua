function Laj_Allergic(Unit, event, miscunit, misc)
	print "Laj Allergic"
	Unit:FullCastSpellOnTarget(34697,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's go Sniff...")
end

function Laj_Flayer(Unit)
	print "Laj Flayer"
	Unit:FullCastSpell(34682)
	Unit:SendChatMessage(11, 0, "Come to help me...")
end

function Laj_Lasher(Unit)
	print "Laj Lasher"
	Unit:FullCastSpell(34681)
	Unit:SendChatMessage(11, 0, "Come to help me...")
end

function Laj(unit, event, miscunit, misc)
	print "Laj"
	unit:RegisterEvent("Laj_Allergic",10000,0)
	unit:RegisterEvent("Laj_Flayer",21000,0)
	unit:RegisterEvent("Laj_Lasher",22000,0)
end

RegisterUnitEvent(17980,1,"Laj")