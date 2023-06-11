ALTER TABLE quests ADD COLUMN ObjectivesCompleteText TEXT CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL AFTER EventObjective;
UPDATE world_db_version SET LastUpdate = '20230611-2000_objectives_complete_text';
