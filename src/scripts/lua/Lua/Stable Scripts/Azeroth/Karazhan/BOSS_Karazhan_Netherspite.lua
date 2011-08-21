function Netherspite_Portal_PhaseA(Unit, event, miscunit, misc)
	print "Netherspite Portal PhaseA"
	Unit:SendAreaTriggerMessage("PORTAL PHASE")
	Unit:FullCastSpell(30400)
	Unit:FullCastSpell(30401)
	Unit:FullCastSpell(30402)
end

function Netherspite_Nether_Burn(Unit, event, miscunit, misc)
	print "Netherspite Nether Burn"
	Unit:FullCastSpellOnTarget(30523,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "BURN...")
end

function Netherspite_Vode_Zone(Unit, event, miscunit, misc)
	print "Netherspite Vode Zone"
	Unit:FullCastSpellOnTarget(30533,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Bad Zone...")
end

function Netherspite_Banish_Phase(Unit, event, miscunit, misc)
	print "Netherspite Banish Phase"
	Unit:SendAreaTriggerMessage("BANISH PHASE")
	Unit:FullCastSpell(35182)
	Unit:FullCastSpellOnTarget(38524,Unit:GetClosestPlayer())
end

function Netherspite_Portal_PhaseB(Unit, event, miscunit, misc)
	print "Netherspite Portal PhaseB"
	Unit:SendAreaTriggerMessage("PORTAL PHASE")
	Unit:FullCastSpell(30400)
	Unit:FullCastSpell(30401)
	Unit:FullCastSpell(30402)
end

function Netherspite_Enrage(Unit, event, miscunit, misc)
	print "Netherspite Enrage"
	Unit:FullCastSpell(35595)
	Unit:SendChatMessage(11, 0, "You will all die...")
end

function Netherspite(unit, event, miscunit, misc)
	print "Netherspite"
	unit:RegisterEvent("Netherspite_Portal_PhaseA",1000,1)
	unit:RegisterEvent("Netherspite_Nether_Burn",10000,0)
	unit:RegisterEvent("Netherspite_Vode_Zone",20000,0)
	unit:RegisterEvent("Netherspite_Banish_Phase",30000,0)
	unit:RegisterEvent("Netherspite_Portal_PhaseB",60000,0)
	unit:RegisterEvent("Netherspite_Enrage",540000,0)
end

RegisterUnitEvent(15689,1,"Netherspite")