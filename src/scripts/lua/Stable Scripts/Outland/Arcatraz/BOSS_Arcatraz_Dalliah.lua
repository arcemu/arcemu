function Dalliah_Red(Unit, event, miscunit, misc)
	print "Dalliah Red"
	Unit:FullCastSpellOnTarget(14264,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You will pay the price...")
end

function Dalliah_Whirlwind_Heal(Unit)
	print "Dalliah Whirlwind Heal"
	Unit:FullCastSpell(36142)
	Unit:SendChatMessage(11, 0, "Let's have some heal..")
end

function Dalliah_Doomsayer(Unit, event, miscunit, misc)
	print "Dalliah Doomsayer"
	Unit:FullCastSpellOnTarget(36173,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "The power of Doom it's on me....")
end
function Dalliah(unit, event, miscunit, misc)
	print "Dalliah"
	unit:RegisterEvent("Dalliah_Red",11000,0)
	unit:RegisterEvent("Dalliah_Whirlwind_Heal",21000,0)
	unit:RegisterEvent("Dalliah_Doomsayer",31000,0)
end

RegisterUnitEvent(20885,1,"Dalliah")