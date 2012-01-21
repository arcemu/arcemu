DELETE FROM `gameobject_quest_starter`
WHERE `id` NOT IN ( SELECT `entry` FROM `gameobject_names` );

DELETE FROM `gameobject_quest_finisher`
WHERE `id` NOT IN ( SELECT `entry` FROM `gameobject_names` );

DELETE FROM `gameobject_quest_item_binding`
WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );

DELETE FROM `gameobject_quest_pickup_binding`
WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );

DELETE FROM `gameobject_teleports`
WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );

DELETE FROM `gameobject_spawns`
WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );

DELETE FROM `loot_gameobjects`
WHERE `entryid` NOT IN ( SELECT `entry` FROM `gameobject_names` );
