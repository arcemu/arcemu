CREATE TABLE IF NOT EXISTS `gameobject_teleports` (
  `entry` int(10) unsigned NOT NULL auto_increment,
  `mapid` int(10) unsigned NOT NULL,
  `x_pos` float NOT NULL,
  `y_pos` float NOT NULL,
  `z_pos` float NOT NULL,
  `orientation` float NOT NULL,
  `required_level` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM AUTO_INCREMENT=800213 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC COMMENT='Optional table to create custom portals';
