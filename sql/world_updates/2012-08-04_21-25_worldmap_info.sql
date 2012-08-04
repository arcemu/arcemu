ALTER TABLE `worldmap_info` ADD COLUMN `required_quest_2` INT(10) UNSIGNED DEFAULT '0' NOT NULL AFTER `required_quest_1`, CHANGE `required_quest` `required_quest_1` INT(10) UNSIGNED DEFAULT '0' NOT NULL;

UPDATE `world_db_version` SET `LastUpdate` = '2012-08-14_21-25_worldmap_info';