/* 
===============================
Title: The Harvest Festival Remove
Author: Nexis
Team: Sun++ (www.sunplusplus.info)
Duration: Sept 8th - Sept 13th
===============================
*/

/* REMOVE CREATURE SPAWNS */
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '400000' AND '400026';

/* REMOVE CREATURE WAYPOINTS */
DELETE FROM `creature_waypoints` WHERE `spawnid` BETWEEN '400000' AND '400025';

/* REMOVE GAMEOBJECT SPAWNS */
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '400000' AND '400130';