function Kalithresh_Reflect(Unit)
	print "Kalithresh Reflect"
	Unit:FullCastSpell(37885)
	Unit:SendChatMessage(11, 0, "I will reflect your sprell NOOB...")
end

function Kalithresh_Empale(Unit, event, miscunit, misc)
	print "Kalithresh Empale"
	Unit:FullCastSpellOnTarget(36453,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Do you like when i bash you...")
end

function Kalithresh_Rage(Unit)
	print "Kalithresh Rage"
	Unit:FullCastSpell(34451)
	Unit:SendChatMessage(11, 0, "Let's go...")
end

function Kalithresh(unit, event, miscunit, misc)
	print "Kalithresh"
	unit:RegisterEvent("Kalithresh_Reflect",8000,0)
	unit:RegisterEvent("Kalithresh_Empale",10000,0)
	unit:RegisterEvent("Kalithresh_Rage",180000,1)
end

RegisterUnitEvent(17798,1,"Kalithresh")