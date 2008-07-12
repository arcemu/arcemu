function ironhand_hammer_punch(Unit, event, miscunit, misc)
	print "ironhand hammer punch"
	Unit:FullCastSpellOnTarget(35326,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good punch on your nose...")
end

function ironhand_steam_of_mahcine_fluid_ihand(Unit, event, miscunit, misc)
	print "ironhand steam of mahcine fluid ihand"
	Unit:FullCastSpellOnTarget(35311,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "FIGHT...")
end

function ironhand_shadow_power_ihand(Unit)
	print "ironhand shadow power ihand"
	Unit:FullCastSpell(35322)
	Unit:SendChatMessage(11, 0, "If my devision is correct, you should be quiet dead!")
end

function ironhand_jack_hammer(Unit)
	print "ironhand jack hammer"
	Unit:FullCastSpell(35327)
	Unit:SendChatMessage(11, 0, "Low tech yet quiet effective!...")
end

function ironhand(unit, event, miscunit, misc)
	print "ironhand"
	unit:RegisterEvent("ironhand_hammer_punch",15000,0)
	unit:RegisterEvent("ironhand_steam_of_mahcine_fluid_ihand",20000,0)
	unit:RegisterEvent("ironhand_shadow_power_ihand",25000,0)
	unit:RegisterEvent("ironhand_jack_hammer",30000,0)
end

RegisterUnitEvent(19710,1,"ironhand")