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

function Dragonflayer_Metalworker_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Metalworker_Enrage", math.random(30000, 34000), 1)
pUnit:RegisterEvent("Dragonflayer_Metalworker_Sunder_Armor", math.random(9000, 12000), 0)
end

function Dragonflayer_Metalworker_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Metalworker_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(24078, 1, "Dragonflayer_Metalworker_OnCombat")
RegisterUnitEvent(24078, 2, "Dragonflayer_Metalworker_OnLeaveCombat")
RegisterUnitEvent(24078, 4, "Dragonflayer_Metalworker_OnDeath")

function Dragonflayer_Metalworker_Enrage(pUnit, Event)
pUnit:FullCastSpell(8599)
end

function Dragonflayer_Metalworker_Sunder_Armor(pUnit, Event)
pUnit:FullCastSpellOnTarget(15572, pUnit:GetMainTank())
end
