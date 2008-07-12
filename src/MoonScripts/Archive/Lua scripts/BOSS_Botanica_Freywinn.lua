function Freywinn_Heal(Unit)
	print "Freywinn Heal"
	Unit:FullCastSpell(25817)
	Unit:SendChatMessage(11, 0, "Heal....")
end

function Freywinn_White(Unit, event, miscunit, misc)
	print "Freywinn White"
	Unit:FullCastSpellOnTarget(34759,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Grrrr...")
end

function Freywinn_Green(Unit, event, miscunit, misc)
	print "Freywinn Green"
	Unit:FullCastSpellOnTarget(34761,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Grrrr...")
end

function Freywinn_Blue(Unit, event, miscunit, misc)
	print "Freywinn Blue"
	Unit:FullCastSpellOnTarget(34762,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Grrrr...")
end

function Freywinn_Red(Unit, event, miscunit, misc)
	print "Freywinn Red"
	Unit:FullCastSpellOnTarget(34763,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Grrrr...")
end

function Freywinn(unit, event, miscunit, misc)
	print "Freywinn"
	unit:RegisterEvent("Freywinn_Heal",15000,0)
	unit:RegisterEvent("Freywinn_White",19000,0)
	unit:RegisterEvent("Freywinn_Green",23000,0)
	unit:RegisterEvent("Freywinn_Blue",27000,0)
	unit:RegisterEvent("Freywinn_Red",35000,0)
end

RegisterUnitEvent(17975,1,"Freywinn")