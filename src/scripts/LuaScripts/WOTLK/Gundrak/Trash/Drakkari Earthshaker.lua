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

function Drakkari_Earthshaker_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Drakkari_Earthshaker_Head_Crack", math.random(27000, 29000), 0)
pUnit:RegisterEvent("Drakkari_Earthshaker_Powerful_Blow", math.random(17000, 18000), 0)
pUnit:RegisterEvent("Drakkari_Earthshaker_Slam_Ground", 33000, 0)
end

function Drakkari_Earthshaker_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Drakkari_Earthshaker_OnKillTarget(pUnit, Event)
end

function Drakkari_Earthshaker_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29829, 1, "Drakkari_Earthshaker_OnCombat")
RegisterUnitEvent(29829, 2, "Drakkari_Earthshaker_OnLeaveCombat")
RegisterUnitEvent(29829, 3, "Drakkari_Earthshaker_OnKillTarget")
RegisterUnitEvent(29829, 4, "Drakkari_Earthshaker_OnDeath")

function Drakkari_Earthshaker_Head_Crack(pUnit, Event)
pUnit:FullCastSpellOnTarget(16172, pUnit:GetMainTank())
end

function Drakkari_Earthshaker_Powerful_Blow(pUnit, Event)
pUnit:FullCastSpellOnTarget(55567, pUnit:GetMainTank())
end

function Drakkari_Earthshaker_Slam_Ground(pUnit, Event)
pUnit:FullCastSpell(55563)
end
