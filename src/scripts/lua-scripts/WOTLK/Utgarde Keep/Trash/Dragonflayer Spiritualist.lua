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

function Dragonflayer_Spiritualist_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Spiritualist_Flame_Shock", math.random(9000, 12000), 0)
pUnit:RegisterEvent("Dragonflayer_Spiritualist_Lightning_Bolt", math.random(5000, 7000), 0)
pUnit:RegisterEvent("Dragonflayer_Spiritualist_Healing_Wave", 20000, 0)
end

function Dragonflayer_Spiritualist_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Spiritualist_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(28410, 1, "Dragonflayer_Spiritualist_OnCombat")
RegisterUnitEvent(28410, 2, "Dragonflayer_Spiritualist_OnLeaveCombat")
RegisterUnitEvent(28410, 4, "Dragonflayer_Spiritualist_OnDeath")

function Dragonflayer_Spiritualist_Flame_Shock(pUnit, Event)
pUnit:FullCastSpellOnTarget(51588, pUnit:GetRandomPlayer(0))
end

function Dragonflayer_Spiritualist_Lightning_Bolt(pUnit, Event)
pUnit:FullCastSpellOnTarget(51587, pUnit:GetMainTank())
end

function Dragonflayer_Spiritualist_Healing_Wave(pUnit, Event)
if pUnit:GetHealthPct() <= 50 then
pUnit:FullCastSpell(51586)
end
end
