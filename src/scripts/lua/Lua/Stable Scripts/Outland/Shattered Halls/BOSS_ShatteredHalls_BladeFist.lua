function BladeFist_MobsA(Unit)
	print "BladeFist MobsA"
	Unit:SpawnCreature(17695, 272.9, -90.7, 1.8, 0, 18, 36000000);
	Unit:SpawnCreature(17695, 275.95, -83.3, 2.3, 0, 18, 36000000);
	Unit:SpawnCreature(17695, 274.8, -77.4, 1.7, 0, 18, 36000000);
	Unit:SendChatMessage(11, 0, "Guards, dont let nobody go out...")
end

function BladeFist_MobsB(Unit)
	print "BladeFist MobsB"
	Unit:SpawnCreature(17401, 253, -88.4, 4.9, 0, 18, 36000000);
	Unit:SpawnCreature(17401, 251.8, -79.5, 4.9, 0, 18, 36000000);
	Unit:SendChatMessage(11, 0, "Come to help me my guards...")
end

function BladeFist_Blade_Dance(Unit, event, miscunit, misc)
	print "BladeFist Blade Dance"
	Unit:FullCastSpellOnTarget(30739,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A good dance...")
end

function BladeFist(unit, event, miscunit, misc)
	print "BladeFist"
	unit:RegisterEvent("BladeFist_MobsA",1000,1)
	unit:RegisterEvent("BladeFist_MobsB",20000,0)
	unit:RegisterEvent("BladeFist_Blade_Dance",33000,0)
end

RegisterUnitEvent(16808,1,"BladeFist")