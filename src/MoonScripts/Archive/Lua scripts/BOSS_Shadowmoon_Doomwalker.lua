function Doomwalker_Enrage(Unit, event, miscunit, misc)
	if Unit:GetHealthPct() < 20 and Didthat == 0 then
		Unit:FullCastSpell(34670)
		Didthat = 1
	else
	end
end

function Doomwalker_Earthquake(Unit, event, miscunit, misc)
	print "Doomwalker Earthquake"
	Unit:FullCastSpellOnTarget(32686,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Earthquake...")
end

function Doomwalker_Chain_Light(Unit, event, miscunit, misc)
	print "Doomwalker Chain_Light"
	Unit:FullCastSpellOnTarget(28167,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Now a Chain Lightning...")
end

function Doomwalker_Overrun(Unit, event, miscunit, misc)
	print "Doomwalker Charge"
	Unit:FullCastSpellOnTarget(32637,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Now you will die...")
end

function Doomwalker(unit, event, miscunit, misc)
	print "Doomwalker"
	unit:RegisterEvent("Doomwalker_Enrage",1000,1)
	unit:RegisterEvent("Doomwalker_Earthquake",10000,0)
	unit:RegisterEvent("Doomwalker_Chain_Light",17000,0)
	unit:RegisterEvent("Doomwalker_Overrun",23000,0)
end

RegisterUnitEvent(17711,1,"Doomwalker")