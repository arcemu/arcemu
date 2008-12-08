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

function SunbladeBloodKnight_OnCombat(Unit, Event)
Unit:RegisterEvent("SunbladeBloodKnight_HolyLight", 6300, 40)
end

-- I dont't know how often he does this just set it to 6, 3 sec
function SunbladeBloodKnight_HolyLight(Unit, Event)
Unit:FullCastSpell(27136)
end

function SunbladeBloodKnight_LeaveCombat(Unit, Event)
Unit:RemoveEvents()
end

function SunbladeBloodKnight_Died(Unit, Event)
Unit:RemoveEvents()
end


RegisterUnitEvent(27136, 1, "SunbladeBloodKnight_OnCombat")
RegisterUnitEvent(27136, 2, "SunbladeBloodKnight_LeaveCombat")
RegisterUnitEvent(27136, 4, "SunbladeBloodKnight_Died")
