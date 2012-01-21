-- Game Objects
UPDATE `quests` SET `ReqKillMobOrGOId1` = '180516' WHERE entry = 8345; -- The Shrine of Dath'Remar
UPDATE `quests` SET `ReqKillMobOrGOId1` = '178090' WHERE entry = 6395; -- Marla's Last Wish


-- Spell cast targets
UPDATE `quests` SET `ReqCastSpellId1` = '28730', `ReqKillMobOrGOId1` = '15274' WHERE `entry` = '8346'; -- Thirst Unending
UPDATE `quests` SET `ReqCastSpellId1` = '17166', `ReqCastSpellId2` = '17166', `ReqCastSpellId3` = '17166', `ReqKillMobOrGOId1` = '10978', `ReqKillMobOrGOId2` = '7583', `ReqKillMobOrGOId3` = '10977' WHERE `entry` = '5163'; -- Are We There, Yeti?



-- Emote quests
ALTER TABLE `quests` ADD COLUMN `ReqEmoteId1` INT UNSIGNED DEFAULT '0' AFTER `ReqCastSpellId4`, ADD COLUMN `ReqEmoteId2` INT UNSIGNED DEFAULT '0' AFTER `ReqEmoteId1`, ADD COLUMN `ReqEmoteId3` INT UNSIGNED DEFAULT '0' AFTER `ReqEmoteId2`, ADD COLUMN `ReqEmoteId4` INT UNSIGNED DEFAULT '0' AFTER `ReqEmoteId3`;
UPDATE `quests` SET `ReqEmoteId1` = '22',  `ReqKillMobOrGOId1` =  '5111' WHERE `entry` = '8353';
UPDATE `quests` SET `ReqEmoteId1` = '22',  `ReqKillMobOrGoId1` =  '6741' WHERE `entry` = '8354';
UPDATE `quests` SET `ReqEmoteId1` = '264', `ReqKillMobOrGOId1` =  '6826' WHERE `entry` = '8355';
UPDATE `quests` SET `ReqEmoteId1` = '41',  `ReqKillMobOrGOId1` =  '6740' WHERE `entry` = '8356';
UPDATE `quests` SET `ReqEmoteId1` = '34',  `ReqKillMobOrGOId1` =  '6735' WHERE `entry` = '8357';
UPDATE `quests` SET `ReqEmoteId1` = '264', `ReqKillMobOrGOId1` = '11814' WHERE `entry` = '8358';
UPDATE `quests` SET `ReqEmoteId1` = '41',  `ReqKillMobOrGOId1` =  '6929' WHERE `entry` = '8359';
UPDATE `quests` SET `ReqEmoteId1` = '34',  `ReqKillMobOrGOId1` =  '6746' WHERE `entry` = '8360';


-- Taming the Beast
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '2956', `ReqCastSpellId1` = '19688' WHERE `entry` = '6061';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '3099', `ReqCastSpellId1` = '19694' WHERE `entry` = '6062';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '1998', `ReqCastSpellId1` = '19693' WHERE `entry` = '6063';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '1126', `ReqCastSpellId1` = '19674' WHERE `entry` = '6064';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '3126', `ReqCastSpellId1` = '19697' WHERE `entry` = '6082';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '3107', `ReqCastSpellId1` = '19696' WHERE `entry` = '6083';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '1201', `ReqCastSpellId1` = '19687' WHERE `entry` = '6084';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '1196', `ReqCastSpellId1` = '19548' WHERE `entry` = '6085';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '2959', `ReqCastSpellId1` = '19689' WHERE `entry` = '6087';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '2970', `ReqCastSpellId1` = '19692' WHERE `entry` = '6088';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '2043', `ReqCastSpellId1` = '19699' WHERE `entry` = '6101';
UPDATE `quests` SET `ReqKillMobOrGOId1` =  '1996', `ReqCastSpellId1` = '19700' WHERE `entry` = '6102';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '15650', `ReqCastSpellId1` = '30099' WHERE `entry` = '9484';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '16353', `ReqCastSpellId1` = '30105' WHERE `entry` = '9485';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '15652', `ReqCastSpellId1` = '30102' WHERE `entry` = '9486';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '17217', `ReqCastSpellId1` = '30646' WHERE `entry` = '9591';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '17374', `ReqCastSpellId1` = '30653' WHERE `entry` = '9592';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '17203', `ReqCastSpellId1` = '30654' WHERE `entry` = '9593';


-- Drakkari Offerings quests
UPDATE `quests` SET `ReqKillMobOrGOId1` = '190535' WHERE `entry` = '12567';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '190657' WHERE `entry` = '12656';
UPDATE `quests` SET `ReqKillMobOrGOId1` = '190602' WHERE `entry` = '12618';
