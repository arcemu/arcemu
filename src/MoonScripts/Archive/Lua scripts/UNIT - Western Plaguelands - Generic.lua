

function Arajs_Phylactery (pUnit, Event)
	x = pUnit:GetX()
	y = pUnit:GetY()
	z = pUnit:GetZ()
	o = pUnit:GetO()
	pUnit:SpawnGameObject(177241, x, y, z, o, 600000)
end
RegisterUnitEvent(1852, 4, "Arajs_Phylactery")