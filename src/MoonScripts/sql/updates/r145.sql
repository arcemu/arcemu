/* gameobject_spawns*/
-- Opera Stage Right Door should be closed by default, whoever sniffed NCDB GO's in kara, did it after the instance was fully cleared.
update `gameobject_spawns` set `State`='1' where `id`='102508';