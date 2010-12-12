ALTER TABLE `trainer_spells` DROP COLUMN `is_prof`;

UPDATE `arcemu_db_version` SET `LastUpdate` = '3955';