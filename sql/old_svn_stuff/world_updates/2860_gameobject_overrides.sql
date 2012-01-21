-- Add the new column
ALTER TABLE `gameobject_spawns` ADD COLUMN `overrides` int(10) UNSIGNED DEFAULT '0' NOT NULL after `phase`;
ALTER TABLE `gameobject_staticspawns` ADD COLUMN `overrides` int(10) UNSIGNED DEFAULT '0' NOT NULL after `phase`;
-- Add/overwrite Ulduar Tram
REPLACE INTO gameobject_names (entry, Type, DisplayID, Name) VALUES (194675, 11, 8587, "UL_TRAM");
REPLACE INTO gameobject_spawns (Entry, map, position_x, position_y, position_z, Facing, Flags, State) VALUES (194675, 603, 2307.2, 284.9, 424.3, 0, 40, 1);
-- Fix up Deeprun Tram
UPDATE `gameobject_spawns` SET `position_x`='4.580645',`position_y`='28.209660',`position_z`='7.011068',`Facing`='1.570796',`orientation3`='1.000000',`orientation4`='-0.000000' WHERE Entry=176080;
UPDATE `gameobject_spawns` SET `position_x`='4.528066',`position_y`='8.435292',`position_z`='7.011068',`Facing`='1.570796',`orientation3`='1.000000',`orientation4`='-0.000000' WHERE Entry=176081;
UPDATE `gameobject_spawns` SET `position_x`='-45.400524',`position_y`='2492.792236',`position_z`='6.988598',`Facing`='1.570796',`orientation3`='1.000000',`orientation4`='-0.000000' WHERE Entry=176082;
UPDATE `gameobject_spawns` SET `position_x`='-45.400742',`position_y`='2512.148193',`position_z`='6.988598',`Facing`='1.570796',`orientation3`='1.000000',`orientation4`='-0.000000' WHERE Entry=176083;
UPDATE `gameobject_spawns` SET `position_x`='-45.393375',`position_y`='2472.930908',`position_z`='6.988598',`Facing`='1.570796',`orientation3`='1.000000',`orientation4`='-0.000000' WHERE Entry=176084;
UPDATE `gameobject_spawns` SET `position_x`='4.498832',`position_y`='-11.347507',`position_z`='7.011068',`Facing`='-1.570796',`orientation3`='1.000000',`orientation4`='-0.000000' WHERE Entry=176085;
-- Change the overrides for Ulduar Tram to: GAMEOBJECT_INFVIS(1) | GAMEOBJECT_MAPWIDE(2)
UPDATE `gameobject_spawns` SET `overrides`='3' WHERE Entry=194675;
-- Change the overrides for Deeprun Tram to: GAMEOBJECT_INFVIS(1) | GAMEOBJECT_MAPWIDE(2) | GAMEOBJECT_OVERRIDE_PARENTROT(64)
UPDATE `gameobject_spawns` SET `overrides`='67' WHERE Entry IN (176080, 176081, 176082, 176083, 176084, 176085);
