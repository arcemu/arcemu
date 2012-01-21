ALTER TABLE `loot_creatures` DROP COLUMN `index`;
ALTER TABLE `loot_fishing` DROP COLUMN `index`;
ALTER TABLE `loot_gameobjects` DROP COLUMN `index`;
ALTER TABLE `loot_items` DROP COLUMN `index`;
ALTER TABLE `loot_pickpocketing` DROP COLUMN `index`;
ALTER TABLE `loot_skinning` DROP COLUMN `index`;

UPDATE `arcemu_db_version` SET `LastUpdate` = '3860';

