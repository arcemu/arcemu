ALTER TABLE `playeritems` CHANGE `charges` `charges` BIGINT SIGNED NOT NULL DEFAULT '0';
UPDATE `playeritems` SET `charges` = `charges` - '4294967296' WHERE `charges` > '4000000000';
ALTER TABLE `playeritems` CHANGE `charges` `charges` INT SIGNED NOT NULL DEFAULT '0';
