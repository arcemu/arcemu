ALTER TABLE `gm_tickets` CHANGE COLUMN `type` `map` INTEGER NOT NULL;
UPDATE `gm_tickets` SET `map`=0;
