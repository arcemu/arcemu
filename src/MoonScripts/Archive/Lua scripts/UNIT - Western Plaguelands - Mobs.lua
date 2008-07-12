

function RottingCadaver_OnDied (pUnit, Event)
	x = pUnit:GetX ()
	y = pUnit:GetY ()
	z = pUnit:GetZ ()
	o = pUnit:GetO ()
	
	pUnit:SpawnCreature (10925, x, y, z, o, 21, 360000)
	
	x = x + 0.3
	
	pUnit:SpawnCreature (10925, x, y, z, o, 21, 360000)
end
RegisterUnitEvent (4474, 4, "RottingCadaver_OnDied")

function RottingWorm_OnSpawn (pUnit, Event)
	pUnit:SetScale (0.2)
end
RegisterUnitEvent (10925, 6, "RottingWorm_OnSpawn")

function VileSlime_OnDied (pUnit, Event)
	x = pUnit:GetX ()
	y = pUnit:GetY ()
	z = pUnit:GetZ ()
	o = pUnit:GetO ()
	
	pUnit:SpawnCreature (8257, x, y, z, o, 104, 360000)
	
	x = x - 1
	
	pUnit:SpawnCreature (8257, x, y, z, o, 104, 360000)
	
	x = x + 2
	
	pUnit:SpawnCreature (8257, x, y, z, o, 104, 360000)
end
RegisterUnitEvent (1806, 4, "VileSlime_OnDied")

function RWOO_OnLeaveCombat (pUnit, Event)
	pUnit:Despawn (1000, 0)
end
RegisterUnitEvent (10925, 2, "RWOO_OnLeaveCombat")
RegisterUnitEvent (8257, 2, "RWOO_OnLeaveCombat") 