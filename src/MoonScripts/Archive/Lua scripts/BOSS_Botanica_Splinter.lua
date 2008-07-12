function Splinter_Heal(Unit)
	if Unit:GetHealthPct() < 25 and Didthat == 0 then
		Unit:FullCastSpell(39321)
		Didthat = 1
	else
	end
end

function Splinter_War_Stomp(Unit, event, miscunit, misc)
	print "Splinter War Stomp"
	Unit:FullCastSpellOnTarget(36835,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function Splinter_Arcane_Volley(Unit, event, miscunit, misc)
	print "Splinter Arcane Volley"
	Unit:FullCastSpellOnTarget(29885,Unit:GetClosestPlayer())
	Unit:SendChatMessage(11, 0, "NOOB...")
end

function Splinter(unit, event, miscunit, misc)
	print "Splinter"
	unit:RegisterEvent("Splinter_Heal",1000,1)
	unit:RegisterEvent("Splinter_War_Stomp",21000,0)
	unit:RegisterEvent("Splinter_Arcane_Volley",31000,0)
end

RegisterUnitEvent(17977,1,"Splinter")