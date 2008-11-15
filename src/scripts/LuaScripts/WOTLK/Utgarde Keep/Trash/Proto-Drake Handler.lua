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

function ProtoDrake_Handler_OnCombat(pUnit, Event)
pUnit:RegisterEvent("ProtoDrake_Handler_Debilitating_Strike", math.random(5000, 7000), 0)
pUnit:RegisterEvent("ProtoDrake_Handler_Throw", math.random(8000, 9000), 0)
pUnit:RegisterEvent("ProtoDrake_Handler_Unholy_Rage", 20000, 0)
end

function ProtoDrake_Handler_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function ProtoDrake_Handler_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(24082, 1, "ProtoDrake_Handler_OnCombat")
RegisterUnitEvent(24082, 2, "ProtoDrake_Handler_OnLeaveCombat")
RegisterUnitEvent(24082, 4, "ProtoDrake_Handler_OnDeath")

function ProtoDrake_Handler_Debilitating_Strike(pUnit, Event)
pUnit:FullCastSpellOnTarget(38621, pUnit:GetMainTank())
end

function ProtoDrake_Handler_Throw(pUnit, Event)
pUnit:FullCastSpellOnTarget(54983, pUnit:GetRandomPlayer(0))
end

function ProtoDrake_Handler_Unholy_Rage(pUnit, Event)
pUnit:FullCastSpell(43664)
end
