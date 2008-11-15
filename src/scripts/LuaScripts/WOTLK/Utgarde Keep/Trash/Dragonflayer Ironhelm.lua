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

function Dragonflayer_Ironhelm_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Dragonflayer_Ironhelm_Heroic_Strike", 5000, 0)
pUnit:RegisterEvent("Dragonflayer_Ironhelm_Ringing_Slap", math.random(12000, 15000), 0)
end

function Dragonflayer_Ironhelm_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Dragonflayer_Ironhelm_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(23961, 1, "Dragonflayer_Ironhelm_OnCombat")
RegisterUnitEvent(23961, 2, "Dragonflayer_Ironhelm_OnLeaveCombat")
RegisterUnitEvent(23961, 4, "Dragonflayer_Ironhelm_OnDeath")

function Dragonflayer_Ironhelm_Heroic_Strike(pUnit, Event)
pUnit:FullCastSpellOnTarget(29426, pUnit:GetMainTank())
end

function Dragonflayer_Ironhelm_Ringing_Slap(pUnit, Event)
pUnit:FullCastSpellOnTarget(42780, pUnit:GetMainTank())
end
