CREATE TABLE `character_achievement_progress` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `criteria` int(10) unsigned DEFAULT NULL,
  `counter` int(10) unsigned DEFAULT NULL,
  `date` int(10) unsigned DEFAULT NULL,
  KEY `a` (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `character_achievement` (
  `guid` int(10) unsigned DEFAULT NULL,
  `achievement` int(10) unsigned DEFAULT NULL,
  `date` int(10) unsigned DEFAULT NULL,
  KEY `a` (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;