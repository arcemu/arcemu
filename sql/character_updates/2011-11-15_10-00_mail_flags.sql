ALTER TABLE `mailbox` DROP COLUMN `copy_made`, CHANGE `read_flag` `checked_flag` INT(30) UNSIGNED DEFAULT '0' NOT NULL;
UPDATE `character_db_version` SET `LastUpdate` = '2011-11-15_10-00_mail_flags';