ALTER TABLE `quests` ADD `RemoveQuests` longtext NOT NULL AFTER `RequiredQuest4`;
UPDATE `quests` set `RemoveQuests` = "" WHERE `RemoveQuests` IS NULL;
