INSERT INTO `loot_items` (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot ) SELECT entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot  FROM `loot_disenchanting`;

INSERT INTO `loot_items` (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot ) SELECT entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot  FROM `loot_milling`;

INSERT INTO `loot_items` (entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot ) SELECT entryid, itemid, percentchance, heroicpercentchance, mincount, maxcount, ffa_loot  FROM `loot_prospecting`;

DROP TABLE `loot_milling`;
DROP TABLE `loot_disenchanting`;
DROP TABLE `loot_prospecting`;


