ALTER TABLE `playeritems` ADD COLUMN `text` TEXT NOT NULL AFTER `refund_costid`;
UPDATE `character_db_version` SET `LastUpdate`='2011-11-16_22-00_saved_mail';
