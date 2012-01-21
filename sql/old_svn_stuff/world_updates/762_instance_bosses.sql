ALTER TABLE `instance_bosses` CHANGE COLUMN `spawnid` `creatureid` INT(11) UNSIGNED NOT NULL DEFAULT '0';
UPDATE `instance_bosses` AS b SET `creatureid`=(SELECT `entry` FROM `creature_spawns` WHERE `id`=b.`creatureid`);
