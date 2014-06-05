UPDATE `worldstring_tables` SET `entry`=81 WHERE `entry`=82 LIMIT 1;	-- Deathknight
ALTER TABLE `worldstring_tables` AUTO_INCREMENT=82; -- reset autoincrement

DELETE FROM `worldstring_tables` WHERE `entry` in(82, 83, 84, 85);
INSERT INTO `worldstring_tables` (`entry`, `text`) VALUES 
(82, 'Learn about Dual Talent Specialization.'),
(83, 'Purchase a Dual Talent Specialization.'),
(84, 'Are you sure you would like to purchase your second talent specialization'),
(85, 'You do not have enough gold to purchase a dual spec.');

UPDATE `world_db_version` SET `LastUpdate` = '2014-06-05_00-47_worldstring' WHERE `LastUpdate` = '2012-08-14_21-25_worldmap_info';