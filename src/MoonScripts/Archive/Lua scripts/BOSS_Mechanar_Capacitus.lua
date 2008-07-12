function capacitus_head_crack(Unit, event, miscunit, misc)
	print "capacitus head crack"
	Unit:FullCastSpellOnTarget(35161,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function capacitus_reflective_damage_shield(Unit)
	print "capacitus reflective damage shield"
	Unit:FullCastSpell(35159)
	Unit:SendChatMessage(11, 0, "Think you can hurt me, huh? Think I'm afraid a' you?...")
end

function capacitus_reflective_magic_shield(Unit)
	print "capacitus reflective magic shield"
	Unit:FullCastSpell(35158)
	Unit:SendChatMessage(11, 0, "Go ahead, gimme your best shot. I can take it!")
end

function capacitus_seed_of_corruption(Unit, event, miscunit, misc)
	print "capacitus seed of corruption"
	Unit:FullCastSpellOnTarget(37826,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function capacitus(unit, event, miscunit, misc)
	print "capacitus"
	unit:RegisterEvent("capacitus_head_crack",15000,0)
	unit:RegisterEvent("capacitus_reflective_damage_shield",20000,0)
	unit:RegisterEvent("capacitus_reflective_magic_shield",30000,0)
	unit:RegisterEvent("capacitus_seed_of_corruption",300000,0)
end

RegisterUnitEvent(19219,1,"capacitus")