function Hungarfen_Spores(Unit)
	if Unit:GetHealthPct() < 20 and Didthat == 0 then
		Unit:FullCastSpell(31697)
		Didthat = 1
	else
	end
end

function Hungarfen_Mushrooms(Unit, event, miscunit, misc)
	print "Anzu Screech"
	Unit:FullCastSpellOnTarget(17990,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "My Mushrooms will surround you...")
end

function Hungarfen(unit, event, miscunit, misc)
	print "Hungarfen"
	unit:RegisterEvent("Hungarfen_Spores",1000,1)
	unit:RegisterEvent("Hungarfen_Mushrooms",10000,0)
end

RegisterUnitEvent(17770,1,"Hungarfen")