DELETE FROM `instances`;

CREATE TABLE `instanceids` (
  `playerguid` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `mapid` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `mode` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `instanceid` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  PRIMARY KEY  (`playerguid`,`mapid`,`mode`), 
  KEY `ix_instanceid` (`playerguid`)
) ENGINE=MyISAM /*!40100 DEFAULT CHARSET=latin1 COMMENT='Player / InstanceID - Reference Table'*/;

ALTER TABLE `instances` ADD COLUMN `persistent` TINYINT NOT NULL DEFAULT '0';
ALTER TABLE `groups` ADD COLUMN `instanceids` TEXT NOT NULL;
