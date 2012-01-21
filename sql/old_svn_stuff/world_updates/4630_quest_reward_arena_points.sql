ALTER TABLE `quests` ADD COLUMN `bonusarenapoints` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `bonushonor`;
UPDATE `arcemu_db_version` SET `LastUpdate` = '4630';