/* GOSSIP SCRIPTS */
-- Warsong Gulch Battlemasters Flag
-- Skorn Whitecloud's Flag (gossip + quest)
UPDATE `creature_proto` SET `npcflags`='3' WHERE (`entry`='3052');

-- Warsong Gulch Battlemasters Flag
UPDATE `creature_proto` SET `npcflags`='1' WHERE entry IN (19910,15105,20118,16696,2804,20272,20269,19908,15102,14981,14982,2302,10360,3890);

-- Arathi Basin Battlemasters Flag
UPDATE `creature_proto` SET `npcflags`='1' WHERE entry IN (20273,16694,20274,15007,19855,19905,20120,15008,857,907,12198,14990,15006,14991);

-- Alterac Valley Battlemasters Flag
UPDATE `creature_proto` SET `npcflags`='1' WHERE entry IN (347,19907,16695,20271,20119,19906,20276,7410,12197,5118,15106,15103,14942);

-- Eye of the Storm Battlemasters Flag
UPDATE `creature_proto` SET `npcflags`='1' WHERE entry IN (20388,20385,20390,20383,22013,22015,20362,20381,20374,20386,20382,20384);

-- Guards Flag (not Deathguard Burgess and Deathguard Dillinger this have 3)
UPDATE `creature_proto` SET `npcflags`='1' WHERE entry IN (1423,68,1976,4262,5624,3571,16222,16733,20674,3296,3084,3222,3224,3220,3219,3217,3215,3218,3221,3223,3212,5953,5725,1738,1746,1745,1743,1744,1742,5595,727,16221,18038,19687,18568,18549);