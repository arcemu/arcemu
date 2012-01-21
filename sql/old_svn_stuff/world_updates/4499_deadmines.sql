UPDATE `creature_proto` SET `auras` = '12544' WHERE `entry` = '596';
UPDATE `creature_proto` SET `auras` = '12544' WHERE `entry` = '1729';
UPDATE `creature_proto` SET `auras` = '6408' WHERE `entry` = '636';

DELETE FROM `ai_agents` WHERE `entry` = '596';
DELETE FROM `ai_agents` WHERE `entry` = '599';
DELETE FROM `ai_agents` WHERE entry = '626';
DELETE FROM `ai_agents` WHERE `entry` = '1729';
DELETE FROM `ai_agents` WHERE `entry` = '4417';
DELETE FROM `ai_agents` WHERE `entry` = '642';
DELETE FROM `ai_agents` WHERE `entry` = '643';
DELETE FROM `ai_agents` WHERE `entry` = '1763';
DELETE FROM `ai_agents` WHERE `entry` = '647';
DELETE FROM `ai_agents` WHERE `entry` = '645';

INSERT INTO `ai_agents`(`entry`,`instance_mode`,`type`,`event`,`chance`,`maxcount`,`spell`,`spelltype`,`targettype_overwrite`,`cooldown_overwrite`,`floatMisc1`,`Misc2`) VALUES (647,4,4,0,20,0,5208,11,1,-1,0,0),(647,4,4,0,30,0,40505,8,1,-1,0,0),(596,4,4,0,25,0,512,3,1,-1,0,0),(596,4,4,0,30,0,9053,8,1,-1,0,0),(596,4,4,0,25,0,228,11,1,-1,0,0),(599,4,4,0,30,0,9053,8,1,-1,0,0),(599,4,4,0,25,0,700,3,1,-1,0,0),(599,4,4,0,25,0,512,3,1,-1,0,0),(599,4,4,0,25,0,228,11,1,-1,0,0),(626,4,4,0,30,0,5219,8,1,-1,0,0),(1729,4,4,0,30,0,11829,8,1,-1,0,0),(1729,4,4,0,30,0,4979,10,4,-1,0,0),(4417,4,4,2,100,0,6660,8,1,-1,0,0),(4417,4,4,2,100,0,6685,8,1,-1,0,0),(642,4,4,0,30,0,3603,11,1,-1,0,0),(642,4,4,0,30,0,7399,4,1,-1,0,0),(643,4,4,0,30,0,6713,11,1,-1,0,0),(1763,4,4,0,30,0,5213,8,1,-1,0,0),(645,4,4,0,30,0,5174,2,4,-1,0,0),(645,4,4,0,25,0,6306,11,1,-1,0,0);
