@ECHO off
REM ############################################################################
REM # Credits 
REM # Base Script by NCDB http://www.nc-db.info
REM # Modified by MrHooHa ArcEmu http://www.arcemu.org
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
REM # chr    - Characture Database
REM # wdb    - World Database
REM #########################################
REM # Logon Database
set user=changeme
set pass=changeme
set logon=changeme
REM #########################################
REM # Characture Database
set user=changeme
set pass=changeme
set chr= changeme
REM #########################################
REM # World Database
SET user=changeme
set pass=changeme
set wdb=changeme
REM ############################################################################
REM #
REM #    A D V A N C E D   U S E R   C O N F I G U R A T I O N   A R E A
REM #
REM ############################################################################
set server=localhost
set port=3306
REM ############################################################################
REM #
REM #     D O   N O T   M O D I F Y   B E Y O N D   T H I S   P O I N T
REM #
REM ############################################################################
if %user% == CHANGEME GOTO error2
if %pass% == CHANGEME GOTO error2
:menu
cls
ECHO.
ECHO.
Echo	 ####################################################
ECHO	 #    Base Script by NCDB http://www.nc-db.info     #
ECHO	 # Modified by MrHooHa ArcEmu http://www.arcemu.org #
ECHO	 ####################################################
ECHO		 ####################################
ECHO		 #######    ArcEmu Database   #######
ECHO		 ######        Revision        ######
ECHO		 #######      Update Tool     #######
ECHO		 ####################################
ECHO.
ECHO		 Please type the letter for the option:
ECHO.
ECHO		  W = Install World Database updates
ECHO		  C = Install Charature updates
ECHO		  L = Install Logon updates
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
if %l%==x goto quit
if %l%==X goto quit
goto error

:world
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] AcrEmu World Updates ...
ECHO [Importing] 104_rituals_objects.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\104_rituals_objects.sql
ECHO [Importing] 185_quest_rewards.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\185_quest_rewards.sql
ECHO [Importing] 456_ai_agents.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\456_ai_agents.sql
ECHO [Importing] 505_worldmap_info.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\505_worldmap_info.sql
ECHO [Importing] 581_items_extendedcost.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\581_items_extendedcost.sql
ECHO [Importing] 616_quests_rewrep.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\616_quests_rewrep.sql
ECHO [Importing] 633_creature_spawns_bytes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\633_creature_spawns_bytes.sql
ECHO [Importing] 672_instance-system.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\672_instance-system.sql
ECHO [Importing] 678_ai_threattospell.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\678_ai_threattospell.sql
ECHO [Importing] 744_spellfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\744_spellfixes.sql
ECHO [Importing] 762_instance_bosses.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\762_instance_bosses.sql
ECHO [Importing] 834_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\834_creature_proto.sql
ECHO [Importing] 1174_worldbroadcast.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1174_worldbroadcast.sql
ECHO [Importing] 1174_worldbroadcast_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1174_worldbroadcast_localized.sql
ECHO [Importing] 1174_worldmap_info_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1174_worldmap_info_localized.sql
ECHO [Importing] 1174_worldstring_tables.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1174_worldstring_tables.sql
ECHO [Importing] 1174_worldstring_tables_localized.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1174_worldstring_tables_localized.sql
ECHO [Importing] 1497_bugfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1497_bugfixes.sql
ECHO [Importing] 1564_items.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1564_items.sql
ECHO [Importing] 1564_removed_spells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1564_removed_spells.sql
ECHO [Importing] 1576_wotlk_worldstring_table.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1576_wotlk_worldstring_table.sql
ECHO [Importing] 1587_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1587_quests.sql
ECHO [Importing] 1658_playercreateinfo_intros.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1658_playercreateinfo_intros.sql
ECHO [Importing] 1694_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1694_quests.sql
ECHO [Importing] 1707_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1707_creature_proto.sql
ECHO [Importing] 1729_creature_proto.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1729_creature_proto.sql
ECHO [Importing] 1735_quests.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\1735_quests.sql
ECHO [Importing] spellfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %wdb% < world_updates\spellfixes.sql
ECHO [Importing] Finished
ECHO.
PAUSE    
GOTO menu

:chr
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu Characture Updates ...
ECHO [Importing] 147_pet_untrain_cost.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\147_pet_untrain_cost.sql
ECHO [Importing] 261_gm_tickets.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\261_gm_tickets.sql
ECHO [Importing] 566_gm_tickets.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\566_gm_tickets.sql
ECHO [Importing] 624_guild_leader_rights.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\624_guild_leader_rights.sql
ECHO [Importing] 711_instance-system.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\711_instance-system.sql
ECHO [Importing] 912_pvp_titles.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\912_pvp_titles.sql
ECHO [Importing] 943_playerpets_spellid.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\943_playerpets_spellid.sql
ECHO [Importing] 1033_playersummons.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\1033_playersummons.sql
ECHO [Importing] 1497_bugfixes.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\1497_bugfixes.sql
ECHO [Importing] 1564_character_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\1564_character_achievements.sql
ECHO [Importing] 1564_character_glyphs.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\1564_character_glyphs.sql
ECHO [Importing] 1564_removed_spells.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\1564_removed_spells.sql
ECHO [Importing] 1653_achievements.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %chr% < character_updates\1653_achievements.sql
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
ECHO [Importing] ArcEmu Characture Updates ...
ECHO [Importing] 1066_add_banreasons.sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < logon_updates\1066_add_banreasons.sql
ECHO [Importing] correction_to_ipbans(allrevs).sql
mysql -h %server% --user=%user% --password=%pass% --port=%port% %logon% < logon_updates\correction_to_ipbans(allrevs).sql
ECHO [Importing] Finished
ECHO  Update Success.
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
ECHO [FAILURE] You did not change the proper directives in this file.
ECHO [FAILURE] Please edit this script and fill in the proper MYSQL Information.
ECHO [FAILURE] When the information is correct: Please Try Again.
PAUSE    
GOTO quit  
:quit