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

function Drakkari_Fire_Weaver_OnCombat(pUnit, Event)
pUnit:RegisterEvent("Drakkari_Fire_Weaver_Blast_Wave", math.random(19000, 23000), 0)
pUnit:RegisterEvent("Drakkari_Fire_Weaver_Lava_Burst", math.random(25000, 27000), 0)
pUnit:RegisterEvent("Drakkari_Fire_Weaver_Flame_Shock", math.random(12000, 14000), 0)
end

function Drakkari_Fire_Weaver_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end

function Drakkari_Fire_Weaver_OnKillTarget(pUnit, Event)
end

function Drakkari_Fire_Weaver_OnDeath(pUnit, Event)
pUnit:RemoveEvents()
end

RegisterUnitEvent(29822, 1, "Drakkari_Fire_Weaver_OnCombat")
RegisterUnitEvent(29822, 2, "Drakkari_Fire_Weaver_OnLeaveCombat")
RegisterUnitEvent(29822, 3, "Drakkari_Fire_Weaver_OnKillTarget")
RegisterUnitEvent(29822, 4, "Drakkari_Fire_Weaver_OnDeath")

function Drakkari_Fire_Weaver_Blast_Wave(pUnit, Event)
pUnit:FullCastSpell(15744)
end

function Drakkari_Fire_Weaver_Lava_Burst(pUnit, Event)
pUnit:FullCastSpellOnTarget(55659, pUnit:GetRandomPlayer(0))
end

function Drakkari_Fire_Weaver_Flame_Shock(pUnit, Event)
pUnit:FullCastSpellOnTarget(55613, pUnit:GetRandomPlayer(0))
end
