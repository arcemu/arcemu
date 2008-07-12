

function Thrall_onCombat (pUnit, Event)
	pUnit:PlaySoundToSet (5880)
end

function OrgrimmarGruntOnCombat(pUnit, Event)
	pUnit:SendChatMessage(11, 0, "For the Warchief!");
end

RegisterUnitEvent(3296, 1, "OrgrimmarGruntOnCombat");
RegisterUnitEvent (4949, 1, "Thrall_onCombat") 