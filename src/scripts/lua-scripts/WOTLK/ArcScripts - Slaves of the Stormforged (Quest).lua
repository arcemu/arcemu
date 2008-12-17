--[[ Slaves of the Stormforged LUA Quest Script


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

function MechaGnome_OnGossip(pUnit, event, player, pMisc)
if (player:HasFinishedQuest(12937) == true) and (player:HasFinishedQuest(12931) == true) and (player:HasFinishedQuest(12937) == true) and (player:HasFinishedQuest(12957) == false) then
pUnit:GossipCreateMenu(100, player, 0)
pUnit:GossipMenuAddItem(0, "I´m not a laborer. I´m here to free you from servitude in the mines.", 1, 0)
pUnit:GossipSendMenu(player)
end
end

function MechaGnome_OnGossipSel(pUnit, event, player, id, intid, code, pMisc)
if (intid == 1) then
local ChatRandom = math.random(1, 4)
player:AdvanceQuestObjective(12957, 0)
pUnit:RegisterEvent("MechaGnome_Despawn", 4500, 1)
if ChatRandom == 1 then
pUnit:SendChatMessage(12, 0, "Does not compute. Unit malfunctioning. Directive: shut down.")
elseif ChatRandom == 2 then
pUnit:SendChatMessage(12, 0, "New directive: assist in the defeat of the iron dwarves.")
elseif ChatRandom == 3 then
pUnit:SendChatMessage(12, 0, "New directive: leave mine and return to Inventor's Library")
elseif ChatRandom == 4 then
pUnit:SendChatMessage(12, 0, "Thank you, I will join your struggle against the stormforged.")
end
end
end

function MechaGnome_Despawn(pUnit, event)
pUnit:Despawn(0, 0)
end

RegisterUnitGossipEvent(666667, 1, "MechaGnome_OnGossip")
RegisterUnitGossipEvent(666667, 2, "MechaGnome_OnGossipSel")