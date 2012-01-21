DROP TABLE IF EXISTS `equipmentsets`;

CREATE TABLE `equipmentsets` (
  `ownerguid` int(10) unsigned NOT NULL DEFAULT '1',
  `setGUID` int(10) unsigned NOT NULL DEFAULT '1',
  `setid` int(10) unsigned NOT NULL DEFAULT '1',
  `setname` varchar(16) NOT NULL DEFAULT '',
  `iconname` varchar(100) NOT NULL DEFAULT '',
  `head` int(10) unsigned NOT NULL DEFAULT '0',
  `neck` int(10) unsigned NOT NULL DEFAULT '0',
  `shoulders` int(10) unsigned NOT NULL DEFAULT '0',
  `body` int(10) unsigned NOT NULL DEFAULT '0',
  `chest` int(10) unsigned NOT NULL DEFAULT '0',
  `waist` int(10) unsigned NOT NULL DEFAULT '0',
  `legs` int(10) unsigned NOT NULL DEFAULT '0',
  `feet` int(10) unsigned NOT NULL DEFAULT '0',
  `wrists` int(10) unsigned NOT NULL DEFAULT '0',
  `hands` int(10) unsigned NOT NULL DEFAULT '0',
  `finger1` int(10) unsigned NOT NULL DEFAULT '0',
  `finger2` int(10) unsigned NOT NULL DEFAULT '0',
  `trinket1` int(10) unsigned NOT NULL DEFAULT '0',
  `trinket2` int(10) unsigned NOT NULL DEFAULT '0',
  `back` int(10) unsigned NOT NULL DEFAULT '0',
  `mainhand` int(10) unsigned NOT NULL DEFAULT '0',
  `offhand` int(10) unsigned NOT NULL DEFAULT '0',
  `ranged` int(10) unsigned NOT NULL DEFAULT '0',
  `tabard` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ownerguid`,`setGUID`,`setid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
