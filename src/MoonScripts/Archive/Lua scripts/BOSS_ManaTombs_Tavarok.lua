function Tavarok_Earthquake(Unit, event, miscunit, misc)
	print "Tavarok Earthquake"
	Unit:FullCastSpellOnTarget(33919,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Do you feel my Earthquake...")
end

function Tavarok_Crystal_Poison(Unit, event, miscunit, misc)
	print "Tavarok Crystal Poison"
	Unit:FullCastSpellOnTarget(32361,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Some poison...")
end

function Tavarok(unit, event, miscunit, misc)
	print "Tavarok"
	unit:RegisterEvent("Tavarok_Earthquake",10000,0)
	unit:RegisterEvent("Tavarok_Crystal_Poison",13000,0)
end

RegisterUnitEvent(18343,1,"Tavarok")