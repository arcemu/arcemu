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

function Drakkari_Golem_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Drakkari_Golem_Body_of_Stone", math.random(45000, 49000), 0)
pUnit:RegisterEvent("Drakkari_Golem_Shockwave", math.random(17000, 24000), 0)
pUnit:RegisterEvent("Drakkari_Golem_Thunderclap", math.random(13000, 16000), 0)
end

function Drakkari_Golem_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Drakkari_Golem_OnKillTarget(pUnit, Event)
end

function Drakkari_Golem_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29832, 1, "Drakkari_Golem_OnCombat")
RegisterUnitEvent(29832, 2, "Drakkari_Golem_OnLeaveCombat")
RegisterUnitEvent(29832, 3, "Drakkari_Golem_OnKillTarget")
RegisterUnitEvent(29832, 4, "Drakkari_Golem_OnDeath")

function Drakkari_Golem_Body_of_Stone(pUnit, Event)
pUnit:FullCastSpell(55633)
end

function Drakkari_Golem_Shockwave(pUnit, Event)
pUnit:FullCastSpell(55636)
end

function Drakkari_Golem_Thunderclap(pUnit, Event)
pUnit:FullCastSpell(55635)
end
