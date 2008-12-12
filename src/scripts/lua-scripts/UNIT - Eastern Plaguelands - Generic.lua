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
