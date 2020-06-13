-- Euph:TODO ,  roar movie to set combatcapable and proper blizzlikeness. Done for now.
function Roar_Frightened_Scream(Unit, event, miscunit, misc)
	print "Roar Frightened Scream"
	unit:FullCastSpellOnTarget(31013,Unit:GetRandomPlayer())
end
function Roar_OnKilledTarget(unit, event, miscunit, misc)
	unit:SendChatMessage(11, 0, "Go ahead and take forty winks")
end
function Roar(unit, event, miscunit, misc)
	print "Roar"
	unit:RegisterEvent("Roar_Frightened_Scream",10000,0)
	unit:SendChatMessage(11, 0, "I'm not afraid a' you! Do you wanna fight? Huh, do ya'? C'mon! I'll fight ya' with both paws behind my back!")
end

function Roar_OnLeaveCombat(pUnit, event, miscunit, misc)
	unit:RemoveEvents()
end
RegisterUnitEvent(17546,1,"Roar")
RegisterUnitEvent(17546,2,"Roar_OnLeaveCombat")
RegisterUnitEvent(17546,3,"Roar_OnKilledTarget")