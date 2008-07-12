function Yor_Fire_Cone(Unit, event, miscunit, misc)
	print "Yor Fire Cone"
	Unit:FullCastSpellOnTarget(19630,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Fire...Fire...Fire...")
end

function Yor_Earthquake(Unit, event, miscunit, misc)
	print "Yor Earthquake"
	Unit:FullCastSpellOnTarget(33919,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Earthquake...")
end

function Yor(unit, event, miscunit, misc)
	print "Yor"
	unit:RegisterEvent("Yor_Fire_Cone",10000,0)
	unit:RegisterEvent("Yor_Earthquake",13000,0)
end

RegisterUnitEvent(22930,1,"Yor")