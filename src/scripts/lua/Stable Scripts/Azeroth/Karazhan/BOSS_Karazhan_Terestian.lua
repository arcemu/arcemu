function Terestian_Shadow_Bolt(Unit, event, miscunit, misc)
	print "Terestian Shadow Bolt"
	Unit:FullCastSpellOnTarget(36868,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "I like shadow...")
end

function Terestian_Sacrifice(Unit, event, miscunit, misc)
	print "Terestian Sacrifice"
	Unit:FullCastSpellOnTarget(30115,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "Sacrifice you for me...")
end

function Terestian_Kilrek(Unit, event, miscunit, misc)
	if Unit:GetHealthPct(17229) < 0 and Didthat == 0 then
		Unit:SpawnCreature(17229, -11236.9, -1695.81, 179.237, 0, 18, 96000000);
		Unit:SendChatMessage(11, 0, "Come to help me Kilrek...")
		Didthat = 1
	else
	end
end

function Terestian_Berserk(Unit, event, miscunit, misc)
	print "Terestian Berserk"
	Unit:FullCastSpell(38110)
	Unit:SendChatMessage(11, 0, "Now, i'am verry mad...")
end

function Terestian(unit, event, miscunit, misc)
	print "Terestian"
	unit:RegisterEvent("Terestian_Shadow_Bolt",8000,0)
	unit:RegisterEvent("Terestian_Sacrifice",33000,0)
	unit:RegisterEvent("Terestian_Kilrek",45000,0)
	unit:RegisterEvent("Terestian_Berserk",600000,0)
end

RegisterUnitEvent(15688,1,"Terestian")