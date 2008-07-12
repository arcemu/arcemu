

function Talen_onSpawn (pUnit, Event)
	pUnit:Emote (13)
end
RegisterUnitEvent (3846, 6, "Talen_onSpawn")
RegisterUnitEvent (3846, 3, "Talen_onSpawn")
RegisterUnitEvent (3846, 2, "Talen_onSpawn")

function Talen_onCombat (pUnit, Event)
	pUnit:Emote (26)
end
RegisterUnitEvent (3846, 1, "Talen_onCombat")