function Zereketh_Void_Zone(Unit, event, miscunit, misc)
	print "Zereketh Void Zone"
	Unit:FullCastSpellOnTarget(30533,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "The Void Zone will steal your life...")
end

function Zereketh_Corruption(Unit, event, miscunit, misc)
	print "Zereketh Corruption"
	Unit:FullCastSpellOnTarget(32863,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "I will corrupt you...")
end

function Zereketh_Shadow_Nova(Unit, event, miscunit, misc)
	print "Zereketh Shadow Nova"
	Unit:FullCastSpellOnTarget(36127,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's Shadow force kill you...")
end

function Zereketh(unit, event, miscunit, misc)
	print "Zereketh"
	unit:RegisterEvent("Zereketh_Void_Zone",10000,0)
	unit:RegisterEvent("Zereketh_Corruption",15000,0)
	unit:RegisterEvent("Zereketh_Shadow_Nova",23000,0)
end

RegisterUnitEvent(20886,1,"Zereketh")