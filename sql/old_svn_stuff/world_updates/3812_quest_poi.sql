DROP TABLE IF EXISTS `quest_poi`;

CREATE TABLE `quest_poi` (
  `questId` int(10) unsigned NOT NULL DEFAULT '0',
  `poiId` int(10) unsigned NOT NULL DEFAULT '0',
  `objIndex` int(10) NOT NULL DEFAULT '0',
  `mapId` int(10) unsigned NOT NULL DEFAULT '0',
  `mapAreaId` int(10) unsigned NOT NULL DEFAULT '0',
  `floorId` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  KEY `questId` (`questId`,`poiId`),
  KEY `id` (`poiId`,`questId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci; 



DROP TABLE IF EXISTS `quest_poi_points`;

CREATE TABLE `quest_poi_points` (
  `questId` int(10) unsigned NOT NULL DEFAULT '0',
  `poiId` int(10) unsigned NOT NULL DEFAULT '0',
  `x` int(10) NOT NULL DEFAULT '0',
  `y` int(10) NOT NULL DEFAULT '0',
  KEY `questId_id` (`questId`,`poiId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;


UPDATE `arcemu_db_version` SET `LastUpdate` = '3812';
