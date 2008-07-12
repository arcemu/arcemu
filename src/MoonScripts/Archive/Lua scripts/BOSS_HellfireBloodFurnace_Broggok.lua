function Broggok_Ring_Poison(Unit, event, miscunit, misc)
	print "Broggok Ring Poison"
	Unit:FullCastSpellOnTarget(29865,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ring of Poison...")
end

function Broggok_Poison_Bolt(Unit, event, miscunit, misc)
	print "Broggok Poison Bolt"
	Unit:FullCastSpellOnTarget(37862,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Rain of Poison Bolt...")
end

function Broggok(unit, event, miscunit, misc)
	print "Broggok"
	unit:RegisterEvent("Broggok_Ring_Poison",9000,0)
	unit:RegisterEvent("Broggok_Poison_Bolt",17000,0)
end

RegisterUnitEvent(17380,1,"Broggok")