ALTER TABLE `worldmap_info` CHANGE `required_quest` `required_quest_A` INT(10) UNSIGNED DEFAULT '0' NOT NULL;
ALTER TABLE `worldmap_info` ADD `required_quest_H` INT(10) UNSIGNED DEFAULT '0' NULL AFTER `required_quest_A`;
UPDATE `world_db_version` SET `LastUpdate`='2012-04-07_09-55_worldmap_info' WHERE (`LastUpdate`='2012-02-09_00-55_earth_shield');