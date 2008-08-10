-- These two statements imports all creature spawns which are flagged as boss and inside an raid or heroic instance into your instance_bosses table.
-- IMPORTS BOSS INFORMATION ONLY! (no trashmobb information and so on)


-- Use this statement to fill your instance_bosses table with missing bosses only.
--INSERT INTO `instance_bosses` (`mapid`, `spawnid`, `trash`, `trash_respawn_override`) SELECT `map`.`entry` AS `mapid`, `spawn`.`id` AS `spawnid`, '' AS `trash`, 0 AS `trash_respawn_override` FROM `creature_names` AS `name` INNER JOIN `creature_spawns` AS `spawn` ON `spawn`.`entry`=`name`.`entry` INNER JOIN `worldmap_info` AS `map` ON `map`.`entry`=`spawn`.`map` LEFT JOIN `instance_bosses` AS `bossinfo` ON `bossinfo`.`mapid`=`map`.`entry` AND `bossinfo`.`spawnid`=`spawn`.`id` WHERE `bossinfo`.`spawnid` IS NULL AND `name`.`rank`=3 AND `map`.`type` IN (1,4);


-- Use this statement to clear your instance_bosses table and fill it with new data.
--DELETE FROM `instance_bosses`;
--INSERT INTO `instance_bosses` (`mapid`, `spawnid`, `trash`, `trash_respawn_override`) SELECT `map`.`entry` AS `mapid`, `spawn`.`id` AS `spawnid`, '' AS `trash`, 0 AS `trash_respawn_override` FROM `creature_names` AS `name` INNER JOIN `creature_spawns` AS `spawn` ON `spawn`.`entry`=`name`.`entry` INNER JOIN `worldmap_info` AS `map` ON `map`.`entry`=`spawn`.`map` WHERE `name`.`rank`=3 AND `map`.`type` IN (1,4);
