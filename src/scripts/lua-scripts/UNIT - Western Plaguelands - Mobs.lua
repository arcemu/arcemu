 --[[
   ******************************************************************
   *	 _____              ___                           _         *
   *	(  _  )            (  _`\               _        ( )_       *
   *	| (_) | _ __   ___ | (_(_)   ___  _ __ (_) _ _   | ,_)      *
   *	|  _  |( '__)/'___)`\__ \  /'___)( '__)| |( '_`\ | |        *
   *	| | | || |  ( (___ ( )_) |( (___ | |   | || (_) )| |_       *
   *	(_) (_)(_)  `\____)`\____)`\____)(_)   (_)| ,__/'`\__)      *
   *	                                          | |               *
   *	                                          (_)               *
   *	                                                            *
   *	               OpenSource Scripting Team                    *
   *	                <http://www.arcemu.org>                     *
   *	                                                            *
   ******************************************************************
  
   This software is provided as free and open source by the
staff of The ArcScript Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of ArcScript Project, Feb 2008
~~End of License Agreement

#############################################################

-- ]]

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