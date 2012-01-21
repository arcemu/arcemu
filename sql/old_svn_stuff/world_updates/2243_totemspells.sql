REPLACE INTO `totemspells` (`spell`, `castspell1`, `castspell2`, `castspell3`) VALUES
-- Fire Resistance Totem
('58737','58738','0','0'),
('58739','58740','0','0'),
-- Frost Resistance Totem
('58741','58742','0','0'),
('58745','58744','0','0'),
-- Nature Resistance Totem
('58746','58748','0','0'),
('58749','58750','0','0'),
-- Magma Totem
('58731','58732','0','0'),
('58734','58735','0','0'),
-- Fire Nova Totem
('61649','61651','0','0'),
('61657','61660','0','0'),
-- Stoneclaw Totem
('58580','25513','0','0'),
('58581','25513','0','0'),
('58582','25513','0','0'),
-- Searing Totem
('58699','58700','0','0'),
('58703','58701','0','0'),
('58704','58702','0','0');

INSERT into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`) values('32775','Fire Nova Totem IX','','','0','11','0','0','0','0','4683','0','0','0','1','1',NULL,'0');
INSERT into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`) values('32776','Fire Nova Totem VIII','','','0','11','0','0','0','0','4683','0','0','0','1','1',NULL,'0');
INSERT into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `can_ranged`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `spell1`, `spell2`, `spell3`, `spell4`, `spell_flags`, `modImmunities`) values('32775','1','80','35','5','5','0','0','0','1500','0','0','0','0','0','0','0','360000','0','0','0','0','0','0','0','1','2.5','0','0','0','0','0','2.5','8','14','0','0','0','0','0','0','0');
INSERT into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `can_ranged`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `spell1`, `spell2`, `spell3`, `spell4`, `spell_flags`, `modImmunities`) values('32776','1','80','35','5','5','0','0','0','1500','0','0','0','0','0','0','0','360000','0','0','0','0','0','0','0','1','2.5','0','0','0','0','0','2.5','8','14','0','0','0','0','0','0','0');
UPDATE `totemspells` SET `castspell1` = '5728' WHERE `spell` = '5730';