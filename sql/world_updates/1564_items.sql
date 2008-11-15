ALTER TABLE `items` ADD COLUMN `ScaledStatsDistributionId` INT(32) UNSIGNED DEFAULT '0' NOT NULL AFTER `stat_value10`;
ALTER TABLE `items` ADD COLUMN `ScaledStatsDistributionFlags` INT(32) UNSIGNED DEFAULT '0' NOT NULL AFTER `ScaledStatsDistributionId`;
ALTER TABLE `items` ADD COLUMN `ItemLimitCategoryId` INT(32) UNSIGNED DEFAULT '0' NOT NULL AFTER `unk2`;