DROP TABLE IF EXISTS `totemdisplayids`;

CREATE TABLE `totemdisplayids` (
  `displayid` int(10) unsigned NOT NULL DEFAULT '0',
  `draeneiid` int(10) unsigned NOT NULL DEFAULT '0',
  `trollid` int(10) unsigned NOT NULL DEFAULT '0',
  `orcid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`displayid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

INSERT INTO `totemdisplayids`(`displayid`,`draeneiid`,`trollid`,`orcid`) values (4587,19075,30763,30759),(4588,19073,30761,30757),(4589,19074,30762,30758),(4590,19071,30760,30756),(4683,19075,30763,30759);

UPDATE `arcemu_db_version` SET `LastUpdate` = '4296';
