function Steamrigger_healtA(Unit)
	if Unit:GetHealthPct() < 75 and Didthat == 0 then
		Unit:SpawnCreature(17951, -339.8, -137.7, 6.9, 0, 0, 18, 36000000);
		Unit:SpawnCreature(17951, -327.5, -136.1, 8.2, 0, 0, 18, 36000000);
		Unit:SpawnCreature(17951, -312, -118.7, 8, 0, 0, 18, 36000000);
		Didthat = 1
	else
	end
end

function Steamrigger_healtB(Unit)
	if Unit:GetHealthPct() < 50 and Didthat == 1 then
		Unit:SpawnCreature(17951, -339.8, -137.7, 6.9, 0, 0, 18, 36000000);
		Unit:SpawnCreature(17951, -327.5, -136.1, 8.2, 0, 0, 18, 36000000);
		Unit:SpawnCreature(17951, -312, -118.7, 8, 0, 0, 18, 36000000);
		Didthat = 2
	else
	end
end

function Steamrigger_healtC(Unit)
	if Unit:GetHealthPct() < 25 and Didthat == 2 then
		Unit:SpawnCreature(17951, -339.8, -137.7, 6.9, 0, 0, 18, 36000000);
		Unit:SpawnCreature(17951, -327.5, -136.1, 8.2, 0, 0, 18, 36000000);
		Unit:SpawnCreature(17951, -312, -118.7, 8, 0, 0, 18, 36000000);
		Didthat = 3
	else
	end
end

function Steamrigger_Shrink(Unit)
	print "Steamrigger Shrink"
	Unit:FullCastSpell(31485)
	Unit:SendChatMessage(11, 0, "You can do better then that...")
end

function Steamrigger_Blade(Unit, event, miscunit, misc)
	print "Steamrigger Blade"
	Unit:FullCastSpellOnTarget(37123,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Taste my blade...")
end

function Steamrigger_Net(Unit, event, miscunit, misc)
	print "Steamrigger Net"
	Unit:FullCastSpellOnTarget(35107,Unit:GetClosestPlaye())
	Unit:SendChatMessage(11, 0, "Let's go...")
end

function Steamrigger(unit, event, miscunit, misc)
	print "Steamrigger"
	unit:RegisterEvent("Steamrigger_healtA",1000,1)
	unit:RegisterEvent("Steamrigger_healtB",1000,1)
	unit:RegisterEvent("Steamrigger_healtC",10000,1)
	unit:RegisterEvent("Steamrigger_Shrink",5000,0)
	unit:RegisterEvent("Steamrigger_Blade",8000,0)
	unit:RegisterEvent("Steamrigger_Net",140000,0)
end

RegisterUnitEvent(17796,1,"Steamrigger")