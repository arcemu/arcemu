/*
Title: Darkmoon Faire
Author: Nexis
Series: Setup
Location: Mulgore
*/

-- Remove Gameobjects
delete from `gameobject_spawns` where `id` between '100000' and '100070';

-- Remove NPCs
delete from `creature_spawns` where `id` between '160000' and '160007';

-- NPC Text
delete from `npc_text` where `entry` between '50000' and '50003';

-- Waypoints
delete from `creature_waypoints` where (`spawnid`='160000');
delete from `creature_waypoints` where (`spawnid`='160001');
delete from `creature_waypoints` where (`spawnid`='160002');
delete from `creature_waypoints` where (`spawnid`='160003');
delete from `creature_waypoints` where (`spawnid`='160004');
delete from `creature_waypoints` where (`spawnid`='160005');
delete from `creature_waypoints` where (`spawnid`='160006');
delete from `creature_waypoints` where (`spawnid`='160007');