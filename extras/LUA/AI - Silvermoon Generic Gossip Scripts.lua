--SILVERMOON GENERIC AI GOSSIP SCRIPTS
--Written by Darknation at http://www.wowlegacy.net

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--ARCANE GUARDIAN
local cry_delay = 90000
local announces = {}
local announcei = 4 --The number of announces
local choice = 1

RegisterUnitEvent(18103, 6, "ArcaneGuardian_Setup")
announces[1] = "Remain strong. Kael'thas will - error - Lor'themar will lead you to power and glory!"
announces[2] = "Obey the laws of silvermoon. Failure to do so will result in termination."
announces[3] = "Do not disturb the serenity of the city. Peace must be upheld."
announces[4] = "Maintain order within these walls."

function ArcaneGuardian_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function ArcaneGuardian_Setup(pUnit, Event)
   pUnit:RegisterEvent("ArcaneGuardian_Tick", cry_delay, 0)
end
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--TRAYANISE (18230):  
local cry_delay = 150000
local announces = {}
local announcei = 3 --The number of announces
local choice = 1

RegisterUnitEvent(18230, 6, "Trayanise_Setup")
announces[1] = "Argh. They told me those crystals would work properly!"
announces[2] = "The magister's going to kill me..."
announces[3] = "This is becoming tiresome."

function Trayanise_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function Trayanise_Setup(pUnit, Event)
   pUnit:RegisterEvent("Trayanise_Tick", cry_delay, 0)
end
