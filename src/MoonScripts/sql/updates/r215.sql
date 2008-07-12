
-- shadow of leotheras
DELETE FROM creature_spawns WHERE entry IN (21875, 21812);
UPDATE `creature_proto` SET `minhealth` = 3700000,`maxhealth` = 3700000 WHERE `entry` = 21875;

UPDATE creature_proto SET faction=14 WHERE entry IN (21875, 21215);
