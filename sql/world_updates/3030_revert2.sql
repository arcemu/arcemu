/*
SQLyog Community Edition- MySQL GUI v8.18 
MySQL - 5.1.41-community : Database - test
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `spell_coef_override` */

DROP TABLE IF EXISTS `spell_coef_override`;

CREATE TABLE `spell_coef_override` (
  `id` double DEFAULT NULL,
  `name` varchar(300) DEFAULT NULL,
  `Dspell_coef_override` float DEFAULT NULL,
  `OTspell_coef_override` float DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `spell_coef_override` */

LOCK TABLES `spell_coef_override` WRITE;

insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8400,'Fireball Rank 5',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8401,'Fireball Rank 6',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8402,'Fireball Rank 7',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10148,'Fireball Rank 8',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10149,'Fireball Rank 9',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10150,'Fireball Rank 10',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10151,'Fireball Rank 11',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (25306,'Fireball Rank 12',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (27070,'Fireball Rank 13',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (38692,'Fireball Rank 14',1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (30455,'Ice Lance',0.1429,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (603,'Curse of Doom Rank 1',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (30910,'Curse of Doom Rank 2',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (980,'Curse of Agony Rank 1',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (1014,'Curse of Agony Rank 2',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6217,'Curse of Agony Rank 3',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (11711,'Curse of Agony Rank 4',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (11712,'Curse of Agony Rank 5',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (11713,'Curse of Agony Rank 6',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (27218,'Curse of Agony Rank 7',0,1.2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (33763,'Libebloom',0.4286,0.518);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (18265,'Siphon Life Rank 1',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (18879,'Siphon Life Rank 2',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (18880,'Siphon Life Rank 3',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (18881,'Siphon Life Rank 4',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (27264,'Siphon Life Rank 5',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (30911,'Siphon Life Rank 6',0,2);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (139,'Renew Rank 1',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6074,'Renew Rank 2',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6075,'Renew Rank 3',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6076,'Renew Rank 4',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6077,'Renew Rank 5',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6078,'Renew Rank 6',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10927,'Renew Rank 7',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10928,'Renew Rank 8',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10929,'Renew Rank 9',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (25315,'Renew Rank 10',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (25221,'Renew Rank 11',0,1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (21084,'Seal of Righteousness Rank 1',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20287,'Seal of Righteousness Rank 2',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20288,'Seal of Righteousness Rank 3',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20289,'Seal of Righteousness Rank 4',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20290,'Seal of Righteousness Rank 5',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20291,'Seal of Righteousness Rank 6',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20292,'Seal of Righteousness Rank 7',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20293,'Seal of Righteousness Rank 8',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (27155,'Seal of Righteousness Rank 9',0.1,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (20424,'Seal of Command',0.2,0);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (17,'Power Word: Shield Rank 1',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (592,'Power Word: Shield Rank 2',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (600,'Power Word: Shield Rank 3',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (3747,'Power Word: Shield Rank 4',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6065,'Power Word: Shield Rank 5',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6066,'Power Word: Shield Rank 6',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10898,'Power Word: Shield Rank 7',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10899,'Power Word: Shield Rank 8',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10900,'Power Word: Shield Rank 9',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10901,'Power Word: Shield Rank 10',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (25217,'Power Word: Shield Rank 11',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (25218,'Power Word: Shield Rank 12',0.3,-1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (11426,'Ice Barrier Rank 1',-1,0.1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (13031,'Ice Barrier Rank 2',-1,0.1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (13032,'Ice Barrier Rank 3',-1,0.1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (13033,'Ice Barrier Rank 4',-1,0.1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (27134,'Ice Barrier Rank 5',-1,0.1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (33405,'Ice Barrier Rank 6',-1,0.1);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6143,'Frost Ward Rank 1',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8461,'Frost Ward Rank 2',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8462,'Frost Ward Rank 3',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10177,'Frost Ward Rank 4',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (28609,'Frost Ward Rank 5',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (32796,'Frost Ward Rank 6',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (543,'Fire Ward Rank 1',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8457,'Fire Ward Rank 2',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (8458,'Fire Ward Rank 3',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10223,'Fire Ward Rank 4',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (10225,'Fire Ward Rank 5',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (27128,'Fire Ward Rank 6',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (6229,'Shadow Ward Rank 1',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (11739,'Shadow Ward Rank 2',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (11740,'Shadow Ward Rank 3',-1,0.3);
insert  into `spell_coef_override`(`id`,`name`,`Dspell_coef_override`,`OTspell_coef_override`) values (28610,'Shadow Ward Rank 4',-1,0.3);

UNLOCK TABLES;

/*Table structure for table `spell_proc` */

DROP TABLE IF EXISTS `spell_proc`;

CREATE TABLE `spell_proc` (
  `spellID` int(30) NOT NULL DEFAULT '0',
  `ProcOnNameHash` int(30) unsigned NOT NULL DEFAULT '0',
  `ProcFlag` int(30) NOT NULL DEFAULT '0',
  `TargetSelf` tinyint(1) NOT NULL DEFAULT '0',
  `ProcChance` int(30) NOT NULL DEFAULT '-1',
  `ProcCharges` smallint(30) NOT NULL DEFAULT '-1',
  `ProcInterval` int(30) NOT NULL DEFAULT '0',
  `EffectTriggerSpell[0]` int(10) NOT NULL DEFAULT '-1',
  `EffectTriggerSpell[1]` int(10) NOT NULL DEFAULT '-1',
  `EffectTriggerSpell[2]` int(10) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`spellID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `spell_proc` */

LOCK TABLES `spell_proc` WRITE;

insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (23920,0,134348800,0,100,1,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (30482,0,10792,0,-1,0,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (43045,0,10792,0,-1,0,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (324,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (325,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (905,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (945,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (8134,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (10431,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (10432,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (25469,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (25472,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (49280,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (49281,0,666152,0,100,3,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (43046,0,10792,0,-1,-1,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (37982,0,4,0,1,0,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (27521,0,16,0,5,0,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (38394,0,1024,0,-1,-1,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (64976,0,16,0,-1,-1,0,-1,-1,-1);
insert  into `spell_proc`(`spellID`,`ProcOnNameHash`,`ProcFlag`,`TargetSelf`,`ProcChance`,`ProcCharges`,`ProcInterval`,`EffectTriggerSpell[0]`,`EffectTriggerSpell[1]`,`EffectTriggerSpell[2]`) values (65156,3808755873,0,0,-1,-1,0,-1,-1,-1);

UNLOCK TABLES;

/*Table structure for table `spelloverride` */

DROP TABLE IF EXISTS `spelloverride`;

CREATE TABLE `spelloverride` (
  `overrideId` int(10) unsigned NOT NULL DEFAULT '0',
  `spellId` int(10) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `overrideId` (`overrideId`,`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

/*Data for the table `spelloverride` */

LOCK TABLES `spelloverride` WRITE;

UNLOCK TABLES;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
