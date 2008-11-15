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
function InjuredDraenei_OnSpawn(pUnit, event)
InjuredDraeneiChance = math.random(1, 2)
if (InjuredDraeneiChance==1) then
	pUnit:SetStandState(3)
end
if (InjuredDraeneiChance==2) then
	 pUnit:SetStandState(1)
     pUnit:SetCombatCapable(0)
end
end
RegisterUnitEvent(16971, 6, "InjuredDraenei_OnSpawn")

function DraeneiSurvivor_OnSpawn(pUnit, event)
SurvivorDraeneiChance = math.random(1, 3)
if (SurvivorDraeneiChance==1) then
	pUnit:SetStandState(3)
end
if (SurvivorDraeneiChance==2) then
     pUnit:CastSpell(28630)
	 pUnit:SetStandState(1)
end
if (SurvivorDraeneiChance==3) then
     pUnit:CastSpell(28630)
     pUnit:SetCombatCapable(0)
end
end

RegisterUnitEvent(16483, 6, "DraeneiSurvivor_OnSpawn")