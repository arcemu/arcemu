
function HoggerCharge(pUnit, Event)
	
	pUnit:SendChatMessage(9, 0, "Hogger charges!");
	pUnit:CastSpellOnTarget(6268, pUnit:GetClosestPlayer());
end

RegisterUnitEvent(448, 1, "HoggerCharge");
