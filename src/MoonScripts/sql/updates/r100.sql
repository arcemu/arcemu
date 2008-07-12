-- Currently standard for NCDB
UPDATE `creature_proto` SET `minlevel` = '70' WHERE `entry` = '21101';
UPDATE `creature_proto` SET `maxlevel` = '70' WHERE `entry` = '21101';
UPDATE `creature_proto` SET `minhealth` = '100000' WHERE `entry` = '21101';
UPDATE `creature_proto` SET `maxhealth` = '100000' WHERE `entry` = '21101';
UPDATE `creature_proto` SET `scale` = '.1' WHERE `entry` = '21101';

UPDATE `creature_proto` SET `attacktype` = '5' WHERE `entry` = '20870';