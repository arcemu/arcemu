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
Special thanks: Janni, Recon, Nandi.
-- ]]
RegisterUnitEvent(1402, 18, "mcnabb")
RegisterUnitEvent(1402, 4, "mcnabb_Died")

function mcnabb_Died(Unit, event, player)
Unit:RemoveEvents()
end

function mcnabb(Unit, event, player)
Unit:RegisterEvent("mcnabb_Say",40000, 0)
end

function mcnabb_Say(Unit, event, player)
local chance = math.random(1,3)
if(chance == 1) then
Unit:SendChatMessage(12, 0, "It's all their fault, stupid Alliance army. Just had to build their towers right behind my farm." )
end
if(chance == 2) then
Unit:SendChatMessage(12, 0, "I will gladly pay you Tuesday for a hamburger today.")
end
if(chance == 3) then
Unit:SendChatMessage(12, 0, "Spare some change for a poor blind man? ...What do you mean I'm not blind? ...I'M NOT BLIND! I CAN SEE! It's a miracle!")
end
end