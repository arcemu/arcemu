function Shirrak_Inhibit_Magic(Unit, event, miscunit, misc)
	print "Shirrak Inhibit Magic"
	Unit:FullCastSpellOnTarget(32264,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Your weak...")
end

function Shirrak_Attract_Magic(Unit, event, miscunit, misc)
	print "Shirrak Attract Magic"
	Unit:FullCastSpellOnTarget(32265,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "No escape for you...")
end

function Shirrak_Carnivurous_Bite(Unit, event, miscunit, misc)
	print "Shirrak Carnivurous Bite"
	Unit:FullCastSpellOnTarget(41092,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's do a good bite...")
end

function Shirrak_Fire_Nova(Unit, event, miscunit, misc)
	print "Shirrak Fire Nova"
	Unit:FullCastSpellOnTarget(38728,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's Fire Nova kill you...")
end

function Shirrak(unit, event, miscunit, misc)
	print "Shirrak"
	unit:RegisterEvent("Shirrak_Inhibit_Magic",9000,0)
	unit:RegisterEvent("Shirrak_Attract_Magic",13000,0)
	unit:RegisterEvent("Shirrak_Carnivurous_Bite",17000,0)
	unit:RegisterEvent("Shirrak_Fire_Nova",23000,0)
end

RegisterUnitEvent(18371,1,"Shirrak")