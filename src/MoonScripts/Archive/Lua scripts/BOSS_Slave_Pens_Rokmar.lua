function Rokmar_Grievous_Wound(Unit, event, miscunit, misc)
	print "Rokmar Grievous Wound"
	Unit:FullCastSpellOnTarget(30923,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Bad wound...")
end

function Rokmar_Water_Split(Unit, event, miscunit, misc)
	print "Rokmar Water Split"
	Unit:FullCastSpellOnTarget(38973,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Grrrr...")
end

function Rokmar_Moss(Unit)
	print "Rokmar Moss"
	Unit:FullCastSpellOnTarget(30925,unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Grrrr...")
end

function Rokmar(unit, event, miscunit, misc)
	print "Rokmar"
	unit:RegisterEvent("Rokmar_Grievous_Wound",10000,0)
	unit:RegisterEvent("Rokmar_Water_Split",13000,0)
	unit:RegisterEvent("Rokmar_Moss",15000,0)
end

RegisterUnitEvent(17991,1,"Rokmar")