/*
MySQL Data Transfer
Source Host: localhost
Source Database: nowa
Target Host: localhost
Target Database: nowa
Date: 2009-07-30 13:52:38
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for spell_proc
-- ----------------------------
DROP TABLE IF EXISTS `spell_proc`;
CREATE TABLE `spell_proc` (
  `spellID` int(30) NOT NULL default '0',
  `ProcOnNameHash` int(30) unsigned NOT NULL default '0',
  `ProcFlag` int(30) NOT NULL default '0',
  `TargetSelf` tinyint(1) NOT NULL default '0',
  `ProcChance` int(30) NOT NULL default '-1',
  `ProcCharges` smallint(30) NOT NULL default '-1',
  `ProcInterval` int(30) NOT NULL default '0',
  `EffectTriggerSpell[0]` int(10) NOT NULL default '-1',
  `EffectTriggerSpell[1]` int(10) NOT NULL default '-1',
  `EffectTriggerSpell[2]` int(10) NOT NULL default '-1',
  PRIMARY KEY  (`spellID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `spell_proc` VALUES ('23920', '0', '134348800', '0', '100', '1', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('30482', '0', '10792', '0', '-1', '0', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('43045', '0', '10792', '0', '-1', '0', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('324', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('325', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('905', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('945', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('8134', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('10431', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('10432', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('25469', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('25472', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('49280', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('49281', '0', '666152', '0', '100', '3', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('43046', '0', '10792', '0', '-1', '-1', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('37982', '0', '4', '0', '1', '0', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('27521', '0', '16', '0', '5', '0', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('38394', '0', '1024', '0', '-1', '-1', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('64976', '0', '16', '0', '-1', '-1', '0', '-1', '-1', '-1');
INSERT INTO `spell_proc` VALUES ('65156', '3808755873', '0', '0', '-1', '-1', '0', '-1', '-1', '-1');
