function calculator_mana_tap(Unit, event, miscunit, misc)
	print "calculator mana tap"
	Unit:FullCastSpellOnTarget(36021,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Your mana is for me...")
end

function calculator_domination(Unit, event, miscunit, misc)
	print "calculator domination"
	Unit:FullCastSpellOnTarget(36866,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "You work for me now...")
end

function calculator_minions_summon(Unit)
	print "calculator minions summon"
	Unit:FullCastSpell(35285)
	Unit:FullCastSpell(35286)
	Unit:FullCastSpell(35287)
	Unit:FullCastSpell(35288)
	Unit:SendChatMessage(11, 0, "Time to supplement my work force...")
end

function calculator(unit, event, miscunit, misc)
	print "calculator"
	unit:RegisterEvent("calculator_mana_tap",8000,0)
	unit:RegisterEvent("calculator_domination",35000,0)
	unit:RegisterEvent("calculator_minions_summon",40000,0)
end

RegisterUnitEvent(19220,1,"calculator")