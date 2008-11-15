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

function Dragonflayer_Weaponsmith_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Weaponsmith_Cleave", math.random(14000, 16000), 0)
pUnit:RegisterEvent("Dragonflayer_Weaponsmith_Concussion_Blow", 30000, 0)
pUnit:RegisterEvent("Dragonflayer_Weaponsmith_Disarm", math.random(17000, 24000), 0)
end

function Dragonflayer_Weaponsmith_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Weaponsmith_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(24080, 1, "Dragonflayer_Weaponsmith_OnCombat")
RegisterUnitEvent(24080, 2, "Dragonflayer_Weaponsmith_OnLeaveCombat")
RegisterUnitEvent(24080, 4, "Dragonflayer_Weaponsmith_OnDeath")

function Dragonflayer_Weaponsmith_Cleave(pUnit, Event)
pUnit:FullCastSpellOnTarget(42724, pUnit:GetMainTank())
end

function Dragonflayer_Weaponsmith_Concussion_Blow(pUnit, Event)
pUnit:FullCastSpellOnTarget(22427, pUnit:GetMainTank())
end

function Dragonflayer_Weaponsmith_Disarm(pUnit, Event)
pUnit:FullCastSpellOnTarget(6713, pUnit:GetMainTank())
end
