function Inciter_Knock(Unit, event, miscunit, misc)
	print "Inciter Knock"
	Unit:FullCastSpellOnTarget(11027,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will break your skull...")
end

function Inciter_Charge(Unit, event, miscunit, misc)
	print "Inciter Charge"
	Unit:FullCastSpellOnTarget(24408,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "I will send over this wall...")
end

function Inciter_Incite(Unit, event, miscunit, misc)
	print "Inciter Incite"
	Unit:FullCastSpellOnTarget(33684,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's the pain come to you...")
end

function Inciter(unit, event, miscunit, misc)
	print "Inciter"
	unit:RegisterEvent("Inciter_Knock",10000,0)
	unit:RegisterEvent("Inciter_Charge",15000,0)
	unit:RegisterEvent("Inciter_Incite",20000,1)
	unit:RegisterEvent("Inciter_Incite",60000,1)
	unit:RegisterEvent("Inciter_Incite",40000,0)
end

RegisterUnitEvent(18667,1,"Inciter")