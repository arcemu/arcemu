ALTER TABLE `vehicle_accessories` CHANGE `accessory_entry` `accessory_entry` INT(10) UNSIGNED DEFAULT '0' NOT NULL, CHANGE `seat` `seat` INT(10) UNSIGNED DEFAULT '0' NOT NULL, DROP PRIMARY KEY, ADD PRIMARY KEY(`creature_entry`, `seat`);
UPDATE `arcemu_db_version` SET `LastUpdate` = '4606';
