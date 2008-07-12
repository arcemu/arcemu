-- Level,HP, Damage fix for Prince Malchezaar Axes
UPDATE `creature_proto` SET `minlevel` = 73, `maxlevel` = 73, `minhealth`=99999999, `maxhealth`=99999999, `maxdamage`=600, `mindamage`=400 WHERE `entry`= 1765;
-- Shade of Aran Water Elemental Damage fix
UPDATE `creature_proto` SET `maxdamage`=1500, `mindamage`=1000 WHERE `entry`=17167;
--Totem of Spirit nerf, so players cannot use them as pets (untill this quest gets scripted)
UPDATE `creature_proto` SET `minhealth`='1', `maxhealth`='1', `maxdamage`='1', `attacktime`='140000', `mindamage`='1' WHERE `entry`='21071';
-- World & Instances viewing distance, to make it blizzlike. This also fixes KaelThas Encounter & Shade of Aran teleportation via First guy inside the instance.
update worldmap_info set viewingDistance = 300;
--Berthold Flag Fix
update `creature_proto` set `npcflags`='1' where `entry`='16153';
update `creature_spawns` set `flags`='1' where `id`='16153';
update `creature_names` set `Flags1`='1' where `entry`='16153';