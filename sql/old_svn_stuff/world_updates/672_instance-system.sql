-- You should also clear your instance table @ character database.

CREATE TABLE `instance_bosses` (
  `mapid` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `spawnid` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  `trash` TEXT NOT NULL,
  `trash_respawn_override` INT(11) UNSIGNED NOT NULL DEFAULT '0',
  PRIMARY KEY  (`mapid`,`spawnid`)
) ENGINE=MyISAM /*!40100 DEFAULT CHARSET=latin1 COMMENT='Instance Boss Definitions'*/;

-- Partial Mini Example for Attumen in Karazhan...
-- INSERT INTO `instance_bosses` (`mapid`, `spawnid`, `trash`, `trash_respawn_override`) VALUES ('532', '4660662', '82148 82336 82354 82370 84387 84423', '600');