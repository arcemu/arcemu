-- tainted elemental
UPDATE `creature_proto` SET `scale` = 2 WHERE `entry` = 22009;
-- despawn seer olum
DELETE FROM creature_spawns WHERE Entry=22820;
