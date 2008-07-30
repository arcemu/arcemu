local quest_event_begin_trigger = 0
local quest_event_begin_trigger2 = 0

local event_part_1_training_wp_trigger = 0
local event_part_1_main_path_wp_trigger = 0
local event_part_2_mercenary_trigger = 0
local event_part_2_mercenary_summoned = 0
local event_part_2_mercenary_die_trigger = 0
local event_part_3_run_wp_trigger = 0
local event_part_3_complete_trigger = 0
local s_count = 0

local player = nil

-- Need party support ?

-- |= created by Dr_Org =======================================================
-- |= Wizzlecrank Shredder =====================================================

function TheEscape_on_accept_q(plr, quest)
player = plr
if(player ~= nil) then
quest_event_begin_trigger = 1
quest_event_begin_trigger2 = 1
end
end

function Event_part_1_training(unit)
event_part_1_training_wp_trigger = 1

unit:SendChatMessage(11, 0, "Arrrgh! This isn't right!")

unit:CreateCustomWaypointMap()
unit:CreateWaypoint(1109.150, -3104.110, 82.410, 2.862, 1000, 256, 0)
unit:CreateWaypoint(1107.018, -3100.745, 82.305, 2.135, 500, 256, 0)
unit:CreateWaypoint(1106.686, -3105.142, 82.722, 4.636, 1000, 256, 0)
unit:CreateWaypoint(1110.602, -3109.538, 82.693, 5.465, 500, 256, 0)
unit:CreateWaypoint(1114.319, -3106.454, 82.386, 0.702, 1000, 256, 0)
unit:CreateWaypoint(1111.491, -3099.218, 82.380, 1.958, 500, 256, 0)
unit:CreateWaypoint(1104.945, -3103.532, 82.879, 3.737, 500, 256, 0)
unit:CreateWaypoint(1114.084, -3108.398, 82.460, 5.787, 1000, 256, 0)
unit:CreateWaypoint(1112.062, -3110.434, 82.518, 3.945, 1000, 256, 0)
unit:CreateWaypoint(1105.158, -3112.974, 82.891, 3.482, 500, 256, 0)
unit:CreateWaypoint(1109.650, -3104.001, 82.410, 2.862, 1000, 256, 0)

unit:StopMovement(100)
end

function Event_part_1_training_say(unit)
unit:SendChatMessage(11, 0, "Alright, alright! I think I can figure out how to operate this thing...")
end

function Event_part_1_main_path(unit)
-- Set combat mode
unit:SetCombatTargetingCapable(0)

unit:StopMovement(100) -- little kick

unit:SendChatMessage(11, 0, "Okay, I think I've got it, now. Follow me, $n!")

unit:CreateCustomWaypointMap()
unit:CreateWaypoint(1109.150, -3104.110, 82.410, 2.862, 2.430, 500, 0, 0)
unit:CreateWaypoint(1100.539, -3099.153, 83.184, 2.474, 500, 0, 0)
unit:CreateWaypoint(1097.978, -3092.197, 83.349, 1.909, 500, 0, 0)
unit:CreateWaypoint(1098.618, -3081.920, 82.705, 1.508, 500, 0, 0)
unit:CreateWaypoint(1099.713, -3070.979, 82.496, 1.496, 500, 0, 0)
unit:CreateWaypoint(1098.426, -3058.362, 82.126, 1.685, 0, 0, 0)
unit:CreateWaypoint(1096.164, -3046.894, 82.689, 1.728, 0, 0, 0)
unit:CreateWaypoint(1096.154, -3034.650, 83.358, 1.532, 0, 0, 0)
unit:CreateWaypoint(1101.205, -3025.946, 84.400, 0.919, 0, 0, 0)
unit:CreateWaypoint(1107.351, -3016.502, 85.405, 0.970, 0, 0, 0)
unit:CreateWaypoint(1114.803, -3008.761, 86.483, 0.715, 500, 0, 0)
unit:CreateWaypoint(1124.581, -3004.198, 88.907, 0.393, 1000, 0, 0)
unit:CreateWaypoint(1131.638, -3005.269, 91.549, 0.004, 120000, 0, 0) -- 120000 should be enough!

unit:MoveToWaypoint(0)
end

function Event_part_2_spawn_mercenary(unit)
unit:SpawnCreature(3282, 1144.802, -2979.540, 98.872, 3.959, 47, 0)
end

function Event_part_3_act_1_move(unit)
unit:MoveTo(1132.281, -3005.326, 91.646, 6.194)
end

function Event_part_3_run_after_merc(unit)
-- Run after mercenary

event_part_3_run_wp_trigger = 1

unit:CreateCustomWaypointMap()
unit:CreateWaypoint(1127.751, -2988.619, 91.904, 2.108, 0, 256, 0)
unit:CreateWaypoint(1119.836, -2984.906, 92.074, 2.705, 700, 256, 0)
unit:CreateWaypoint(1112.820, -2993.455, 91.333, 4.025, 0, 256, 0)
unit:CreateWaypoint(1107.726, -2977.295, 91.666, 1.861, 0, 256, 0)
unit:CreateWaypoint(1094.337, -2991.589, 91.673, 4.009, 0, 256, 0)
unit:CreateWaypoint(1091.051, -2982.491, 91.922, 1.932, 0, 256, 0)
unit:CreateWaypoint(1086.159, -2990.490, 91.685, 3.255, 10000, 256, 0)

unit:MoveToWaypoint(0)
end

function Event_part_3_say(unit)
unit:SendChatMessage(11, 0, "Looks like we're out of the woods, eh? Wonder what this does...")
end

function Event_part_3_shredder_wreck(unit)
unit:DestroyCustomWaypointMap()

s_count = s_count + 1

if(s_count == 1) then
unit:SendChatMessage(11, 0, "Come on, don't break down on me now!")
end
if(s_count == 2) then
unit:SendChatMessage(11, 0, "That was a close one! Well, let's get going, it's still a ways to Ratchet!")
end
if(s_count == 3) then
unit:SendChatMessage(11, 0, "Hm... I don't think this blinking red light is a good thing...")
end

if(s_count > 3) then
event_part_3_complete_trigger = 1
unit:SpawnCreature(3451, 1086.159, -2990.490, 93.123, 3.255, 69, 0)
unit:RegisterEvent("Event_part_3_destroy_shredder", 100, 1)
end
end

function Event_part_3_destroy_shredder(unit)
-- Suicide
unit:CastSpell(5162)
end

function Wizzlecrank_on_tick(unit)
-- Lost player - lost event!

if(quest_event_begin_trigger2 == 1) then
if(player == nil or player:GetHealthPct() < 1) then
-- need player:MarkQuestFailed(863) !!!
-- need something like: if(...:GetDistance(player) > blah blah) then ...
-- also need player:OnAbandonQuest(questid) or if(player:HasQuest(questid) == nil) then ...

unit:Despawn(1000, 120000)
quest_event_begin_trigger2 = 0
end
end

if(quest_event_begin_trigger == 1) then

-- Training before begin

unit:RegisterEvent("Event_part_1_training_say", 1000, 1)
unit:RegisterEvent("Event_part_1_training", 3000, 1)
quest_event_begin_trigger = 0
end

if(event_part_2_mercenary_die_trigger == 1) then
if(not unit:IsInCombat()) then

-- Say out of woods

unit:RegisterEvent("Event_part_3_say", 2000, 1)
unit:RegisterEvent("Event_part_3_run_after_merc", 4000, 1)
event_part_2_mercenary_die_trigger = 0
end
end
end

function Wizzlecrank_on_died(unit)
if(part3_complete == 0) then
-- player:MarkQuestFailed(863)
end
unit:RemoveEvents()
end

function Wizzlecrank_on_reach_wp(unit, unk1, unk2, wp_id)
if(event_part_1_training_wp_trigger == 1) then
if(wp_id == 11) then
unit:DestroyCustomWaypointMap()
event_part_1_training_wp_trigger = 0
event_part_1_main_path_wp_trigger = 1
unit:RegisterEvent("Event_part_1_main_path", 2000, 1)
end
end

if(event_part_1_main_path_wp_trigger == 1) then
if(wp_id == 13) then
unit:DestroyCustomWaypointMap()
event_part_1_main_path_wp_trigger = 0

if(not unit:IsInCombat()) then
event_part_2_mercenary_trigger = 1
unit:RegisterEvent("Event_part_2_spawn_mercenary", 1000, 1)
end
end
end

if(event_part_3_run_wp_trigger == 1) then
if(wp_id == 7) then
unit:DestroyCustomWaypointMap()
event_part_3_run_wp_trigger = 0
unit:RegisterEvent("Event_part_3_shredder_wreck", 3000, 4)
end
end
end

function Wizzlecrank_on_spawn(unit)
quest_event_begin_trigger = 0
quest_event_begin_trigger2 = 0

event_part_1_training_wp_trigger = 0
event_part_1_main_path_wp_trigger = 0
event_part_2_mercenary_trigger = 0
event_part_2_mercenary_summoned = 0
event_part_2_mercenary_die_trigger = 0
event_part_3_run_wp_trigger = 0
event_part_3_complete_trigger = 0
s_count = 0

player = nil

unit:SetCombatTargetingCapable(1) -- disable combat. hack!
unit:SetFaction(1812) -- 113 must be when event begins. But 113 didnt work
unit:RegisterEvent("Wizzlecrank_on_tick", 1000, 0)
end

-- |= Wenture Co. Mercenary ====================================================

function VentureCo_Mercenary_aggro_run(unit)
unit:SendChatMessage(12, 0, "There's the stolen shredder! Stop it or Lugwizzle will have our hides!")

unit:CreateCustomWaypointMap()

-- need MoveTo() with speed modifier!
unit:CreateWaypoint(1134.751, -3002.147, 91.667, 4.197, 0, 256, 0)
unit:MoveToWaypoint(0)
unit:DestroyCustomWaypointMap()
end

function VentureCo_Mercenary_on_died(unit)
if(event_part_2_mercenary_summoned == 1) then
event_part_2_mercenary_die_trigger = 1
end
end

function VentureCo_Mercenary_on_spawn(unit)
if(event_part_2_mercenary_trigger == 1) then
event_part_2_mercenary_summoned = 1
unit:RegisterEvent("VentureCo_Mercenary_aggro_run", 1000, 1)
end
end

-- |= Wizzlecrank Pilot ========================================================

function WizzlecrankPilot_say1(unit)
unit:SendChatMessage(11, 0, "Looks like you'll have to go ahead to Ratchet and tell Sputtervalve that I've wrecked the shredder.")
end

function WizzlecrankPilot_say2(unit)
unit:SendChatMessage(11, 0, "I'll stay behind and guard the wreck. Hurry! Hopefully no one will notice the smoke.")
end

function WizzlecrankPilot_on_spawn(unit)
if(event_part_3_complete_trigger == 1) then
if(player ~= nil) then
player:MarkQuestObjectiveAsComplete(863, 1)

-- Bugged! Quest already completed. Need correct work for ObjectiveX
-- I think need to add columns in quests like: Objective1, Objective2, Objective3, Objective4

event_part_3_complete_trigger = 0

unit:RegisterEvent("WizzlecrankPilot_despawn", 30000, 1)
unit:RegisterEvent("WizzlecrankPilot_move", 500, 1)
unit:RegisterEvent("WizzlecrankPilot_say1", 4000, 1)
unit:RegisterEvent("WizzlecrankPilot_say2", 8000, 1)
end
end
unit:RegisterEvent("WizzlecrankPilot_despawn", 30000, 1)
end

function WizzlecrankPilot_despawn(unit)
unit:Despawn(0, 0)
end

function WizzlecrankPilot_move(unit)
unit:MoveTo(1082.385, -2994.907, 91.708, 4.005)
end

-- |============================================================================

RegisterQuestEvent(863, 2, "TheEscape_on_accept_q")

RegisterUnitEvent(3439, 6, "Wizzlecrank_on_spawn")
RegisterUnitEvent(3439, 4, "Wizzlecrank_on_died")
RegisterUnitEvent(3439, 8, "Wizzlecrank_on_reach_wp")

RegisterUnitEvent(3282, 6, "VentureCo_Mercenary_on_spawn")
RegisterUnitEvent(3282, 4, "VentureCo_Mercenary_on_died")

RegisterUnitEvent(3451, 6, "WizzlecrankPilot_on_spawn")