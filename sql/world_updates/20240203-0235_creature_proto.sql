-- First normalize resistances. Some rows have negative values here
UPDATE creature_proto SET resistance3 = 0 WHERE resistance3 < 0;
UPDATE creature_proto SET resistance4 = 0 WHERE resistance4 < 0;
UPDATE creature_proto SET resistance5 = 0 WHERE resistance5 < 0;
UPDATE creature_proto SET resistance6 = 0 WHERE resistance6 < 0;

-- Re-create temporary table
DROP TABLE IF EXISTS creature_proto_temp;
CREATE TABLE `creature_proto_temp` (
  `entry` int(30) unsigned NOT NULL DEFAULT '0',
  `name` varchar(100) NOT NULL,
  `subname` varchar(100) NOT NULL,
  `info_str` varchar(500) NOT NULL,
  `flags1` int(10) unsigned NOT NULL DEFAULT '0',
  `type` int(10) unsigned NOT NULL DEFAULT '0',
  `family` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `killcredit1` int(10) unsigned NOT NULL DEFAULT '0',
  `killcredit2` int(10) unsigned NOT NULL DEFAULT '0',
  `male_displayid` int(30) unsigned NOT NULL DEFAULT '0',
  `female_displayid` int(30) unsigned NOT NULL DEFAULT '0',
  `male_displayid2` int(30) unsigned NOT NULL DEFAULT '0',
  `female_displayid2` int(30) unsigned NOT NULL DEFAULT '0',
  `unknown_float1` float NOT NULL DEFAULT '1',
  `unknown_float2` float NOT NULL DEFAULT '1',
  `leader` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `questitem1` int(11) unsigned NOT NULL DEFAULT '0',
  `questitem2` int(11) unsigned NOT NULL DEFAULT '0',
  `questitem3` int(11) unsigned NOT NULL DEFAULT '0',
  `questitem4` int(11) unsigned NOT NULL DEFAULT '0',
  `questitem5` int(11) unsigned NOT NULL DEFAULT '0',
  `questitem6` int(11) unsigned NOT NULL DEFAULT '0',
  `waypointid` int(10) unsigned NOT NULL DEFAULT '0',
  `minlevel` int(30) unsigned NOT NULL,
  `maxlevel` int(30) unsigned NOT NULL,
  `faction` int(30) unsigned NOT NULL DEFAULT '0',
  `minhealth` int(30) unsigned NOT NULL,
  `maxhealth` int(30) unsigned NOT NULL,
  `mana` int(30) unsigned NOT NULL DEFAULT '0',
  `scale` float NOT NULL DEFAULT '0',
  `npcflags` int(30) unsigned NOT NULL DEFAULT '0',
  `attacktime` int(30) unsigned NOT NULL DEFAULT '0',
  `attacktype` int(4) NOT NULL DEFAULT '0',
  `mindamage` float NOT NULL DEFAULT '0',
  `maxdamage` float NOT NULL DEFAULT '0',
  `can_ranged` int(11) unsigned NOT NULL DEFAULT '0',
  `rangedattacktime` int(30) unsigned NOT NULL DEFAULT '0',
  `rangedmindamage` float unsigned NOT NULL DEFAULT '0',
  `rangedmaxdamage` float unsigned NOT NULL DEFAULT '0',
  `respawntime` int(30) unsigned NOT NULL DEFAULT '0',
  `armor` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance1` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance2` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance3` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance4` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance5` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance6` int(30) unsigned NOT NULL DEFAULT '0',
  `combat_reach` float NOT NULL DEFAULT '0',
  `bounding_radius` float NOT NULL DEFAULT '0',
  `auras` longtext NOT NULL,
  `boss` int(11) unsigned NOT NULL DEFAULT '0',
  `money` int(30) NOT NULL DEFAULT '0',
  `invisibility_type` int(30) unsigned NOT NULL,
  `walk_speed` float NOT NULL DEFAULT '2.5',
  `run_speed` float NOT NULL DEFAULT '8',
  `fly_speed` float NOT NULL DEFAULT '14',
  `extra_a9_flags` int(30) NOT NULL DEFAULT '0',
  `spell1` int(30) unsigned NOT NULL DEFAULT '0',
  `spell2` int(30) unsigned NOT NULL DEFAULT '0',
  `spell3` int(30) unsigned NOT NULL DEFAULT '0',
  `spell4` int(30) unsigned NOT NULL DEFAULT '0',
  `spell5` int(30) unsigned NOT NULL DEFAULT '0',
  `spell6` int(30) unsigned NOT NULL DEFAULT '0',
  `spell7` int(30) unsigned NOT NULL DEFAULT '0',
  `spell8` int(30) unsigned NOT NULL DEFAULT '0',
  `spell_flags` int(30) NOT NULL DEFAULT '0',
  `modImmunities` int(30) unsigned NOT NULL DEFAULT '0',
  `isTrainingDummy` int(10) unsigned NOT NULL DEFAULT '0',
  `guardtype` int(10) unsigned NOT NULL DEFAULT '0',
  `summonguard` int(10) unsigned NOT NULL DEFAULT '0',
  `spelldataid` int(10) unsigned NOT NULL DEFAULT '0',
  `vehicleid` int(10) unsigned NOT NULL DEFAULT '0',
  `rooted` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- Merge data and copy rows to temporary table
INSERT INTO creature_proto_temp
SELECT n.entry, n.name, n.subname, n.info_str, n.flags1, n.type, n.family, n.rank, n.killcredit1, n.killcredit2,
       n.male_displayid, n.female_displayid, n.male_displayid2, n.female_displayid2, n.unknown_float1, n.unknown_float2,
       n.leader, n.questitem1, n.questitem2, n.questitem3, n.questitem4, n.questitem5, n.questitem6, n.waypointid,
       p.minlevel, p.maxlevel, p.faction, p.minhealth, p.maxhealth, p.mana, p.scale, p.npcflags, p.attacktime, p.attacktype,
       p.mindamage, p.maxdamage, p.can_ranged, p.rangedattacktime, p.rangedmindamage, p.rangedmaxdamage, p.respawntime, p.armor,
       p.resistance1, p.resistance2, p.resistance3, p.resistance4, p.resistance5, p.resistance6, p.combat_reach, p.bounding_radius,
       p.auras, p.boss, p.money, p.invisibility_type, p.walk_speed, p.run_speed, p.fly_speed, p.extra_a9_flags,
       p.spell1, p.spell2, p.spell3, p.spell4, p.spell5, p.spell6, p.spell7, p.spell8, p.spell_flags, p.modImmunities, p.isTrainingDummy, p.guardtype,
       p.summonguard, p.spelldataid, p.vehicleid, p.rooted
FROM creature_names n, creature_proto p
WHERE n.entry = p.entry;

-- Drop original tables
DROP TABLE creature_names;
DROP TABLE creature_proto;

-- Temp table is now the new creature_proto
RENAME TABLE creature_proto_temp TO creature_proto;


UPDATE world_db_version SET LastUpdate = '20240203-0235_creature_proto';
