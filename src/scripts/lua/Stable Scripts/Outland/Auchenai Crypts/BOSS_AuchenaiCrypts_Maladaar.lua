function Maladaar_HealthCheck(Unit)
	if Unit:GetHealthPct() < 25 and Didthat == 0 then
		Unit:SpawnCreature(18478, 57.4, -390.8, 26.6, 0, 18, 36000000);
		Didthat = 1
	else
	end
end

function Maladaar_SoulSteal(Unit, event, miscunit, misc)
	print "Maladaar SoulSteal"
	Unit:FullCastSpellOnTarget(36778,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "I will steal your life...")
end

function Maladaar_Ribbon(Unit, event, miscunit, misc)
	print "Maladaar Ribbon"
	Unit:FullCastSpellOnTarget(32422,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Let's do a good move...")
end

function Maladaar_Fear(Unit, event, miscunit, misc)
	print "Maladaar Fear"
	Unit:FullCastSpellOnTarget(40453,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's fear you guys...")
end

function Maladaar(unit, event, miscunit, misc)
	print "Maladaar"
	unit:RegisterEvent("Maladaar_HealthCheck",1000,1)
	unit:RegisterEvent("Maladaar_SoulSteal",8000,0)
	unit:RegisterEvent("Maladaar_Ribbon",13000,0)
	unit:RegisterEvent("Maladaar_Fear",21000,0)
end

RegisterUnitEvent(18373,1,"Maladaar")