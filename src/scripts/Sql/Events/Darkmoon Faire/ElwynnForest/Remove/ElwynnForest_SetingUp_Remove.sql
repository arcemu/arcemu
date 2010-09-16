/*
Title: Darkmoon Faire
Author: Nexis
Series: Setup
Location: Elwynn Forest
*/

-- Delete Gameobject Spawns
delete from `gameobject_spawns` where `id` between '150000' and '150064';

-- Delete NPCs
delete from `creature_spawns` where `id` between '150000' and '150006';

-- Delete Waypoints
delete from `creature_waypoints` where (`spawnid`='150005');
delete from `creature_waypoints` where (`spawnid`='150006');

-- Delete Custom Creatures
delete from `creature_names` where `entry` between '30000' and '30003';
delete from `creature_proto` where `entry` between '30000' and '30003';

-- Delete NPC Text
delete from `npc_text` where `entry` between '50000' and '50003';