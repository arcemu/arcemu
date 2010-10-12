/* 
===============================
The Darkmoon Faire Project
Author: Nexis
Team: Sun++ (www.sunplusplus.info)
===============================
*/

/* REMOVE CREATURE SPAWNS */
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '500000' AND '500007';

/* REMOVE CREATURE WAYPOINTS */
DELETE FROM `creature_waypoints` WHERE `spawnid` BETWEEN '500000' AND '500002';

/* REMOVE GAMEOBJECT SPAWNS */
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '80000' AND '80068';

/* REMOVE NOC TEXT */
DELETE FROM `npc_text` WHERE `entry` BETWEEN '50000' AND '50003';

/* END OF FILE */