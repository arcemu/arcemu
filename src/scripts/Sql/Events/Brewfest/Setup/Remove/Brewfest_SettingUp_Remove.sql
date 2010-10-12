/*
=========================
	Title: Brewfest
	Author: Nexis
	Series: Setup
=========================
*/

/* REMOVE CREATURE SPAWNS */
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500000' AND '1500017';

/* REMOVE CREATURE WAYPOINTS */
DELETE FROM `creature_waypoints` WHERE `spawnid` BETWEEN '1500000' AND '1500017';

/* REMOVE GAMEOBJECT SPAWNS */
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70000' AND '70152';

/* END OF FILE */