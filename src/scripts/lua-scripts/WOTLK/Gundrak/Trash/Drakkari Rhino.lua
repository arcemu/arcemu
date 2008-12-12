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

function Drakkari_Rhino_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Drakkari_Rhino_Charge", math.random(15000, 18000), 0)
pUnit:RegisterEvent("Drakkari_Rhino_Deafening_Roar", math.random(20000, 23000), 0)
end

function Drakkari_Rhino_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Drakkari_Rhino_OnKillTarget(pUnit, Event)
end

function Drakkari_Rhino_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29838, 1, "Drakkari_Rhino_OnCombat")
RegisterUnitEvent(29838, 2, "Drakkari_Rhino_OnLeaveCombat")
RegisterUnitEvent(29838, 3, "Drakkari_Rhino_OnKillTarget")
RegisterUnitEvent(29838, 4, "Drakkari_Rhino_OnDeath")

function Drakkari_Rhino_Charge(pUnit, Event)
pUnit:FullCastSpellOnTarget(55530, pUnit:GetRandomPlayer(0))
end

function Drakkari_Rhino_Deafening_Roar(pUnit, Event)
pUnit:FullCastSpell(55663)
end
