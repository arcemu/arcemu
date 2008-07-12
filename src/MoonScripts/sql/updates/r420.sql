/* Quest : Disrupt Their Reinforcements (10144/10208) */

UPDATE `gameobject_names` SET `Type`='10' WHERE (`entry`='184289');
UPDATE `gameobject_names` SET `Type`='10' WHERE (`entry`='184290');
UPDATE `gameobject_names` SET `Type`='10' WHERE (`entry`='184415');
UPDATE `gameobject_names` SET `Type`='10' WHERE (`entry`='184414');

INSERT INTO `creature_names` (`entry`,`name`) VALUES ('184289','Port Kruul');
INSERT INTO `creature_names` (`entry`,`name`) VALUES ('184290','Port Xilus');
INSERT INTO `creature_names` (`entry`,`name`) VALUES ('184415','Port Kaalez');
INSERT INTO `creature_names` (`entry`,`name`) VALUES ('184414','Port Grimh');

UPDATE `quests` SET `ReqItemId4`='0',`ReqItemCount4`='0',`ReqKillMobOrGOId1`='184414',`ReqKillMobOrGOId2`='184415',`ReqCastSpellId1`='0',`ReqCastSpellId2`='0' WHERE (`entry`='10144');
UPDATE `quests` SET `ReqItemId4`='0',`ReqItemCount4`='0',`ReqKillMobOrGOId1`='184290',`ReqKillMobOrGOId2`='184289',`ReqCastSpellId1`='0',`ReqCastSpellId2`='0' WHERE (`entry`='10208');

UPDATE `items` SET `class`='12',`field4`='0' WHERE (`entry`='28106');