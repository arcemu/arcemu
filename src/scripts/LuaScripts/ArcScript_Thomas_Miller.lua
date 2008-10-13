   /******************************************************************
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
   ******************************************************************/

--[[
********************************
*                              *
*      The ArcScript Project   *
*                              *
********************************

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
Special thanks: Janni, Recon, Nandi.
-- ]]

function baker_Died(Unit, event, player)
Unit:RemoveEvents()
end

function baker(Unit, event, player)
Unit:RegisterEvent("baker_Say",28000, 0)
end

function baker_Say(Unit, event, player)
local chance = math.random(1,3)
if(chance == 1) then
Unit:SendChatMessage(12, 0, "Fresh bread, baked this very morning!")
end
if(chance == 2) then
Unit:SendChatMessage(12, 0, "Come get yer fresh bread!")
end
if(chance == 3) then
Unit:SendChatMessage(12, 0, "Fresh bread for sale!")
end
end

RegisterUnitEvent(3518, 18, "baker")
RegisterUnitEvent(3518, 4, "baker_Died")