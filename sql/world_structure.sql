/*
MySQL Data Transfer
Source Host: localhost
Source Database: testse
Target Host: localhost
Target Database: testse
Date: 5/12/2008 11:54:24 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for ai_agents
-- ----------------------------
CREATE TABLE `ai_agents` (
  `entry` int(11) unsigned NOT NULL default '0',
  `type` smallint(5) unsigned NOT NULL default '0',
  `event` int(11) unsigned NOT NULL default '0',
  `chance` int(11) unsigned NOT NULL default '0',
  `maxcount` int(11) unsigned NOT NULL default '0',
  `spell` int(11) unsigned NOT NULL default '0',
  `spelltype` int(11) unsigned NOT NULL default '0',
  `targettype` int(11) unsigned NOT NULL default '0',
  `cooldown` int(8) NOT NULL default '0',
  `floatMisc1` float NOT NULL default '0',
  `Misc2` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`,`type`,`spell`),
  UNIQUE KEY `a` (`entry`,`spell`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='AI System';

-- ----------------------------
-- Table structure for ai_threattospellid
-- ----------------------------
CREATE TABLE `ai_threattospellid` (
  `spell` int(11) unsigned NOT NULL default '0',
  `mod` int(11) NOT NULL default '0',
  PRIMARY KEY  (`spell`),
  UNIQUE KEY `a` (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='AI System';

-- ----------------------------
-- Table structure for areatriggers
-- ----------------------------
CREATE TABLE `areatriggers` (
  `entry` int(11) unsigned NOT NULL default '0',
  `type` tinyint(3) unsigned default '0',
  `map` int(11) unsigned default NULL,
  `screen` int(11) unsigned default NULL,
  `name` varchar(100) default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `required_honor_rank` int(11) unsigned NOT NULL default '0',
  `required_level` tinyint(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `a` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Trigger System';

-- ----------------------------
-- Table structure for auctionhouse
-- ----------------------------
CREATE TABLE `auctionhouse` (
  `id` int(32) NOT NULL auto_increment,
  `creature_entry` bigint(64) unsigned NOT NULL default '0',
  `group` int(32) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `a` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Auction House';

-- ----------------------------
-- Table structure for banned_phrases
-- ----------------------------
CREATE TABLE `banned_phrases` (
  `phrase` varchar(250) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`phrase`),
  UNIQUE KEY `a` (`phrase`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for clientaddons
-- ----------------------------
CREATE TABLE `clientaddons` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(50) character set utf8 collate utf8_unicode_ci default NULL,
  `crc` bigint(20) unsigned default NULL,
  `banned` int(10) unsigned NOT NULL default '0',
  `showinlist` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `a` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Client Addons';

-- ----------------------------
-- Table structure for command_overrides
-- ----------------------------
CREATE TABLE `command_overrides` (
  `command_name` varchar(100) NOT NULL,
  `access_level` varchar(10) NOT NULL,
  PRIMARY KEY  (`command_name`),
  UNIQUE KEY `a` (`command_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Commands System';

-- ----------------------------
-- Table structure for creature_formations
-- ----------------------------
CREATE TABLE `creature_formations` (
  `spawn_id` int(10) unsigned NOT NULL default '0',
  `target_spawn_id` int(10) unsigned NOT NULL default '0',
  `follow_angle` float NOT NULL default '0',
  `follow_dist` float NOT NULL default '0',
  PRIMARY KEY  (`spawn_id`),
  UNIQUE KEY `a` (`spawn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for creature_names
-- ----------------------------
CREATE TABLE `creature_names` (
  `entry` int(10) unsigned NOT NULL default '0',
  `name` varchar(100) NOT NULL,
  `subname` varchar(100) NOT NULL,
  `info_str` varchar(500) NOT NULL,
  `Flags1` int(10) unsigned default '0',
  `type` int(10) unsigned default '0',
  `family` int(10) unsigned default '0',
  `rank` int(10) unsigned default '0',
  `unk4` int(10) unsigned default '0',
  `spelldataid` int(10) unsigned default NULL,
  `male_displayid` int(30) NOT NULL,
  `female_displayid` int(30) NOT NULL,
  `male_displayid2` int(30) NOT NULL default '0',
  `female_displayid2` int(30) NOT NULL default '0',
  `unknown_float1` float NOT NULL default '1',
  `unknown_float2` float NOT NULL default '1',
  `civilian` int(4) unsigned default NULL,
  `leader` tinyint(3) unsigned default NULL,
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `a` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for creature_names_localized
-- ----------------------------
CREATE TABLE `creature_names_localized` (
  `id` int(30) unsigned NOT NULL,
  `language_code` varchar(5) character set latin1 NOT NULL,
  `name` varchar(100) character set latin1 NOT NULL,
  `subname` varchar(100) character set latin1 NOT NULL,
  PRIMARY KEY  (`id`,`language_code`),
  KEY `lol` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for creature_proto
-- ----------------------------
CREATE TABLE `creature_proto` (
  `entry` int(30) unsigned NOT NULL default '0',
  `minlevel` int(30) unsigned NOT NULL,
  `maxlevel` int(30) unsigned NOT NULL,
  `faction` int(30) unsigned NOT NULL default '0',
  `minhealth` int(30) unsigned NOT NULL,
  `maxhealth` int(30) unsigned NOT NULL,
  `mana` int(30) unsigned NOT NULL default '0',
  `scale` float NOT NULL default '0',
  `npcflags` int(30) unsigned NOT NULL default '0',
  `attacktime` int(30) unsigned NOT NULL default '0',
  `attacktype` int(4) NOT NULL default '0',
  `mindamage` float NOT NULL default '0',
  `maxdamage` float NOT NULL default '0',
  `rangedattacktime` int(30) unsigned NOT NULL default '0',
  `rangedmindamage` float unsigned NOT NULL default '0',
  `rangedmaxdamage` float unsigned NOT NULL default '0',
  `mountdisplayid` int(30) unsigned NOT NULL default '0',
  `equipmodel1` int(30) unsigned NOT NULL default '0',
  `equipinfo1` int(30) unsigned NOT NULL default '0',
  `equipslot1` int(30) unsigned NOT NULL default '0',
  `equipmodel2` int(30) unsigned NOT NULL default '0',
  `equipinfo2` int(30) unsigned NOT NULL default '0',
  `equipslot2` int(30) unsigned NOT NULL default '0',
  `equipmodel3` int(30) unsigned NOT NULL default '0',
  `equipinfo3` int(30) unsigned NOT NULL default '0',
  `equipslot3` int(30) unsigned NOT NULL default '0',
  `respawntime` int(30) unsigned NOT NULL default '0',
  `armor` int(30) unsigned NOT NULL default '0',
  `resistance1` int(30) unsigned NOT NULL default '0',
  `resistance2` int(30) unsigned NOT NULL default '0',
  `resistance3` int(30) unsigned NOT NULL default '0',
  `resistance4` int(30) unsigned NOT NULL default '0',
  `resistance5` int(30) unsigned NOT NULL default '0',
  `resistance6` int(30) unsigned NOT NULL default '0',
  `combat_reach` float NOT NULL default '0',
  `bounding_radius` float NOT NULL default '0',
  `auras` longtext NOT NULL,
  `boss` int(11) unsigned NOT NULL default '0',
  `money` int(30) NOT NULL default '0',
  `invisibility_type` int(30) unsigned NOT NULL,
  `death_state` int(30) unsigned NOT NULL,
  `walk_speed` float NOT NULL default '2.5',
  `run_speed` float NOT NULL default '8',
  `fly_speed` float NOT NULL default '14',
  `extra_a9_flags` int(30) NOT NULL default '0',
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `ID` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for creature_quest_finisher
-- ----------------------------
CREATE TABLE `creature_quest_finisher` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='Creature System';

-- ----------------------------
-- Table structure for creature_quest_starter
-- ----------------------------
CREATE TABLE `creature_quest_starter` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='Creature System';

-- ----------------------------
-- Table structure for creature_spawns
-- ----------------------------
CREATE TABLE `creature_spawns` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `entry` int(30) NOT NULL,
  `map` int(30) NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL,
  `movetype` int(30) NOT NULL default '0',
  `displayid` int(30) unsigned NOT NULL default '0',
  `faction` int(30) NOT NULL default '14',
  `flags` int(30) NOT NULL default '0',
  `bytes` int(30) NOT NULL default '0',
  `bytes2` int(30) NOT NULL default '0',
  `emote_state` int(30) NOT NULL default '0',
  `npc_respawn_link` int(30) NOT NULL default '0',
  `channel_spell` int(30) NOT NULL default '0',
  `channel_target_sqlid` int(30) NOT NULL default '0',
  `channel_target_sqlid_creature` int(30) NOT NULL default '0',
  `standstate` int(10) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `a` (`id`),
  KEY `b` (`map`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spawn System';

-- ----------------------------
-- Table structure for creature_staticspawns
-- ----------------------------
CREATE TABLE `creature_staticspawns` (
  `id` int(30) unsigned NOT NULL auto_increment,
  `entry` int(30) NOT NULL,
  `Map` int(30) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `o` float NOT NULL,
  `movetype` int(30) NOT NULL default '0',
  `displayid` int(30) unsigned NOT NULL default '0',
  `factionid` int(30) NOT NULL default '35',
  `flags` int(30) NOT NULL default '0',
  `bytes` int(30) NOT NULL default '0',
  `bytes2` int(30) NOT NULL default '0',
  `emote_state` int(30) NOT NULL default '0',
  `npc_respawn_link` int(30) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `a` (`Map`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

-- ----------------------------
-- Table structure for creature_waypoints
-- ----------------------------
CREATE TABLE `creature_waypoints` (
  `spawnid` int(10) unsigned NOT NULL default '0',
  `waypointid` int(10) unsigned NOT NULL default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `waittime` int(10) unsigned NOT NULL default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `forwardemoteoneshot` tinyint(3) unsigned NOT NULL default '0',
  `forwardemoteid` int(10) unsigned NOT NULL default '0',
  `backwardemoteoneshot` tinyint(3) unsigned NOT NULL default '0',
  `backwardemoteid` int(10) unsigned NOT NULL default '0',
  `forwardskinid` int(10) unsigned NOT NULL default '0',
  `backwardskinid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`spawnid`,`waypointid`),
  UNIQUE KEY `a` (`spawnid`,`waypointid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for creatureloot
-- ----------------------------
CREATE TABLE `creatureloot` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(11) NOT NULL default '0',
  `itemid` int(11) NOT NULL default '0',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float NOT NULL default '0',
  `mincount` int(30) NOT NULL default '1',
  `maxcount` int(30) NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`,`percentchance`,`heroicpercentchance`,`mincount`,`maxcount`),
  UNIQUE KEY `index` (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 PACK_KEYS=0 ROW_FORMAT=FIXED COMMENT='Loot System';

-- ----------------------------
-- Table structure for disenchantingloot
-- ----------------------------
CREATE TABLE `disenchantingloot` (
  `index` int(11) NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float default '0',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for fishing
-- ----------------------------
CREATE TABLE `fishing` (
  `Zone` int(10) unsigned NOT NULL default '0',
  `MinSkill` int(10) unsigned default NULL,
  `MaxSkill` int(10) unsigned default NULL,
  UNIQUE KEY `Zone` (`Zone`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Fishing System';

-- ----------------------------
-- Table structure for fishingloot
-- ----------------------------
CREATE TABLE `fishingloot` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '0',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`),
  UNIQUE KEY `index` (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Table structure for gameobject_names
-- ----------------------------
CREATE TABLE `gameobject_names` (
  `entry` int(10) unsigned NOT NULL default '0',
  `Type` int(10) unsigned NOT NULL default '0',
  `DisplayID` int(10) unsigned NOT NULL default '0',
  `Name` varchar(100) character set utf8 collate utf8_unicode_ci NOT NULL,
  `spellfocus` int(10) unsigned NOT NULL default '0',
  `sound1` int(10) unsigned NOT NULL default '0',
  `sound2` int(10) unsigned NOT NULL default '0',
  `sound3` int(10) unsigned NOT NULL default '0',
  `sound4` int(10) unsigned NOT NULL default '0',
  `sound5` int(10) unsigned NOT NULL default '0',
  `sound6` int(10) unsigned NOT NULL default '0',
  `sound7` int(10) unsigned NOT NULL default '0',
  `sound8` int(10) unsigned NOT NULL default '0',
  `sound9` int(10) unsigned NOT NULL default '0',
  `unknown1` int(10) unsigned NOT NULL default '0',
  `unknown2` int(10) unsigned NOT NULL default '0',
  `unknown3` int(10) unsigned NOT NULL default '0',
  `unknown4` int(10) unsigned NOT NULL default '0',
  `unknown5` int(10) unsigned NOT NULL default '0',
  `unknown6` int(10) unsigned NOT NULL default '0',
  `unknown7` int(10) unsigned NOT NULL default '0',
  `unknown8` int(10) unsigned NOT NULL default '0',
  `unknown9` int(10) unsigned NOT NULL default '0',
  `unknown10` int(10) unsigned NOT NULL default '0',
  `unknown11` int(10) unsigned NOT NULL default '0',
  `unknown12` int(10) unsigned NOT NULL default '0',
  `unknown13` int(10) unsigned NOT NULL default '0',
  `unknown14` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Gameobject System';

-- ----------------------------
-- Table structure for gameobject_names_localized
-- ----------------------------
CREATE TABLE `gameobject_names_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) character set latin1 NOT NULL,
  `name` varchar(100) character set latin1 NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for gameobject_quest_finisher
-- ----------------------------
CREATE TABLE `gameobject_quest_finisher` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Table structure for gameobject_quest_item_binding
-- ----------------------------
CREATE TABLE `gameobject_quest_item_binding` (
  `entry` int(11) NOT NULL default '0',
  `quest` int(11) NOT NULL default '0',
  `item` int(11) NOT NULL default '0',
  `item_count` int(11) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Table structure for gameobject_quest_pickup_binding
-- ----------------------------
CREATE TABLE `gameobject_quest_pickup_binding` (
  `entry` int(11) NOT NULL default '0',
  `quest` int(11) NOT NULL default '0',
  `required_count` int(11) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Table structure for gameobject_quest_starter
-- ----------------------------
CREATE TABLE `gameobject_quest_starter` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Table structure for gameobject_spawns
-- ----------------------------
CREATE TABLE `gameobject_spawns` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `Entry` int(10) unsigned NOT NULL default '0',
  `map` int(10) unsigned NOT NULL default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `Facing` float NOT NULL default '0',
  `orientation1` float NOT NULL default '0',
  `orientation2` float NOT NULL default '0',
  `orientation3` float NOT NULL default '0',
  `orientation4` float NOT NULL default '0',
  `State` int(10) unsigned NOT NULL default '0',
  `Flags` int(10) unsigned NOT NULL default '0',
  `Faction` int(10) unsigned NOT NULL default '0',
  `Scale` float NOT NULL default '0',
  `stateNpcLink` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `Map` (`map`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251 COLLATE=cp1251_general_cs PACK_KEYS=0 ROW_FORMAT=FIXED COMMENT='Spawn System';

-- ----------------------------
-- Table structure for gameobject_staticspawns
-- ----------------------------
CREATE TABLE `gameobject_staticspawns` (
  `id` int(30) NOT NULL auto_increment,
  `entry` int(30) NOT NULL,
  `map` int(11) NOT NULL default '0',
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `facing` float NOT NULL,
  `o` float NOT NULL,
  `o1` float NOT NULL,
  `o2` float NOT NULL,
  `o3` float NOT NULL,
  `state` int(11) NOT NULL default '0',
  `flags` int(30) NOT NULL default '0',
  `faction` int(11) NOT NULL default '0',
  `scale` float NOT NULL,
  `respawnNpcLink` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

-- ----------------------------
-- Table structure for graveyards
-- ----------------------------
CREATE TABLE `graveyards` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `zoneid` int(10) unsigned NOT NULL default '0',
  `adjacentzoneid` int(10) unsigned NOT NULL default '0',
  `mapid` int(10) unsigned NOT NULL default '0',
  `faction` int(10) unsigned NOT NULL default '0',
  `name` varchar(255) character set utf8 collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `index` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Graveyard System';

-- ----------------------------
-- Table structure for item_quest_association
-- ----------------------------
CREATE TABLE `item_quest_association` (
  `item` int(11) NOT NULL default '0',
  `quest` int(11) NOT NULL default '0',
  `item_count` int(11) NOT NULL default '0',
  UNIQUE KEY `item` (`item`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Table structure for item_randomprop_groups
-- ----------------------------
CREATE TABLE `item_randomprop_groups` (
  `entry_id` int(30) NOT NULL,
  `randomprops_entryid` int(30) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY  (`entry_id`,`randomprops_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Item System';

-- ----------------------------
-- Table structure for item_randomsuffix_groups
-- ----------------------------
CREATE TABLE `item_randomsuffix_groups` (
  `entry_id` int(30) NOT NULL,
  `randomsuffix_entryid` int(30) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY  (`entry_id`,`randomsuffix_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

-- ----------------------------
-- Table structure for itemloot
-- ----------------------------
CREATE TABLE `itemloot` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`),
  UNIQUE KEY `index` (`index`),
  KEY `i_gameobj_loot_entry` (`entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Table structure for itempages
-- ----------------------------
CREATE TABLE `itempages` (
  `entry` int(10) unsigned NOT NULL default '0',
  `text` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `next_page` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

-- ----------------------------
-- Table structure for itempages_localized
-- ----------------------------
CREATE TABLE `itempages_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) character set latin1 NOT NULL,
  `text` text character set latin1 NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `a` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for itempetfood
-- ----------------------------
CREATE TABLE `itempetfood` (
  `entry` int(11) NOT NULL,
  `food_type` int(11) NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for items
-- ----------------------------
CREATE TABLE `items` (
  `entry` int(255) unsigned NOT NULL default '0',
  `class` int(30) NOT NULL default '0',
  `subclass` int(30) NOT NULL default '0',
  `field4` int(10) NOT NULL default '-1',
  `name1` varchar(255) NOT NULL,
  `name2` varchar(255) NOT NULL default '',
  `name3` varchar(255) NOT NULL default '',
  `name4` varchar(255) NOT NULL default '',
  `displayid` int(70) unsigned NOT NULL default '0',
  `quality` int(30) NOT NULL default '0',
  `flags` int(30) NOT NULL default '0',
  `buyprice` int(30) NOT NULL default '0',
  `sellprice` int(30) NOT NULL default '0',
  `inventorytype` int(30) NOT NULL default '0',
  `allowableclass` int(30) NOT NULL default '0',
  `allowablerace` int(30) NOT NULL default '0',
  `itemlevel` int(30) NOT NULL default '0',
  `requiredlevel` int(30) NOT NULL default '0',
  `RequiredSkill` int(30) NOT NULL default '0',
  `RequiredSkillRank` int(30) NOT NULL default '0',
  `RequiredSkillSubRank` int(30) NOT NULL default '0',
  `RequiredPlayerRank1` int(30) NOT NULL default '0',
  `RequiredPlayerRank2` int(30) NOT NULL default '0',
  `RequiredFaction` int(30) NOT NULL default '0',
  `RequiredFactionStanding` int(30) NOT NULL default '0',
  `Unique` int(30) NOT NULL default '0',
  `maxcount` int(30) NOT NULL default '0',
  `ContainerSlots` int(30) NOT NULL default '0',
  `stat_type1` int(30) NOT NULL default '0',
  `stat_value1` int(30) NOT NULL default '0',
  `stat_type2` int(30) NOT NULL default '0',
  `stat_value2` int(30) NOT NULL default '0',
  `stat_type3` int(30) NOT NULL default '0',
  `stat_value3` int(30) NOT NULL default '0',
  `stat_type4` int(30) NOT NULL default '0',
  `stat_value4` int(30) NOT NULL default '0',
  `stat_type5` int(30) NOT NULL default '0',
  `stat_value5` int(30) NOT NULL default '0',
  `stat_type6` int(30) NOT NULL default '0',
  `stat_value6` int(30) NOT NULL default '0',
  `stat_type7` int(30) NOT NULL default '0',
  `stat_value7` int(30) NOT NULL default '0',
  `stat_type8` int(30) NOT NULL default '0',
  `stat_value8` int(30) NOT NULL default '0',
  `stat_type9` int(30) NOT NULL default '0',
  `stat_value9` int(30) NOT NULL default '0',
  `stat_type10` int(30) NOT NULL default '0',
  `stat_value10` int(30) NOT NULL default '0',
  `dmg_min1` float NOT NULL default '0',
  `dmg_max1` float NOT NULL default '0',
  `dmg_type1` int(30) NOT NULL default '0',
  `dmg_min2` float NOT NULL default '0',
  `dmg_max2` float NOT NULL default '0',
  `dmg_type2` int(30) NOT NULL default '0',
  `dmg_min3` float NOT NULL default '0',
  `dmg_max3` float NOT NULL default '0',
  `dmg_type3` int(30) NOT NULL default '0',
  `dmg_min4` float NOT NULL default '0',
  `dmg_max4` float NOT NULL default '0',
  `dmg_type4` int(30) NOT NULL default '0',
  `dmg_min5` float NOT NULL default '0',
  `dmg_max5` float NOT NULL default '0',
  `dmg_type5` int(30) NOT NULL default '0',
  `armor` int(30) NOT NULL default '0',
  `holy_res` int(30) NOT NULL default '0',
  `fire_res` int(30) NOT NULL default '0',
  `nature_res` int(30) NOT NULL default '0',
  `frost_res` int(30) NOT NULL default '0',
  `shadow_res` int(30) NOT NULL default '0',
  `arcane_res` int(30) NOT NULL default '0',
  `delay` int(30) NOT NULL default '0',
  `ammo_type` int(30) NOT NULL default '0',
  `range` float NOT NULL default '0',
  `spellid_1` int(30) NOT NULL default '0',
  `spelltrigger_1` int(30) NOT NULL default '0',
  `spellcharges_1` int(30) NOT NULL default '0',
  `spellcooldown_1` int(30) NOT NULL default '0',
  `spellcategory_1` int(30) NOT NULL default '0',
  `spellcategorycooldown_1` int(30) NOT NULL default '0',
  `spellid_2` int(30) NOT NULL default '0',
  `spelltrigger_2` int(30) NOT NULL default '0',
  `spellcharges_2` int(30) NOT NULL default '0',
  `spellcooldown_2` int(30) NOT NULL default '0',
  `spellcategory_2` int(30) NOT NULL default '0',
  `spellcategorycooldown_2` int(30) NOT NULL default '0',
  `spellid_3` int(30) NOT NULL default '0',
  `spelltrigger_3` int(30) NOT NULL default '0',
  `spellcharges_3` int(30) NOT NULL default '0',
  `spellcooldown_3` int(30) NOT NULL default '0',
  `spellcategory_3` int(30) NOT NULL default '0',
  `spellcategorycooldown_3` int(30) NOT NULL default '0',
  `spellid_4` int(30) NOT NULL default '0',
  `spelltrigger_4` int(30) NOT NULL default '0',
  `spellcharges_4` int(30) NOT NULL default '0',
  `spellcooldown_4` int(30) NOT NULL default '0',
  `spellcategory_4` int(30) NOT NULL default '0',
  `spellcategorycooldown_4` int(30) NOT NULL default '0',
  `spellid_5` int(30) NOT NULL default '0',
  `spelltrigger_5` int(30) NOT NULL default '0',
  `spellcharges_5` int(30) NOT NULL default '0',
  `spellcooldown_5` int(30) NOT NULL default '0',
  `spellcategory_5` int(30) NOT NULL default '0',
  `spellcategorycooldown_5` int(30) NOT NULL default '0',
  `bonding` int(30) NOT NULL default '0',
  `description` varchar(255) NOT NULL default '',
  `page_id` int(30) NOT NULL default '0',
  `page_language` int(30) NOT NULL default '0',
  `page_material` int(30) NOT NULL default '0',
  `quest_id` int(30) NOT NULL default '0',
  `lock_id` int(30) NOT NULL default '0',
  `lock_material` int(30) NOT NULL default '0',
  `sheathID` int(30) NOT NULL default '0',
  `randomprop` int(30) NOT NULL default '0',
  `unk203_1` int(11) NOT NULL default '0',
  `block` int(30) NOT NULL default '0',
  `itemset` int(30) NOT NULL default '0',
  `MaxDurability` int(30) NOT NULL default '0',
  `ZoneNameID` int(30) NOT NULL default '0',
  `mapid` int(30) default NULL,
  `bagfamily` int(30) NOT NULL default '0',
  `TotemCategory` int(30) default NULL,
  `socket_color_1` int(30) default NULL,
  `unk201_3` int(30) NOT NULL default '0',
  `socket_color_2` int(30) default NULL,
  `unk201_5` int(30) NOT NULL default '0',
  `socket_color_3` int(30) default NULL,
  `unk201_7` int(30) NOT NULL default '0',
  `socket_bonus` int(30) default NULL,
  `GemProperties` int(30) default NULL,
  `ReqDisenchantSkill` int(30) NOT NULL default '-1',
  `unk2` int(30) NOT NULL default '0',
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

-- ----------------------------
-- Table structure for items_extendedcost
-- ----------------------------
CREATE TABLE `items_extendedcost` (
  `ItemId` int(30) NOT NULL,
  `CostId` int(30) NOT NULL,
  PRIMARY KEY  (`ItemId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for items_localized
-- ----------------------------
CREATE TABLE `items_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) NOT NULL,
  `name` varchar(255) NOT NULL,
  `description` varchar(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for map_checkpoint
-- ----------------------------
CREATE TABLE `map_checkpoint` (
  `entry` int(30) NOT NULL,
  `prereq_checkpoint_id` int(30) NOT NULL,
  `creature_id` int(30) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Map System';

-- ----------------------------
-- Table structure for npc_gossip_textid
-- ----------------------------
CREATE TABLE `npc_gossip_textid` (
  `creatureid` int(10) unsigned NOT NULL default '0',
  `textid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`creatureid`),
  UNIQUE KEY `index` (`creatureid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Table structure for npc_monstersay
-- ----------------------------
CREATE TABLE `npc_monstersay` (
  `entry` int(10) unsigned NOT NULL default '0',
  `event` int(10) unsigned NOT NULL default '0',
  `chance` float NOT NULL default '0',
  `language` int(10) unsigned NOT NULL default '0',
  `type` int(10) unsigned NOT NULL default '0',
  `monstername` longtext character set utf8 collate utf8_unicode_ci,
  `text0` longtext character set utf8 collate utf8_unicode_ci,
  `text1` longtext character set utf8 collate utf8_unicode_ci,
  `text2` longtext character set utf8 collate utf8_unicode_ci,
  `text3` longtext character set utf8 collate utf8_unicode_ci,
  `text4` longtext character set utf8 collate utf8_unicode_ci
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Table structure for npc_text
-- ----------------------------
CREATE TABLE `npc_text` (
  `entry` int(10) unsigned NOT NULL default '0',
  `prob0` float NOT NULL default '0',
  `text0_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text0_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang0` int(10) unsigned NOT NULL default '0',
  `em0_0` int(10) unsigned NOT NULL default '0',
  `em0_1` int(10) unsigned NOT NULL default '0',
  `em0_2` int(10) unsigned NOT NULL default '0',
  `em0_3` int(10) unsigned NOT NULL default '0',
  `em0_4` int(10) unsigned NOT NULL default '0',
  `em0_5` int(10) unsigned NOT NULL default '0',
  `prob1` float NOT NULL default '0',
  `text1_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text1_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang1` int(10) unsigned NOT NULL default '0',
  `em1_0` int(10) unsigned NOT NULL default '0',
  `em1_1` int(10) unsigned NOT NULL default '0',
  `em1_2` int(10) unsigned NOT NULL default '0',
  `em1_3` int(10) unsigned NOT NULL default '0',
  `em1_4` int(10) unsigned NOT NULL default '0',
  `em1_5` int(10) unsigned NOT NULL default '0',
  `prob2` float NOT NULL default '0',
  `text2_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text2_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang2` int(10) unsigned NOT NULL default '0',
  `em2_0` int(10) unsigned NOT NULL default '0',
  `em2_1` int(10) unsigned NOT NULL default '0',
  `em2_2` int(10) unsigned NOT NULL default '0',
  `em2_3` int(10) unsigned NOT NULL default '0',
  `em2_4` int(10) unsigned NOT NULL default '0',
  `em2_5` int(10) unsigned NOT NULL default '0',
  `prob3` float NOT NULL default '0',
  `text3_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text3_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang3` int(10) unsigned NOT NULL default '0',
  `em3_0` int(10) unsigned NOT NULL default '0',
  `em3_1` int(10) unsigned NOT NULL default '0',
  `em3_2` int(10) unsigned NOT NULL default '0',
  `em3_3` int(10) unsigned NOT NULL default '0',
  `em3_4` int(10) unsigned NOT NULL default '0',
  `em3_5` int(10) unsigned NOT NULL default '0',
  `prob4` float NOT NULL default '0',
  `text4_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text4_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang4` int(10) unsigned NOT NULL default '0',
  `em4_0` int(10) unsigned NOT NULL default '0',
  `em4_1` int(10) unsigned NOT NULL default '0',
  `em4_2` int(10) unsigned NOT NULL default '0',
  `em4_3` int(10) unsigned NOT NULL default '0',
  `em4_4` int(10) unsigned NOT NULL default '0',
  `em4_5` int(10) unsigned NOT NULL default '0',
  `prob5` float NOT NULL default '0',
  `text5_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text5_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang5` int(10) unsigned NOT NULL default '0',
  `em5_0` int(10) unsigned NOT NULL default '0',
  `em5_1` int(10) unsigned NOT NULL default '0',
  `em5_2` int(10) unsigned NOT NULL default '0',
  `em5_3` int(10) unsigned NOT NULL default '0',
  `em5_4` int(10) unsigned NOT NULL default '0',
  `em5_5` int(10) unsigned NOT NULL default '0',
  `prob6` float NOT NULL default '0',
  `text6_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text6_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang6` int(10) unsigned NOT NULL default '0',
  `em6_0` int(10) unsigned NOT NULL default '0',
  `em6_1` int(10) unsigned NOT NULL default '0',
  `em6_2` int(10) unsigned NOT NULL default '0',
  `em6_3` int(10) unsigned NOT NULL default '0',
  `em6_4` int(10) unsigned NOT NULL default '0',
  `em6_5` int(10) unsigned NOT NULL default '0',
  `prob7` float NOT NULL default '0',
  `text7_0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text7_1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `lang7` int(10) unsigned NOT NULL default '0',
  `em7_0` int(10) unsigned NOT NULL default '0',
  `em7_1` int(10) unsigned NOT NULL default '0',
  `em7_2` int(10) unsigned NOT NULL default '0',
  `em7_3` int(10) unsigned NOT NULL default '0',
  `em7_4` int(10) unsigned NOT NULL default '0',
  `em7_5` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Table structure for npc_text_localized
-- ----------------------------
CREATE TABLE `npc_text_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) character set latin1 NOT NULL,
  `text0` varchar(200) character set latin1 NOT NULL,
  `text0_1` varchar(200) character set latin1 NOT NULL,
  `text1` varchar(200) character set latin1 NOT NULL,
  `text1_1` varchar(200) character set latin1 NOT NULL,
  `text2` varchar(200) character set latin1 NOT NULL,
  `text2_1` varchar(200) character set latin1 NOT NULL,
  `text3` varchar(200) character set latin1 NOT NULL,
  `text3_1` varchar(200) character set latin1 NOT NULL,
  `text4` varchar(200) character set latin1 NOT NULL,
  `text4_1` varchar(200) character set latin1 NOT NULL,
  `text5` varchar(200) character set latin1 NOT NULL,
  `text5_1` varchar(200) character set latin1 NOT NULL,
  `text6` varchar(200) character set latin1 NOT NULL,
  `text6_1` varchar(200) character set latin1 NOT NULL,
  `text7` varchar(200) character set latin1 NOT NULL,
  `text7_1` varchar(200) character set latin1 NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for objectloot
-- ----------------------------
CREATE TABLE `objectloot` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '0',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`),
  UNIQUE KEY `index` (`index`),
  KEY `entryid` (`entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Table structure for petdefaultspells
-- ----------------------------
CREATE TABLE `petdefaultspells` (
  `entry` int(11) NOT NULL default '0',
  `spell` int(11) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Pet System';

-- ----------------------------
-- Table structure for pickpocketingloot
-- ----------------------------
CREATE TABLE `pickpocketingloot` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '100',
  `heroicpercentchance` float default '0',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`),
  UNIQUE KEY `index` (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Table structure for playercreateinfo
-- ----------------------------
CREATE TABLE `playercreateinfo` (
  `Index` tinyint(3) unsigned NOT NULL auto_increment,
  `race` tinyint(3) unsigned NOT NULL default '0',
  `factiontemplate` int(10) unsigned NOT NULL default '0',
  `class` tinyint(3) unsigned NOT NULL default '0',
  `mapID` int(10) unsigned NOT NULL default '0',
  `zoneID` int(10) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  `displayID` smallint(5) unsigned NOT NULL default '0',
  `BaseStrength` tinyint(3) unsigned NOT NULL default '0',
  `BaseAgility` tinyint(3) unsigned NOT NULL default '0',
  `BaseStamina` tinyint(3) unsigned NOT NULL default '0',
  `BaseIntellect` tinyint(3) unsigned NOT NULL default '0',
  `BaseSpirit` tinyint(3) unsigned NOT NULL default '0',
  `BaseHealth` int(10) unsigned NOT NULL default '0',
  `BaseMana` int(10) unsigned NOT NULL default '0',
  `BaseRage` int(10) unsigned NOT NULL default '0',
  `BaseFocus` int(10) unsigned NOT NULL default '0',
  `BaseEnergy` int(10) unsigned NOT NULL default '0',
  `attackpower` int(10) unsigned NOT NULL default '0',
  `mindmg` float NOT NULL default '0',
  `maxdmg` float NOT NULL default '0',
  PRIMARY KEY  (`Index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Table structure for playercreateinfo_bars
-- ----------------------------
CREATE TABLE `playercreateinfo_bars` (
  `race` tinyint(3) unsigned default NULL,
  `class` tinyint(3) unsigned default NULL,
  `button` int(10) unsigned default NULL,
  `action` int(10) unsigned default NULL,
  `type` int(10) unsigned default NULL,
  `misc` int(10) unsigned default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Table structure for playercreateinfo_items
-- ----------------------------
CREATE TABLE `playercreateinfo_items` (
  `indexid` tinyint(3) unsigned NOT NULL default '0',
  `protoid` int(10) unsigned NOT NULL default '0',
  `slotid` tinyint(3) unsigned NOT NULL default '0',
  `amount` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Table structure for playercreateinfo_skills
-- ----------------------------
CREATE TABLE `playercreateinfo_skills` (
  `indexid` tinyint(3) unsigned NOT NULL default '0',
  `skillid` int(10) unsigned NOT NULL default '0',
  `level` int(10) unsigned NOT NULL default '0',
  `maxlevel` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Table structure for playercreateinfo_spells
-- ----------------------------
CREATE TABLE `playercreateinfo_spells` (
  `indexid` tinyint(3) unsigned NOT NULL default '0',
  `spellid` smallint(5) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Table structure for prospectingloot
-- ----------------------------
CREATE TABLE `prospectingloot` (
  `index` int(11) NOT NULL auto_increment,
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float default '0',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for quests
-- ----------------------------
CREATE TABLE `quests` (
  `entry` int(10) unsigned NOT NULL default '0',
  `ZoneId` int(10) unsigned NOT NULL default '0',
  `sort` int(10) unsigned NOT NULL default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `MinLevel` int(10) unsigned NOT NULL default '0',
  `questlevel` int(10) unsigned NOT NULL default '0',
  `Type` int(10) unsigned NOT NULL default '0',
  `RequiredRaces` int(10) unsigned NOT NULL default '0',
  `RequiredClass` int(10) unsigned NOT NULL default '0',
  `RequiredTradeskill` int(10) unsigned NOT NULL default '0',
  `RequiredTradeskillValue` int(5) unsigned NOT NULL default '0',
  `RequiredRepFaction` int(10) unsigned NOT NULL default '0',
  `RequiredRepValue` int(10) unsigned NOT NULL default '0',
  `LimitTime` int(10) unsigned NOT NULL default '0',
  `SpecialFlags` int(10) unsigned NOT NULL default '0',
  `PrevQuestId` int(10) unsigned NOT NULL default '0',
  `NextQuestId` int(10) unsigned NOT NULL default '0',
  `srcItem` int(10) unsigned NOT NULL default '0',
  `SrcItemCount` int(10) unsigned NOT NULL default '0',
  `Title` char(255) NOT NULL,
  `Details` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `Objectives` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `CompletionText` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `IncompleteText` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `EndText` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `ObjectiveText1` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `ObjectiveText2` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `ObjectiveText3` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `ObjectiveText4` text character set utf8 collate utf8_unicode_ci NOT NULL,
  `ReqItemId1` int(10) unsigned NOT NULL default '0',
  `ReqItemId2` int(10) unsigned NOT NULL default '0',
  `ReqItemId3` int(10) unsigned NOT NULL default '0',
  `ReqItemId4` int(10) unsigned NOT NULL default '0',
  `ReqItemCount1` int(10) unsigned NOT NULL default '0',
  `ReqItemCount2` int(10) unsigned NOT NULL default '0',
  `ReqItemCount3` int(10) unsigned NOT NULL default '0',
  `ReqItemCount4` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId1` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId2` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId3` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId4` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount1` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount2` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount3` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount4` int(10) unsigned NOT NULL default '0',
  `ReqCastSpellId1` int(11) NOT NULL default '0',
  `ReqCastSpellId2` int(11) NOT NULL default '0',
  `ReqCastSpellId3` int(11) NOT NULL default '0',
  `ReqCastSpellId4` int(11) NOT NULL default '0',
  `RewChoiceItemId1` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId2` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId3` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId4` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId5` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId6` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount1` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount2` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount3` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount4` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount5` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount6` int(10) unsigned NOT NULL default '0',
  `RewItemId1` int(10) unsigned NOT NULL default '0',
  `RewItemId2` int(10) unsigned NOT NULL default '0',
  `RewItemId3` int(10) unsigned NOT NULL default '0',
  `RewItemId4` int(10) unsigned NOT NULL default '0',
  `RewItemCount1` int(10) unsigned NOT NULL default '0',
  `RewItemCount2` int(10) unsigned NOT NULL default '0',
  `RewItemCount3` int(10) unsigned NOT NULL default '0',
  `RewItemCount4` int(10) unsigned NOT NULL default '0',
  `RewRepFaction1` int(10) unsigned NOT NULL default '0',
  `RewRepFaction2` int(10) unsigned NOT NULL default '0',
  `RewRepValue1` int(10) NOT NULL default '0',
  `RewRepValue2` int(10) NOT NULL default '0',
  `RewRepLimit` int(10) unsigned NOT NULL default '0',
  `RewMoney` int(10) unsigned NOT NULL default '0',
  `RewXP` int(10) unsigned NOT NULL default '0',
  `RewSpell` int(10) unsigned NOT NULL default '0',
  `CastSpell` int(10) unsigned NOT NULL default '0',
  `PointMapId` int(10) unsigned NOT NULL default '0',
  `PointX` float NOT NULL default '0',
  `PointY` float NOT NULL default '0',
  `PointOpt` int(10) unsigned NOT NULL default '0',
  `RequiredMoney` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger1` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger2` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger3` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger4` int(10) unsigned NOT NULL default '0',
  `RequiredQuest1` int(10) unsigned NOT NULL default '0',
  `RequiredQuest2` int(10) unsigned NOT NULL default '0',
  `RequiredQuest3` int(10) unsigned NOT NULL default '0',
  `RequiredQuest4` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId1` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId2` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId3` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId4` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount1` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount2` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount3` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount4` int(10) unsigned NOT NULL default '0',
  `IsRepeatable` int(11) NOT NULL default '0',
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quests System';

-- ----------------------------
-- Table structure for quests_localized
-- ----------------------------
CREATE TABLE `quests_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) character set latin1 NOT NULL,
  `Title` text character set latin1 NOT NULL,
  `Details` text character set latin1 NOT NULL,
  `Objectives` text character set latin1 NOT NULL,
  `CompletionText` text character set latin1 NOT NULL,
  `IncompleteText` text character set latin1 NOT NULL,
  `EndText` text character set latin1 NOT NULL,
  `ObjectiveText1` text character set latin1 NOT NULL,
  `ObjectiveText2` text character set latin1 NOT NULL,
  `ObjectiveText3` text character set latin1 NOT NULL,
  `ObjectiveText4` text character set latin1 NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for recall
-- ----------------------------
CREATE TABLE `recall` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `name` varchar(100) NOT NULL,
  `MapId` int(10) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Tele Command';

-- ----------------------------
-- Table structure for reputation_creature_onkill
-- ----------------------------
CREATE TABLE `reputation_creature_onkill` (
  `creature_id` int(30) NOT NULL,
  `faction_change_alliance` int(30) NOT NULL,
  `faction_change_horde` int(30) NOT NULL,
  `change_value` int(30) NOT NULL,
  `rep_limit` int(30) NOT NULL,
  KEY `index` (`creature_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for reputation_faction_onkill
-- ----------------------------
CREATE TABLE `reputation_faction_onkill` (
  `faction_id` int(30) NOT NULL,
  `change_factionid_alliance` int(30) NOT NULL,
  `change_deltamin_alliance` int(30) NOT NULL,
  `change_deltamax_alliance` int(30) NOT NULL,
  `change_factionid_horde` int(30) NOT NULL,
  `change_deltamin_horde` int(30) NOT NULL,
  `change_deltamax_horde` int(30) NOT NULL,
  KEY `factindex` (`faction_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for reputation_instance_onkill
-- ----------------------------
CREATE TABLE `reputation_instance_onkill` (
  `mapid` int(30) NOT NULL,
  `mob_rep_reward` int(30) NOT NULL,
  `mob_rep_limit` int(30) NOT NULL,
  `boss_rep_reward` int(30) NOT NULL,
  `boss_rep_limit` int(30) NOT NULL,
  `faction_change_alliance` int(30) NOT NULL,
  `faction_change_horde` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Table structure for skinningloot
-- ----------------------------
CREATE TABLE `skinningloot` (
  `index` int(11) unsigned NOT NULL auto_increment,
  `entryid` int(10) unsigned NOT NULL default '0',
  `itemid` int(10) unsigned NOT NULL default '0',
  `percentchance` float NOT NULL default '0',
  `heroicpercentchance` float default '0',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`index`),
  UNIQUE KEY `index` (`index`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Table structure for spell_coef_override
-- ----------------------------
CREATE TABLE `spell_coef_override` (
  `id` double default NULL,
  `name` varchar(300) default NULL,
  `Dspell_coef_override` float default NULL,
  `OTspell_coef_override` float default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for spell_disable
-- ----------------------------
CREATE TABLE `spell_disable` (
  `spellid` int(30) NOT NULL,
  `replacement_spellid` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

-- ----------------------------
-- Table structure for spell_disable_trainers
-- ----------------------------
CREATE TABLE `spell_disable_trainers` (
  `spellid` int(30) NOT NULL,
  `replacement_spellid` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Trainer System';

-- ----------------------------
-- Table structure for spell_effects_override
-- ----------------------------
CREATE TABLE `spell_effects_override` (
  `spellId` int(30) NOT NULL default '0',
  `EffectID` int(30) NOT NULL,
  `Disable` int(30) NOT NULL,
  `Effect` int(30) NOT NULL default '0',
  `BasePoints` int(30) NOT NULL default '0',
  `ApplyAuraName` int(30) NOT NULL default '0',
  `SpellGroupRelation` int(30) NOT NULL default '0',
  `MiscValue` int(30) NOT NULL default '0',
  `TriggerSpell` int(30) NOT NULL default '0',
  `ImplicitTargetA` int(30) NOT NULL default '0',
  `ImplicitTargetB` int(30) NOT NULL default '0',
  PRIMARY KEY  (`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for spell_proc
-- ----------------------------
CREATE TABLE `spell_proc` (
  `spellID` int(30) NOT NULL default '0',
  `ProcOnNameHash` int(30) unsigned NOT NULL default '0',
  PRIMARY KEY  (`spellID`,`ProcOnNameHash`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for spellfixes
-- ----------------------------
CREATE TABLE `spellfixes` (
  `spellId` int(30) NOT NULL default '0',
  `procFlags` int(30) NOT NULL default '0',
  `SpellGroupType` int(30) NOT NULL default '0',
  `procChance` int(30) NOT NULL default '0',
  `procCharges` int(30) NOT NULL default '0',
  PRIMARY KEY  (`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for spelloverride
-- ----------------------------
CREATE TABLE `spelloverride` (
  `overrideId` int(10) unsigned NOT NULL default '0',
  `spellId` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `overrideId` (`overrideId`,`spellId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Spell System';

-- ----------------------------
-- Table structure for teleport_coords
-- ----------------------------
CREATE TABLE `teleport_coords` (
  `id` int(10) unsigned NOT NULL default '0',
  `name` char(255) character set utf8 collate utf8_unicode_ci NOT NULL,
  `mapId` int(10) unsigned NOT NULL default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `totrigger` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Table structure for totemspells
-- ----------------------------
CREATE TABLE `totemspells` (
  `spell` int(10) unsigned NOT NULL default '0',
  `castspell1` int(10) unsigned NOT NULL default '0',
  `castspell2` int(10) unsigned NOT NULL default '0',
  `castspell3` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

-- ----------------------------
-- Table structure for trainer_defs
-- ----------------------------
CREATE TABLE `trainer_defs` (
  `entry` int(11) unsigned NOT NULL default '0',
  `required_skill` int(11) unsigned NOT NULL default '0',
  `required_skillvalue` int(11) unsigned default '0',
  `req_class` int(11) unsigned NOT NULL default '0',
  `trainer_type` int(11) unsigned NOT NULL default '0',
  `trainer_ui_window_message` text,
  `can_train_gossip_textid` int(11) NOT NULL,
  `cannot_train_gossip_textid` int(11) NOT NULL,
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `entry` (`entry`),
  KEY `entry_2` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

-- ----------------------------
-- Table structure for trainer_spells
-- ----------------------------
CREATE TABLE `trainer_spells` (
  `entry` int(11) unsigned NOT NULL default '0',
  `cast_spell` int(11) unsigned NOT NULL default '0',
  `learn_spell` int(11) unsigned NOT NULL,
  `spellcost` int(11) unsigned NOT NULL default '0',
  `reqspell` int(11) unsigned NOT NULL default '0',
  `reqskill` int(11) unsigned NOT NULL default '0',
  `reqskillvalue` int(11) unsigned NOT NULL default '0',
  `reqlevel` int(11) unsigned NOT NULL default '0',
  `deletespell` int(11) unsigned NOT NULL default '0',
  `is_prof` tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`,`cast_spell`),
  KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

-- ----------------------------
-- Table structure for trainerspelloverride
-- ----------------------------
CREATE TABLE `trainerspelloverride` (
  `spellid` int(10) unsigned NOT NULL default '0',
  `cost` int(10) unsigned NOT NULL default '0',
  `requiredspell` int(10) unsigned NOT NULL default '0',
  `deletespell` int(10) unsigned NOT NULL default '0',
  `requiredskill` int(10) unsigned NOT NULL default '0',
  `requiredskillvalue` int(10) unsigned NOT NULL default '0',
  `reqlevel` int(10) unsigned NOT NULL default '0',
  `requiredclass` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `spellid` (`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Trainer System';

-- ----------------------------
-- Table structure for transport_creatures
-- ----------------------------
CREATE TABLE `transport_creatures` (
  `transport_entry` int(10) unsigned NOT NULL,
  `creature_entry` int(10) unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for transport_data
-- ----------------------------
CREATE TABLE `transport_data` (
  `entry` int(10) unsigned NOT NULL default '0',
  `name` text character set utf8 collate utf8_unicode_ci,
  `period` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Table structure for vendors
-- ----------------------------
CREATE TABLE `vendors` (
  `entry` int(10) unsigned NOT NULL default '0',
  `item` int(10) unsigned NOT NULL default '0',
  `amount` int(11) NOT NULL default '0',
  `max_amount` int(11) NOT NULL default '0',
  `inctime` bigint(20) NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Table structure for weather
-- ----------------------------
CREATE TABLE `weather` (
  `zoneId` int(11) unsigned NOT NULL default '0',
  `high_chance` int(11) unsigned NOT NULL default '0',
  `high_type` int(11) unsigned NOT NULL default '0',
  `med_chance` int(11) unsigned NOT NULL default '0',
  `med_type` int(11) unsigned NOT NULL default '0',
  `low_chance` int(11) unsigned NOT NULL default '0',
  `low_type` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`zoneId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Weather System';

-- ----------------------------
-- Table structure for wordfilter_character_names
-- ----------------------------
CREATE TABLE `wordfilter_character_names` (
  `regex_match` varchar(500) NOT NULL,
  `regex_ignore_if_matched` varchar(500) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for wordfilter_chat
-- ----------------------------
CREATE TABLE `wordfilter_chat` (
  `regex_match` varchar(500) NOT NULL,
  `regex_ignore_if_matched` varchar(500) NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for worldmap_info
-- ----------------------------
CREATE TABLE `worldmap_info` (
  `entry` int(10) unsigned NOT NULL default '0',
  `screenid` int(10) unsigned default '0',
  `type` int(10) unsigned default '0',
  `maxplayers` int(10) unsigned default '0',
  `minlevel` int(10) unsigned default '1',
  `repopx` float default '0',
  `repopy` float default '0',
  `repopz` float default '0',
  `repopentry` int(10) unsigned default '0',
  `area_name` varchar(100) character set utf8 collate utf8_unicode_ci default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `cooldown` int(10) unsigned NOT NULL default '0',
  `lvl_mod_a` int(10) unsigned NOT NULL default '0',
  `required_quest` int(10) unsigned NOT NULL default '0',
  `required_item` int(10) unsigned NOT NULL default '0',
  `heroic_keyid_1` int(30) NOT NULL default '0',
  `heroic_keyid_2` int(30) NOT NULL default '0',
  `viewingDistance` float NOT NULL default '80',
  `required_checkpoint` int(30) NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Table structure for zoneguards
-- ----------------------------
CREATE TABLE `zoneguards` (
  `zone` int(10) unsigned NOT NULL,
  `horde_entry` int(10) unsigned default NULL,
  `alliance_entry` int(10) unsigned default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `item_quest_association` VALUES ('12220', '1016', '5');
INSERT INTO `spell_coef_override` VALUES ('8400', 'Fireball Rank 5', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('8401', 'Fireball Rank 6', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('8402', 'Fireball Rank 7', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('10148', 'Fireball Rank 8', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('10149', 'Fireball Rank 9', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('10150', 'Fireball Rank 10', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('10151', 'Fireball Rank 11', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('25306', 'Fireball Rank 12', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('27070', 'Fireball Rank 13', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('38692', 'Fireball Rank 14', '1', '0');
INSERT INTO `spell_coef_override` VALUES ('30455', 'Ice Lance', '0.1429', '0');
INSERT INTO `spell_coef_override` VALUES ('603', 'Curse of Doom Rank 1', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('30910', 'Curse of Doom Rank 2', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('980', 'Curse of Agony Rank 1', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('1014', 'Curse of Agony Rank 2', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('6217', 'Curse of Agony Rank 3', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('11711', 'Curse of Agony Rank 4', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('11712', 'Curse of Agony Rank 5', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('11713', 'Curse of Agony Rank 6', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('27218', 'Curse of Agony Rank 7', '0', '1.2');
INSERT INTO `spell_coef_override` VALUES ('33763', 'Libebloom', '0.4286', '0.518');
INSERT INTO `spell_coef_override` VALUES ('18265', 'Siphon Life Rank 1', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('18879', 'Siphon Life Rank 2', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('18880', 'Siphon Life Rank 3', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('18881', 'Siphon Life Rank 4', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('27264', 'Siphon Life Rank 5', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('30911', 'Siphon Life Rank 6', '0', '2');
INSERT INTO `spell_coef_override` VALUES ('139', 'Renew Rank 1', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('6074', 'Renew Rank 2', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('6075', 'Renew Rank 3', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('6076', 'Renew Rank 4', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('6077', 'Renew Rank 5', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('6078', 'Renew Rank 6', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('10927', 'Renew Rank 7', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('10928', 'Renew Rank 8', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('10929', 'Renew Rank 9', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('25315', 'Renew Rank 10', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('25221', 'Renew Rank 11', '0', '1');
INSERT INTO `spell_coef_override` VALUES ('21084', 'Seal of Righteousness Rank 1', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20287', 'Seal of Righteousness Rank 2', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20288', 'Seal of Righteousness Rank 3', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20289', 'Seal of Righteousness Rank 4', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20290', 'Seal of Righteousness Rank 5', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20291', 'Seal of Righteousness Rank 6', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20292', 'Seal of Righteousness Rank 7', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20293', 'Seal of Righteousness Rank 8', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('27155', 'Seal of Righteousness Rank 9', '0.1', '0');
INSERT INTO `spell_coef_override` VALUES ('20424', 'Seal of Command', '0.2', '0');
INSERT INTO `spell_coef_override` VALUES ('17', 'Power Word: Shield Rank 1', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('592', 'Power Word: Shield Rank 2', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('600', 'Power Word: Shield Rank 3', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('3747', 'Power Word: Shield Rank 4', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('6065', 'Power Word: Shield Rank 5', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('6066', 'Power Word: Shield Rank 6', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('10898', 'Power Word: Shield Rank 7', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('10899', 'Power Word: Shield Rank 8', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('10900', 'Power Word: Shield Rank 9', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('10901', 'Power Word: Shield Rank 10', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('25217', 'Power Word: Shield Rank 11', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('25218', 'Power Word: Shield Rank 12', '0.3', '-1');
INSERT INTO `spell_coef_override` VALUES ('11426', 'Ice Barrier Rank 1', '-1', '0.1');
INSERT INTO `spell_coef_override` VALUES ('13031', 'Ice Barrier Rank 2', '-1', '0.1');
INSERT INTO `spell_coef_override` VALUES ('13032', 'Ice Barrier Rank 3', '-1', '0.1');
INSERT INTO `spell_coef_override` VALUES ('13033', 'Ice Barrier Rank 4', '-1', '0.1');
INSERT INTO `spell_coef_override` VALUES ('27134', 'Ice Barrier Rank 5', '-1', '0.1');
INSERT INTO `spell_coef_override` VALUES ('33405', 'Ice Barrier Rank 6', '-1', '0.1');
INSERT INTO `spell_coef_override` VALUES ('6143', 'Frost Ward Rank 1', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('8461', 'Frost Ward Rank 2', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('8462', 'Frost Ward Rank 3', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('10177', 'Frost Ward Rank 4', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('28609', 'Frost Ward Rank 5', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('32796', 'Frost Ward Rank 6', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('543', 'Fire Ward Rank 1', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('8457', 'Fire Ward Rank 2', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('8458', 'Fire Ward Rank 3', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('10223', 'Fire Ward Rank 4', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('10225', 'Fire Ward Rank 5', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('27128', 'Fire Ward Rank 6', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('6229', 'Shadow Ward Rank 1', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('11739', 'Shadow Ward Rank 2', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('11740', 'Shadow Ward Rank 3', '-1', '0.3');
INSERT INTO `spell_coef_override` VALUES ('28610', 'Shadow Ward Rank 4', '-1', '0.3');


CREATE TABLE `professiondiscoveries` (
  `skillid` int(11) NOT NULL,
  `spellid` int(11) NOT NULL,
  `chance` float(11,0) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

