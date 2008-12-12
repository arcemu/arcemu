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

function Sladran_Viper_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Slad'ran_Viper_Venomous_Bite", math.random(12000, 16000), 0)
end

function Sladran_Viper_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Sladran_Viper_OnKillTarget(pUnit, Event)
end

function Sladran_Viper_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29680, 1, "Sladran_Viper_OnCombat")
RegisterUnitEvent(29680, 2, "Sladran_Viper_OnLeaveCombat")
RegisterUnitEvent(29680, 3, "Sladran_Viper_OnKillTarget")
RegisterUnitEvent(29680, 4, "Sladran_Viper_OnDeath")

function Sladran_Viper_Venomous_Bite(pUnit, Event)
pUnit:FullCastSpellOnTarget(54987, pUnit:GetMainTank())
end