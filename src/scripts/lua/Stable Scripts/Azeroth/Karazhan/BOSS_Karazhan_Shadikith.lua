function Shadikith_Charge(Unit, event, miscunit, misc)
	print "Shadikith Charge"
	Unit:FullCastSpellOnTarget(38461,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Charge...")
end

function Shadikith_Sonic_Burst(Unit, event, miscunit, misc)
	print "Shadikith Sonic Burst"
	Unit:FullCastSpellOnTarget(39052,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, A nice burst...")
end

function Shadikith_Wing_Buffet(Unit, event, miscunit, misc)
	print "Romulo Daring"
	Unit:FullCastSpellOnTarget(38110,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's Rock...")
end

function Shadikith_Knockback(Unit, event, miscunit, misc)
	print "Romulo Poisoned Thrust"
	Unit:FullCastSpellOnTarget(37966,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Now, let's do more damage...")
end

function Shadikith(unit, event, miscunit, misc)
	print "Shadikith"
	unit:RegisterEvent("Shadikith_Charge",10000,0)
	unit:RegisterEvent("Shadikith_Sonic_Burst",17000,0)
	unit:RegisterEvent("Shadikith_Wing_Buffet",27000,0)
	unit:RegisterEvent("Shadikith_Knockback",33000,0)
end

RegisterUnitEvent(16180,1,"Shadikith")