function Huntsman_Mount(Unit, event, miscunit, misc)
	if Unit:GetHealthPct(16151) < 25 and Didthat == 0 then
		print "Huntsman Mount"
		Unit:FullCastSpell(29770)
		Didthat = 1
	else
	end
end

function Huntsman_Shadow_Cleave(Unit, event, miscunit, misc)
	print "Huntsman Shadow Cleave"
	Unit:FullCastSpellOnTarget(29832,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Shadow Cleave on you...")
end

function Huntsman_Intangible_Presence(Unit, event, miscunit, misc)
	print "Huntsman Intangible Presence"
	Unit:FullCastSpellOnTarget(29833,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Try to hit me now...")
end

function Huntsman_Berserker_Charge(Unit, event, miscunit, misc)
	print "Huntsman Berserker Charge"
	Unit:FullCastSpellOnTarget(26561,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "CHARGE...")
end

function Huntsman(unit, event, miscunit, misc)
	print "Huntsman"
	unit:RegisterEvent("Huntsman_Mount",1000,1)
	unit:RegisterEvent("Huntsman_Shadow_Cleave",8000,0)
	unit:RegisterEvent("Huntsman_Intangible_Presence",30000,0)
	unit:RegisterEvent("Huntsman_Berserker_Charge",600000,0)
end

RegisterUnitEvent(15550,1,"Huntsman")