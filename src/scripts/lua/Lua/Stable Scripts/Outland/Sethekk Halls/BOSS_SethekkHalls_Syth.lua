function Syth_HealthCheckA(Unit)
	if Unit:GetHealthPct() < 75 and Didthat == 0 then
		Unit:SpawnCreature(19204, -143.9, 162.9, 0, 0, 18, 36000000);
		Unit:SpawnCreature(19204, -136.8, 174.3, 0, 0, 18, 36000000);
		Unit:SpawnCreature(19204, -142.6, 183.7, 0, 0, 18, 36000000);
		Didthat = 1
	else
	end
end

function Syth_HealthCheckB(Unit)
	if Unit:GetHealthPct() < 50 and Didthat == 1 then
		Unit:SpawnCreature(19204, -143.9, 162.9, 0, 0, 18, 36000000);
		Unit:SpawnCreature(19204, -136.8, 174.3, 0, 0, 18, 36000000);
		Unit:SpawnCreature(19204, -142.6, 183.7, 0, 0, 18, 36000000);
		Didthat = 2
	else
	end
end

function Syth_HealthCheckC(Unit)
	if Unit:GetHealthPct() < 25 and Didthat == 2 then
		Unit:SpawnCreature(19204, -143.9, 162.9, 0, 0, 18, 36000000);
		Unit:SpawnCreature(19204, -136.8, 174.3, 0, 0, 18, 36000000);
		Unit:SpawnCreature(19204, -142.6, 183.7, 0, 0, 18, 36000000);
		Didthat = 3
	else
	end
end

function Syth_Frost(Unit, event, miscunit, misc)
	print "Syth Frost"
	Unit:FullCastSpellOnTarget(37865,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Do you some ice cream...")
end

function Syth_Flame(Unit, event, miscunit, misc)
	print "Syth_Flame"
	Unit:FullCastSpellOnTarget(34354,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Let's do a campfire with your bones...")
end

function Syth_Shadow(Unit, event, miscunit, misc)
	print "Syth Shadow"
	Unit:FullCastSpellOnTarget(30138,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "The shadow surround you...")
end

function Syth_Arcane(Unit, event, miscunit, misc)
	print "Syth Arcane"
	Unit:FullCastSpellOnTarget(37132,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I like arcane...")
end

function Syth_Chain(Unit, event, miscunit, misc)
	print "Syth Chain"
	Unit:FullCastSpellOnTarget(39945,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You are weak...")
end

function Syth(unit, event, miscunit, misc)
	print "Syth"
	unit:RegisterEvent("Syth_HealthCheckA",1000,1)
	unit:RegisterEvent("Syth_HealthCheckB",1000,1)
	unit:RegisterEvent("Syth_HealthCheckC",1000,1)
	unit:RegisterEvent("Syth_Frost",10000,0)
	unit:RegisterEvent("Syth_Flame",15000,0)
	unit:RegisterEvent("Syth_Shadow",21000,0)
	unit:RegisterEvent("Syth_Arcane",26000,0)
	unit:RegisterEvent("Syth_Chain",29000,0)
end

RegisterUnitEvent(18472,1,"Syth")