 --[[
   ******************************************************************
   *	 _____              ___                           _         *
   *	(  _  )            (  _`\               _        ( )_       *
   *	| (_) | _ __   ___ | (_(_)   ___  _ __ (_) _ _   | ,_)      *
   *	|  _  |( '__)/'___)`\__ \  /'___)( '__)| |( '_`\ | |        *
   *	| | | || |  ( (___ ( )_) |( (___ | |   | || (_) )| |_       *
   *	(_) (_)(_)  `\____)`\____)`\____)(_)   (_)| ,__/  \__)      *
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

This files Contents:
-Muiltiple NPCs that only say things when players walk by or randomly.

Todo:
-Children in SW

Special thanks: Janni, Recon, Nandi.
-- ]]

--Kira Songshine
RegisterUnitEvent(3937, 18, "kira")
RegisterUnitEvent(3937, 4, "kira_Died")

function kira_Died(Unit, event, player)
Unit:RemoveEvents()
end

function kira(Unit, event, player)
Unit:RegisterEvent("kira_Say",49000, 0)
end

function kira_Say(Unit, event, player)
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

--Mcnabb
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

--Myra Tyrngaarde
RegisterUnitEvent(5109, 18, "tyrnge")
RegisterUnitEvent(5109, 4, "tyrnge_Died")

function tyrnge_Died(Unit, event, player)
Unit:RemoveEvents()
end

function tyrnge(Unit, event, player)
Unit:RegisterEvent("tyrnge_Say",29000, 0)
end

function tyrnge_Say(Unit, event, player)
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

--Baker
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

--Timmy
RegisterUnitEvent(8666, 18, "timmy")
RegisterUnitEvent(8666, 4, "timmy_Died")

function timmy_Died(Unit, event, player)
Unit:RemoveEvents()
end

function timmy(Unit, event, player)
Unit:RegisterEvent("timmy_Say",68000, 0)
end

function timmy_Say(Unit, event, player)
local chance = math.random(1,6)
if(chance == 1) then
Unit:SendChatMessage(12, 0, "Kitten for sale, looking for a good home." )
end
if(chance == 2) then
Unit:SendChatMessage(12, 0, "I can't believe dad won't let me keep your sister.")
end
if(chance == 3) then
Unit:SendChatMessage(12, 0, "Can anyone give my adorable, extra little kitty a home?")
end
if(chance == 4) then
Unit:SendChatMessage(12, 0, "What does allergic mean anyway? And what does it have to do with either of my kitties?")
end
if(chance == 5) then
Unit:SendChatMessage(12, 0, "Will someone please give my little kitten a good home?")
end
if(chance == 6) then
Unit:SendChatMessage(12, 0, "Don't worry, I'll find a good home for ya.")
end
end

--Tosi
RegisterUnitEvent(14498, 18, "tosi")
RegisterUnitEvent(14498, 4, "tosi_Died")

function tosi_Died(Unit, event, player)
Unit:RemoveEvents()
end

function tosi(Unit, event, player)
Unit:RegisterEvent("tosi_Say",69000, 0)
end

function tosi_Say(Unit, event, player)
Unit:SendChatMessage(12, 0, "You are mine now children. You will grow up to be a strong of the horde!")
end
