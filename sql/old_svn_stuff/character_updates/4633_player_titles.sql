ALTER TABLE `characters` ADD `available_pvp_titles2` bigint(10) unsigned NOT NULL DEFAULT '0' AFTER `available_pvp_titles1`;
UPDATE `arcemu_db_version` SET `LastUpdate` = '4633';