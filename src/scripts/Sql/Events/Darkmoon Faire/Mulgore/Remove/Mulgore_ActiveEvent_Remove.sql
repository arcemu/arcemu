/* 
===============================
The Darkmoon Faire Project
Author: Nexis
Team: Sun++ (www.sunplusplus.info)
===============================
*/

/* REMOVE CREATURE SPAWNS */
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '500000' AND '500027';

/* REMOVE CREATURE WAYPOINTS */
DELETE FROM `creature_waypoints` WHERE `spawnid` BETWEEN '500000' AND '500027';

/* REMOVE NPC TEXT */
DELETE FROM `npc_text` WHERE `entry` BETWEEN '60000' AND '60050';

/* REMOVE NPC GOSSIP TEXTID */
DELETE FROM `npc_gossip_textid` WHERE `textid` BETWEEN '60000' AND '60050';

/* REMOVE GAMEOBJECT SPAWNS */
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '500000' AND '500129';

/* REMOVE CREATURE FORMATIONS */
DELETE FROM `creature_formations` WHERE `spawn_id` BETWEEN '500000' AND '500027';

/* DMF MUSIC DOODAD */
DELETE FROM `creature_names` WHERE (`entry`='500000');
DELETE FROM `creature_proto` WHERE (`entry`='500000');

/* END OF FILE */