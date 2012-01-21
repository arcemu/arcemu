ALTER TABLE `creature_proto` ADD COLUMN `vehicleid` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `spelldataid`;

ALTER TABLE `creature_proto` ADD COLUMN `spell5` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `spell4`, ADD COLUMN `spell6` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `spell5`, ADD COLUMN `spell7` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `spell6`, ADD COLUMN `spell8` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `spell7`, CHANGE `spell1` `spell1` INT UNSIGNED DEFAULT '0' NOT NULL, CHANGE `spell2` `spell2` INT UNSIGNED DEFAULT '0' NOT NULL, CHANGE `spell3` `spell3` INT UNSIGNED DEFAULT '0' NOT NULL, CHANGE `spell4` `spell4` INT UNSIGNED DEFAULT '0' NOT NULL;


ALTER TABLE `creature_proto` ADD COLUMN `rooted` INT UNSIGNED DEFAULT '0' NOT NULL AFTER `vehicleid`;

CREATE TABLE `vehicle_accessories` (
  `creature_entry` int(10) unsigned NOT NULL DEFAULT '0',
  `accessory_entry` int(10) unsigned NOT NULL DEFAULT '0',
  `seat` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`creature_entry`,`accessory_entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

UPDATE `arcemu_db_version` SET `LastUpdate` = '4590';
