-- ----------------------------
-- Table structure for npc_monstersay_localized
-- ----------------------------
DROP TABLE IF EXISTS `npc_monstersay_localized`;
CREATE TABLE `npc_monstersay_localized` (
  `entry` int(10) unsigned NOT NULL DEFAULT '',
  `language_code` varchar(5) unsigned NOT NULL DEFAULT '0',
  `monstername` longtext COLLATE latin1_swedish_ci,
  `text0` longtext COLLATE latin1_swedish_ci,
  `text1` longtext COLLATE latin1_swedish_ci,
  `text2` longtext COLLATE latin1_swedish_ci,
  `text3` longtext COLLATE latin1_swedish_ci,
  `text4` longtext COLLATE latin1_swedish_ci,
  PRIMARY KEY (`entry`,`language_code`)
) COMMENT='NPC System localized';