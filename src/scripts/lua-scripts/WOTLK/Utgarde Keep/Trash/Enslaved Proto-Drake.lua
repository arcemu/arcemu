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

function Enslaved_ProtoDrake_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Enslaved_ProtoDrake_Flame_Breath", math.random(9000, 12000), 0)
pUnit:RegisterEvent("Enslaved_ProtoDrake_Knock_Away", math.random(25000, 27000), 0)
pUnit:RegisterEvent("Enslaved_ProtoDrake_Rend", 15000, 0)
end

function Enslaved_ProtoDrake_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Enslaved_ProtoDrake_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(24083, 1, "Enslaved_ProtoDrake_OnCombat")
RegisterUnitEvent(24083, 2, "Enslaved_ProtoDrake_OnLeaveCombat")
RegisterUnitEvent(24083, 4, "Enslaved_ProtoDrake_OnDeath")

function Enslaved_ProtoDrake_Flame_Breath(pUnit, Event)
pUnit:FullCastSpell(50653)
end

function Enslaved_ProtoDrake_Knock_Away(pUnit, Event)
pUnit:FullCastSpellOnTarget(49722, pUnit:GetMainTank())
end

function Enslaved_ProtoDrake_Rend(pUnit, Event)
pUnit:FullCastSpellOnTarget(43931, pUnit:GetMainTank())
end
