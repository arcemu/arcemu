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

function Dragonflayer_Runecaster_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Runecaster_Bolthorns_Rune_of_Flame", 10000, 0)
pUnit:RegisterEvent("Dragonflayer_Runecaster_Njords_Rune_of_Protection", math.random(20000, 25000), 0)
end

function Dragonflayer_Runecaster_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Runecaster_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(23960, 1, "Dragonflayer_Runecaster_OnCombat")
RegisterUnitEvent(23960, 2, "Dragonflayer_Runecaster_OnLeaveCombat")
RegisterUnitEvent(23960, 4, "Dragonflayer_Runecaster_OnDeath")

function Dragonflayer_Runecaster_Bolthorns_Rune_of_Flame(pUnit, Event)
pUnit:FullCastSpell(54965)
end

function Dragonflayer_Runecaster_Njords_Rune_of_Protection(pUnit, Event)
pUnit:FullCastSpell(42740)
end
