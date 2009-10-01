/*
=========================
	Title: Brewfest
	Author: this_is_junk
	Series: Active
=========================
*/

/* CREATURES */
DELETE FROM `creature_names` WHERE `entry` IN ('24710', '24468', '23558', '27215', '23486', '23525', '23481', '23684', '23628', '23510', '23521', '23710', '23683', '23627', '23482', '23522', '24364', '27584');
DELETE FROM `creature_names` WHERE `entry` IN ('24711', '27216', '24657', '23603', '24498', '24501', '24492', '24495', '23604', '24499', '23533', '24493', '23605', '23606', '24497', '24510', '27707' ,'24527', '28329', '23698');
DELETE FROM `creature_names` WHERE `entry` IN ('23511', '23685', '23696', '24484', '24462', '23700', '23702', '23706', '24372', '24373', '24463');

DELETE FROM `creature_proto` WHERE `entry` IN ('24710', '24468', '23558', '27215', '23486', '23525', '23481', '23684', '23628', '23510', '23521', '23710', '23683', '23627', '23482', '23522', '24364', '27584');
DELETE FROM `creature_proto` WHERE `entry` IN ('24711', '27216', '24657', '23603', '24498', '24501', '24492', '24495', '23604', '24499', '23533', '24493', '23605', '23606', '24497', '24510', '27707' ,'24527', '28329', '23698');
DELETE FROM `creature_proto` WHERE `entry` IN ('23511', '23685', '23696', '24484', '24462', '23700', '23702', '23706', '24372', '24373', '24463');

DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500000' AND '1500052';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500053' AND '1500103';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500104' AND '1500118';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500119' AND '1500133';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500134' AND '1500148';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500149' AND '1500164';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500165' AND '1500179';
DELETE FROM `creature_spawns` WHERE `id` BETWEEN '1500180' AND '1500194';

DELETE FROM `creature_waypoints` WHERE `spawnid` BETWEEN '1500000' AND '1500103';

DELETE FROM `gameobject_names` WHERE `entry` IN ('186763', '186807', '186808', '186809', '189989', '190394');

DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70000' AND '70184';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70185' AND '70351';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70352' AND '70367';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70368' AND '70383';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70384' AND '70400';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70401' AND '70415';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70416' AND '70430';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70431' AND '70443';
DELETE FROM `gameobject_spawns` WHERE `id` BETWEEN '70444' AND '70446';

DELETE FROM `npc_text` WHERE `entry` IN ('12152', '12154', '12155', '12156', '12158', '12165', '12183', '12195', '12873');

DELETE FROM `npc_gossip_textid` WHERE `creatureid` IN ('23486');

DELETE FROM `creature_quest_starter` WHERE `id` IN ('19148', '23872', '23486');

DELETE FROM `creature_quest_finisher` WHERE `id` IN ('24710', '23486');	

DELETE FROM `vendors` WHERE `entry` IN ('23525', '23511', '23481');
