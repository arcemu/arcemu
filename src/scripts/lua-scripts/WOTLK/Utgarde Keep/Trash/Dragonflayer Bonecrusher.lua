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

function Dragonflayer_Bonecrusher_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Bonecrusher_Head_Crack", 24000, 0)
pUnit:RegisterEvent("Dragonflayer_Bonecrusher_Knockdown_Spin", math.random(12000, 16000), 0)
end

function Dragonflayer_Bonecrusher_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Bonecrusher_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(24069, 1, "Dragonflayer_Bonecrusher_OnCombat")
RegisterUnitEvent(24069, 2, "Dragonflayer_Bonecrusher_OnLeaveCombat")
RegisterUnitEvent(24069, 4, "Dragonflayer_Bonecrusher_OnDeath")

function Dragonflayer_Bonecrusher_Head_Crack(pUnit, Event)
pUnit:FullCastSpellOnTarget(9791, pUnit:GetMainTank())
end

function Dragonflayer_Bonecrusher_Knockdown_Spin(pUnit, Event)
pUnit:FullCastSpell(43935)
end
