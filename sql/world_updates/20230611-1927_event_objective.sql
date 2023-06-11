ALTER TABLE quests CHANGE EndText EventObjective TEXT;
UPDATE world_db_version SET LastUpdate = '20230611-1927_event_objective';
