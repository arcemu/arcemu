ALTER TABLE `creature_spawns` ADD COLUMN `phase` int(10) UNSIGNED DEFAULT '1' NOT NULL after `CanFly`;

ALTER TABLE `gameobject_spawns` ADD COLUMN `phase` int(10) UNSIGNED DEFAULT '1' NOT NULL after `stateNpcLink`;

ALTER TABLE `creature_staticspawns` ADD COLUMN `channel_spell` int(30) DEFAULT '0' NOT NULL after `npc_respawn_link`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `channel_target_sqlid` int(30) DEFAULT '0' NOT NULL after `channel_spell`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `channel_target_sqlid_creature` int(30) DEFAULT '0' NOT NULL after `channel_target_sqlid`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `standstate` int(10) DEFAULT '0' NOT NULL after `channel_target_sqlid_creature`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `mountdisplayid` int(10) UNSIGNED DEFAULT '0' NOT NULL after `standstate`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `slot1item` int(10) UNSIGNED DEFAULT '0' NOT NULL after `mountdisplayid`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `slot2item` int(10) UNSIGNED DEFAULT '0' NOT NULL after `slot1item`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `slot3item` int(10) UNSIGNED DEFAULT '0' NOT NULL after `slot2item`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `CanFly` smallint(3) DEFAULT '0' NOT NULL after `slot3item`;
ALTER TABLE `creature_staticspawns` ADD COLUMN `phase` int(10) UNSIGNED DEFAULT '1' NOT NULL after `CanFly`;

ALTER TABLE `gameobject_staticspawns` ADD COLUMN `phase` int(10) UNSIGNED DEFAULT '1' NOT NULL after `respawnNpcLink`;
