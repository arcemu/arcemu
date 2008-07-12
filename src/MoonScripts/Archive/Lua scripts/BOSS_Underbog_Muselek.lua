function Muselek_Aimed_Shot(Unit, event, miscunit, misc)
	print "Muselek Aimed Shot"
	Unit:FullCastSpellOnTarget(31623,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Don't move, i will miss you...")
end

function Muselek_Freezing_Trap(Unit, event, miscunit, misc)
	print "Muselek Freezing Trap"
	Unit:FullCastSpellOnTarget(41085,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Your are all freeze....")
end

function Muselek_Multi_Shot(Unit, event, miscunit, misc)
	print "Muselek Multi Shot"
	Unit:FullCastSpellOnTarget(30990,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, i will hit you all...")
end

function Muselek_Roar(Unit, event, miscunit, misc)
	print "Muselek Roar"
	Unit:FullCastSpellOnTarget(10967,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function Muselek_Charge(Unit, event, miscunit, misc)
	print "Muselek Charge"
	Unit:FullCastSpellOnTarget(39435,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "CHARGE....")
end

function Muselek(unit, event, miscunit, misc)
	print "Muselek"
	unit:RegisterEvent("Muselek_Aimed_Shot",6000,0)
	unit:RegisterEvent("Muselek_Freezing_Trap",10000,0)
	unit:RegisterEvent("Muselek_Multi_Shot",12000,0)
	unit:RegisterEvent("Muselek_Roar",14000,0)
	unit:RegisterEvent("Muselek_Charge",18000,0)
end

RegisterUnitEvent(17826,1,"Muselek")