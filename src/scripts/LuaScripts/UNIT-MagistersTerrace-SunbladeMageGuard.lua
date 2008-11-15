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

function SunbladeMageGuard_OnCombat(Unit, Event)
Unit:RegisterEvent("SunbladeMageGuard_GlaiveThrow", 7000, 0)
end

function SunbladeMageGuard_GlaiveThrow(Unit, Event)
FlipGlaive = math.random(1, 2)
if FlipGlaive==1 then
Unit:CastSpellOnTarget(44478, Unit:GetRandomPlayer(7))
else
Unit:CastSpellOnTarget(46028, Unit:GetRandomPlayer(7))
end
end

function SunbladeMageGuard_LeaveCombat(Unit, Event)
Unit:RemoveEvents()
end

function SunbladeMageGuard_Died(Unit, Event)
Unit:RemoveEvents()
end



RegisterUnitEvent(24683, 1, "SunbladeMageGuard_OnCombat")
RegisterUnitEvent(24683, 2, "SunbladeMageGuard_LeaveCombat")
RegisterUnitEvent(24683, 4, "SunbladeMageGuard_Died")
