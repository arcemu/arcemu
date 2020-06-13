function MarshalIsildor_Talk(pUnit, Event)
	pUnit:RemoveEvents();
	pUnit:SendChatMessage(12, 0, "Do you think that demons will play fair? These beasts are more savage than orcs and more cunning that the undead. Do not attempt to engage Legion by normal standards of war.")
end

function MarshalIsildor_Start(pUnit, Event)
	pUnit:RegisterEvent("MarshalIsildor_Talk", 15000, 0)
end

RegisterUnitEvent(19308, 6, "MarshalIsildor_Start")