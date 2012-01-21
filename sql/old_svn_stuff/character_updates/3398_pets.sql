ALTER TABLE `playerpets` DROP COLUMN `summon`,    ADD COLUMN `type` INT UNSIGNED DEFAULT '1' NOT NULL AFTER `renamable`;
