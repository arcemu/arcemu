function sepethrea_minions_summon(Unit)
	print "sepethrea minions summon"
	Unit:FullCastSpell(35275)
	Unit:SendChatMessage(11, 0, "I am not alone!...")
end

function sepethrea_arcane_blast(Unit, event, miscunit, misc)
	print "sepethrea arcane blast"
	Unit:FullCastSpellOnTarget(35314,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "FIGHT...")
end

function sepethrea_knockback(Unit, event, miscunit, misc)
	print "ironhand shadow power ihand"
	Unit:FullCastSpellOnTarget(37317,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "FIGHT...")
end

function sepethrea_frost_attack(Unit, event, miscunit, misc)
	print "sepethrea frost attack"
	Unit:FullCastSpellOnTarget(35263,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "FIGHT...")
end

function sepethrea_dragons_breath(Unit, event, miscunit, misc)
	print "sepethrea dragons breath"
	Unit:FullCastSpellOnTarget(35250,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Think you can take the heat?...")
end

function sepethrea(unit, event, miscunit, misc)
	print "sepethrea"
	unit:RegisterEvent("sepethrea_minions_summon",3000,1)
	unit:RegisterEvent("sepethrea_arcane_blast",8000,0)
	unit:RegisterEvent("sepethrea_knockback",8000,0)
	unit:RegisterEvent("sepethrea_frost_attack",10000,0)
	unit:RegisterEvent("sepethrea_dragons_breath",25000,0)
end

RegisterUnitEvent(19221,1,"sepethrea")