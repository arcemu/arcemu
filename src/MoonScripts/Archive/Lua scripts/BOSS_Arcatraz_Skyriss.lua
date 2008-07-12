function Skyriss_Red(Unit, event, miscunit, misc)
	print "Skyriss_Red"
	Unit:FullCastSpellOnTarget(14264,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Kill...")
end

function Skyriss_Blue(Unit, event, miscunit, misc)
	print "Skyriss Blue"
	Unit:FullCastSpellOnTarget(14261,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Kill...")
end

function Skyriss_Rend(Unit, event, miscunit, misc)
	print "Skyriss Rend"
	Unit:FullCastSpellOnTarget(36924,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Die...")
end

function Skyriss_Control(Unit, event, miscunit, misc)
	print "Skyriss Control"
	Unit:FullCastSpellOnTarget(11446,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will take your Mind...")
end

function Skyriss_Fear(Unit, event, miscunit, misc)
	print "Skyriss Fear"
	Unit:FullCastSpellOnTarget(30584,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's fear you guys...")
end

function Skyriss(unit, event, miscunit, misc)
	print "Skyriss"
	unit:RegisterEvent("Skyriss_Red",11000,0)
	unit:RegisterEvent("Skyriss_Blue",15000,0)
	unit:RegisterEvent("Skyriss_Rend",21000,0)
	unit:RegisterEvent("Skyriss_Control",27000,0)
	unit:RegisterEvent("Skyriss_Fear",31000,0)
end

RegisterUnitEvent(20912,1,"Skyriss")