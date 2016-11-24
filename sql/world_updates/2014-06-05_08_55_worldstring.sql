DELETE FROM `worldstring_tables` WHERE `entry` between 88 and 91;
INSERT INTO `worldstring_tables` (`text`) VALUES 
('You\'re not in a battleground!'),
('You are in a group that is already queued for a battleground or inside a battleground. Leave this first.');
UPDATE `world_db_version` SET `LastUpdate` = '2014-06-05_00-47_worldstring' WHERE `LastUpdate` = '2014-06-05_01-15_worldstring';