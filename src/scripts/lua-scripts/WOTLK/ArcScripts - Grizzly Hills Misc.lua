--[[ Miscellaneous in Grizzly Hills. Not yet 100% finished, however it´s 100% functionable :)


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

 Script Made By FaKiE ]]

function Death(pUnit, event)
pUnit:RemoveEvents()
end

--[[function Deciphering_the_Journal_Completed(pUnit, event)
pUnit:SendChatMessage(12, 0, "Let us see what this journal reveals.")
pUnit:SetNPCFlags(0)
pUnit:RegisterEvent]]

function RunicBattleGolem_EnterCombat(pUnit, event)
local timer1=math.random(250, 8500)
pUnit:RegisterEvent("Battle_Runes", timer1, 1)
end

function Battle_Runes(pUnit, event)
pUnit:CastSpell(52630)
end

RegisterUnitEvent (26284, 1, "RunicBattleGolem_EnterCombat")
RegisterUnitEvent (26284, 4, "Death")
RegisterUnitEvent (26284, 2, "Death")

----------------------------------------------------------------

function Iron_Rune_Shaper_EnterCombat(pUnit, event)
local timer1=math.random(1200, 12500)
pUnit:RegisterEvent("Rune_of_Retribution", timer1, 1)
end

function Rune_of_Retribution(pUnit, event)
pUnit:CastSpell(52628)
end

RegisterUnitEvent (26270, 1, "Iron_Rune_Shaper_EnterCombat")
RegisterUnitEvent (26270, 4, "Death")
RegisterUnitEvent (26270, 2, "Death")

----------------------------------------------------------------

function Iron_Rune_Weaver_EnterCombat(pUnit, event)
local timer1=math.random(1200, 4500)
pUnit:RegisterEvent("Rune_of_Retribution", timer1, 1)
end

function Rune_Weaving(pUnit, event)
local timer2=math.random(3500, 12500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(52713, pUnit:GetMainTank())
pUnit:RegisterEvent("Rune_Weaving", timer2, 0)
end
end

RegisterUnitEvent (26820, 1, "Iron_Rune_Weaver_EnterCombat")
RegisterUnitEvent (26820, 4, "Death")
RegisterUnitEvent (26820, 2, "Death")

----------------------------------------------------------------

function Duskhowl_Prowler_EnterCombat(pUnit, event)
local timer1=math.random(3200, 6500)
pUnit:RegisterEvent("Gore", timer1, 1)
end

function Gore(pUnit, event)
local timer2=math.random(12500, 19500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(32019, pUnit:GetMainTank())
pUnit:RegisterEvent("Gore", timer2, 0)
end
end

RegisterUnitEvent (27408, 1, "Duskhowl_Prowler_EnterCombat")
RegisterUnitEvent (27408, 4, "Death")
RegisterUnitEvent (27408, 2, "Death")
RegisterUnitEvent (26592, 1, "Duskhowl_Prowler_EnterCombat")
RegisterUnitEvent (26592, 4, "Death")
RegisterUnitEvent (26592, 2, "Death")

----------------------------------------------------------------

function Overseer_Durval_EnterCombat(pUnit, event)
local timer1=math.random(1200, 6500)
local RandomYell=math.random(1, 4)
pUnit:RegisterEvent("Rune_of_Destruction", timer1, 1)
if RandomYell==1 then
pUnit:SendChatMessage(12, 0, "Fool! You will never get away with this!")
elseif RandomYell==2 then
pUnit:SendChatMessage(12, 0, "What do you think you´re doing?")
elseif RandomYell==3 then
pUnit:SendChatMessage(12, 0, "You´ll pay for this!")
elseif RandomYell==4 then
pUnit:SendChatMessage(12, 0, "You dare to defy the sons of iron?")
end
end

function Rune_of_Destruction(pUnit, event)
local timer2=math.random(12500, 15500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(52715, pUnit:GetMainTank())
pUnit:RegisterEvent("Rune_of_Destruction", timer2, 0)
end
end

RegisterUnitEvent (26920, 1, "Overseer_Durval_EnterCombat")
RegisterUnitEvent (26920, 4, "Death")
RegisterUnitEvent (26920, 2, "Death")

----------------------------------------------------------------

function Overseer_Lochli_EnterCombat(pUnit, event)
local timer1=math.random(1200, 6500)
local RandomYell=math.random(1, 4)
pUnit:RegisterEvent("Thunderstorm", timer1, 1)
if RandomYell==1 then
pUnit:SendChatMessage(12, 0, "Fool! You will never get away with this!")
elseif RandomYell==2 then
pUnit:SendChatMessage(12, 0, "What do you think you´re doing?")
elseif RandomYell==3 then
pUnit:SendChatMessage(12, 0, "You´ll pay for this!")
elseif RandomYell==4 then
pUnit:SendChatMessage(12, 0, "You dare to defy the sons of iron?")
end
end

function Thunderstorm(pUnit, event)
local timer2=math.random(12500, 15500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(52717, pUnit:GetMainTank())
pUnit:RegisterEvent("Thunderstorm", timer2, 0)
end
end

RegisterUnitEvent (26922, 1, "Overseer_Lochli_EnterCombat")
RegisterUnitEvent (26922, 4, "Death")
RegisterUnitEvent (26922, 2, "Death")

----------------------------------------------------------------

function Overseer_Korgan_EnterCombat(pUnit, event)
local timer1=math.random(1200, 6500)
local RandomYell=math.random(1, 4)
pUnit:RegisterEvent("Call_Lightning", timer1, 1)
pUnit:RegisterEvent("Korgan_Heal", 1000, 0)
if RandomYell==1 then
pUnit:SendChatMessage(12, 0, "Fool! You will never get away with this!")
elseif RandomYell==2 then
pUnit:SendChatMessage(12, 0, "What do you think you´re doing?")
elseif RandomYell==3 then
pUnit:SendChatMessage(12, 0, "You´ll pay for this!")
elseif RandomYell==4 then
pUnit:SendChatMessage(12, 0, "You dare to defy the sons of iron?")
end
end

function Call_Lightning(pUnit, event)
local timer2=math.random(12500, 15500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(32018, pUnit:GetMainTank())
pUnit:RegisterEvent("Call_Lightning", timer2, 0)
end
end

function Korgan_Heal(pUnit, event)
if healed==1 then
else
if pUnit:GetHealthPct() < 20 then
pUnit:CastSpell(52714)
healed=1
end
end
end

RegisterUnitEvent (26921, 1, "Overseer_Korgan_EnterCombat")
RegisterUnitEvent (26921, 4, "Death")
RegisterUnitEvent (26921, 2, "Death")

----------------------------------------------------------------

function Iron_Rune_Overseer_EnterCombat(pUnit, event)
local timer1=math.random(1200, 4500)
pUnit:RegisterEvent("Call_Lignthing", timer1, 1)
end

RegisterUnitEvent (27177, 1, "Iron_Rune_Overseer_EnterCombat")
RegisterUnitEvent (27177, 4, "Death")
RegisterUnitEvent (27177, 2, "Death")

----------------------------------------------------------------

function Overseer_Brunon_EnterCombat(pUnit, event)
local RandomYell=math.random(1, 4)
if RandomYell==1 then
pUnit:SendChatMessage(12, 0, "Fool! You will never get away with this!")
elseif RandomYell==2 then
pUnit:SendChatMessage(12, 0, "What do you think you´re doing?")
elseif RandomYell==3 then
pUnit:SendChatMessage(12, 0, "You´ll pay for this!")
elseif RandomYell==4 then
pUnit:SendChatMessage(12, 0, "You dare to defy the sons of iron?")
end
end

RegisterUnitEvent (26923, 1, "Overseer_Brunon_EnterCombat")
RegisterUnitEvent (26923, 4, "Death")
RegisterUnitEvent (26923, 2, "Death")

----------------------------------------------------------------

function Iron_Rune_Smith_EnterCombat(pUnit, event)
local timer1=math.random(3200, 6500)
pUnit:RegisterEvent("Smelt_Rune", timer1, 1)
pUnit:RegisterEvent("Lightning_Charged", 100, 1)
end

function Smelt_Rune(pUnit, event)
local timer2=math.random(12500, 15500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(52699, pUnit:GetMainTank())
pUnit:RegisterEvent("Smelt_Rune", timer2, 0)
end
end

RegisterUnitEvent (26408, 1, "Iron_Rune_Smith_EnterCombat")
RegisterUnitEvent (26408, 4, "Death")
RegisterUnitEvent (26408, 2, "Death")

----------------------------------------------------------------

function Rune_Smith_Durar_EnterCombat(pUnit, event)
pUnit:RegisterEvent("Lightning_Charged", 100, 1)
end

function Lightning_Charged(pUnit, event)
pUnit:CastSpell(52710)
end

RegisterUnitEvent (26408, 1, "Iron_Rune_Smith_EnterCombat")
RegisterUnitEvent (26408, 4, "Death")
RegisterUnitEvent (26408, 2, "Death")

----------------------------------------------------------------

function Frostpaw_Warrior_EnterCombat(pUnit, event)
local timer1=math.random(5200, 13500)
local timer3=math.random(5200, 13500)
pUnit:RegisterEvent("Frostpaw_Rend", timer1, 1)
pUnit:RegisterEvent("Demoralizing_Shout", timer3, 1)
end

function Frostpaw_Rend(pUnit, event)
local timer2=math.random(10000, 22500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:FullCastSpellOnTarget(12054, pUnit:GetMainTank())
pUnit:RegisterEvent("Frostpaw_Rend", timer2, 0)
end
end

function Demoralizing_Shout(pUnit, event)
local timer4=math.random(30000, 45000)
pUnit:CastSpell(13730)
pUnit:RegisterEvent("Demoralizing_Shout", timer4, 0)
end

RegisterUnitEvent (26357, 1, "Frostpaw_Warrior_EnterCombat")
RegisterUnitEvent (26357, 4, "Death")
RegisterUnitEvent (26357, 2, "Death")

----------------------------------------------------------------

function Tallhorn_Stag_EnterCombat(pUnit, event)
local timer1=math.random(3200, 13500)
pUnit:RegisterEvent("Gore", timer1, 1)
end

RegisterUnitEvent (26363, 1, "Tallhorn_Stag_EnterCombat")
RegisterUnitEvent (26363, 4, "Death")
RegisterUnitEvent (26363, 2, "Death")

----------------------------------------------------------------

function Warlord_ZimBo_EnterCombat(pUnit, event)
pUnit:SendChatMessage(12, 0, "For Drak'Tharon!")
pUnit:RegisterEvent("ZimBo_Talk", 1500, 1)
end

function ZimBo_Talk(pUnit, event)
local timer1=math.random(50, 7500)
pUnit:SendChatMessage(12, 0, "Zim'Bo must live to slay our betrayer!")
pUnit:CastSpell(52283)
pUnit:RegisterEvent("ZimBo_Talk2", 1800, 1)
pUnit:RegisterEvent("SkullCrack", timer1, 1)
end

function SkullCrack(pUnit, event)
local timer2=math.random(3500, 8500)
if (pUnit:GetMainTank() ~= nil) then
pUnit:CastSpellOnTarget(3551, pUnit:GetMainTank())
pUnit:RegisterEvent("SkullCrack", timer2, 1)
end
end

function ZimBo_Talk2(pUnit, event)
pUnit:SendChatMessage(12, 0, "Zim'Bo Cannot be stopped! Da keep must be liberated!")
pUnit:RegisterEvent("ZimBo_Talk3", 2000, 1)
end

function ZimBo_Talk3(pUnit, event)
pUnit:SendChatMessage(12, 0, "You be dyin' along with all dese scourge!")
end

function ZimboDeath(pUnit, event)
pUnit:SendChatMessage(12, 0, "Betrayed by one of our own we were. Disgraced...")
pUnit:RemoveEvents()
end

RegisterUnitEvent (26544, 1, "Warlord_ZimBo_EnterCombat")
RegisterUnitEvent (26544, 4, "ZimboDeath")
RegisterUnitEvent (26544, 2, "Death")



--[[function First_Prophecy(pUnit, event)
pUnit:MarkQuestObjectiveAsComplete (12058, 0)
pUnit:SendChatMessage(
end

function Second_Prophecy(pUnit, event)
pUnit:MarkQuestObjectiveAsComplete (x, 1)
end

function Third_Prophecy(pUnit, event)
pUnit:MarkQuestObjectiveAsComplete (x, 2)
end

RegisterGameObjectEvent (x, 2, "x")
"'Greatest of the Maker's children, arise and claim your birthright.'"
"'Shall return to the cradle of iron and stone.'"
"'Return to Ulduar, nestled in the embrace of the storm.'"

BRINGING DOWN THE IRON THANE
"I´ll admit, dwarf, I was pleasantly surprised to see your plan succeed."
"We can´t just let you return to your companions and we can´t take you with us..."
"Wait a minute here! You promised to spare me if I helped you. I held up my end o' the bargain!"
"Stay your blade, Chieftain. He may yet have value to us."
"Remember the dwarven journal that was discovered? Well, this dwarf could be exactly what we need to track down Brann Bronzebeard and capture him."
"By the light, Brann's alive? I'd never help you capture him!"
"You will if you wish to keep your head attached to your shoulders."
"He will live so long as he proves useful. I'd be careful if I was you, dwarf."
MISSING JOURNAL PAGE?
"This is an intriguing find, NAME, but I don´t know what to make of it."
"The language is unfamiliar and for all we know, it´s just some soldier's journal."
"Let me take a look at that."
"Hmm..."
"This journal is written in dwarven. I can´t read this scrawl, but I can make out one thing."
"'Brann Bronzebeard.' The brother of Ironforge's king? You may have spoken too soon, Paluna."]]

