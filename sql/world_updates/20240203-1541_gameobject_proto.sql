ALTER TABLE gameobject_proto ADD COLUMN faction INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER QuestItem6;

UPDATE world_db_version SET LastUpdate = '20240203-1541_gameobject_proto';
