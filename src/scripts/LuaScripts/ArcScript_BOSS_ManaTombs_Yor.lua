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
--Scripted: recon
function Yor_Fire(pUnit, Event)
	print "Yor Double Breath"
	pUnit:FullCastSpellOnTarget(38361,Unit:GetClosestPlayer(0))
end

function Yor_stomp(pUnit, Event)
	print "Yor stomp"
	pUnit:CastSpell(36405)
end

function Yor_OnCombat(pUnit, Event)
	print "Yor"
	pUnit:RegisterEvent("Yor_Fire",10000,0)
	pUnit:RegisterEvent("Yor_stomp",13000,0)
end


function Yor_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Yor_OnDied(pUnit, Event)
	pUnit:RemoveEvents()
end


RegisterUnitEvent(22930, 1, "Yor_OnCombat")
RegisterUnitEvent(22930, 2, "Yor_OnLeaveCombat")
RegisterUnitEvent(22930, 3, "Yor_OnDied")