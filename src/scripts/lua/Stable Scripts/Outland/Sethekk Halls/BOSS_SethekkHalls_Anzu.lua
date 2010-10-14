function Anzu_HealthCheckA(Unit)
	if Unit:GetHealthPct() < 66 and Didthat == 0 then
		Unit:FullCastSpell(41196)
		Unit:SpawnCreature(23134, -90.7, 265.7, 26.86, 0, 18, 36000000);
		Didthat = 1
	else
	end
end

function Anzu_HealthCheckB(Unit)
	if Unit:GetHealthPct() < 33 and Didthat == 1 then
		Unit:FullCastSpell(41196)
		Unit:SpawnCreature(23134, -90.7, 265.7, 26.86, 0, 18, 36000000);
		Didthat = 2
	else
	end
end

function Anzu_Cyclone(Unit, event, miscunit, misc)
	print "Anzu Cyclone"
	Unit:FullCastSpellOnTarget(40303,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's blow this place...")
end

function Anzu_Screech(Unit, event, miscunit, misc)
	print "Anzu Screech"
	Unit:FullCastSpellOnTarget(40321,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Some Cyclone...")
end

function Anzu(unit, event, miscunit, misc)
	print "Anzu"
	unit:RegisterEvent("Anzu_HealthCheckA",1000,1)
	unit:RegisterEvent("Anzu_HealthCheckB",1000,1)
	unit:RegisterEvent("Anzu_Cyclone",10000,0)
	unit:RegisterEvent("Anzu_Screech",17000,0)
end

RegisterUnitEvent(23035,1,"Anzu")