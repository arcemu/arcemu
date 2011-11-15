DROP TABLE IF EXISTS `arcemu_db_version`;
DROP TABLE IF EXISTS `world_db_version`;

CREATE TABLE `world_db_version`(
  `LastUpdate` VARCHAR(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`LastUpdate`)
);

INSERT INTO `world_db_version`(`LastUpdate`) VALUES ( '2011-11-12_20-00_initial');

ALTER TABLE `items` CHANGE `faction` `flags2` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `loot_creatures` DROP `ffa_loot`;
ALTER TABLE `loot_fishing` DROP `ffa_loot`;
ALTER TABLE `loot_gameobjects` DROP `ffa_loot`;
ALTER TABLE `loot_items` DROP `ffa_loot`;
ALTER TABLE `loot_pickpocketing` DROP `ffa_loot`;
ALTER TABLE `loot_skinning` DROP `ffa_loot`;
