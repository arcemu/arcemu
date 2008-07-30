--IRONFORGE GENERIC AI GOSSIP SCRIPTS
--Written by Darknation at http://www.wowlegacy.net

--Myra Tyrngaarde(5109)
local cry_delay = 90000
local announces = {}
local announcei = 3 --The number of announces
local choice = 1

RegisterUnitEvent(5109, 6, "Myra_Setup")
announces[1] = "Fresh bread, baked this very morning."
announces[2] = "Come get yer fresh bread."
announces[3] = "Fresh bread for sale."


function Myra_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end