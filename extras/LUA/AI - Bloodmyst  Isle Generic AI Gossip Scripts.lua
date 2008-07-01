--BLOODMYST  ISLE GENERIC AI GOSSIP SCRIPTS
--Written by Darknation at http://www.wowlegacy.net

--Messenger Hermesius(17703)
local cry_delay = 90000
local announces = {}
local announcei = 4 --The number of announces
local choice = 1

RegisterUnitEvent(17703, 6, "Hermesius_Setup")
announces[1] = "One of these days I'm going to bust out the crystalline doomhammer and go positively orc on this town. Then they'll be sorry.."
announces[2] = "He thinks he's so damned important. Always pushing me around... I'll show him. I'll show all of them!"
announces[3] = "I need to be re-assigned. This job is pushing me over the edge..."
announces[4] = "Don't you people ever check the mail?"


function Hermesius_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function Hermesius_Setup(pUnit, Event)
   pUnit:RegisterEvent("Hermesius_Tick", cry_delay, 0)
end

--Magus Filinthus(16821)
local cry_delay = 90000
local announces = {}
local announcei = 7 --The number of announces
local choice = 1

RegisterUnitEvent(16821, 6, "MagusFilinthus_Setup")
announces[1] = "What manner of fool opens the very thing we were sent to close."
announces[2] = "And have you heard? Have you heard the perposterous claims? Alliance and Horde supporting peace? Why the very thought of such things makes my blood boil!"
announces[3] = "A mailbox? Well is that not the salt on the wounds! Twenty years cut-off from the world and now a mailbox! WONDERFUL!"
announces[4] = "Sid! Ale, now!"
announces[5] = "To trust an orc? Even if it is the progeny of Durotan himself! This is madness!"
announces[6] = "Twenty years lost! Twenty! I won't stand for this!"
announces[7] = "I have a good mind to tear down that portal myself! This is outrageous!"

function MagusFilinthus_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function MagusFilinthus_Setup(pUnit, Event)
   pUnit:RegisterEvent("MagusFilinthus_Tick", cry_delay, 0)
end
