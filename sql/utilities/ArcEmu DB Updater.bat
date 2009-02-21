@ECHO off
REM ############################################################################
REM # Credits 
REM # Base Script by NCDB http://www.nc-db.info
REM # Modified by MrHooHa and Maven ArcEmu http://www.arcemu.org
REM ############################################################################
REM # 
REM #      B A S I C   U S E R   C O N F I G U R A T I O N   A R E A
REM # 
REM ############################################################################
REM #########################################
REM # server - MySql Host
REM # user   - MySql Username
REM # pass   - MySql Password
REM # logon  - Logon Database
REM # chr    - Character Database
REM # wdb    - World Database
REM #########################################
REM MySQL Username and Password
:dbinfo
CLS
ECHO.
ECHO Welcome to the ArcEmu DB Updater! Please enter your MySQL Info...
ECHO.
set /p server= MySQL Server Address (e.g. localhost): 
set /p port= MySQL Server Port (e.g. 3306): 
ECHO.
set /p user= MySQL Username: 
set /p pass= MySQL Password: 
REM # Logon Database
ECHO.
set /p logon= Logon Database (Accounts): 
REM # Character Database
set /p chr= Character Database: 
REM # World Database
set /p wdb= World Database: 
ECHO.
ECHO.
ECHO.
set /p imp=Is your MySQL information correct [(Y)es / (N)o]? 
ECHO.
ECHO       (Y)es / (N)o
ECHO.
IF %imp%==y GOTO menu
IF %imp%==Y GOTO menu
IF %imp%==n GOTO dbinfo
IF %imp%==N GOTO dbinfo
IF %imp%==* GOTO error2
goto error2

REM ############################################################################
REM #
REM #     D O   N O T   M O D I F Y   B E Y O N D   T H I S   P O I N T
REM #
REM ############################################################################
:menu
cls
ECHO.
ECHO.
ECHO	 ##############################################################
ECHO	 #    	   Base Script by NCDB http://www.nc-db.info          #
ECHO	 # Modified by MrHooHa and Maven ArcEmu http://www.arcemu.org #
ECHO	 ##############################################################
ECHO		 ####################################
ECHO		 #######    ArcEmu Database   #######
ECHO		 ######        Revision        ######
ECHO		 #######      Update Tool     #######
ECHO		 ####################################
ECHO.
ECHO		 Please type the letter for the option:
ECHO.
ECHO		  W = Install World Database updates
ECHO		  C = Install Character updates
ECHO		  L = Install Logon updates
ECHO		  A = Install All updates
ECHO.
ECHO		  D = Reconfirm MySQL Information
ECHO.
ECHO.
ECHO		  x - Exit
ECHO.
set /p l=             Enter Letter:
if %l%==* goto error
if %l%==w goto world
if %l%==W goto world
if %l%==c goto chr
if %l%==C goto chr
if %l%==l goto logon
if %l%==L goto logon
if %l%==A goto installall
if %l%==a goto installall
if %l%==x goto quit
if %l%==X goto quit
if %l%==D goto dbinfo
if %l%==d goto dbinfo
goto error

:world
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu World Updates ...
ECHO [Importing] 104_rituals_objects.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\104_rituals_objects.sql
ECHO [Importing] 185_quest_rewards.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\185_quest_rewards.sql
ECHO [Importing] 456_ai_agents.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\456_ai_agents.sql
ECHO [Importing] 505_worldmap_info.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\505_worldmap_info.sql
ECHO [Importing] 581_items_extendedcost.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\581_items_extendedcost.sql
ECHO [Importing] 616_quests_rewrep.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\616_quests_rewrep.sql
ECHO [Importing] 633_creature_spawns_bytes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\633_creature_spawns_bytes.sql
ECHO [Importing] 672_instance-system.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\672_instance-system.sql
ECHO [Importing] 678_ai_threattospell.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\678_ai_threattospell.sql
ECHO [Importing] 744_spellfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\744_spellfixes.sql
ECHO [Importing] 762_instance_bosses.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\762_instance_bosses.sql
ECHO [Importing] 834_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\834_creature_proto.sql
ECHO [Importing] 1174_worldbroadcast.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldbroadcast.sql
ECHO [Importing] 1174_worldbroadcast_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldbroadcast_localized.sql
ECHO [Importing] 1174_worldmap_info_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldmap_info_localized.sql
ECHO [Importing] 1174_worldstring_tables.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldstring_tables.sql
ECHO [Importing] 1174_worldstring_tables_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldstring_tables_localized.sql
ECHO [Importing] 1497_bugfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1497_bugfixes.sql
ECHO [Importing] 1564_items.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1564_items.sql
ECHO [Importing] 1564_removed_spells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1564_removed_spells.sql
ECHO [Importing] 1576_wotlk_worldstring_table.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1576_wotlk_worldstring_table.sql
ECHO [Importing] 1587_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1587_quests.sql
ECHO [Importing] 1658_playercreateinfo_intros.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1658_playercreateinfo_intros.sql
ECHO [Importing] 1694_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1694_quests.sql
ECHO [Importing] 1707_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1707_creature_proto.sql
ECHO [Importing] 1729_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1729_creature_proto.sql
ECHO [Importing] 1735_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1735_quests.sql
ECHO [Importing] 1746_spawns.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1746_spawns.sql
ECHO [Importing] 1797_transport_data.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1797_transport_data.sql
ECHO [Importing] 1860_loot.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1860_loot.sql
ECHO [Importing] 1902_trainers.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1902_trainers.sql
ECHO [Importing] 2118_items.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2118_items.sql
ECHO [Importing] 2157_worldmap_info.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2157_worldmap_info.sql
ECHO [Importing] 2177_sota_battlemaster_gossip_text.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2177_sota_battlemaster_gossip_text.sql
ECHO [Importing] 2199_gosandquests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2199_gosandquests.sql
ECHO [Importing] 2227_totems.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\SpellFixes\2227_totems.sql
ECHO [Importing] 2243_totemspells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2243_totemspells.sql
ECHO [Importing] ArcScript_full.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\ArcScripts\ArcScript_full.sql
ECHO [Importing] spellfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\spellfixes.sql
ECHO [Importing] Finished
ECHO.
PAUSE    
GOTO menu

:chr
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu Character Updates ...
ECHO [Importing] 147_pet_untrain_cost.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\147_pet_untrain_cost.sql
ECHO [Importing] 261_gm_tickets.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\261_gm_tickets.sql
ECHO [Importing] 566_gm_tickets.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\566_gm_tickets.sql
ECHO [Importing] 624_guild_leader_rights.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\624_guild_leader_rights.sql
ECHO [Importing] 711_instance-system.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\711_instance-system.sql
ECHO [Importing] 912_pvp_titles.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\912_pvp_titles.sql
ECHO [Importing] 943_playerpets_spellid.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\943_playerpets_spellid.sql
ECHO [Importing] 1033_playersummons.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1033_playersummons.sql
ECHO [Importing] 1497_bugfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1497_bugfixes.sql
ECHO [Importing] 1564_character_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1564_character_achievements.sql
ECHO [Importing] 1564_character_glyphs.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1564_character_glyphs.sql
ECHO [Importing] 1564_removed_spells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1564_removed_spells.sql
ECHO [Importing] 1653_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1653_achievements.sql
ECHO [Importing] 1835_playerpets_loyalty.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1835_playerpets_loyalty.sql
ECHO [Importing] 1911_playertitles.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1911_playertitles.sql
ECHO [Importing] 1996_character_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1996_character_achievements.sql
ECHO [Importing] 2005_guildbank_tabinfo.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\2005_guildbank_tabinfo.sql
ECHO [Importing] 2084_character_achievement_progress.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\2084_character_achievement_progress.sql
ECHO [Importing] Finished
ECHO  Update Success.
ECHO.
PAUSE    
GOTO menu

:logon
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu Logon Updates ...
ECHO [Importing] 1066_add_banreasons.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < ..\logon_updates\1066_add_banreasons.sql
ECHO [Importing] correction_to_ipbans(allrevs).sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < ..\logon_updates\correction_to_ipbans(allrevs).sql
ECHO [Importing] 1842_Account_Flags.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < ..\logon_updates\1842_Account_Flags.sql
ECHO [Importing] Finished
ECHO  Update Success.
ECHO.
PAUSE    
GOTO menu

:installall
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu World Updates ...
ECHO [Importing] 104_rituals_objects.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\104_rituals_objects.sql
ECHO [Importing] 185_quest_rewards.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\185_quest_rewards.sql
ECHO [Importing] 456_ai_agents.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\456_ai_agents.sql
ECHO [Importing] 505_worldmap_info.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\505_worldmap_info.sql
ECHO [Importing] 581_items_extendedcost.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\581_items_extendedcost.sql
ECHO [Importing] 616_quests_rewrep.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\616_quests_rewrep.sql
ECHO [Importing] 633_creature_spawns_bytes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\633_creature_spawns_bytes.sql
ECHO [Importing] 672_instance-system.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\672_instance-system.sql
ECHO [Importing] 678_ai_threattospell.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\678_ai_threattospell.sql
ECHO [Importing] 744_spellfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\744_spellfixes.sql
ECHO [Importing] 762_instance_bosses.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\762_instance_bosses.sql
ECHO [Importing] 834_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\834_creature_proto.sql
ECHO [Importing] 1174_worldbroadcast.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldbroadcast.sql
ECHO [Importing] 1174_worldbroadcast_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldbroadcast_localized.sql
ECHO [Importing] 1174_worldmap_info_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldmap_info_localized.sql
ECHO [Importing] 1174_worldstring_tables.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldstring_tables.sql
ECHO [Importing] 1174_worldstring_tables_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1174_worldstring_tables_localized.sql
ECHO [Importing] 1497_bugfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1497_bugfixes.sql
ECHO [Importing] 1564_items.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1564_items.sql
ECHO [Importing] 1564_removed_spells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1564_removed_spells.sql
ECHO [Importing] 1576_wotlk_worldstring_table.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1576_wotlk_worldstring_table.sql
ECHO [Importing] 1587_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1587_quests.sql
ECHO [Importing] 1658_playercreateinfo_intros.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1658_playercreateinfo_intros.sql
ECHO [Importing] 1694_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1694_quests.sql
ECHO [Importing] 1707_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1707_creature_proto.sql
ECHO [Importing] 1729_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1729_creature_proto.sql
ECHO [Importing] 1735_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1735_quests.sql
ECHO [Importing] 1746_spawns.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1746_spawns.sql
ECHO [Importing] 1797_transport_data.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1797_transport_data.sql
ECHO [Importing] 1860_loot.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1860_loot.sql
ECHO [Importing] 1902_trainers.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\1902_trainers.sql
ECHO [Importing] 2118_items.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2118_items.sql
ECHO [Importing] 2157_worldmap_info.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2157_worldmap_info.sql
ECHO [Importing] 2177_sota_battlemaster_gossip_text.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2177_sota_battlemaster_gossip_text.sql
ECHO [Importing] 2199_gosandquests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2199_gosandquests.sql
ECHO [Importing] 2227_totems.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\SpellFixes\2227_totems.sql
ECHO [Importing] 2243_totemspells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\2243_totemspells.sql
ECHO [Importing] ArcScript_full.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\ArcScripts\ArcScript_full.sql
ECHO [Importing] spellfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% --force < ..\world_updates\spellfixes.sql

ECHO [Importing] World Updates Finished
ECHO.
ECHO.
REM ####################################################################################################
REM End World, start Character
ECHO [Importing] ArcEmu Character Updates ...
ECHO [Importing] 147_pet_untrain_cost.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\147_pet_untrain_cost.sql
ECHO [Importing] 261_gm_tickets.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\261_gm_tickets.sql
ECHO [Importing] 566_gm_tickets.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\566_gm_tickets.sql
ECHO [Importing] 624_guild_leader_rights.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\624_guild_leader_rights.sql
ECHO [Importing] 711_instance-system.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\711_instance-system.sql
ECHO [Importing] 912_pvp_titles.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\912_pvp_titles.sql
ECHO [Importing] 943_playerpets_spellid.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\943_playerpets_spellid.sql
ECHO [Importing] 1033_playersummons.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1033_playersummons.sql
ECHO [Importing] 1497_bugfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1497_bugfixes.sql
ECHO [Importing] 1564_character_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1564_character_achievements.sql
ECHO [Importing] 1564_character_glyphs.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1564_character_glyphs.sql
ECHO [Importing] 1564_removed_spells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1564_removed_spells.sql
ECHO [Importing] 1653_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1653_achievements.sql
ECHO [Importing] 1835_playerpets_loyalty.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1835_playerpets_loyalty.sql
ECHO [Importing] 1911_playertitles.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1911_playertitles.sql
ECHO [Importing] 1996_character_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\1996_character_achievements.sql
ECHO [Importing] 2005_guildbank_tabinfo.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\2005_guildbank_tabinfo.sql
ECHO [Importing] 2084_character_achievement_progress.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < ..\character_updates\2084_character_achievement_progress.sql
ECHO [Importing] Character Updates Finished
ECHO.
ECHO.
REM ####################################################################################################
REM End Character, start logon.
ECHO [Importing] ArcEmu Logon Updates ...
ECHO [Importing] 1066_add_banreasons.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < ..\logon_updates\1066_add_banreasons.sql
ECHO [Importing] correction_to_ipbans(allrevs).sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < ..\logon_updates\correction_to_ipbans(allrevs).sql
ECHO [Importing] 1842_Account_Flags.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < ..\logon_updates\1842_Account_Flags.sql
ECHO [Importing] Logon Updates Finished
ECHO  Update Success.
ECHO [Importing] All Updates Finished
ECHO.
PAUSE    
GOTO menu

:error
CLS
ECHO.
ECHO.
ECHO [ERROR] An error has occured, you will be directed back to the
ECHO [ERROR] main menu.
PAUSE    
GOTO menu

:error2
CLS
ECHO.
ECHO.
ECHO [ERROR] An error has occured, you will be directed back to the
ECHO [ERROR] login screen.
PAUSE    
GOTO dbinfo

:quit
rem in case we're in a command prompt, unset the variables to free up environment
set server=
set port=
set user=
set pass=
set logon=
set chr=
set wdb=
set l=
set imp=