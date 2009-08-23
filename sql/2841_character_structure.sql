/*
SQLyog Community Edition- MySQL GUI v6.56
MySQL - 5.0.51a-24+lenny1 : Database - arccharacter
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

/*Table structure for table `account_data` */

DROP TABLE IF EXISTS `account_data`;

CREATE TABLE `account_data` (
  `acct` int(30) NOT NULL,
  `uiconfig0` blob,
  `uiconfig1` blob,
  `uiconfig2` blob,
  `uiconfig3` blob,
  `uiconfig4` blob,
  `uiconfig5` blob,
  `uiconfig6` blob,
  `uiconfig7` blob,
  `uiconfig8` blob,
  PRIMARY KEY  (`acct`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `account_data` */

LOCK TABLES `account_data` WRITE;

UNLOCK TABLES;

/*Table structure for table `account_forced_permissions` */

DROP TABLE IF EXISTS `account_forced_permissions`;

CREATE TABLE `account_forced_permissions` (
  `login` varchar(50) NOT NULL,
  `permissions` varchar(100) NOT NULL,
  PRIMARY KEY  (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `account_forced_permissions` */

LOCK TABLES `account_forced_permissions` WRITE;

UNLOCK TABLES;

/*Table structure for table `arenateams` */

DROP TABLE IF EXISTS `arenateams`;

CREATE TABLE `arenateams` (
  `id` int(30) NOT NULL,
  `type` int(30) NOT NULL,
  `leader` int(30) NOT NULL,
  `name` varchar(150) NOT NULL,
  `emblemstyle` int(40) NOT NULL,
  `emblemcolour` bigint(40) NOT NULL,
  `borderstyle` int(40) NOT NULL,
  `bordercolour` bigint(40) NOT NULL,
  `backgroundcolour` bigint(40) NOT NULL,
  `rating` int(30) NOT NULL,
  `data` varchar(150) NOT NULL,
  `ranking` int(30) NOT NULL,
  `player_data1` varchar(60) NOT NULL,
  `player_data2` varchar(60) NOT NULL,
  `player_data3` varchar(60) NOT NULL,
  `player_data4` varchar(60) NOT NULL,
  `player_data5` varchar(60) NOT NULL,
  `player_data6` varchar(60) NOT NULL,
  `player_data7` varchar(60) NOT NULL,
  `player_data8` varchar(60) NOT NULL,
  `player_data9` varchar(60) NOT NULL,
  `player_data10` varchar(60) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `arenateams` */

LOCK TABLES `arenateams` WRITE;

UNLOCK TABLES;

/*Table structure for table `auctions` */

DROP TABLE IF EXISTS `auctions`;

CREATE TABLE `auctions` (
  `auctionId` int(32) NOT NULL auto_increment,
  `auctionhouse` int(32) default NULL,
  `item` bigint(10) default NULL,
  `owner` bigint(10) default NULL,
  `buyout` int(32) default NULL,
  `time` int(32) default NULL,
  `bidder` bigint(10) default NULL,
  `bid` int(32) default NULL,
  `deposit` int(32) default NULL,
  PRIMARY KEY  (`auctionId`),
  KEY `b` (`auctionhouse`)
) ENGINE=MyISAM AUTO_INCREMENT=1027356 DEFAULT CHARSET=latin1;

/*Data for the table `auctions` */

LOCK TABLES `auctions` WRITE;

UNLOCK TABLES;

/*Table structure for table `banned_names` */

DROP TABLE IF EXISTS `banned_names`;

CREATE TABLE `banned_names` (
  `name` varchar(30) collate utf8_unicode_ci NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Data for the table `banned_names` */

LOCK TABLES `banned_names` WRITE;

UNLOCK TABLES;

/*Table structure for table `character_achievement` */

DROP TABLE IF EXISTS `character_achievement`;

CREATE TABLE `character_achievement` (
  `guid` int(10) unsigned NOT NULL default '0',
  `achievement` int(10) unsigned NOT NULL default '0',
  `date` int(10) unsigned default NULL,
  PRIMARY KEY  (`guid`,`achievement`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `character_achievement` */

LOCK TABLES `character_achievement` WRITE;

UNLOCK TABLES;

/*Table structure for table `character_achievement_progress` */

DROP TABLE IF EXISTS `character_achievement_progress`;

CREATE TABLE `character_achievement_progress` (
  `guid` int(10) unsigned NOT NULL default '0',
  `criteria` int(10) unsigned NOT NULL default '0',
  `counter` int(10) default NULL,
  `date` int(10) unsigned default NULL,
  PRIMARY KEY  (`guid`,`criteria`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `character_achievement_progress` */

LOCK TABLES `character_achievement_progress` WRITE;

UNLOCK TABLES;

/*Table structure for table `characters` */

DROP TABLE IF EXISTS `characters`;

CREATE TABLE `characters` (
  `guid` int(6) unsigned NOT NULL default '0',
  `acct` int(20) unsigned NOT NULL default '0',
  `name` varchar(21) NOT NULL default '',
  `race` smallint(3) NOT NULL,
  `class` smallint(3) NOT NULL,
  `gender` tinyint(1) NOT NULL,
  `custom_faction` int(30) NOT NULL default '0',
  `level` int(3) NOT NULL,
  `xp` int(30) NOT NULL,
  `active_cheats` int(10) unsigned NOT NULL default '0',
  `exploration_data` longtext NOT NULL,
  `skills` longtext NOT NULL,
  `watched_faction_index` bigint(40) NOT NULL,
  `selected_pvp_title` int(30) NOT NULL,
  `available_pvp_titles` bigint(10) unsigned NOT NULL default '0',
  `available_pvp_titles1` bigint(10) NOT NULL,
  `gold` int(30) NOT NULL,
  `ammo_id` int(30) NOT NULL,
  `available_prof_points` int(30) NOT NULL,
  `available_talent_points` int(30) NOT NULL,
  `current_hp` int(30) NOT NULL,
  `current_power` int(30) NOT NULL,
  `pvprank` int(30) NOT NULL,
  `bytes` int(30) NOT NULL,
  `bytes2` int(30) NOT NULL,
  `player_flags` int(30) NOT NULL,
  `player_bytes` int(30) NOT NULL,
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `mapId` int(8) unsigned NOT NULL default '0',
  `zoneId` int(8) unsigned NOT NULL default '0',
  `taximask` longtext NOT NULL,
  `banned` int(40) unsigned NOT NULL default '0',
  `banReason` varchar(255) NOT NULL,
  `timestamp` int(30) default NULL,
  `online` int(11) default NULL,
  `bindpositionX` float NOT NULL default '0',
  `bindpositionY` float NOT NULL default '0',
  `bindpositionZ` float NOT NULL default '0',
  `bindmapId` int(8) unsigned NOT NULL default '0',
  `bindzoneId` int(8) unsigned NOT NULL default '0',
  `isResting` int(3) NOT NULL default '0',
  `restState` int(5) NOT NULL default '0',
  `restTime` int(5) NOT NULL default '0',
  `playedtime` text NOT NULL,
  `deathstate` int(5) NOT NULL default '0',
  `TalentResetTimes` int(5) NOT NULL default '0',
  `first_login` tinyint(1) NOT NULL default '0',
  `forced_rename_pending` tinyint(1) NOT NULL default '0',
  `arenaPoints` int(10) NOT NULL,
  `totalstableslots` int(10) unsigned NOT NULL default '0',
  `instance_id` int(10) NOT NULL,
  `entrypointmap` int(10) NOT NULL,
  `entrypointx` float NOT NULL,
  `entrypointy` float NOT NULL,
  `entrypointz` float NOT NULL,
  `entrypointo` float NOT NULL,
  `entrypointinstance` int(10) NOT NULL,
  `taxi_path` int(10) NOT NULL,
  `taxi_lastnode` int(10) NOT NULL,
  `taxi_mountid` int(10) NOT NULL,
  `transporter` int(10) NOT NULL,
  `transporter_xdiff` float NOT NULL,
  `transporter_ydiff` float NOT NULL,
  `transporter_zdiff` float NOT NULL,
  `spells` longtext NOT NULL,
  `deleted_spells` longtext NOT NULL,
  `reputation` longtext NOT NULL,
  `actions` longtext NOT NULL,
  `auras` longtext NOT NULL,
  `finished_quests` longtext NOT NULL,
  `finisheddailies` longtext NOT NULL,
  `honorRolloverTime` int(30) NOT NULL default '0',
  `killsToday` int(10) NOT NULL default '0',
  `killsYesterday` int(10) NOT NULL default '0',
  `killsLifeTime` int(10) NOT NULL default '0',
  `honorToday` int(10) NOT NULL default '0',
  `honorYesterday` int(10) NOT NULL default '0',
  `honorPoints` int(10) NOT NULL default '0',
  `difficulty` int(4) NOT NULL default '0',
  `glyphs` longtext NOT NULL,
  `phase` int(10) unsigned NOT NULL default '1',
  PRIMARY KEY  (`guid`),
  KEY `acct` (`acct`),
  KEY `name` (`name`),
  KEY `b` (`banned`),
  KEY `c` (`online`),
  KEY `d` (`forced_rename_pending`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `characters` */

LOCK TABLES `characters` WRITE;

UNLOCK TABLES;

/*Table structure for table `characters_insert_queue` */

DROP TABLE IF EXISTS `characters_insert_queue`;

CREATE TABLE `characters_insert_queue` (
  `insert_temp_guid` int(10) unsigned NOT NULL default '0',
  `acct` int(10) unsigned NOT NULL default '0',
  `name` varchar(21) collate utf8_unicode_ci NOT NULL,
  `race` tinyint(3) unsigned NOT NULL default '0',
  `class` tinyint(3) unsigned NOT NULL default '0',
  `gender` tinyint(3) unsigned NOT NULL default '0',
  `custom_faction` int(30) NOT NULL default '0',
  `level` int(10) unsigned NOT NULL default '0',
  `xp` int(10) unsigned NOT NULL default '0',
  `exploration_data` longtext collate utf8_unicode_ci NOT NULL,
  `skills` longtext collate utf8_unicode_ci NOT NULL,
  `watched_faction_index` int(10) unsigned NOT NULL default '0',
  `selected_pvp_title` int(10) unsigned NOT NULL default '0',
  `available_pvp_titles` int(10) unsigned NOT NULL default '0',
  `gold` int(10) unsigned NOT NULL default '0',
  `ammo_id` int(10) unsigned NOT NULL default '0',
  `available_prof_points` int(10) unsigned NOT NULL default '0',
  `available_talent_points` int(10) unsigned NOT NULL default '0',
  `current_hp` int(10) unsigned NOT NULL default '0',
  `current_power` int(10) unsigned NOT NULL default '0',
  `pvprank` tinyint(3) unsigned NOT NULL default '0',
  `bytes` int(10) unsigned NOT NULL default '0',
  `bytes2` int(10) unsigned NOT NULL default '0',
  `player_flags` int(10) unsigned NOT NULL default '0',
  `player_bytes` int(10) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `mapId` int(10) unsigned NOT NULL default '0',
  `zoneId` int(10) unsigned NOT NULL default '0',
  `taximask` longtext collate utf8_unicode_ci NOT NULL,
  `banned` int(40) NOT NULL,
  `banReason` varchar(50) collate utf8_unicode_ci NOT NULL,
  `timestamp` int(11) NOT NULL,
  `online` int(11) default NULL,
  `bindpositionX` float NOT NULL default '0',
  `bindpositionY` float NOT NULL default '0',
  `bindpositionZ` float NOT NULL default '0',
  `bindmapId` int(10) unsigned NOT NULL default '0',
  `bindzoneId` int(10) unsigned NOT NULL default '0',
  `isResting` tinyint(3) unsigned NOT NULL default '0',
  `restState` tinyint(3) unsigned NOT NULL default '0',
  `restTime` int(10) unsigned NOT NULL default '0',
  `playedtime` longtext collate utf8_unicode_ci NOT NULL,
  `deathstate` int(10) unsigned NOT NULL default '0',
  `TalentResetTimes` int(10) unsigned NOT NULL default '0',
  `first_login` tinyint(3) unsigned NOT NULL default '0',
  `forced_rename_pending` tinyint(3) unsigned NOT NULL default '0',
  `publicNote` varchar(32) collate utf8_unicode_ci NOT NULL,
  `officerNote` varchar(32) collate utf8_unicode_ci NOT NULL,
  `guildid` int(10) unsigned NOT NULL default '0',
  `guildRank` int(10) unsigned NOT NULL default '0',
  `arenaPoints` int(10) NOT NULL,
  `totalstableslots` int(10) unsigned NOT NULL default '0',
  `instance_id` int(10) unsigned NOT NULL default '0',
  `entrypointmap` int(10) unsigned NOT NULL default '0',
  `entrypointx` float NOT NULL default '0',
  `entrypointy` float NOT NULL default '0',
  `entrypointz` float NOT NULL default '0',
  `entrypointo` float NOT NULL default '0',
  `entrypointinstance` int(10) unsigned NOT NULL default '0',
  `taxi_path` int(10) unsigned NOT NULL default '0',
  `taxi_lastnode` int(10) unsigned NOT NULL default '0',
  `taxi_mountid` int(10) unsigned NOT NULL default '0',
  `transporter` int(10) unsigned NOT NULL default '0',
  `transporter_xdiff` float NOT NULL default '0',
  `transporter_ydiff` float NOT NULL default '0',
  `transporter_zdiff` float NOT NULL default '0',
  `spells` longtext collate utf8_unicode_ci NOT NULL,
  `deleted_spells` longtext collate utf8_unicode_ci NOT NULL,
  `reputation` longtext collate utf8_unicode_ci NOT NULL,
  `actions` longtext collate utf8_unicode_ci NOT NULL,
  `auras` longtext collate utf8_unicode_ci NOT NULL,
  `finished_quests` longtext collate utf8_unicode_ci NOT NULL,
  `honorPointsToAdd` int(10) NOT NULL,
  `killsToday` int(10) unsigned NOT NULL default '0',
  `killsYesterday` int(10) unsigned NOT NULL default '0',
  `killsLifeTime` int(10) unsigned NOT NULL default '0',
  `honorToday` int(10) unsigned NOT NULL default '0',
  `honorYesterday` int(10) unsigned NOT NULL default '0',
  `honorPoints` int(10) unsigned NOT NULL default '0',
  `difficulty` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `guid` (`insert_temp_guid`),
  KEY `acct` (`acct`),
  KEY `guildid` (`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Data for the table `characters_insert_queue` */

LOCK TABLES `characters_insert_queue` WRITE;

UNLOCK TABLES;

/*Table structure for table `charters` */

DROP TABLE IF EXISTS `charters`;

CREATE TABLE `charters` (
  `charterId` int(30) NOT NULL,
  `charterType` int(30) NOT NULL default '0',
  `leaderGuid` int(20) unsigned NOT NULL default '0',
  `guildName` varchar(32) NOT NULL default '',
  `itemGuid` bigint(40) unsigned NOT NULL default '0',
  `signer1` int(10) unsigned NOT NULL default '0',
  `signer2` int(10) unsigned NOT NULL default '0',
  `signer3` int(10) unsigned NOT NULL default '0',
  `signer4` int(10) unsigned NOT NULL default '0',
  `signer5` int(10) unsigned NOT NULL default '0',
  `signer6` int(10) unsigned NOT NULL default '0',
  `signer7` int(10) unsigned NOT NULL default '0',
  `signer8` int(10) unsigned NOT NULL default '0',
  `signer9` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`charterId`),
  UNIQUE KEY `leaderGuid` (`leaderGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='InnoDB free: 11264 kB; InnoDB free: 18432 kB';

/*Data for the table `charters` */

LOCK TABLES `charters` WRITE;

UNLOCK TABLES;

/*Table structure for table `clientaddons` */

DROP TABLE IF EXISTS `clientaddons`;

CREATE TABLE `clientaddons` (
  `id` int(10) NOT NULL auto_increment,
  `name` varchar(50) default NULL,
  `crc` bigint(50) default NULL,
  `banned` int(1) NOT NULL default '0',
  `showinlist` int(1) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `index` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=153 DEFAULT CHARSET=latin1;

/*Data for the table `clientaddons` */

LOCK TABLES `clientaddons` WRITE;

UNLOCK TABLES;

/*Table structure for table `command_overrides` */

DROP TABLE IF EXISTS `command_overrides`;

CREATE TABLE `command_overrides` (
  `command_name` varchar(100) NOT NULL,
  `access_level` varchar(10) NOT NULL,
  PRIMARY KEY  (`command_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `command_overrides` */

LOCK TABLES `command_overrides` WRITE;

UNLOCK TABLES;

/*Table structure for table `corpses` */

DROP TABLE IF EXISTS `corpses`;

CREATE TABLE `corpses` (
  `guid` bigint(20) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `zoneId` int(11) NOT NULL default '38',
  `mapId` int(11) NOT NULL default '0',
  `instanceId` int(11) NOT NULL default '0',
  `data` longtext NOT NULL,
  PRIMARY KEY  (`guid`),
  KEY `b` (`instanceId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `corpses` */

LOCK TABLES `corpses` WRITE;

UNLOCK TABLES;

/*Table structure for table `gm_tickets` */

DROP TABLE IF EXISTS `gm_tickets`;

CREATE TABLE `gm_tickets` (
  `guid` int(11) NOT NULL,
  `playerGuid` int(11) NOT NULL,
  `name` varchar(200) NOT NULL default '',
  `level` int(6) NOT NULL default '0',
  `map` int(11) NOT NULL default '0',
  `posX` float NOT NULL default '0',
  `posY` float NOT NULL default '0',
  `posZ` float NOT NULL default '0',
  `message` text NOT NULL,
  `timestamp` text,
  `deleted` bit(1) NOT NULL default '\0',
  `assignedto` int(11) NOT NULL default '0',
  `comment` text NOT NULL,
  UNIQUE KEY `guid` (`guid`),
  UNIQUE KEY `guid_2` (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `gm_tickets` */

LOCK TABLES `gm_tickets` WRITE;

UNLOCK TABLES;

/*Table structure for table `groups` */

DROP TABLE IF EXISTS `groups`;

CREATE TABLE `groups` (
  `group_id` int(30) NOT NULL,
  `group_type` tinyint(2) NOT NULL,
  `subgroup_count` tinyint(2) NOT NULL,
  `loot_method` tinyint(2) NOT NULL,
  `loot_threshold` tinyint(2) NOT NULL,
  `difficulty` int(30) NOT NULL default '0',
  `assistant_leader` int(30) NOT NULL default '0',
  `main_tank` int(30) NOT NULL default '0',
  `main_assist` int(30) NOT NULL default '0',
  `group1member1` int(50) NOT NULL,
  `group1member2` int(50) NOT NULL,
  `group1member3` int(50) NOT NULL,
  `group1member4` int(50) NOT NULL,
  `group1member5` int(50) NOT NULL,
  `group2member1` int(50) NOT NULL,
  `group2member2` int(50) NOT NULL,
  `group2member3` int(50) NOT NULL,
  `group2member4` int(50) NOT NULL,
  `group2member5` int(50) NOT NULL,
  `group3member1` int(50) NOT NULL,
  `group3member2` int(50) NOT NULL,
  `group3member3` int(50) NOT NULL,
  `group3member4` int(50) NOT NULL,
  `group3member5` int(50) NOT NULL,
  `group4member1` int(50) NOT NULL,
  `group4member2` int(50) NOT NULL,
  `group4member3` int(50) NOT NULL,
  `group4member4` int(50) NOT NULL,
  `group4member5` int(50) NOT NULL,
  `group5member1` int(50) NOT NULL,
  `group5member2` int(50) NOT NULL,
  `group5member3` int(50) NOT NULL,
  `group5member4` int(50) NOT NULL,
  `group5member5` int(50) NOT NULL,
  `group6member1` int(50) NOT NULL,
  `group6member2` int(50) NOT NULL,
  `group6member3` int(50) NOT NULL,
  `group6member4` int(50) NOT NULL,
  `group6member5` int(50) NOT NULL,
  `group7member1` int(50) NOT NULL,
  `group7member2` int(50) NOT NULL,
  `group7member3` int(50) NOT NULL,
  `group7member4` int(50) NOT NULL,
  `group7member5` int(50) NOT NULL,
  `group8member1` int(50) NOT NULL,
  `group8member2` int(50) NOT NULL,
  `group8member3` int(50) NOT NULL,
  `group8member4` int(50) NOT NULL,
  `group8member5` int(50) NOT NULL,
  `timestamp` int(30) NOT NULL,
  `instanceids` text NOT NULL,
  PRIMARY KEY  (`group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `groups` */

LOCK TABLES `groups` WRITE;

UNLOCK TABLES;

/*Table structure for table `guild_bankitems` */

DROP TABLE IF EXISTS `guild_bankitems`;

CREATE TABLE `guild_bankitems` (
  `guildId` int(30) NOT NULL,
  `tabId` int(30) NOT NULL,
  `slotId` int(30) NOT NULL,
  `itemGuid` int(30) NOT NULL,
  PRIMARY KEY  (`guildId`,`tabId`,`slotId`),
  KEY `a` (`guildId`),
  KEY `b` (`tabId`),
  KEY `c` (`slotId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `guild_bankitems` */

LOCK TABLES `guild_bankitems` WRITE;

UNLOCK TABLES;

/*Table structure for table `guild_banklogs` */

DROP TABLE IF EXISTS `guild_banklogs`;

CREATE TABLE `guild_banklogs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `tabid` int(30) NOT NULL COMMENT 'tab 6 is money logs',
  `action` int(5) NOT NULL,
  `player_guid` int(30) NOT NULL,
  `item_entry` int(30) NOT NULL,
  `stack_count` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  PRIMARY KEY  (`log_id`,`guildid`),
  KEY `a` (`guildid`),
  KEY `b` (`tabid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `guild_banklogs` */

LOCK TABLES `guild_banklogs` WRITE;

UNLOCK TABLES;

/*Table structure for table `guild_banktabs` */

DROP TABLE IF EXISTS `guild_banktabs`;

CREATE TABLE `guild_banktabs` (
  `guildId` int(30) NOT NULL,
  `tabId` int(30) NOT NULL,
  `tabName` varchar(200) NOT NULL,
  `tabIcon` varchar(200) NOT NULL,
  `tabInfo` varchar(200) NOT NULL,
  PRIMARY KEY  (`guildId`,`tabId`),
  KEY `a` (`guildId`),
  KEY `b` (`tabId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `guild_banktabs` */

LOCK TABLES `guild_banktabs` WRITE;

UNLOCK TABLES;

/*Table structure for table `guild_data` */

DROP TABLE IF EXISTS `guild_data`;

CREATE TABLE `guild_data` (
  `guildid` int(30) NOT NULL,
  `playerid` int(30) NOT NULL,
  `guildRank` int(30) NOT NULL,
  `publicNote` varchar(300) NOT NULL,
  `officerNote` varchar(300) NOT NULL,
  `lastWithdrawReset` int(30) NOT NULL default '0',
  `withdrawlsSinceLastReset` int(30) NOT NULL default '0',
  `lastItemWithdrawReset0` int(30) NOT NULL default '0',
  `itemWithdrawlsSinceLastReset0` int(30) NOT NULL default '0',
  `lastItemWithdrawReset1` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset1` int(30) NOT NULL,
  `lastItemWithdrawReset2` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset2` int(30) NOT NULL,
  `lastItemWithdrawReset3` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset3` int(30) NOT NULL,
  `lastItemWithdrawReset4` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset4` int(30) NOT NULL,
  `lastItemWithdrawReset5` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset5` int(30) NOT NULL,
  KEY `a` (`guildid`),
  KEY `b` (`playerid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `guild_data` */

LOCK TABLES `guild_data` WRITE;

UNLOCK TABLES;

/*Table structure for table `guild_logs` */

DROP TABLE IF EXISTS `guild_logs`;

CREATE TABLE `guild_logs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  `event_type` int(30) NOT NULL,
  `misc1` int(30) NOT NULL,
  `misc2` int(30) NOT NULL,
  `misc3` int(30) NOT NULL,
  PRIMARY KEY  (`log_id`,`guildid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `guild_logs` */

LOCK TABLES `guild_logs` WRITE;

UNLOCK TABLES;

/*Table structure for table `guild_ranks` */

DROP TABLE IF EXISTS `guild_ranks`;

CREATE TABLE `guild_ranks` (
  `guildId` int(6) unsigned NOT NULL default '0',
  `rankId` int(1) NOT NULL default '0',
  `rankName` varchar(255) NOT NULL default '',
  `rankRights` int(3) unsigned NOT NULL default '0',
  `goldLimitPerDay` int(30) NOT NULL default '0',
  `bankTabFlags0` int(30) NOT NULL default '0',
  `itemStacksPerDay0` int(30) NOT NULL default '0',
  `bankTabFlags1` int(30) NOT NULL default '0',
  `itemStacksPerDay1` int(30) NOT NULL default '0',
  `bankTabFlags2` int(30) NOT NULL default '0',
  `itemStacksPerDay2` int(30) NOT NULL default '0',
  `bankTabFlags3` int(30) NOT NULL default '0',
  `itemStacksPerDay3` int(30) NOT NULL default '0',
  `bankTabFlags4` int(30) NOT NULL default '0',
  `itemStacksPerDay4` int(30) NOT NULL default '0',
  `bankTabFlags5` int(30) NOT NULL default '0',
  `itemStacksPerDay5` int(30) NOT NULL default '0',
  PRIMARY KEY  (`guildId`,`rankId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `guild_ranks` */

LOCK TABLES `guild_ranks` WRITE;

UNLOCK TABLES;

/*Table structure for table `guilds` */

DROP TABLE IF EXISTS `guilds`;

CREATE TABLE `guilds` (
  `guildId` bigint(20) NOT NULL auto_increment,
  `guildName` varchar(32) NOT NULL default '',
  `leaderGuid` bigint(20) NOT NULL default '0',
  `emblemStyle` int(10) NOT NULL default '0',
  `emblemColor` int(10) NOT NULL default '0',
  `borderStyle` int(10) NOT NULL default '0',
  `borderColor` int(10) NOT NULL default '0',
  `backgroundColor` int(10) NOT NULL default '0',
  `guildInfo` varchar(300) NOT NULL default '',
  `motd` varchar(300) NOT NULL default '',
  `createdate` int(30) NOT NULL,
  `bankBalance` bigint(30) unsigned NOT NULL,
  PRIMARY KEY  (`guildId`),
  UNIQUE KEY `guildId` (`guildId`)
) ENGINE=MyISAM AUTO_INCREMENT=8189 DEFAULT CHARSET=latin1;

/*Data for the table `guilds` */

LOCK TABLES `guilds` WRITE;

UNLOCK TABLES;

/*Table structure for table `instanceids` */

DROP TABLE IF EXISTS `instanceids`;

CREATE TABLE `instanceids` (
  `playerguid` int(11) unsigned NOT NULL default '0',
  `mapid` int(11) unsigned NOT NULL default '0',
  `mode` int(11) unsigned NOT NULL default '0',
  `instanceid` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`playerguid`,`mapid`,`mode`),
  KEY `ix_instanceid` (`playerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player / InstanceID - Reference Table';

/*Data for the table `instanceids` */

LOCK TABLES `instanceids` WRITE;

UNLOCK TABLES;

/*Table structure for table `instances` */

DROP TABLE IF EXISTS `instances`;

CREATE TABLE `instances` (
  `id` int(30) NOT NULL,
  `mapid` int(30) NOT NULL,
  `creation` int(30) NOT NULL,
  `expiration` int(30) NOT NULL,
  `killed_npc_guids` text NOT NULL,
  `difficulty` int(30) NOT NULL,
  `creator_group` int(30) NOT NULL,
  `creator_guid` int(30) NOT NULL,
  `persistent` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `a` (`mapid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `instances` */

LOCK TABLES `instances` WRITE;

UNLOCK TABLES;

/*Table structure for table `mailbox` */

DROP TABLE IF EXISTS `mailbox`;

CREATE TABLE `mailbox` (
  `message_id` int(30) NOT NULL default '0',
  `message_type` int(30) NOT NULL default '0',
  `player_guid` int(30) NOT NULL default '0',
  `sender_guid` int(30) NOT NULL default '0',
  `subject` varchar(255) NOT NULL default '',
  `body` longtext NOT NULL,
  `money` int(30) NOT NULL default '0',
  `attached_item_guids` varchar(200) NOT NULL default '',
  `cod` int(30) NOT NULL default '0',
  `stationary` int(30) NOT NULL default '0',
  `expiry_time` int(30) NOT NULL default '0',
  `delivery_time` int(30) NOT NULL default '0',
  `copy_made` int(30) NOT NULL default '0',
  `read_flag` int(30) NOT NULL default '0',
  `deleted_flag` int(30) NOT NULL default '0',
  PRIMARY KEY  (`message_id`),
  KEY `b` (`player_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `mailbox` */

LOCK TABLES `mailbox` WRITE;

UNLOCK TABLES;

/*Table structure for table `mailbox_insert_queue` */

DROP TABLE IF EXISTS `mailbox_insert_queue`;

CREATE TABLE `mailbox_insert_queue` (
  `sender_guid` int(30) NOT NULL,
  `receiver_guid` int(30) NOT NULL,
  `subject` varchar(200) NOT NULL,
  `body` varchar(500) NOT NULL,
  `stationary` int(30) NOT NULL,
  `money` int(30) NOT NULL,
  `item_id` int(30) NOT NULL,
  `item_stack` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `mailbox_insert_queue` */

LOCK TABLES `mailbox_insert_queue` WRITE;

UNLOCK TABLES;

/*Table structure for table `playercooldowns` */

DROP TABLE IF EXISTS `playercooldowns`;

CREATE TABLE `playercooldowns` (
  `player_guid` int(30) NOT NULL,
  `cooldown_type` int(30) NOT NULL COMMENT '0 is spell, 1 is item, 2 is spell category',
  `cooldown_misc` int(30) NOT NULL COMMENT 'spellid/itemid/category',
  `cooldown_expire_time` int(30) NOT NULL COMMENT 'expiring time in unix epoch format',
  `cooldown_spellid` int(30) NOT NULL COMMENT 'spell that cast it',
  `cooldown_itemid` int(30) NOT NULL COMMENT 'item that cast it'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `playercooldowns` */

LOCK TABLES `playercooldowns` WRITE;

UNLOCK TABLES;

/*Table structure for table `playeritems` */

DROP TABLE IF EXISTS `playeritems`;

CREATE TABLE `playeritems` (
  `ownerguid` int(10) unsigned NOT NULL default '0',
  `guid` bigint(10) NOT NULL auto_increment,
  `entry` int(10) unsigned NOT NULL default '0',
  `wrapped_item_id` int(30) NOT NULL default '0',
  `wrapped_creator` int(30) NOT NULL default '0',
  `creator` int(10) unsigned NOT NULL default '0',
  `count` int(10) unsigned NOT NULL default '0',
  `charges` int(10) unsigned NOT NULL default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `randomprop` int(10) unsigned NOT NULL default '0',
  `randomsuffix` int(10) NOT NULL,
  `itemtext` int(10) unsigned NOT NULL default '0',
  `durability` int(10) unsigned NOT NULL default '0',
  `containerslot` int(11) default '-1',
  `slot` int(10) NOT NULL default '0',
  `enchantments` longtext NOT NULL,
  `expireson` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`guid`),
  KEY `ownerguid` (`ownerguid`),
  KEY `itemtext` (`itemtext`)
) ENGINE=MyISAM AUTO_INCREMENT=87328711 DEFAULT CHARSET=latin1;

/*Data for the table `playeritems` */

LOCK TABLES `playeritems` WRITE;

UNLOCK TABLES;

/*Table structure for table `playeritems_insert_queue` */

DROP TABLE IF EXISTS `playeritems_insert_queue`;

CREATE TABLE `playeritems_insert_queue` (
  `ownerguid` int(10) unsigned NOT NULL default '0',
  `entry` int(10) unsigned NOT NULL default '0',
  `wrapped_item_id` int(30) NOT NULL default '0',
  `wrapped_creator` int(30) NOT NULL default '0',
  `creator` int(10) unsigned NOT NULL default '0',
  `count` int(10) unsigned NOT NULL default '0',
  `charges` int(10) unsigned NOT NULL default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `randomprop` int(10) unsigned NOT NULL default '0',
  `randomsuffix` int(30) NOT NULL,
  `itemtext` int(10) unsigned NOT NULL default '0',
  `durability` int(10) unsigned NOT NULL default '0',
  `containerslot` int(11) NOT NULL default '-1' COMMENT 'couldnt find this being used in source',
  `slot` tinyint(4) NOT NULL default '0',
  `enchantments` longtext collate utf8_unicode_ci NOT NULL,
  KEY `ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Data for the table `playeritems_insert_queue` */

LOCK TABLES `playeritems_insert_queue` WRITE;

UNLOCK TABLES;

/*Table structure for table `playerpets` */

DROP TABLE IF EXISTS `playerpets`;

CREATE TABLE `playerpets` (
  `ownerguid` bigint(20) NOT NULL default '0',
  `petnumber` int(11) NOT NULL default '0',
  `name` varchar(21) NOT NULL default '',
  `entry` int(10) unsigned NOT NULL default '0',
  `fields` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `xp` int(11) NOT NULL default '0',
  `active` tinyint(1) NOT NULL default '0',
  `level` int(11) NOT NULL default '0',
  `actionbar` varchar(200) NOT NULL default '',
  `happinessupdate` int(11) NOT NULL default '0',
  `summon` int(11) NOT NULL default '0',
  `reset_time` int(10) unsigned NOT NULL default '0',
  `reset_cost` int(10) NOT NULL default '0',
  `spellid` int(10) unsigned NOT NULL default '0',
  `petstate` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ownerguid`,`petnumber`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `playerpets` */

LOCK TABLES `playerpets` WRITE;

UNLOCK TABLES;

/*Table structure for table `playerpetspells` */

DROP TABLE IF EXISTS `playerpetspells`;

CREATE TABLE `playerpetspells` (
  `ownerguid` bigint(20) NOT NULL default '0',
  `petnumber` int(4) NOT NULL default '0',
  `spellid` int(4) NOT NULL default '0',
  `flags` int(4) NOT NULL default '0',
  KEY `a` (`ownerguid`),
  KEY `b` (`petnumber`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `playerpetspells` */

LOCK TABLES `playerpetspells` WRITE;

UNLOCK TABLES;

/*Table structure for table `playersummons` */

DROP TABLE IF EXISTS `playersummons`;

CREATE TABLE `playersummons` (
  `ownerguid` int(11) unsigned NOT NULL default '0',
  `entry` int(11) unsigned NOT NULL default '0',
  `name` varchar(64) NOT NULL,
  KEY `a` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `playersummons` */

LOCK TABLES `playersummons` WRITE;

UNLOCK TABLES;

/*Table structure for table `playersummonspells` */

DROP TABLE IF EXISTS `playersummonspells`;

CREATE TABLE `playersummonspells` (
  `ownerguid` bigint(20) NOT NULL default '0',
  `entryid` int(4) NOT NULL default '0',
  `spellid` int(4) NOT NULL default '0',
  KEY `a` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `playersummonspells` */

LOCK TABLES `playersummonspells` WRITE;

UNLOCK TABLES;

/*Table structure for table `questlog` */

DROP TABLE IF EXISTS `questlog`;

CREATE TABLE `questlog` (
  `player_guid` bigint(20) unsigned NOT NULL default '0',
  `quest_id` bigint(20) unsigned NOT NULL default '0',
  `slot` int(20) unsigned NOT NULL default '0',
  `time_left` bigint(20) NOT NULL default '0',
  `explored_area1` bigint(20) unsigned NOT NULL default '0',
  `explored_area2` bigint(20) unsigned NOT NULL default '0',
  `explored_area3` bigint(20) unsigned NOT NULL default '0',
  `explored_area4` bigint(20) unsigned NOT NULL default '0',
  `mob_kill1` bigint(20) NOT NULL default '0',
  `mob_kill2` bigint(20) NOT NULL default '0',
  `mob_kill3` bigint(20) NOT NULL default '0',
  `mob_kill4` bigint(20) NOT NULL default '0',
  PRIMARY KEY  (`player_guid`,`quest_id`)
) ENGINE=MyISAM AUTO_INCREMENT=33067588 DEFAULT CHARSET=latin1;

/*Data for the table `questlog` */

LOCK TABLES `questlog` WRITE;

UNLOCK TABLES;

/*Table structure for table `server_settings` */

DROP TABLE IF EXISTS `server_settings`;

CREATE TABLE `server_settings` (
  `setting_id` varchar(200) NOT NULL,
  `setting_value` int(50) NOT NULL,
  PRIMARY KEY  (`setting_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `server_settings` */

LOCK TABLES `server_settings` WRITE;

UNLOCK TABLES;

/*Table structure for table `social_friends` */

DROP TABLE IF EXISTS `social_friends`;

CREATE TABLE `social_friends` (
  `character_guid` int(30) NOT NULL,
  `friend_guid` int(30) NOT NULL,
  `note` varchar(100) NOT NULL,
  PRIMARY KEY  (`character_guid`,`friend_guid`),
  KEY `a` (`character_guid`),
  KEY `b` (`friend_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `social_friends` */

LOCK TABLES `social_friends` WRITE;

UNLOCK TABLES;

/*Table structure for table `social_ignores` */

DROP TABLE IF EXISTS `social_ignores`;

CREATE TABLE `social_ignores` (
  `character_guid` int(30) NOT NULL,
  `ignore_guid` int(30) NOT NULL,
  PRIMARY KEY  (`character_guid`,`ignore_guid`),
  KEY `a` (`character_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `social_ignores` */

LOCK TABLES `social_ignores` WRITE;

UNLOCK TABLES;

/*Table structure for table `tutorials` */

DROP TABLE IF EXISTS `tutorials`;

CREATE TABLE `tutorials` (
  `playerId` bigint(20) unsigned NOT NULL default '0',
  `tut0` bigint(20) unsigned NOT NULL default '0',
  `tut1` bigint(20) unsigned NOT NULL default '0',
  `tut2` bigint(20) unsigned NOT NULL default '0',
  `tut3` bigint(20) unsigned NOT NULL default '0',
  `tut4` bigint(20) unsigned NOT NULL default '0',
  `tut5` bigint(20) unsigned NOT NULL default '0',
  `tut6` bigint(20) unsigned NOT NULL default '0',
  `tut7` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `tutorials` */

LOCK TABLES `tutorials` WRITE;

UNLOCK TABLES;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;