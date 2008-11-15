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

function Dragonflayer_Strategist_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Strategist_Blind", math.random(12000, 15000), 0)
pUnit:RegisterEvent("Dragonflayer_Strategist_Hurl_Dagger", math.random(3000, 6000), 0)
pUnit:RegisterEvent("Dragonflayer_Strategist_Ticking_Bomb", math.random(20000, 24000), 0)
end

function Dragonflayer_Strategist_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Strategist_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(23956, 1, "Dragonflayer_Strategist_OnCombat")
RegisterUnitEvent(23956, 2, "Dragonflayer_Strategist_OnLeaveCombat")
RegisterUnitEvent(23956, 4, "Dragonflayer_Strategist_OnDeath")

function Dragonflayer_Strategist_Blind(pUnit, Event)
pUnit:FullCastSpellOnTarget(42972, pUnit:GetRandomPlayer(0))
end

function Dragonflayer_Strategist_Hurl_Dagger(pUnit, Event)
pUnit:FullCastSpellOnTarget(42772, pUnit:GetMainTank())
end

function Dragonflayer_Strategist_Ticking_Bomb(pUnit, Event)
pUnit:FullCastSpellOnTarget(54955, pUnit:GetRandomPlayer(0))
end
