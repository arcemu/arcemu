--ARCANE PATROLLER 

local cry_delay = 18000
local announces = {}
local announcei = 2 --The number of announces
local choice = 1

RegisterUnitEvent(15638, 1, "ArcanePatroller_Setup")
announces[1] = "Serve the public trust!"
announces[2] = "Uphold the law!"

function ArcanePatroller_OnEnterCombat(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function ArcanePatroller_Setup(pUnit, Event)
   pUnit:RegisterEvent("ArcanePatroller_OnEnterCombat", cry_delay, 0)
end

--WRETCHED URCHIN

local cry_delay = 21000
local announces = {}
local announcei = 2 --The number of announces
local choice = 1

RegisterUnitEvent(15644, 1, "WretchedUrchin_Setup")
announces[1] = "I cannot stand the pain. I must find a new source."
announces[2] = "The pain... the need... I hunger for more..."

function WretchedUrchin_OnEnterCombat(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 10, announces[choice])
end

function WretchedUrchin_Setup(pUnit, Event)
   pUnit:RegisterEvent("WretchedUrchin_OnEnterCombat", cry_delay, 0)
end

--FELENDREN THE BANISHED

local cry_delay = 18000
local announces = {}
local announcei = 2 --The number of announces
local choice = 1

RegisterUnitEvent(15367, 1, "Felendren_Setup")
announces[1] = "Take heart! Your friends will not mourn your passing!"
announces[2] = "Take heart! Your friends will not mourn your passing!"

function Felendren_OnEnterCombat(pUnit, Event)
   choice = math.random(0.5, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function Felendren_Setup(pUnit, Event)
   pUnit:RegisterEvent("Felendren_OnEnterCombat", cry_delay, 0)
end

--APOTHECRAY VENUSTUS

local cry_delay = 90000
local announces = {}
local announcei = 1 --The number of announces
local choice = 1

RegisterUnitEvent(16464, 5, "Venustus_Setup")
announces[1] = "Drat! This bloodthistle extract doesn't seem to want to combine properly with my poison. I'll not be deterred - those trolls will be dealt with! Prehaps a different mixture?"

function Venustus_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function Venustus_Setup(pUnit, Event)
   pUnit:RegisterEvent("Venustus_Tick", cry_delay, 0)
end

--PROSPECTOR ANVILWARD

local cry_delay = 1
local announces = {}
local announcei = 1 --The number of announces
local choice = 1

RegisterUnitEvent(15420, 10, "Anvilward_Setup")
announces[1] = "These arcane sanctums of yours are quite... interesting Mr. Caidanis. I appreciate the information you have given me today."

function Anvilward_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function Anvilward_Setup(pUnit, Event)
   pUnit:RegisterEvent("Anvilward_Tick", cry_delay, 0)
end

--LEY-KEEPER CAIDANIS

local cry_delay = 1
local announces = {}
local announcei = 1 --The number of announces
local choice = 1

RegisterUnitEvent(15405, 10, "Caidanis_Setup")
announces[1] = "It's been a pleasure Mr. Anvilward. I must say your entheusastic interest in our operations is most... unexpected."

function Caidanis_Tick(pUnit, Event)
   choice = math.random(1, announcei)
   pUnit:SendChatMessage(12, 0, announces[choice])
end

function Caidanis_Setup(pUnit, Event)
   pUnit:RegisterEvent("Caidanis_Tick", cry_delay, 0)
end
