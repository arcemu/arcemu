function Kilrek_Broken_Pact(Unit, event, miscunit, misc)
	if Unit:GetHealthPct() < 2 and Didthat == 0 then
		Unit:FullCastSpellOnTarget(30065,Unit:GetUnitBySqlId(15688))
		Unit:SendChatMessage(11, 0, "You let me down Terestian, you will pay for this...")
		Didthat = 1
	else
	end
end

function Kilrek_FireBolt(Unit, event, miscunit, misc)
	print "Kilrek FireBolt"
	Unit:FullCastSpellOnTarget(15592,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Take that...")
end

function Kilrek_Summon_Imps(Unit, event, miscunit, misc)
	print "Kilrek Summon Imps"
	Unit:FullCastSpell(34237)
	Unit:SendChatMessage(11, 0, "Help me...")
end

function Kilrek_Amplify_Flames(Unit, event, miscunit, misc)
	print "Kilrek Amplify Flames"
	Unit:FullCastSpellOnTarget(30053,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Take fire will hurt you more...")
end

function Kilrek(unit, event, miscunit, misc)
	print "Kilrek"
	unit:RegisterEvent("Kilrek_Broken_Pact",1000,1)
	unit:RegisterEvent("Kilrek_FireBolt",8000,0)
	unit:RegisterEvent("Kilrek_Summon_Imps",30000,0)
	unit:RegisterEvent("Kilrek_Amplify_Flames",45000,0)
end

RegisterUnitEvent(17229,1,"Kilrek")
