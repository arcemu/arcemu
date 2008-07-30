function Tinhead_Rust(Unit, event, miscunit, misc)
	print "Tinhead Rust"
	Unit:FullCastSpellOnTarget(31086,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, some rust...")
end

function Tinhead_Cleave(Unit, event, miscunit, misc)
	print "Tinhead Cleave"
	Unit:FullCastSpellOnTarget(39174,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Cleave...")
end

function Tinhead(unit, event, miscunit, misc)
	print "Tinhead"
	unit:RegisterEvent("Tinhead_Rust",5000,0)
	unit:RegisterEvent("Tinhead_Cleave",10000,0)
end

RegisterUnitEvent(17547,1,"Tinhead")