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
-- Scripted By:  n4xD

function Living_Mojo_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Living_Mojo_Mojo_Puddle", math.random(20000, 23000), 0)
pUnit:RegisterEvent("Living_Mojo_Mojo_Wave", math.random(27000, 29000), 0)
end

function Living_Mojo_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Living_Mojo_OnKillTarget(pUnit, Event)
end

function Living_Mojo_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29830, 1, "Living_Mojo_OnCombat")
RegisterUnitEvent(29830, 2, "Living_Mojo_OnLeaveCombat")
RegisterUnitEvent(29830, 3, "Living_Mojo_OnKillTarget")
RegisterUnitEvent(29830, 4, "Living_Mojo_OnDeath")

function Living_Mojo_Mojo_Puddle(pUnit, Event)
pUnit:FullCastSpell(55627)
end

function Living_Mojo_Mojo_Wave(pUnit, Event)
pUnit:FullCastSpellOnTarget(55626, pUnit:GetRandomPlayer(0))
end
