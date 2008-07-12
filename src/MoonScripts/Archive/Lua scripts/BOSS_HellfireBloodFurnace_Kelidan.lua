function Kelidan_Shadow_Bolt_Volley(Unit, event, miscunit, misc)
	print "Kelidan Shadow Bolt Volley"
	Unit:FullCastSpellOnTarget(36736,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Rain of shadow Bolt...")
end

function Kelidan_Burning_Nova(Unit, event, miscunit, misc)
	print "Kelidan Burning Nova"
	Unit:FullCastSpellOnTarget(20229,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Fire...Fire...on you...")
end

function Kelidan(unit, event, miscunit, misc)
	print "Kelidan"
	unit:RegisterEvent("Kelidan_Shadow_Bolt_Volley",9000,0)
	unit:RegisterEvent("Kelidan_Burning_Nova",14000,0)
end

RegisterUnitEvent(17377,1,"Kelidan")