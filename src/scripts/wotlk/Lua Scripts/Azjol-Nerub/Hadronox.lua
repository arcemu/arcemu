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
Made by: Recon
-- ]]

function Hadronox_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:RegisterEvent("WebGrab",15000, 0)--53406--
	pUnit:RegisterEvent("Pierce_Armor",6000, 0)--53418--
	pUnit:RegisterEvent("Leech_Poison",10000, 0)--53030--
	pUnit:RegisterEvent("Acidcloud",20000, 0)	--53400--
end

function WebGrab (pUnit,Event)
	pUnit:FullCastSpellOnTarget(53406,pUnit:GetClosestPlayer(0))
end

function Pierce_Armor (pUnit,Event)
	pUnit:FullCastSpellOnTarget(53418,pUnit:GetClosestPlayer(0))
end

function Leech_Poison (pUnit,Event)
	pUnit:FullCastSpellOnTarget(53030,pUnit:GetClosestPlayer(0))
end

function Acidcloud (pUnit,Event)
	pUnit:FullCastSpellOnTarget(53400,pUnit:GetClosestPlayer(0))
end

function Hadronox_OnKilledTarget (pUnit, Event)
 --Every time Hadronox kills a player or attacking Mob, he re-gains health. --
end

function Hadronox_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Hadronox_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29309, 1, "Hadronox_OnCombat")
RegisterUnitEvent(29309, 2, "Hadronox_OnLeaveCombat")
RegisterUnitEvent(29309, 3, "Hadronox_OnDied")
RegisterUnitEvent(29309, 4, "Hadronox_OnKilledTarget")