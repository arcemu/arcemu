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
--Recon from Hungary :)

function Tavarok_Earthquake(pUnit, Event)
	print "Tavarok Earthquake"
	pUnit:CastSpell(33919)
	pUnit:SendChatMessage(11, 0, "Do you feel my Earthquake...")
end

function Tavarok_Crystal_Poison(pUnit, Event)
	print "Tavarok Crystal Poison"
	pUnit:FullCastSpellOnTarget(32361,Unit:GetRandomPlayer(0))
	pUnit:SendChatMessage(11, 0, "Some poison...")
end
--  Heroic spell --
function Tavarok_smash(pUnit, Event)
	print "Tavarok_smash"
	pUnit:CastSpellOnTarget(38761,pUnit:GetClosestPlayer(0))
end

function Tavarok_OnCombat(pUnit, Event)
	print "Tavarok"
	pUnit:RegisterEvent("Tavarok_Earthquake",10000,0)
	pUnit:RegisterEvent("Tavarok_Crystal_Poison",13000,0)
	pUnit:RegisterEvent("Tavarok_smash",13000,0)
end

function Tavarok_OnKilledTarget (pUnit, Event)
	
end

function Tavarok_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Tavarok_OnDied(pUnit, Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(18343, 1, "Tavarok_OnCombat")
RegisterUnitEvent(18343, 2, "Tavarok_OnLeaveCombat")
RegisterUnitEvent(18343, 3, "Tavarok_OnKilledTarget")
RegisterUnitEvent(18343, 4, "Tavarok_OnDied")