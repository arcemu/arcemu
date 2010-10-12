DELETE FROM `character_achievement_progress` WHERE `counter`=0;
DELETE FROM `character_achievement_progress` WHERE `counter`>2147483647;
ALTER TABLE `character_achievement_progress` MODIFY COLUMN `counter` INT(10) SIGNED;
