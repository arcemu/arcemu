ALTER TABLE `creature_proto` ADD COLUMN `spelldataid` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `summonguard`;
UPDATE `arcemu_db_version` SET `LastUpdate` = '4275';
