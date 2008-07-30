function Maker_Domination(Unit, event, miscunit, misc)
	print "Maker_Domination"
	Unit:FullCastSpellOnTarget(30923,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "I will dominate you...")
end

function Maker_Acid_Spray(Unit, event, miscunit, misc)
	print "Maker Acid Spray"
	Unit:FullCastSpellOnTarget(38973,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Acid Spray...on you...")
end

function Maker_Trow_Beaker(Unit, event, miscunit, misc)
	print "Maker_Trow_Beaker"
	Unit:FullCastSpellOnTarget(30925,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Catch that...")
end

function Maker(unit, event, miscunit, misc)
	print "Maker"
	unit:RegisterEvent("Maker_Domination",10000,0)
	unit:RegisterEvent("Maker_Acid_Spray",14000,0)
	unit:RegisterEvent("Maker_Trow_Beaker",21000,0)
end

RegisterUnitEvent(17381,1,"Maker")