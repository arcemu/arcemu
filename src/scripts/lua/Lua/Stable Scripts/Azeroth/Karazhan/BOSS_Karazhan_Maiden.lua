function Maiden_Holy_Fire(Unit, event, miscunit, misc)
	print "Maiden Holy Fire"
	Unit:FullCastSpellOnTarget(29522,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "You like Holy Fire...")
end

function Maiden_Holy_Wrath(Unit, event, miscunit, misc)
	print "Maiden Holy Wrath"
	Unit:FullCastSpellOnTarget(32445,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now, Holy Wrath...")
end

function Maiden_Repentance(Unit, event, miscunit, misc)
	print "Maiden Repentance"
	Unit:FullCastSpellOnTarget(29511,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Repentance on you guys...")
end

function Maiden_Holy_Ground(Unit, event, miscunit, misc)
	print "Maiden Holy Ground"
	Unit:FullCastSpellOnTarget(29512,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now, Holy Ground...")
end

function Maiden(unit, event, miscunit, misc)
	print "Maiden"
	unit:RegisterEvent("Maiden_Holy_Fire",15000,0)
	unit:RegisterEvent("Maiden_Holy_Wrath",20000,0)
	unit:RegisterEvent("Maiden_Repentance",25000,0)
	unit:RegisterEvent("Maiden_Holy_Ground",30000,0)
end

RegisterUnitEvent(16457,1,"Maiden")