-- Mounts at 30?! Yes, it's true: Apprentice Riding and mounts are now available at level 30. Training costs 35 gold.
UPDATE `trainer_spells` SET reqlevel = '30' WHERE learn_spell = '33388' || cast_spell = '33389';


-- Summon Warhorse is now available at level 30.
UPDATE `trainer_spells` SET reqlevel = '30' WHERE learn_spell = '34769' || cast_spell = '13820' || cast_spell = '34768';


-- Curse of Shadows: This spell has been removed.
DELETE FROM `trainer_spells` WHERE learn_spell = '27229';

-- Summon Felsteed is now available at level 30.
UPDATE `trainer_spells` SET `reqlevel` = '30' WHERE `learn_spell` = '5784' || `cast_spell` = '1710';


-- Alchemy
-- Philosopher's Stone now requires 200 Alchemy.
UPDATE `items` SET `RequiredSkill` = '171' AND `RequiredSkillRank` = '200' WHERE `entry` = '9149';


-- Tailoring
-- Herb bags can now hold Un'goro Soil and Evergreen Pouches.
UPDATE `items` SET `bagfamily` = '32' WHERE `entry` IN ('11018', '11020');

-- Mycah of Sporeggar has discovered how to create 28 slot herb bags.
REPLACE INTO `vendors`(`entry`, `item`, `amount`, `max_amount`, `inctime`) VALUES ('18382', '38229', '1', '1', '0');

-- New Tailoring patterns are available from Haughty Modiste in Steamwheedle Port, Tanaris: Haliscan Jacket and Haliscan Pantaloons.
REPLACE INTO `vendors`(`entry`, `item`, `amount`, `max_amount`, `inctime`) VALUES ('15165', '38327', '1', '1', '0'), ('15165', '38328', '1', '1', '0');


-- Items
-- Brutal Gladiator's Pummeler has had its Stamina value increased to the appropriate value.
UPDATE `items` SET `stat_value1` = '31' WHERE `entry` = '35071' AND `stat_type1` = '7';


-- World Environment
-- Young Sporebats and Greater Sporebats in Zangarmarsh now drop Sporebat Eyes for the quest Gathering the Reagents.
REPLACE INTO `creatureloot`(`entryid`, `itemid`, `percentchance`, `mincount`, `maxcount`) VALUES ('20387', '24426', '0.2', '1', '1'), ('18129', '24426', '0.1', '1', '1');


-- Bug Fixes
-- The quest, "Going Going, Guano!" will now have a more consistent level requirement for the dungeon.
UPDATE `quests` SET `MinLevel` = '22', `questlevel` = '26' WHERE `entry` = '1109';
-- The quest, "Into The Scarlet Monastery" is now available to players of the appropriate level for the instance.
UPDATE `quests` SET `MinLevel` = '30' WHERE `entry` = '1048';
