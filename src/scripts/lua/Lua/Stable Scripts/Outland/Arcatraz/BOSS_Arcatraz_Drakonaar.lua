function Drakonaar_Red(Unit, event, miscunit, misc)
	print "Drakonaar Red"
	Unit:FullCastSpellOnTarget(14264,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "This spell will kill you...")
end

function Drakonaar_Blue(Unit, event, miscunit, misc)
	print "Drakonaar Blue"
	Unit:FullCastSpellOnTarget(14261,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Look behind you...")
end

function Drakonaar_Green(Unit, event, miscunit, misc)
	print "Drakonaar Green"
	Unit:FullCastSpellOnTarget(14262,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Look your face, you are afraid...")
end

function Drakonaar_Black(Unit, event, miscunit, misc)
	print "Drakonaar Black"
	Unit:FullCastSpellOnTarget(14265,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now the final blast...")
end

function Drakonaar_Bronze(Unit, event, miscunit, misc)
	print "Drakonaar Bronze"
	Unit:FullCastSpellOnTarget(14263,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I will crush you in my hand...")
end

function Drakonaar(unit, event, miscunit, misc)
	print "Drakonaar"
	unit:RegisterEvent("Drakonaar_Red",11000,0)
	unit:RegisterEvent("Drakonaar_Blue",15000,0)
	unit:RegisterEvent("Drakonaar_Green",21000,0)
	unit:RegisterEvent("Drakonaar_Black",27000,0)
	unit:RegisterEvent("Drakonaar_Bronze",31000,0)
end

RegisterUnitEvent(20910,1,"Drakonaar")