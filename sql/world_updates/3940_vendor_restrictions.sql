ALTER TABLE `vendor_restrictions` ADD `classmask` INT NOT NULL DEFAULT '-1' AFTER `racemask`; 
ALTER TABLE `vendor_restrictions` ADD `flags` INT UNSIGNED NOT NULL DEFAULT '0' COMMENT '0 - check for all values, 1 - classic mount vendor'; 
-- As all current rows in that table are mount vendors, set their proper flags. 
UPDATE `vendor_restrictions` SET `flags` = '1';

UPDATE `arcemu_db_version` SET `LastUpdate` = '3940';