function Curator_Evocation(Unit, event, miscunit, misc)
	if Unit:GetManaPct() < 1 and Didthat == 0 then
		print "Curator Evocation"
		Unit:FullCastSpell(30254)
		Didthat = 1
	else
	end
end

function Curator_Enrage(Unit, event, miscunit, misc)
	if Unit:GetHealthPct() < 15 and Didthat == 1 then
		print "Curator_Enrage"
		Unit:FullCastSpell(41447)
		Didthat = 2
	else
	end
end

function Curator_Summon_Astral_Flare(Unit, event, miscunit, misc)
	print "Curator Summon Astral Flare"
	Unit:SpawnCreature(17096, -1168.601, 1699.915, 91.477, 0, 18, 96000000);
	Unit:SendChatMessage(11, 0, "Help me...")
end

function Curator_Hateful_Bolt(Unit, event, miscunit, misc)
	print "Curator Hateful Bolt"
	Unit:FullCastSpellOnTarget(30383,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Catch that...")
end

function Curator_Berserk(Unit, event, miscunit, misc)
	print "Curator Berserk"
	Unit:FullCastSpell(35595)
	Unit:SendChatMessage(11, 0, "Now you will die...")
end

function Curator(unit, event, miscunit, misc)
	print "Curator"
	unit:RegisterEvent("Curator_Evocation",1000,0)
	unit:RegisterEvent("Curator_Enrage",1000,0)
	unit:RegisterEvent("Curator_Summon_Astral_Flare",10000,0)
	unit:RegisterEvent("Curator_Hateful_Bolt",27000,0)
	unit:RegisterEvent("Curator_Berserk",720000,0)
end

RegisterUnitEvent(15691,1,"Curator")