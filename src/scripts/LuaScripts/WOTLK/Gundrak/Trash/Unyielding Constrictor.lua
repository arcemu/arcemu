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

function Unyielding_Constrictor_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Unyielding_Constrictor_Vicious_Bite", math.random(17000, 19000), 0)
end

function Unyielding_Constrictor_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Unyielding_Constrictor_OnKillTarget(pUnit, Event)
end

function Unyielding_Constrictor_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29768, 1, "Unyielding_Constrictor_OnCombat")
RegisterUnitEvent(29768, 2, "Unyielding_Constrictor_OnLeaveCombat")
RegisterUnitEvent(29768, 3, "Unyielding_Constrictor_OnKillTarget")
RegisterUnitEvent(29768, 4, "Unyielding_Constrictor_OnDeath")

function Unyielding_Constrictor_Vicious_Bite(pUnit, Event)
pUnit:FullCastSpellOnTarget(55602, pUnit:GetMainTank())
end
