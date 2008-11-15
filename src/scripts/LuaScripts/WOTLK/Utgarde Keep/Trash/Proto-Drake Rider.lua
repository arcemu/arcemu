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

function ProtoDrake_Rider_OnCombat(pUnit, Event)
pUnit:RegisterEvent("ProtoDrake_Rider_Piercing_Jab", math.random(16000, 18000), 0)
pUnit:RegisterEvent("ProtoDrake_Rider_Throw", math.random(6000, 8000), 0)
pUnit:RegisterEvent("ProtoDrake_Rider_Wing_Clip", 12000, 0)
end

function ProtoDrake_Rider_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function ProtoDrake_Rider_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(24849, 1, "ProtoDrake_Rider_OnCombat")
RegisterUnitEvent(24849, 2, "ProtoDrake_Rider_OnLeaveCombat")
RegisterUnitEvent(24849, 4, "ProtoDrake_Rider_OnDeath")

function ProtoDrake_Rider_Piercing_Jab(pUnit, Event)
pUnit:FullCastSpellOnTarget(31551, pUnit:GetMainTank())
end

function ProtoDrake_Rider_Throw(pUnit, Event)
pUnit:FullCastSpellOnTarget(43665, pUnit:GetRandomPlayer(0))
end

function ProtoDrake_Rider_Wing_Clip(pUnit, Event)
pUnit:FullCastSpellOnTarget(32908, pUnit:GetMainTank())
end
