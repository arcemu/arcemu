/* Cleanup */



/* Laying Waste to the Unwanted */

DELETE FROM `gameobject_names` WHERE `spellfocus` = '1399';

INSERT INTO gameobject_names
  (entry, Type, DisplayID, Name, spellfocus, sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8, sound9, unknown1, unknown2, unknown3, unknown4, unknown5, unknown6, unknown7, unknown8, unknown9, unknown10, unknown11, unknown12, unknown13, unknown14)
VALUES
  (300154, 8, 0, "Eastern Thrower", 1399, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
  (300155, 8, 0, "Central Eastern Thrower", 1399, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
  (300156, 8, 0, "Central Western Thrower", 1399, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
  (300157, 8, 0, "Western Thrower", 1399, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

INSERT INTO gameobject_spawns
  (id, Entry, map, position_x, position_y, position_z, Facing, orientation1, orientation2, orientation3, orientation4, State, Flags, Faction, Scale, stateNpcLink)
VALUES
  (181791, 300154, 530, "-155.181", "2517.65", "43.5829", "3.28096", 0, 0, "0.997573", "-0.0696277", 1, 0, 0, 1, 0),
  (181790, 300155, 530, "-152.651", "2661.57", "44.8859", "2.94499", 0, 0, "0.995172", "0.098145", 1, 0, 0, 1, 0),
  (181789, 300156, 530, "-174.245", "2772.8", "32.3732", "3.28096", 0, 0, "0.997573", "-0.0696288", 1, 0, 0, 1, 0),
  (181788, 300157, 530, "-166.082", "2818.61", "29.2268", "2.93059", 0, 0, "0.99444", "0.105307", 1, 0, 0, 1, 0);
  
  

/* Zeth'Gor must Burn! Allianz (10895) */

DELETE FROM `gameobject_names` WHERE `spellfocus` = '1447';

INSERT INTO gameobject_names
  (entry, Type, DisplayID, Name, spellfocus, sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8, sound9, unknown1, unknown2, unknown3, unknown4, unknown5, unknown6, unknown7, unknown8, unknown9, unknown10, unknown11, unknown12, unknown13, unknown14)
VALUES
  (300150, 8, 0, "Zeth\'Gor must Burn Alliance", 1447, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `gameobject_spawns` WHERE `Entry` = '300150';
DELETE FROM `gameobject_spawns` WHERE `Entry` = '300151';
DELETE FROM `gameobject_spawns` WHERE `Entry` = '300152';
DELETE FROM `gameobject_spawns` WHERE `Entry` = '300153';

INSERT INTO gameobject_spawns
  (id, Entry, map, position_x, position_y, position_z, Facing, orientation1, orientation2, orientation3, orientation4, State, Flags, Faction, Scale, stateNpcLink)
VALUES
  (NULL, 300150, 530, "-822.39", "2031.01", "39.25", 0, 0, 0, 0, 0, 1, 0, 0, 1, 0),
  (NULL, 300150, 530, "-1153.73", "2107.55", "68.1", 0, 0, 0, 0, 0, 1, 0, 0, 1, 0),
  (NULL, 300150, 530, "-897.39", "1920.97", "66.93", 0, 0, 0, 0, 0, 1, 0, 0, 1, 0),
  (NULL, 300150, 530, "-977.39", "1877.65", "94.81", 0, 0, 0, 0, 0, 1, 0, 0, 1, 0);



/* Zeth'Gor must Burn! Horde (10792) */

DELETE FROM `gameobject_names` WHERE `spellfocus` = '1443';
DELETE FROM `gameobject_names` WHERE `Entry` = '300158';
DELETE FROM `gameobject_names` WHERE `Entry` = '300159';
DELETE FROM `gameobject_names` WHERE `Entry` = '300160';
DELETE FROM `gameobject_names` WHERE `Entry` = '300161';

INSERT INTO gameobject_names
  (entry, Type, DisplayID, Name, spellfocus, sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8, sound9, unknown1, unknown2, unknown3, unknown4, unknown5, unknown6, unknown7, unknown8, unknown9, unknown10, unknown11, unknown12, unknown13, unknown14)
VALUES
  (300116, 8, 0, "TEMP Bleeding Hollow Forge", 1443, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
  (300151, 8, 0, "Zeth\'Gor must Burn Horde", 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `gameobject_spawns` WHERE `Entry` = '300158';
DELETE FROM `gameobject_spawns` WHERE `Entry` = '300159';
DELETE FROM `gameobject_spawns` WHERE `Entry` = '300160';
DELETE FROM `gameobject_spawns` WHERE `Entry` = '300161';

INSERT INTO gameobject_spawns
  (id, Entry, map, position_x, position_y, position_z, Facing, orientation1, orientation2, orientation3, orientation4, State, Flags, Faction, Scale, stateNpcLink)
VALUES
  (NULL, 300116, 530, "-898.93", "1964.52", "68.76", 0, 0, 0, 0, 0, 1, 0, 0, 1, 0),
  (NULL, 300151, 530, "-1137.05", "1970.16", "74.0588", "4.14273", 0, 0, "0.877311", "-0.479923", 1, 0, 0, 1, 0),
  (NULL, 300151, 530, "-940.713", "1920.26", "69.2312", "5.87628", 0, 0, "0.202053", "-0.979375", 1, 0, 0, 1, 0),
  (NULL, 300151, 530, "-1155.45", "2061.87", "68.5052", "0.682613", 0, 0, "0.334718", "0.942318", 1, 0, 0, 1, 0),
  (NULL, 300151, 530, "-1052.76", "2007.01", "66.9565", "1.07435", 0, 0, "0.51171", "0.859158", 1, 0, 0, 1, 0);