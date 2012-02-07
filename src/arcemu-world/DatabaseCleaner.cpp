/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 #include "StdAfx.h"
 initialiseSingleton(DatabaseCleaner);
 
/* This will need examination every time a table index or key entry is added/changed to prevent unneeded rows. */
 void DatabaseCleaner::Run()
 {
		Log.Notice("DatabaseCleaner", "Stage 1 of 5: Cleaning NonExistant Spells \n
		REQUIRES DBC_SPELL BE EXTRACTED WITH SPELLTOSQL AND BE IN THE WORLD DATABASE");
        CleanNonExistantSpells();
 
        Log.Notice("DatabaseCleaner", "Stage 2 of 5: Cleaning Logon");
        CleanLogon();
 
        Log.Notice("DatabaseCleaner", "Stage 3 of 5: Cleaning World");
        CleanWorld();
 
        Log.Notice("DatabaseCleaner", "Stage 4 of 5: Cleaning Characters");
        CleanCharacters();
 
        Log.Notice("DatabaseCleaner", "Stage 5 of 5: Optimizing Tables");
        Optimize();
 }

 void DatabaseCleaner::CleanNonExistantSpells()
 {
	string worlddatabasename;
	string characterdatabasename;
	bool WorldDBName = Config.MainConfig.GetString( "WorldDatabase", "Name", &worlddatabasename );
	bool CharacterDBName = Config.MainConfig.GetString( "CharacterDatabase", "Name", &characterdatabasename );
	/* needs updating if world config files change */
	WorldDatabase.Query("DELETE FROM `SpellTargetConstraints` WHERE `SpellID` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	WorldDatabase.Query("DELETE FROM `ai_agents` WHERE `spell` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	WorldDatabase.Query("DELETE FROM `ai_threattospellid` WHERE `spell` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	CharacterDatabase.Query("DELETE FROM `playerpetspells` WHERE `spellid` NOT IN ( SELECT `dbc_spell`.`Id` FROM `%s`.`dbc_spell` );", worlddatabasename);
	WorldDatabase.Query("DELETE FROM `playercreateinfo_spells` WHERE `spellid` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	WorldDatabase.Query("DELETE FROM `spell_coef_override` WHERE `id` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	WorldDatabase.Query("DELETE FROM `spell_effects_override` WHERE `spellId` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	WorldDatabase.Query("DELETE FROM `spell_proc` WHERE `spellID` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
	WorldDatabase.Query("DELETE FROM `trainer_spells` WHERE `learn_spell` NOT IN ( SELECT `dbc_spell`.`Id` FROM `dbc_spell` );");
}

void DatabaseCleaner::CleanLogon()
{	 
	string logondatabasename;
	bool LogonDBName = Config.MainConfig.GetString( "MaintenanceExtendedData", "LogonDatabaseName", &logondatabasename );
	string characterdatabasename;
	bool CharacterDBName = Config.MainConfig.GetString( "CharacterDatabase", "Name", &characterdatabasename );
	/*
	* needs updating if world config files change 
	* Reason for a new entry in the config files is that logon's database name doesnt get loaded in the world
	*/
	WorldDatabase.Query("DELETE FROM `%s`.`accounts` WHERE (`lastlogin`<'2010-00-00 00:00:00');", logondatabasename); /* like this is going to trigger ;) */
	WorldDatabase.Query("DELETE FROM `%s`.`characters` WHERE `acct` NOT IN (SELECT `accounts`.`acct` FROM `%s`.`accounts`);", characterdatabasename, logondatabasename);
 }

 void DatabaseCleaner::CleanWorld()
 {	
	/* vendor bugfix, deletes very common ghost spawn in NCDB based databases */
	WorldDatabase.Query("DELETE FROM `creature_spawns` WHERE `entry` = 29238;");
	WorldDatabase.Query("UPDATE `vendors` SET amount = 1 WHERE amount < 1;");
	/*****************************SEPERATOR***********************************/
	WorldDatabase.Query("DELETE FROM `creature_names` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_proto`);");
	WorldDatabase.Query("DELETE FROM `creature_proto` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `creature_quest_finisher` WHERE `quest` NOT IN ( SELECT `entry` FROM `quests` );");
	WorldDatabase.Query("DELETE FROM `creature_quest_starter` WHERE `quest` NOT IN ( SELECT `entry` FROM `quests` );");
	WorldDatabase.Query("DELETE FROM `creature_spawns` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `creature_waypoints` WHERE `spawnid` NOT IN (SELECT `id` FROM `creature_spawns`);");
	WorldDatabase.Query("DELETE FROM `gameobject_spawns` WHERE `entry` NOT IN (SELECT `entry` FROM `gameobject_names`);");
	WorldDatabase.Query("DELETE FROM `loot_creatures` WHERE `itemid` NOT IN (SELECT `entry` FROM `items`);");
	WorldDatabase.Query("DELETE FROM `loot_gameobjects` WHERE `itemid` NOT IN (SELECT `entry` FROM `items`);");
	WorldDatabase.Query("DELETE FROM `npc_gossip_textid` WHERE `creatureid` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `npc_monstersay` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `vendors` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `vendors` WHERE `item` NOT IN (SELECT `entry` FROM `items`);");
	WorldDatabase.Query("DELETE FROM `creature_proto` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_quest_starter` WHERE `id` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_quest_finisher` WHERE `id` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `ai_agents` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `battlemasters` WHERE `creature_entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `loot_creatures` WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `loot_skinning` WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `loot_pickpocketing` WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `npc_gossip_textid` WHERE `creatureid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `npc_monstersay` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `reputation_creature_onkill` WHERE `creature_id` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `trainer_defs` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `trainer_spells` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `vendor_restrictions` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `vendors` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `zoneguards` WHERE `horde_entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `zoneguards` WHERE `alliance_entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_spawns` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_formations` WHERE `spawn_id` NOT IN ( SELECT `id` FROM `creature_spawns` );");
	WorldDatabase.Query("DELETE FROM `creature_waypoints` WHERE `spawnid` NOT IN ( SELECT `id` FROM `creature_spawns` );");
	WorldDatabase.Query("DELETE FROM `creature_timed_emotes` WHERE `spawnid` NOT IN ( SELECT `id` FROM `creature_spawns` );");
	WorldDatabase.Query("DELETE FROM `gameobject_quest_starter` WHERE `id` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `gameobject_quest_finisher` WHERE `id` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `gameobject_quest_item_binding` WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `gameobject_quest_pickup_binding` WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `gameobject_teleports` WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `gameobject_spawns` WHERE `entry` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `loot_gameobjects` WHERE `entryid` NOT IN ( SELECT `entry` FROM `gameobject_names` );");
	WorldDatabase.Query("DELETE FROM `creature_proto` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_quest_starter` WHERE `id` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_quest_finisher` WHERE `id` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `ai_agents` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `battlemasters` WHERE `creature_entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `loot_creatures` WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `loot_skinning` WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `loot_pickpocketing` WHERE `entryid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `npc_gossip_textid` WHERE `creatureid` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `npc_monstersay` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `reputation_creature_onkill` WHERE `creature_id` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `trainer_defs` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `trainer_spells` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `vendor_restrictions` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `vendors` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `zoneguards` WHERE `horde_entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `zoneguards` WHERE `alliance_entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_spawns` WHERE `entry` NOT IN ( SELECT `entry` FROM `creature_names` );");
	WorldDatabase.Query("DELETE FROM `creature_formations` WHERE `spawn_id` NOT IN ( SELECT `id` FROM `creature_spawns` );");
	WorldDatabase.Query("DELETE FROM `creature_waypoints` WHERE `spawnid` NOT IN ( SELECT `id` FROM `creature_spawns` );");
	WorldDatabase.Query("DELETE FROM `creature_timed_emotes` WHERE `spawnid` NOT IN ( SELECT `id` FROM `creature_spawns` );");
	WorldDatabase.Query("DELETE FROM `loot_creatures` WHERE `entryid` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `loot_fishing` WHERE `entryid` NOT IN (SELECT `entry` FROM `items`);");
	WorldDatabase.Query("DELETE FROM `loot_gameobjects` WHERE `entryid` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `loot_items` WHERE `entryid` NOT IN (SELECT `entry` FROM `items`);");
	WorldDatabase.Query("DELETE FROM `loot_pickpocketing` WHERE `entryid` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `loot_skinning` WHERE `entryid` NOT IN (SELECT `entry` FROM `creature_names`);");
	WorldDatabase.Query("DELETE FROM `trainer_defs` WHERE `entry` NOT IN (SELECT `entry` FROM `trainer_spells`);");
	WorldDatabase.Query("DELETE FROM `quest_poi` WHERE `questid` NOT IN (SELECT `entry` FROM `quests`);");
	WorldDatabase.Query("DELETE FROM `quest_poi_points` WHERE `questid` NOT IN (SELECT `entry` FROM `quests`);");
	WorldDatabase.Query("CREATE TABLE `creature_spawns1` AS SELECT * FROM `creature_spawns` WHERE 1 GROUP BY `position_x`,`position_y`;");
	WorldDatabase.Query("CREATE TABLE `creature_spawns2` AS SELECT * FROM `creature_spawns1` WHERE 1 GROUP BY `id`;");
	WorldDatabase.Query("CREATE TABLE `gameobject_spawns1` AS SELECT * FROM `gameobject_spawns` WHERE 1 GROUP BY `position_x`,`position_y`,`position_z`;");
	WorldDatabase.Query("CREATE TABLE `gameobject_spawns2` AS SELECT * FROM `gameobject_spawns1` WHERE 1 GROUP BY `id`;");
	WorldDatabase.Query("CREATE TABLE `creature_proto1` AS SELECT * FROM `creature_proto` WHERE 1 GROUP BY `entry`;");
	WorldDatabase.Query("DROP TABLE IF EXISTS `creature_spawns`,`creature_spawns1`,`gameobject_spawns`,`gameobject_spawns1`,`creature_proto`;");
	WorldDatabase.Query("RENAME TABLE `creature_proto1` TO `creature_proto`;");
	WorldDatabase.Query("RENAME TABLE `creature_spawns2` TO `creature_spawns`;");
	WorldDatabase.Query("RENAME TABLE `gameobject_spawns2` TO `gameobject_spawns`;");
	WorldDatabase.Query("ALTER TABLE `creature_proto` ADD PRIMARY KEY (`entry`);");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` ADD PRIMARY KEY (`id`);");
	WorldDatabase.Query("ALTER TABLE `gameobject_spawns` ADD PRIMARY KEY (`id`);");
	WorldDatabase.Query("DELETE FROM `creature_waypoints` WHERE `spawnid` NOT IN (SELECT `id` FROM `creature_spawns`);");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` ORDER BY `id` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` ADD COLUMN `temp` INTEGER AFTER `phase`;");
	WorldDatabase.Query("UPDATE `creature_spawns` SET `temp`=`id`;");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` DROP `id`;");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` ADD COLUMN `id` int(10) unsigned NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (`id`);");
	WorldDatabase.Query("UPDATE `creature_waypoints`,`creature_spawns` SET `creature_waypoints`.`spawnid` = `creature_spawns`.`id` WHERE `creature_waypoints`.`spawnid` = `creature_spawns`.`temp`;");
	WorldDatabase.Query("UPDATE `creature_formations`,`creature_spawns` SET `creature_formations`.`spawn_id` = `creature_spawns`.`id` WHERE `creature_formations`.`spawn_id` = `creature_spawns`.`temp`;");
	WorldDatabase.Query("UPDATE `creature_formations`,`creature_spawns` SET `creature_formations`.`target_spawn_id` = `creature_spawns`.`id` WHERE `creature_formations`.`target_spawn_id` = `creature_spawns`.`temp`;");
	WorldDatabase.Query("UPDATE `creature_timed_emotes`,`creature_spawns` SET `creature_timed_emotes`.`spawnid` = `creature_spawns`.`id` WHERE `creature_timed_emotes`.`spawnid` = `creature_spawns`.`temp`;");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` DROP `temp`;");
	WorldDatabase.Query("ALTER TABLE `gameobject_spawns` ORDER BY `Entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_spawns` DROP `id`;");
	WorldDatabase.Query("ALTER TABLE `gameobject_spawns` ADD COLUMN `id` int(10) unsigned NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (`id`);");
	WorldDatabase.Query("ALTER TABLE `gameobject_staticspawns` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_teleports` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `graveyards` ORDER BY `id` ASC;");
	WorldDatabase.Query("ALTER TABLE `graveyards` DROP `id`;");
	WorldDatabase.Query("ALTER TABLE `graveyards` ADD COLUMN `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (`id`);");
	WorldDatabase.Query("ALTER TABLE `loot_creatures` ORDER BY `entryid` ASC;");
	WorldDatabase.Query("ALTER TABLE `loot_fishing` ORDER BY `entryid` ASC;");
	WorldDatabase.Query("ALTER TABLE `loot_gameobjects` ORDER BY `entryid` ASC;");
	WorldDatabase.Query("ALTER TABLE `loot_items` ORDER BY `entryid` ASC;");
	WorldDatabase.Query("ALTER TABLE `loot_pickpocketing` ORDER BY `entryid` ASC;");
	WorldDatabase.Query("ALTER TABLE `loot_skinning` ORDER BY `entryid` ASC;");
	WorldDatabase.Query("ALTER TABLE `recall` ORDER BY `name` ASC;");
	WorldDatabase.Query("ALTER TABLE `recall` DROP `id`;");
	WorldDatabase.Query("ALTER TABLE `recall` ADD COLUMN `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (`id`), ADD UNIQUE KEY (`id`);");
	WorldDatabase.Query("ALTER TABLE `SpellTargetConstraints` ORDER BY `SpellID` ASC;");
	WorldDatabase.Query("ALTER TABLE `ai_agents` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `ai_threattospellid` ORDER BY `spell` ASC;");
	WorldDatabase.Query("ALTER TABLE `areatriggers` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `auctionhouse` ORDER BY `id` ASC;");
	WorldDatabase.Query("ALTER TABLE `banned_phrases` ORDER BY `phrase` ASC;");
	WorldDatabase.Query("ALTER TABLE `battlemasters` ORDER BY `creature_entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `clientaddons` ORDER BY `id`;");
	WorldDatabase.Query("ALTER TABLE `command_overrides` ORDER BY `command_name` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_formations` ORDER BY `spawn_id` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_names_localized` ORDER BY `id` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_names` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_proto` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_quest_finisher` ORDER BY `quest` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_quest_starter` ORDER BY `quest` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_spawns` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_staticspawns` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_timed_emotes` ORDER BY `spawnid` ASC;");
	WorldDatabase.Query("ALTER TABLE `creature_waypoints` ORDER BY `spawnid` ASC;");
	WorldDatabase.Query("ALTER TABLE `display_bounding_boxes` ORDER BY `displayid` ASC;");
	WorldDatabase.Query("ALTER TABLE `fishing` ORDER BY `Zone` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_names_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_names` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_quest_finisher` ORDER BY `quest` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_quest_item_binding` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_quest_pickup_binding` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `gameobject_quest_starter` ORDER BY `quest` ASC;");
	WorldDatabase.Query("ALTER TABLE `instance_bosses` ORDER BY `creatureid` ASC;");
	WorldDatabase.Query("ALTER TABLE `item_quest_association` ORDER BY `item` ASC;");
	WorldDatabase.Query("ALTER TABLE `item_randomprop_groups` ORDER BY `entry_id` ASC;");
	WorldDatabase.Query("ALTER TABLE `item_randomsuffix_groups` ORDER BY `entry_id` ASC;");
	WorldDatabase.Query("ALTER TABLE `itemnames` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `itempages_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `itempages` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `itempetfood` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `items_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `items` ORDER BY `entry`;");
	WorldDatabase.Query("ALTER TABLE `map_checkpoint` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `npc_gossip_textid` ORDER BY `creatureid`;");
	WorldDatabase.Query("ALTER TABLE `npc_monstersay` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `npc_text_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `npc_text` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `petdefaultspells` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `playercreateinfo_bars` ORDER BY `race` ASC;");
	WorldDatabase.Query("ALTER TABLE `playercreateinfo_items` ORDER BY `indexid` ASC;");
	WorldDatabase.Query("ALTER TABLE `playercreateinfo_skills` ORDER BY `indexid` ASC;");
	WorldDatabase.Query("ALTER TABLE `playercreateinfo_spells` ORDER BY `indexid` ASC;");
	WorldDatabase.Query("ALTER TABLE `playercreateinfo` ORDER BY `race` ASC;");
	WorldDatabase.Query("ALTER TABLE `professiondiscoveries` ORDER BY `SpellId` ASC;");
	WorldDatabase.Query("ALTER TABLE `quests_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `quests` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `reputation_creature_onkill` ORDER BY `creature_id` ASC;");
	WorldDatabase.Query("ALTER TABLE `reputation_faction_onkill` ORDER BY `faction_id` ASC;");
	WorldDatabase.Query("ALTER TABLE `reputation_instance_onkill` ORDER BY `mapid` ASC;");
	WorldDatabase.Query("ALTER TABLE `spell_coef_override` ORDER BY `id` ASC;");
	WorldDatabase.Query("ALTER TABLE `spell_disable_trainers` ORDER BY `spellid` ASC;");
	WorldDatabase.Query("ALTER TABLE `spell_disable` ORDER BY `spellid` ASC;");
	WorldDatabase.Query("ALTER TABLE `spell_effects_override` ORDER BY `spellId` ASC;");
	WorldDatabase.Query("ALTER TABLE `spell_proc` ORDER BY `spellID` ASC;");
	WorldDatabase.Query("ALTER TABLE `spelloverride` ORDER BY `overrideId` ASC;");
	WorldDatabase.Query("ALTER TABLE `teleport_coords` ORDER BY `id` ASC;");
	WorldDatabase.Query("ALTER TABLE `totemspells` ORDER BY `spell` ASC;");
	WorldDatabase.Query("ALTER TABLE `trainer_defs` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `trainer_spells` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `trainerspelloverride` ORDER BY `spellid` ASC;");
	WorldDatabase.Query("ALTER TABLE `transport_creatures` ORDER BY `transport_entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `transport_data` ORDER BY `name` ASC;");
	WorldDatabase.Query("ALTER TABLE `unit_display_sizes` ORDER BY `DisplayID` ASC;");
	WorldDatabase.Query("ALTER TABLE `vendor_restrictions` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `vendors` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `weather` ORDER BY `zoneId` ASC;");
	WorldDatabase.Query("ALTER TABLE `wordfilter_character_names` ORDER BY `regex_match` ASC;");
	WorldDatabase.Query("ALTER TABLE `wordfilter_chat` ORDER BY `regex_match` ASC;");
	WorldDatabase.Query("ALTER TABLE `worldbroadcast_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `worldbroadcast` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `worldmap_info_localized` ORDER BY `text` ASC;");
	WorldDatabase.Query("ALTER TABLE `worldmap_info` ORDER BY `area_name` ASC;");
	WorldDatabase.Query("ALTER TABLE `worldstring_tables_localized` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `worldstring_tables` ORDER BY `entry` ASC;");
	WorldDatabase.Query("ALTER TABLE `zoneguards` ORDER BY `zone` ASC;");
 }
 
 void DatabaseCleaner::CleanCharacters()
 {
	string worlddatabasename;
	bool WorldDBName = Config.MainConfig.GetString( "WorldDatabase", "Name", &worlddatabasename );
	
	CharacterDatabase.Query("DELETE FROM `questlog` WHERE `player_guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `social_friends` WHERE `character_guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `social_ignores` WHERE `character_guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `gm_tickets` WHERE (`deleted`='1');");
	CharacterDatabase.Query("DELETE FROM `gm_tickets` WHERE `playerGuid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `corpses` WHERE `guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `character_achievement` WHERE `guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `character_achievement_progress` WHERE `guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `arenateams` WHERE `leader` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `auctions` WHERE `owner` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `charters` WHERE `leaderGuid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `tutorials` WHERE `playerId` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `character_achievement` WHERE `guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playeritems` WHERE `ownerguid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playeritems` WHERE `entry` NOT IN (SELECT `items`.`entry` FROM `%s`.`items`);", worlddatabasename);
	CharacterDatabase.Query("DELETE FROM `mailbox` WHERE `player_guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playercooldowns` WHERE `player_guid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playerpets` WHERE `ownerguid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playerpetspells` WHERE `ownerguid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playersummons` WHERE `ownerguid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `playersummonspells` WHERE `ownerguid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `guilds` WHERE `leaderGuid` NOT IN (SELECT `characters`.`guid` FROM `characters`);");
	CharacterDatabase.Query("DELETE FROM `guild_bankitems` WHERE `guildId` NOT IN (SELECT `guilds`.`guildId` FROM `guilds`);");
	CharacterDatabase.Query("DELETE FROM `guild_banklogs` WHERE `guildid` NOT IN (SELECT `guilds`.`guildId` FROM `guilds`);");
	CharacterDatabase.Query("DELETE FROM `guild_banktabs` WHERE `guildId` NOT IN (SELECT `guilds`.`guildId` FROM `guilds`);");
	CharacterDatabase.Query("DELETE FROM `guild_data` WHERE `guildid` NOT IN (SELECT `guilds`.`guildId` FROM `guilds`);");
	CharacterDatabase.Query("DELETE FROM `guild_logs` WHERE `guildid` NOT IN (SELECT `guilds`.`guildId` FROM `guilds`);");
	CharacterDatabase.Query("DELETE FROM `guild_ranks` WHERE `guildId` NOT IN (SELECT `guilds`.`guildId` FROM `guilds`);");
}

void DatabaseCleaner::Optimize()
{
	string logondatabasename;
	bool LogonDBName = Config.MainConfig.GetString( "MaintenanceExtendedData", "LogonDatabaseName", &logondatabasename );
	
	WorldDatabase.Query("OPTIMIZE TABLE `SpellTargetConstraints`;");
	WorldDatabase.Query("OPTIMIZE TABLE `ai_agents`;");
	WorldDatabase.Query("OPTIMIZE TABLE `ai_threattospellid`;");
	WorldDatabase.Query("OPTIMIZE TABLE `areatriggers`;");
	WorldDatabase.Query("OPTIMIZE TABLE `auctionhouse`;");
	WorldDatabase.Query("OPTIMIZE TABLE `banned_phrases`;");
	WorldDatabase.Query("OPTIMIZE TABLE `battlemasters`;");
	WorldDatabase.Query("OPTIMIZE TABLE `clientaddons`;");
	WorldDatabase.Query("OPTIMIZE TABLE `command_overrides`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_formations`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_names_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_names`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_proto`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_quest_finisher`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_quest_starter`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_spawns`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_staticspawns`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_timed_emotes`;");
	WorldDatabase.Query("OPTIMIZE TABLE `creature_waypoints`;");
	WorldDatabase.Query("OPTIMIZE TABLE `display_bounding_boxes`;");
	WorldDatabase.Query("OPTIMIZE TABLE `fishing`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_names_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_names`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_quest_finisher`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_quest_item_binding`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_quest_pickup_binding`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_quest_starter`;");
	WorldDatabase.Query("OPTIMIZE TABLE `instance_bosses`;");
	WorldDatabase.Query("OPTIMIZE TABLE `item_quest_association`;");
	WorldDatabase.Query("OPTIMIZE TABLE `item_randomprop_groups`;");
	WorldDatabase.Query("OPTIMIZE TABLE `item_randomsuffix_groups`;");
	WorldDatabase.Query("OPTIMIZE TABLE `itemnames`;");
	WorldDatabase.Query("OPTIMIZE TABLE `itempages_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `itempages`;");
	WorldDatabase.Query("OPTIMIZE TABLE `itempetfood`;");
	WorldDatabase.Query("OPTIMIZE TABLE `items_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `items`;");
	WorldDatabase.Query("OPTIMIZE TABLE `map_checkpoint`;");
	WorldDatabase.Query("OPTIMIZE TABLE `npc_gossip_textid`;");
	WorldDatabase.Query("OPTIMIZE TABLE `npc_monstersay`;");
	WorldDatabase.Query("OPTIMIZE TABLE `npc_text_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `npc_text`;");
	WorldDatabase.Query("OPTIMIZE TABLE `petdefaultspells`;");
	WorldDatabase.Query("OPTIMIZE TABLE `playercreateinfo_bars`;");
	WorldDatabase.Query("OPTIMIZE TABLE `playercreateinfo_items`;");
	WorldDatabase.Query("OPTIMIZE TABLE `playercreateinfo_skills`;");
	WorldDatabase.Query("OPTIMIZE TABLE `playercreateinfo_spells`;");
	WorldDatabase.Query("OPTIMIZE TABLE `playercreateinfo`;");
	WorldDatabase.Query("OPTIMIZE TABLE `professiondiscoveries`;");
	WorldDatabase.Query("OPTIMIZE TABLE `quests_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `quests`;");
	WorldDatabase.Query("OPTIMIZE TABLE `reputation_creature_onkill`;");
	WorldDatabase.Query("OPTIMIZE TABLE `reputation_faction_onkill`;");
	WorldDatabase.Query("OPTIMIZE TABLE `reputation_instance_onkill`;");
	WorldDatabase.Query("OPTIMIZE TABLE `spell_coef_override`;");
	WorldDatabase.Query("OPTIMIZE TABLE `spell_disable_trainers`;");
	WorldDatabase.Query("OPTIMIZE TABLE `spell_disable`;");
	WorldDatabase.Query("OPTIMIZE TABLE `spell_effects_override`;");
	WorldDatabase.Query("OPTIMIZE TABLE `spell_proc`;");
	WorldDatabase.Query("OPTIMIZE TABLE `spelloverride`;");
	WorldDatabase.Query("OPTIMIZE TABLE `teleport_coords`;");
	WorldDatabase.Query("OPTIMIZE TABLE `totemspells`;");
	WorldDatabase.Query("OPTIMIZE TABLE `trainer_defs`;");
	WorldDatabase.Query("OPTIMIZE TABLE `trainer_spells`;");
	WorldDatabase.Query("OPTIMIZE TABLE `trainerspelloverride`;");
	WorldDatabase.Query("OPTIMIZE TABLE `transport_creatures`;");
	WorldDatabase.Query("OPTIMIZE TABLE `transport_data`;");
	WorldDatabase.Query("OPTIMIZE TABLE `unit_display_sizes`;");
	WorldDatabase.Query("OPTIMIZE TABLE `vendor_restrictions`;");
	WorldDatabase.Query("OPTIMIZE TABLE `vendors`;");
	WorldDatabase.Query("OPTIMIZE TABLE `weather`;");
	WorldDatabase.Query("OPTIMIZE TABLE `wordfilter_character_names`;");
	WorldDatabase.Query("OPTIMIZE TABLE `wordfilter_chat`;");
	WorldDatabase.Query("OPTIMIZE TABLE `worldbroadcast_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `worldbroadcast`;");
	WorldDatabase.Query("OPTIMIZE TABLE `worldmap_info_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `worldmap_info`;");
	WorldDatabase.Query("OPTIMIZE TABLE `worldstring_tables_localized`;");
	WorldDatabase.Query("OPTIMIZE TABLE `worldstring_tables`;");
	WorldDatabase.Query("OPTIMIZE TABLE `zoneguards`;");
	WorldDatabase.Query("OPTIMIZE TABLE `recall`;");
	WorldDatabase.Query("OPTIMIZE TABLE `loot_skinning`;");
	WorldDatabase.Query("OPTIMIZE TABLE `loot_pickpocketing`;");
	WorldDatabase.Query("OPTIMIZE TABLE `loot_items`;");
	WorldDatabase.Query("OPTIMIZE TABLE `loot_gameobjects`;");
	WorldDatabase.Query("OPTIMIZE TABLE `loot_fishing`;");
	WorldDatabase.Query("OPTIMIZE TABLE `loot_creatures`;");
	WorldDatabase.Query("OPTIMIZE TABLE `graveyards`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_teleports`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_staticspawns`;");
	WorldDatabase.Query("OPTIMIZE TABLE `gameobject_spawns`;");
	WorldDatabase.Query("OPTIMIZE TABLE `%s`.`accounts`;", logondatabasename);
	WorldDatabase.Query("OPTIMIZE TABLE `%s`.`ipbans`;", logondatabasename);
	CharacterDatabase.Query("OPTIMIZE TABLE `account_data`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `account_forced_permissions`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `arcemu_db_version`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `arenateams`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `auctions`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `banned_names`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `character_achievement`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `character_achievement_progress`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `characters`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `characters_insert_queue`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `charters`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `clientaddons`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `command_overrides`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `corpses`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `equipmentsets`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `gm_tickets`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `groups`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guild_bankitems`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guild_banklogs`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guild_banktabs`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guild_data`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guild_logs`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guild_ranks`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `guilds`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `instanceids`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `instances`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `mailbox`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `mailbox_insert_queue`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playerbugreports`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playercooldowns`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playerdeletedspells`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playeritems`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playeritems_insert_queue`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playerpets`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playerpetspells`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playerskills`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playerspells`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playersummons`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `playersummonspells`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `questlog`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `server_settings`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `social_friends`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `social_ignores`;");
	CharacterDatabase.Query("OPTIMIZE TABLE `tutorials`;");
}