DELETE FROM `loot_gameobjects` WHERE `entryid` NOT IN ( SELECT `entry` FROM `gameobject_names` WHERE `Type` = '3' );

DROP TABLE IF EXISTS `loot_gameobjects_temp`;

CREATE TABLE `loot_gameobjects_temp` (
  `index` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `normal10percentchance` float NOT NULL DEFAULT '0',
  `normal25percentchance` float NOT NULL DEFAULT '0',
  `heroic10percentchance` float NOT NULL DEFAULT '0',
  `heroic25percentchance` float NOT NULL DEFAULT '0',
  `mincount` int(30) unsigned NOT NULL DEFAULT '1',
  `maxcount` int(30) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`index`),
  UNIQUE KEY `index` (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

INSERT INTO `loot_gameobjects_temp` ( `entryid`, `itemid`, `normal10percentchance`, `normal25percentchance`, `heroic10percentchance`, `heroic25percentchance`, `mincount`, `maxcount`, `ffa_loot` )
SELECT * FROM `loot_gameobjects`;

UPDATE `loot_gameobjects_temp` SET `entryid` = ( SELECT `sound1` FROM `gameobject_names` WHERE `entry` = `entryid` );

DELETE FROM `loot_gameobjects_temp` WHERE `entryid` = '0';

DELETE FROM `loot_gameobjects`;

INSERT INTO `loot_gameobjects`
SELECT DISTINCT `entryid`, `itemid`, `normal10percentchance`, `normal25percentchance`, `heroic10percentchance`, `heroic25percentchance`, `mincount`, `maxcount`, `ffa_loot` FROM `loot_gameobjects_temp` ORDER BY `entryid`, `itemid`;

DROP TABLE `loot_gameobjects_temp`;


UPDATE `arcemu_db_version` SET `LastUpdate` = '3861';
