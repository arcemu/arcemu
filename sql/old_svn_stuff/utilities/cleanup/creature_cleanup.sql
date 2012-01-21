DELETE FROM `creature_proto`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `creature_quest_starter`
WHERE `id` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `creature_quest_finisher`
WHERE `id` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `ai_agents`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `battlemasters`
WHERE `creature_entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `loot_creatures`
WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `loot_skinning`
WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `loot_pickpocketing`
WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `npc_gossip_textid`
WHERE `creatureid` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `npc_monstersay`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `reputation_creature_onkill`
WHERE `creature_id` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `trainer_defs`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `trainer_spells`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `vendor_restrictions`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `vendors`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `zoneguards`
WHERE `horde_entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `zoneguards`
WHERE `alliance_entry` NOT IN ( SELECT `entry` FROM `creature_names` );


DELETE FROM `creature_spawns`
WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );

DELETE FROM `creature_formations`
WHERE `spawn_id` NOT IN ( SELECT `id` FROM `creature_spawns` );

DELETE FROM `creature_waypoints`
WHERE `spawnid` NOT IN ( SELECT `id` FROM `creature_spawns` );

DELETE FROM `creature_timed_emotes`
WHERE `spawnid` NOT IN ( SELECT `id` FROM `creature_spawns` );
