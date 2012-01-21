delete from gameobject_spawns where entry in (select entry from gameobject_names where type=33);
delete from gameobject_spawns where entry in (select entry from gameobject_names where displayid=8404);
