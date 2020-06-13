function Trickster_FireBoltt(Unit, event, miscunit, misc)
	print "Trickster FireBoltt"
	Unit:FullCastSpellOnTarget(36906,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Try to catch that...")
end

function Trickster_FireShield(Unit)
	print "Trickster FireShield"
	Unit:FullCastSpell(36907)
	Unit:SendChatMessage(11, 0, "Now i will be protect form fire.... Noob.")
end

function Trickster_Charge(Unit, event, miscunit, misc)
	print "Trickster Charge"
	Unit:FullCastSpellOnTarget(36058,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will send you over the wall...")
end

function Trickster(unit, event, miscunit, misc)
	print "Trickster"
	unit:RegisterEvent("Trickster_FireBolt",10000,0)
	unit:RegisterEvent("Trickster_FireShield",15000,0)
	unit:RegisterEvent("Trickster_Charge",21000,0)
end

RegisterUnitEvent(20905,1,"Trickster")