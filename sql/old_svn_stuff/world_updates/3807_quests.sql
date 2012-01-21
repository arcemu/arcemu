ALTER TABLE `quests` ADD COLUMN `RewXPId` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `iscompletedbyspelleffect`;
UPDATE `arcemu_db_version` SET `LastUpdate` = '3807';

