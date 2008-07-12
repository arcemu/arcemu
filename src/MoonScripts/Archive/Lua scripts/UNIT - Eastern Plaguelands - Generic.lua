

function Darrowshire_Spirit_Spawn (pUnit, Event)
	x = pUnit:GetX()
	y = pUnit:GetY()
	z = pUnit:GetZ()
	o = pUnit:GetO()
	DoSpawn = math.random (2) -- Random number, either 1 or 2
	
	if (DoSpawn == 2) then
		pUnit:SpawnCreature(11064, x, y, z, o, 35, 300000)
	end
end
RegisterUnitEvent (8530, 4, "Darrowshire_Spirit_Spawn")
RegisterUnitEvent (8531, 4, "Darrowshite_Spirit_Spawn")
RegisterUnitEvent (8532, 4, "Darrowshire_Spirit_Spawn")
