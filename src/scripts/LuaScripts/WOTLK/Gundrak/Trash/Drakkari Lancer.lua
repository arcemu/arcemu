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

function Drakkari_Lancer_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Drakkari_Lancer_Disarm", math.random(17000, 26000), 0)
pUnit:RegisterEvent("Drakkari_Lancer_Impale", math.random(13000, 16000), 0)
pUnit:RegisterEvent("Drakkari_Lancer_Retaliation", 30000, 0)
end

function Drakkari_Lancer_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Drakkari_Lancer_OnKillTarget(pUnit, Event)
end

function Drakkari_Lancer_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29819, 1, "Drakkari_Lancer_OnCombat")
RegisterUnitEvent(29819, 2, "Drakkari_Lancer_OnLeaveCombat")
RegisterUnitEvent(29819, 3, "Drakkari_Lancer_OnKillTarget")
RegisterUnitEvent(29819, 4, "Drakkari_Lancer_OnDeath")

function Drakkari_Lancer_Disarm(pUnit, Event)
pUnit:FullCastSpellOnTarget(6713, pUnit:GetMainTank())
end

function Drakkari_Lancer_Impale(pUnit, Event)
pUnit:FullCastSpellOnTarget(55622, pUnit:GetMainTank())
end

function Drakkari_Lancer_Retaliation(pUnit, Event)
pUnit:FullCastSpell(40546)
end
