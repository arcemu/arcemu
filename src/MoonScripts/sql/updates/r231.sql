
UPDATE creature_names SET male_displayid =11686,female_displayid =0 WHERE entry =19870;
UPDATE creature_proto SET faction=14 WHERe entry IN (21212, 21958, 22055, 22056);
UPDATE creature_spawns SET faction=14 WHERE entry IN (21212, 21958, 22055, 22056);
INSERT IGNORE INTO `gameobject_names` (`entry`, `Type`, `DisplayID`, `Name`, `spellfocus`, `sound1`, `sound2`, `sound3`, `sound4`, `sound5`, `sound6`, `sound7`, `sound8`, `sound9`, `unknown1`, `unknown2`, `unknown3`, `unknown4`, `unknown5`, `unknown6`, `unknown7`, `unknown8`, `unknown9`, `unknown10`, `unknown11`, `unknown12`, `unknown13`, `unknown14`) VALUES
(185051, 0, 7265, 'Shield Generator', 0, 1718, 131072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DELETE FROM gameobject_spawns WHERE entry=185051;
INSERT INTO `gameobject_spawns` (`Entry`, `map`, `position_x`, `position_y`, `position_z`, `Facing`, `orientation1`, `orientation2`, `orientation3`, `orientation4`, `State`, `Flags`, `Faction`, `Scale`, `stateNpcLink`) VALUES
(185051, 548, 49.256, -902.354, 42.9769, 3.93902, 0, 0, 0.929729, -0.368245, 1, 0, 0, 1.5, 0),
(185051, 548, 49.687, -944.406, 42.7324, 2.31089, 0, 0, 0.914975, 0.403512, 1, 0, 0, 1.5, 0),
(185051, 548, 10.4122, -944.613, 42.8262, 0.817042, 0, 0, 0.39257, 0.919722, 1, 0, 0, 1.5, 0),
(185051, 548, 9.78695, -902.882, 42.9, 5.4399, 0, 0, 0.382205, -0.924078, 1, 0, 0, 1.5, 0);
