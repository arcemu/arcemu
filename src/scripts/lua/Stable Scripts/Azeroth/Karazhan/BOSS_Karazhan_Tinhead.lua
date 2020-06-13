function Tinhead_Rust(Unit, event, miscunit, misc)
	print "Tinhead Rust"
	if Unit:GetHealthPct() < 50 and Didthat == 0 then
	Unit:FullCastSpellOnTarget(31086,Unit:GetClosestPlayer())
	Didthat = 1 
	end
end
function Tinhead_OnKilledTarget(unit, event, miscunit, misc)
	print "Tinhead Killed"
	unit:SendChatMessage(12, 0, "Guess I'm not so rusty, after all.")
end

function Tinhead_OnDeath(unit, event, miscunit, misc)
	unit:SendChatMessage(12, 0, "Back to being an old rustbucket.")
	unit:RemoveEvents()
end

function Tinhead_Cleave(Unit, event, miscunit, misc)
	print "Tinhead Cleave"
	Unit:FullCastSpellOnTarget(39174,Unit:GetClosestPlayer())
end

function Tinhead(unit, event, miscunit, misc)
	print "Tinhead"
	unit:SendChatMessage(12, 0, "I could really use a heart. Say, can I have yours?")
	unit:RegisterEvent("Tinhead_Rust",5000,0)
	unit:RegisterEvent("Tinhead_Cleave",10000,0)
end

RegisterUnitEvent(17547,1,"Tinhead")
RegisterUnitEvent(17547,3,"Tinhead_OnKilledTarget")
RegisterUnitEvent(17547,4,"Tinhead_OnDeath")