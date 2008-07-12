function Murmur_Sonic_Boom(Unit, event, miscunit, misc)
	print "Murmur Sonic Boom"
	Unit:FullCastSpellOnTarget(33666,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Boom...")
end

function Murmur_Touch(Unit, event, miscunit, misc)
	print "Murmur Touch"
	Unit:FullCastSpellOnTarget(33711,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Let me squeeze you...")
end

function Murmur_Resonnance(Unit, event, miscunit, misc)
	print "Murmur Resonnance"
	Unit:FullCastSpellOnTarget(33657,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "The ground will Collapse...")
end

function Murmur(unit, event, miscunit, misc)
	print "Murmur"
	unit:RegisterEvent("Murmur_Sonic_Boom",60000,0)
	unit:RegisterEvent("Murmur_Touch",65000,0)
	unit:RegisterEvent("Murmur_Resonnance",85000,0)
end

RegisterUnitEvent(18708,1,"Murmur")