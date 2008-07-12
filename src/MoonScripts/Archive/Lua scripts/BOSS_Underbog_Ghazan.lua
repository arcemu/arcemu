function Ghazan_Acid_Split(Unit, event, miscunit, misc)
	print "Ghazan Acid Split"
	Unit:FullCastSpellOnTarget(34290,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now, Acid split...")
end

function Ghazan_Acid_Breath(Unit, event, miscunit, misc)
	print "Ghazan Acid Breath"
	Unit:FullCastSpellOnTarget(24839,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good breath....")
end

function Ghazan_Sweep(Unit, event, miscunit, misc)
	print "Ghazan Sweep"
	Unit:FullCastSpellOnTarget(25653,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Sweep....srry...")
end

function Ghazan(unit, event, miscunit, misc)
	print "Ghazan"
	unit:RegisterEvent("Ghazan_Acid_Split",5000,0)
	unit:RegisterEvent("Ghazan_Acid_Breath",11000,0)
	unit:RegisterEvent("Ghazan_Sweep",15000,0)
end

RegisterUnitEvent(18105,1,"Ghazan")