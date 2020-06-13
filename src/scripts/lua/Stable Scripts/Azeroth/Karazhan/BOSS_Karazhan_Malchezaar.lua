function Malchezaar_Hellfire(Unit, event, miscunit, misc)
	print "Malchezaar Hellfire"
	Unit:FullCastSpellOnTarget(43465,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "BURN...")
end

function Malchezaar_Thrash(Unit, event, miscunit, misc)
	print "Malchezaar Thrash"
	Unit:FullCastSpell(21919)
	Unit:SendChatMessage(11, 0, "Watch the speed...")
end

function Malchezaar_Parry(Unit, event, miscunit, misc)
	print "Malchezaar Parry"
	Unit:FullCastSpell(13668)
	Unit:SendChatMessage(11, 0, "Miss...")
end

function Malchezaar_Sunder_Armor(Unit, event, miscunit, misc)
	print "Malchezaar Sunder Armor"
	Unit:FullCastSpellOnTarget(30901,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "You lose some protection!...")
end

function Malchezaar_Cleave(Unit, event, miscunit, misc)
	print "Malchezaar Cleave"
	Unit:FullCastSpellOnTarget(39174,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "Cleave...")
end

function Malchezaar_Amplify(Unit, event, miscunit, misc)
	print "Malchezaar Amplify"
	Unit:FullCastSpellOnTarget(39095,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "More damage to you...")
end


function Malchezaar_Flying_Axes(Unit, event, miscunit, misc)
	print "Malchezaar Flying_Axes"
	Unit:FullCastSpell(17650)
	Unit:SendChatMessage(11, 0, "My Axes will kill you...")
end

function Malchezaar_Shadow_Word_Pain(Unit, event, miscunit, misc)
	print "Malchezaar Shadow_Word Pain"
	Unit:FullCastSpellOnTarget(30854,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "More Pain...")
end

function Malchezaar_Shadow_Word_Pain2(Unit, event, miscunit, misc)
	print "Malchezaar Shadow_Word Pain2"
	Unit:FullCastSpellOnTarget(30854,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "More Pain...")
end

function Malchezaar_Summon_Infernal(Unit, event, miscunit, misc)
	print "Malchezaar Summon Infernal"
     	Unit:SpawnCreature(17646, -10924.5, -1974.66, 275.522, 0, 18, 180000);
	Unit:SendChatMessage(11, 0, "Help me infernal creature...")
end

function Malchezaar_Enfeeble(Unit, event, miscunit, misc)
	print "Malchezaar Enfeeble"
	Unit:FullCastSpellOnTarget(30843,Unit:GetRandomPlayer())
	Unit:FullCastSpellOnTarget(30843,Unit:GetRandomPlayer())
	Unit:FullCastSpellOnTarget(30843,Unit:GetRandomPlayer())
	Unit:FullCastSpellOnTarget(30843,Unit:GetRandomPlayer())
	Unit:FullCastSpellOnTarget(30843,Unit:GetRandomPlayer())
	Unit:SendChatMessage(11, 0, "More Pain...")
end

function Malchezaar_Shadow_Nova(Unit, event, miscunit, misc)
	print "Malchezaar Shadow Nova"
	Unit:FullCastSpellOnTarget(30852,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "A big blast to finish this fight...")
end

function Malchezaar(unit, event, miscunit, misc)
	print "Malchezaar"

	if unit:GetHealthPct() < 100 and Didthat == 0 then
		unit:SendAreaTriggerMessage("PHASE 1")
		unit:SendChatMessage(11, 0, "You face not Malchezaar alone, but the legions I command!")
		unit:RegisterEvent("Malchezaar_Hellfire",2000,0)
		unit:RegisterEvent("Malchezaar_Shadow_Word_Pain",25000,0)
		unit:RegisterEvent("Malchezaar_Summon_Infernal",35000,0)
		unit:RegisterEvent("Malchezaar_Enfeeble",45000,0)
		unit:RegisterEvent("Malchezaar_Shadow_Nova",47000,0)
		Didthat = 1
	else
	end

	if unit:GetHealthPct() < 60 and Didthat == 1 then
		unit:SendAreaTriggerMessage("PHASE 2")
		unit:RegisterEvent("Malchezaar_Hellfire",2000,0)
		unit:RegisterEvent("Malchezaar_Thrash",5000,0)
		unit:RegisterEvent("Malchezaar_Parry",6000,0)
		unit:RegisterEvent("Malchezaar_Sunder_Armor",7000,0)
		unit:RegisterEvent("Malchezaar_Cleave",8000,0)
		unit:RegisterEvent("Malchezaar_Summon_Infernal",45000,0)
		unit:RegisterEvent("Malchezaar_Enfeeble",46000,0)
		unit:RegisterEvent("Malchezaar_Shadow_Nova",47000,0)
		Didthat = 2
	else
	end

	if unit:GetHealthPct() < 30 and Didthat == 2 then
		unit:SendAreaTriggerMessage("PHASE 3")
		unit:RegisterEvent("Malchezaar_Hellfire",2000,0)
		unit:RegisterEvent("Malchezaar_Amplify",10000,0)
		unit:RegisterEvent("Malchezaar_Summon_Infernal",15000,0)
		unit:RegisterEvent("Malchezaar_Shadow_Word_Pain2",25000,0)
		unit:RegisterEvent("Malchezaar_Flying_Axes",17000,0)
		unit:RegisterEvent("Malchezaar_Shadow_Nova",47000,0)
		Didthat = 3
	else
	end

end

RegisterUnitEvent(15690,1,"Malchezaar")