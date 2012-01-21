ALTER TABLE `characters` CHANGE `talentpoint_override` `talentpoints` LONGTEXT CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL; 
ALTER TABLE `characters` ADD COLUMN `resettalents` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `data`;
UPDATE `arcemu_db_version` SET `LastUpdate` = '4637';


