function Talon_Arcane_Volley(Unit, event, miscunit, misc)
	print "Talon Arcane Volley"
	Unit:FullCastSpellOnTarget(36738,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's acarne rain start...")
end

function Talon_Arcane_Explosion(Unit, event, miscunit, misc)
	print "Talon Arcane Explosion"
	Unit:FullCastSpellOnTarget(38197,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "BOOM...")
end

function Talon_Blink(Unit)
	print "Talon Blink"
	Unit:FullCastSpell(1953)
	Unit:SendChatMessage(11, 0, "Catch me...")
end

function Talon_Polymorph(Unit, event, miscunit, misc)
	print "Talon Polymorph"
	Unit:FullCastSpellOnTarget(12826,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will transforme you...")
end

function Talon(unit, event, miscunit, misc)
	print "Talon"
	unit:RegisterEvent("Talon_Arcane_Volley",10000,0)
	unit:RegisterEvent("Talon_Arcane_Explosion",16000,0)
	unit:RegisterEvent("Talon_Blink",23000,0)
	unit:RegisterEvent("Talon_Polymorph",33000,0)
end

RegisterUnitEvent(18473,1,"Talon")