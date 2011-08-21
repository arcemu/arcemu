function Watchkeeper_Gargolmar_Hamstring(Unit, event, miscunit, misc)
	print "Watchkeeper Gargolmar Hamstring"
	Unit:FullCastSpellOnTarget(26141,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Hamstring on you...")
end

function Watchkeeper_Gargolmar_Stun(Unit, event, miscunit, misc)
	print "Watchkeeper Gargolmar Stun"
	Unit:FullCastSpellOnTarget(20170,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good Stun...")
end

function Watchkeeper_Gargolmar_Revenge(Unit, event, miscunit, misc)
	print "Watchkeeper Gargolmar Revenge"
	Unit:FullCastSpellOnTarget(11603,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Revenge...")
end

function Watchkeeper(unit, event, miscunit, misc)
	print "Watchkeeper"
	unit:RegisterEvent("Watchkeeper_Gargolmar_Hamstring",5000,0)
	unit:RegisterEvent("Watchkeeper_Gargolmar_Stun",9000,0)
	unit:RegisterEvent("Watchkeeper_Gargolmar_Revenge",13000,0)
end

RegisterUnitEvent(17306,1,"Watchkeeper")