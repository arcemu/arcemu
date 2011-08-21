function Akkiris_Red(Unit, event, miscunit, misc)
	print "Akkiris Red"
	Unit:FullCastSpellOnTarget(14264,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You will pay the price...")
end

function Akkiris_Wave(Unit, event, miscunit, misc)
	print "Akkiris Wave"
	Unit:FullCastSpellOnTarget(23331,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Pain is coming for you...")
end

function Akkiris_FireBolt(Unit, event, miscunit, misc)
	print "Akkiris FireBolt"
	Unit:FullCastSpellOnTarget(36906,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's put some heat...")
end
function Akkiris(unit, event, miscunit, misc)
	print "Akkiris"
	unit:RegisterEvent("Akkiris_Red",10000,0)
	unit:RegisterEvent("Akkiris_Wave",13000,0)
	unit:RegisterEvent("Akkiris_FireBolt",21000,0)
end

RegisterUnitEvent(20908,1,"Akkiris")