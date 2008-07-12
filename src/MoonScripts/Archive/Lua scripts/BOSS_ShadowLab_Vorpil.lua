function Vorpil_HealthCheck(Unit)
	if Unit:GetHealthPct() < 20 and Didthat == 0 then
		Unit:FullCastSpell(39321)
		Unit:SpawnCreature(18631, -235, -270, 17, 0, 0, 18, 36000000);
		Unit:SpawnCreature(18631, -236.5, -256.9, 17, 0, 0, 18, 36000000);
		Didthat = 1
	else
	end
end

function Vorpil_Draw(Unit, event, miscunit, misc)
	print "Vorpil Draw"
	Unit:FullCastSpellOnTarget(33563,Unit:CastSpellOnTarget())
	Unit:SendChatMessage(11, 0, "OK...")
end

function Vorpil_Shadow(Unit, event, miscunit, misc)
	print "Vorpil_Shadow"
	Unit:FullCastSpellOnTarget(32963,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You don't like shadow...")
end

function Vorpil(unit, event, miscunit, misc)
	print "Vorpil"
	unit:RegisterEvent("Vorpil_HealthCheck",1000,1)
	unit:RegisterEvent("Vorpil_Draw",15000,0)
	unit:RegisterEvent("Vorpil_Shadow",27000,0)
end

RegisterUnitEvent(18732,1,"Vorpil")