/*
SQLyog Community Edition- MySQL GUI v6.56
MySQL - 5.0.45-community-nt : Database - dbc
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

/*Table structure for table `SpellTargetConstraints` */

DROP TABLE IF EXISTS `SpellTargetConstraints`;

CREATE TABLE `SpellTargetConstraints` (
  `SpellID` int(10) unsigned NOT NULL default '0' COMMENT 'Identifier of the spell',
  `TargetType` int(10) unsigned NOT NULL default '0' COMMENT 'Type of the target ( 0 = Creature, 1 = Gameobject )',
  `TargetID` int(10) unsigned NOT NULL default '0' COMMENT 'Identifier of the target',
  PRIMARY KEY  (`SpellID`,`TargetType`,`TargetID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `SpellTargetConstraints` */

insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (0,0,0);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (2699,0,5307);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (3607,0,2530);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (4130,0,2760);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (4131,0,2761);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (4132,0,2762);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19548,0,1196);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19674,0,1126);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19687,0,1201);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19688,0,2956);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19689,0,2959);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19692,0,2970);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19693,0,1998);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19694,0,3099);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19696,0,3107);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19697,0,3126);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19699,0,2043);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19700,0,1996);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (19938,0,10556);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (27907,0,15941);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (27907,0,15945);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (28369,0,18879);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (29528,0,16518);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30077,0,17226);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30099,0,15650);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30102,0,15652);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30105,0,16353);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30646,0,17217);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30653,0,17374);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30654,0,17203);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (30877,0,17326);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (32578,0,20748);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (32825,0,22357);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (34665,0,16880);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (35772,0,20774);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (36310,0,20058);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (36314,0,20132);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (37136,0,21731);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (38177,0,21387);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (38554,0,19440);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (41291,0,22357);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (41621,0,23487);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (44997,0,24972);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (47394,0,26261);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (52244,0,28750);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (52252,0,28750);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (52389,0,28802);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (52390,0,28802);
insert  into `SpellTargetConstraints`(`SpellID`,`TargetType`,`TargetID`) values (52487,0,28843);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
