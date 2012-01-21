alter table `gameobject_names` add column `Name2` varchar(100) DEFAULT '' NOT NULL after `Name`;
alter table `gameobject_names` add column `Name3` varchar(100) DEFAULT '' NOT NULL after `Name2`;
alter table `gameobject_names` add column `Name4` varchar(100) DEFAULT '' NOT NULL after `Name3`;
alter table `gameobject_names` add column `Category` varchar(100) DEFAULT '' NOT NULL after `Name4`;
alter table `gameobject_names` add column `CastBarText` varchar(100) DEFAULT '' NOT NULL after `Category`;
alter table `gameobject_names` add column `UnkStr` varchar(100) DEFAULT '' NOT NULL after `CastBarText`;

alter table `quests` add column `detailemotecount` int UNSIGNED DEFAULT '0' NOT NULL after `suggestedplayers`;
alter table `quests` add column `detailemote1` int UNSIGNED DEFAULT '0' NOT NULL after `detailemotecount`;
alter table `quests` add column `detailemote2` int UNSIGNED DEFAULT '0' NOT NULL after `detailemote1`;
alter table `quests` add column `detailemote3` int UNSIGNED DEFAULT '0' NOT NULL after `detailemote2`;
alter table `quests` add column `detailemote4` int UNSIGNED DEFAULT '0' NOT NULL after `detailemote3`;
alter table `quests` add column `detailemotedelay1` int UNSIGNED DEFAULT '0' NOT NULL after `detailemote4`;
alter table `quests` add column `detailemotedelay2` int UNSIGNED DEFAULT '0' NOT NULL after `detailemotedelay1`;
alter table `quests` add column `detailemotedelay3` int UNSIGNED DEFAULT '0' NOT NULL after `detailemotedelay2`;
alter table `quests` add column `detailemotedelay4` int UNSIGNED DEFAULT '0' NOT NULL after `detailemotedelay3`;
alter table `quests` add column `completionemotecnt` int UNSIGNED DEFAULT '0' NOT NULL after `detailemotedelay4`;
alter table `quests` add column `completionemote1` int UNSIGNED DEFAULT '0' NOT NULL after `completionemotecnt`;
alter table `quests` add column `completionemote2` int UNSIGNED DEFAULT '0' NOT NULL after `completionemote1`;
alter table `quests` add column `completionemote3` int UNSIGNED DEFAULT '0' NOT NULL after `completionemote2`;
alter table `quests` add column `completionemote4` int UNSIGNED DEFAULT '0' NOT NULL after `completionemote3`;
alter table `quests` add column `completionemotedelay1` int UNSIGNED DEFAULT '0' NOT NULL after `completionemote4`;
alter table `quests` add column `completionemotedelay2` int UNSIGNED DEFAULT '0' NOT NULL after `completionemotedelay1`;
alter table `quests` add column `completionemotedelay3` int UNSIGNED DEFAULT '0' NOT NULL after `completionemotedelay2`;
alter table `quests` add column `completionemotedelay4` int UNSIGNED DEFAULT '0' NOT NULL after `completionemotedelay3`;
alter table `quests` add column `completeemote` int UNSIGNED DEFAULT '0' NOT NULL after `completionemotedelay4`;
alter table `quests` add column `incompleteemote` int UNSIGNED DEFAULT '0' NOT NULL after `completeemote`;

CREATE TABLE `vendor_restrictions` (
 `entry` int(10) unsigned NOT NULL,
 `racemask` int(11) NOT NULL default '-1',
 `reqrepfaction` int(10) unsigned NOT NULL default '0',
 `reqrepfactionvalue` int(10) unsigned NOT NULL default '0',
 `canbuyattextid` int(10) unsigned NOT NULL default '0',
 `cannotbuyattextid` int(10) unsigned NOT NULL default '0',
 PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `itemnames` (
 `entry` int(10) unsigned NOT NULL default '0',
 `name` varchar(100) NOT NULL default '',
 `slotid` int(10) unsigned NOT NULL default '0',
 PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
