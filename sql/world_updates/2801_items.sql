/* dmg 3 - 5 is unused on 3.1. */
ALTER TABLE `items`
  DROP `dmg_min3`,
  DROP `dmg_max3`,
  DROP `dmg_type3`,
  DROP `dmg_min4`,
  DROP `dmg_max4`,
  DROP `dmg_type4`,
  DROP `dmg_min5`,
  DROP `dmg_max5`,
  DROP `dmg_type5`;

/* there is HolidayId column added in 3.1 */
ALTER TABLE `items` ADD `HolidayId` INT( 32 ) UNSIGNED NOT NULL DEFAULT '0' after `ItemLimitCategoryId`;

/* Cosmetic changes */
ALTER TABLE `items` CHANGE `RequiredSkillSubrank` `RequiredSpell` INT( 30 ) NOT NULL DEFAULT '0';
ALTER TABLE `items` CHANGE `unk203_1` `randomsuffix` INT( 30 ) NOT NULL DEFAULT '0';
ALTER TABLE `items` CHANGE `unk2` `ArmorDamageModifier` INT( 30 ) NOT NULL DEFAULT '0';
ALTER TABLE `items` DROP `name2`, DROP `name3`, DROP `name4`;

/* update for vellums */
UPDATE `items` SET `flags` = '268435520' WHERE name1 LIKE '%Scroll of Enchant%';