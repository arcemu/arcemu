rename table `creatureloot` to `loot_creatures`;
rename table `objectloot` to `loot_gameobjects`;
rename table `skinningloot` to `loot_skinning`;
rename table `fishingloot` to `loot_fishing`;
rename table `itemloot` to `loot_items`;
rename table `prospectingloot` to `loot_prospecting`;
rename table `disenchantingloot` to `loot_disenchanting`;
rename table `pickpocketingloot` to `loot_pickpocketing`;

CREATE TABLE `loot_milling` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `index` (`index`)
) ENGINE=MyISAM;