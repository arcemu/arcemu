RENAME TABLE gameobject_names TO gameobject_proto;
UPDATE world_db_version SET LastUpdate = '20240202-2225_gameobject_proto';
