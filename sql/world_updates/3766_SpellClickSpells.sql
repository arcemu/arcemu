/*
SQLyog Community v8.5 
MySQL - 5.1.49-1ubuntu8 : Database - dfighterworld
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `SpellClickSpells` */

DROP TABLE IF EXISTS `SpellClickSpells`;

CREATE TABLE `SpellClickSpells` (
  `CreatureID` int(10) unsigned NOT NULL,
  `SpellID` int(10) unsigned NOT NULL,
  PRIMARY KEY (`CreatureID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `SpellClickSpells` */

LOCK TABLES `SpellClickSpells` WRITE;

insert  into `SpellClickSpells`(`CreatureID`,`SpellID`) values (28605,52263),(32633,61425),(29929,55531),(31897,7001),(31896,27873),(31895,27874),(31894,28276),(31893,48084),(31883,48085);

UNLOCK TABLES;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
