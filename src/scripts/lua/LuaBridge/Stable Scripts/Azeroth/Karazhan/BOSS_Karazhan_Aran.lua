function Aran_Water_Elementals(Unit, event, miscunit, misc)
	if Unit:GetHealthPct() < 40 and Didthat == 0 then
		Unit:SpawnCreature(21160, -11167.2, -1914.13, 232.009, 0, 18, 96000000);
		Unit:SpawnCreature(21160, -11163.2, -1910.13, 232.009, 0, 18, 96000000);
		Unit:SpawnCreature(21160, -11165.2, -1916.13, 232.009, 0, 18, 96000000);
		Unit:SpawnCreature(21160, -11162.2, -1911.13, 232.009, 0, 18, 96000000);
		Didthat = 1
	else
	end
end

function Aran_Polymorph(Unit, event, miscunit, misc)
	if Unit:GetManaPct() < 20 and Didthat == 1 then
		Unit:FullCastSpellOnTarget(23603,Unit:GetClosestPlayer())
		Unit:FullCastSpell(32453)
		Didthat = 2
	else
	end
end

function Aran_Fireball(Unit, event, miscunit, misc)
	print "Aran Fireball"
	Unit:FullCastSpellOnTarget(20678,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Fire ball...")
end

function Aran_Conterspell(Unit, event, miscunit, misc)
	print "Aran Conterspell"
	Unit:FullCastSpellOnTarget(29961,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Try to cast now...")
end

function Aran_Conflagration(Unit, event, miscunit, misc)
	print "Aran Conflagration"
	Unit:FullCastSpellOnTarget(23023,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Your so weak guys...")
end

function Aran_FrostBolt(Unit, event, miscunit, misc)
	print "Aran FrostBolt"
	Unit:FullCastSpellOnTarget(41486,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Catch that, if you can...")
end

function Aran_Chains_Ice(Unit, event, miscunit, misc)
	print "Aran Chains Ice"
	Unit:FullCastSpellOnTarget(29991,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Ho, guys you cannot move...")
end

function Aran_Arcane_Missiles(Unit, event, miscunit, misc)
	print "Aran Arcane Missiles"
	Unit:FullCastSpellOnTarget(29955,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Rain of arcane...")
end

function Aran_Flame_Wreath(Unit, event, miscunit, misc)
	print "Aran Flame Wreath"
	Unit:FullCastSpellOnTarget(30004,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "My Flame Wreath...")
end

function Aran_Circular_Blizzard(Unit, event, miscunit, misc)
	print "Aran Circular Blizzard"
	Unit:FullCastSpellOnTarget(29952,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Blizzard...")
end

function Aran_Magnetic_Pull(Unit, event, miscunit, misc)
	print "Aran Magnetic Pull"
	Unit:FullCastSpellOnTarget(29979,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Come to me guys...")
end

function Aran_Arcane_Explosion(Unit, event, miscunit, misc)
	print "Aran Arcane Explosion"
	Unit:FullCastSpellOnTarget(29973,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Now, the arcane Explosion...")
end

function Aran(unit, event, miscunit, misc)
	print "Aran"
	unit:RegisterEvent("Aran_Water_Elementals",1000,1)
	unit:RegisterEvent("Aran_Polymorph",1000,1)
	unit:RegisterEvent("Aran_Fireball",9000,0)
	unit:RegisterEvent("Aran_Conterspell",13000,0)
	unit:RegisterEvent("Aran_Conflagration",15000,0)
	unit:RegisterEvent("Aran_FrostBolt",17000,0)
	unit:RegisterEvent("Aran_Chains_Ice",20000,0)
	unit:RegisterEvent("Aran_Arcane_Missiles",25000,0)
	unit:RegisterEvent("Aran_Flame_Wreath",30000,0)
	unit:RegisterEvent("Aran_Circular_Blizzard",60000,0)
	unit:RegisterEvent("Aran_Magnetic_Pull",90000,0)
	unit:RegisterEvent("Aran_Arcane_Explosion",91000,0)
end

RegisterUnitEvent(16524,1,"Aran")