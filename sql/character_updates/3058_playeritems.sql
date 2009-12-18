ALTER TABLE `playeritems` CHANGE `charges` `charges` INT(10) SIGNED DEFAULT '0' NOT NULL;
UPDATE `playeritems` SET `charges` = '-1' WHERE `charges` = '2147483647';
