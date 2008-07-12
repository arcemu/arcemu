

function Kobold_onAgro (pUnit, Event)
	pUnit:SendChatMessage (11, 0, "You no take candle!")
end
RegisterUnitEvent (6, 1, "Kobold_onAgro")