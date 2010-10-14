function Vazruden_Hamstring(Unit, event, miscunit, misc)
	print "Vazruden Hamstring"
	Unit:FullCastSpellOnTarget(26141,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's do a Hamstring...")
end

function Vazruden_Stun(Unit, event, miscunit, misc)
	print "Vazruden Stun"
	Unit:FullCastSpellOnTarget(20170,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good stun...")
end

function Vazruden_Revenge(Unit, event, miscunit, misc)
	print "Vazruden Revenge"
	Unit:FullCastSpellOnTarget(11603,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "REVENGE...")
end

function Vazruden_Knockback(Unit, event, miscunit, misc)
	print "Vazruden Knockback"
	Unit:FullCastSpellOnTarget(30056,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will send on this wall...")
end

function Vazruden(unit, event, miscunit, misc)
	print "Vazruden"
	unit:RegisterEvent("Vazruden_Hamstring",9000,0)
	unit:RegisterEvent("Vazruden_Stun",13000,0)
	unit:RegisterEvent("Vazruden_Revenge",17000,0)
	unit:RegisterEvent("Vazruden_Knockback",25000,0)
end

RegisterUnitEvent(17537,1,"Vazruden")