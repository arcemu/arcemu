-- Moving death_state into creature_spawns table
ALTER TABLE `creature_spawns` ADD `death_state` TINYINT( 3 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `standstate`;
ALTER TABLE `creature_staticspawns` ADD `death_state` TINYINT( 3 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `standstate`;
UPDATE creature_spawns cs INNER JOIN creature_proto cp ON cs.Entry = cp.Entry SET cs.death_state = cp.death_state;
ALTER TABLE `creature_proto` DROP `death_state`;
