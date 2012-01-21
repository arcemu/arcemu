ALTER TABLE `questlog` CHANGE `time_left` `expirytime` INT(20) UNSIGNED DEFAULT '0' NOT NULL;
UPDATE `arcemu_db_version` SET `LastUpdate` = '4479';

