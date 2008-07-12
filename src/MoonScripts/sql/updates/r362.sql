/* Claw of the Raven God */

DELETE FROM `gameobject_names` WHERE `entry`='185554';
DELETE FROM `gameobject_names` WHERE `entry`='300154';

INSERT INTO `gameobject_names` VALUES ('185554', '10', '7363', 'The Raven\'s Claw', '1732', '196608', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `gameobject_names` VALUES ('300154', '10', '4891', 'The Raven\'s Claw', '1732', '196608', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

DELETE FROM `gameobject_spawns` WHERE `Entry`='185554';
DELETE FROM `gameobject_spawns` WHERE `Entry`='300154';

INSERT INTO gameobject_spawns
  (id, Entry, map, position_x, position_y, position_z, Facing, orientation1, orientation2, orientation3, orientation4, State, Flags, Faction, Scale, stateNpcLink)
VALUES
  (181815, 185554, 556, "-86.6862", "287.625", "26.4832", "2.27127", 0, 0, 0, 0, 1, 0, 0, 1, 0);
  
DELETE FROM `creature_spawns` WHERE `entry`='23035';
  
  
/* The Dreghood Elders (10368) */

UPDATE `quests` SET `ReceiveItemId1`='29501',`ReceiveItemCount1`='1' WHERE (`entry`='10368');


/* The Bait for Lar'korwi (4292) */

UPDATE `gameobject_names` SET `Type`='8',`spellfocus`='603' WHERE (`entry`='169217');
UPDATE `gameobject_names` SET `Type`='8',`spellfocus`='604' WHERE (`entry`='169216');

DELETE FROM `creature_spawns` WHERE `entry`='9684';