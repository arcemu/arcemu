-- Portals.sql

 /********************************
 *                               *
 *        The NCDB Project       *
 *                               *
 ********************************/

-- This software is provided as free and open source by the
-- staff of The NCDB Project, in accordance with 
-- the GPL license. This means we provide the software we have 
-- created freely and it has been thoroughly tested to work for 
-- the developers, but NO GUARANTEE is made it will work for you 
-- as well. Please give credit where credit is due, if modifying,
-- redistributing and/or using this software. Thank you.

-- Staff of The NCDB Project, December 2007
-- ~~End of License Agreement

-- Credits for this file go to TruWoW Online and TheRadiator, with editing done by Moldy.

-- NCDB Project, Dcember 2007

-- This is a kludge system because the zeppelins and boats in Ascent are currently messed up.

-- To run, compile the C++ Portals Script and put those in your scripts_bin directory for your
-- server. Then execute these queries. Restart your server to see the changes.

DELETE FROM `gameobject_names` WHERE `entry` IN (20000000,20000001,20000002,20000003,20000004,20000005,20000006,20000007,20000008,20000009,20000010,20000011,20000012,20000013,20000014,20000015,20000016,20000017);

-- XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    gameobject_names    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

-- Portal 1 (Orgrimmar to Undercity)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000000,10,4394,'Orgrimmar Portal to Undercity');

-- Portal 2 (Undercity to Orgrimmar)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000001,10,4394,'Undercity Portal to Orgrimmar');

-- Portal 3 (Orgrimmar to Grom'Gol)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000002,10,4394,'Orgrimmar Portal to Grom Gol');

-- Portal 4 (Grom'Gol to Orgrimmar )
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000003,10,4394,'Grom Gol Portal to Orgrimmar');

-- Portal 5 (Undercity to Grom'Gol)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000004,10,4394,'Undercity Portal to Grom Gol');

-- Portal 6(Grom'Gol to Undercity)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000005,10,4394,'Grom Gol Portal to Undercity');

-- Portal 7 (Ratchet to Booty Bay)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000006,10,4394,'Ratchet Portal to Booty Bay');

-- Portal 8 (Booty Bay to Ratchet)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000007,10,4394,'Booty Bay Portal to Ratchet');

-- Portal 9 (Auberdine to Menethil Harbor)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000008,10,4394,'Auberdine Portal to Menethil Harbor');

-- Portal 10 (Menethil Harbor to Auberdine)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000009,10,4394,'Menethil Harbor Portal to Auberdine');

-- Portal 11 (Menathil Harbor to Theramore Isle)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000010,10,4394,'Menathil Harbor Portal to Theramore Isle');

-- Portal 12 (Thereamore Isle to Menethil Harbor)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000011,10,4394,'Theramore Isle Portal to Menethil Harbor');

-- Portal 13 (Auberdine to Azuremyst Isle)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000012,10,4394,'Auberdine Portal to Azuremyst Isle');

-- Portal 14 (Azuremyst Isle to Auberdine)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000013,10,4394,'Azuremyst Isle Portal to Auberdine');

-- Portal 15 (Auberdine to Teldrassil)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000014,10,4394,'Auberdine Portal to Teldrassil');

-- Portal 16 (Teldrassil to Auberdine)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000015,10,4394,'Teldrassil Portal to Auberdine');

-- Portal 17 (Stormwind to Ironforge)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000016,10,4394,'Stormwind Portal to Ironforge');

-- Portal 18 (Ironforge to Stormwind)
INSERT INTO `gameobject_names` (`entry`,`type`,`displayid`,`name`) VALUES (20000017,10,4394,'Ironforge Portal to Stormwind');

-- XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX     gameobject_spawns    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

-- Portal 1 (Orgrimmar to Undercity)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000000,1,1322.011963,-4638.138672,54.512932,5.585752,1);

-- Portal 2 (Undercity to Orgrimmar)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000001,0,2069.507813,283.250397,97.031502,4.280417,1);

-- Portal 3 (Orgrimmar to Grom'Gol)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000002,1,1368.40454,-4646.730469,54.291367,2.660515,1);

-- Portal 4 (Grom'Gol to Orgrimmar )
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000003,0,-12442.848633,228.773758,32.021626,5.032833,1);

-- Portal 5 (Undercity to Grom'Gol)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000004,0,2061.267334,241.865860,99.767365,1.40822,1);

-- Portal 6(Grom'Gol to Undercity)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000005,0,-12409.104492,215.795135,31.527250,3.874365,1);

-- Portal 7 (Ratchet to Booty Bay)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000006,1,-986.216003,-3831.011230,5.633424,2.446515,1);

-- Portal 8 (Booty Bay to Ratchet)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000007,0,-14296.986328,551.905518,8.695939,5.960384,1);

-- Portal 9 (Auberdine to Menethil Harbor)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000008,1,6422.215332,821.354004,5.480999,5.55541,1);

-- Portal 10 (Menethil Harbor to Auberdine)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000009,0,-3734.417236,-584.786438,4.9997908,1.463867,1);

-- Portal 11 (Menathil Harbor to Theramore Isle)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000010,0,-3889.680420,-596.451721,6.260849,4.549423,1);

-- Portal 12 (Thereamore Isle to Menethil Harbor)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000011,1,-3990.916748,-4733.024414,4.9997908,1.463867,1);

-- Portal 13 (Auberdine to Azuremyst Isle)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000012,1,6548.476563,924.657959,6.046269,4.281991,1);

-- Portal 14 (Azuremyst Isle to Auberdine)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000013,530,-4259.575684,-11333.785156,5.789986,4.583838,1);

-- Portal 15 (Teldrassil to Auberdine)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000014,1,6577.7773344,763.997742,5.741073,5.815087,1);

-- Portal 16 (Auberdine to Teldrassil)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000015,1,8551.734375,1023.275146,5.642698,2.354622,1);

-- Portal 17 (Stormwind to Ironforge)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000016,369,14.795420,2490.307861,4.292574,6.253338,1);

-- Portal 18 (Ironforge to Stormwind)
INSERT INTO `gameobject_spawns` (`Entry`,`map`,`position_x`,`position_y`,`position_z`,`Facing`,`Scale`) Values (20000017,369,14.673943,8.185436,-4.489475,0.031412,1);