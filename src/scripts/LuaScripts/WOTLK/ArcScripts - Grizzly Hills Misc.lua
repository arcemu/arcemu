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
pUnit:SendChatMessage(13, 0, "Let us see what this journal reveals.")
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
pUnit:FullCastSpellOnTarget(52713, pUnit:GetMainTank())
pUnit:RegisterEvent("Rune_Weaving", timer2, 0)
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
local timer2=math.random(12500, 15500)
pUnit:FullCastSpellOnTarget(32019, pUnit:GetMainTank())
pUnit:RegisterEvent("Gore", timer2, 0)
end

RegisterUnitEvent (27408, 1, "Duskhowl_Prowler_EnterCombat")
RegisterUnitEvent (27408, 4, "Death")
RegisterUnitEvent (27408, 2, "Death")

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
pUnit:FullCastSpellOnTarget(52715, pUnit:GetMainTank())
pUnit:RegisterEvent("Rune_of_Destruction", timer2, 0)
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
pUnit:FullCastSpellOnTarget(52717, pUnit:GetMainTank())
pUnit:RegisterEvent("Thunderstorm", timer2, 0)
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
pUnit:FullCastSpellOnTarget(32018, pUnit:GetMainTank())
pUnit:RegisterEvent("Call_Lightning", timer2, 0)
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
pUnit:FullCastSpellOnTarget(52699, pUnit:GetMainTank())
pUnit:RegisterEvent("Smelt_Rune", timer2, 0)
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


--[[function First_Prophecy(pUnit, event)
pUnit:MarkQuestObjectiveAsComplete (12058, 0)
pUnit:SendChatMessage(
end

function Second_Prophecy(pUnit, event)
pUnit:MarkQuestObjectiveAsComplete (12058, 1)
end

function Third_Prophecy(pUnit, event)
pUnit:MarkQuestObjectiveAsComplete (12058, 2)
end

RegisterGameObjectEvent (180516, 2, "ShrineOfDathRemar")
"'Greatest of the Maker's children, arise and claim your birthright.'"
"'Shall return to the cradle of iron and stone.'"
"'Return to Ulduar, nestled in the embrace of the storm.'"]]

