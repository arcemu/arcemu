DROP TABLE IF EXISTS `arcemu_db_version`;

CREATE TABLE `arcemu_db_version` (
  `LastUpdate` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`LastUpdate`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

insert  into `arcemu_db_version`(`LastUpdate`) values (3800);
