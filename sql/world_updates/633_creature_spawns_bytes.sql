ALTER TABLE creature_spawns CHANGE `bytes` `bytes0` INT( 30 ) NOT NULL DEFAULT '0';
ALTER TABLE creature_spawns ADD bytes1 INT( 30 ) NOT NULL DEFAULT '0' AFTER bytes0;

ALTER TABLE creature_staticspawns CHANGE `bytes` `bytes0` INT( 30 ) NOT NULL DEFAULT '0';
ALTER TABLE creature_staticspawns ADD bytes1 INT( 30 ) NOT NULL DEFAULT '0' AFTER bytes0;
