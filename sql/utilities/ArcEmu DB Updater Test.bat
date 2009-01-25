@ECHO off
REM ############################################################################
REM # Credits 
REM # Base Script by NCDB http://www.nc-db.info
REM # Modified by MrHooHa and Maven whoami ArcEmu http://www.arcemu.org
REM ############################################################################
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
IF '%imp%'=='y' GOTO menu
IF '%imp%'=='Y' GOTO menu
IF '%imp%'=='n' GOTO dbinfo
IF '%imp%'=='N' GOTO dbinfo
goto error2

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
set next=menu
if '%l%'=='w' goto world
if '%l%'=='W' goto world
if '%l%'=='c' goto chr
if '%l%'=='C' goto chr
if '%l%'=='l' goto logon
if '%l%'=='L' goto logon
if '%l%'=='A' goto installall
if '%l%'=='a' goto installall
if '%l%'=='x' goto quit
if '%l%'=='X' goto quit
if '%l%'=='D' goto dbinfo
if '%l%'=='d' goto dbinfo
goto error

:world
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu World Updates ...
for %%W in (..\world_updates\*.sql) do (
   ECHO [Importing] %%~nxW
	mysql -h %server% --user=%user% --password=%pass% --port=%port% --force %wdb% < "%%~fW"
)
ECHO [Importing] World Updates Finished
ECHO.
PAUSE    
GOTO %next%

:chr
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu Character Updates ...
for %%C in (..\character_updates\*.sql) do (
	ECHO [Importing] %%~nxC
	mysql -h %server% --user=%user% --password=%pass% --port=%port% --force %chr% < "%%~fC"
)
ECHO [Importing] Character Updates Finished
ECHO  Update Success.
ECHO.
PAUSE    
GOTO %next%

:logon
CLS
ECHO.
ECHO.
ECHO [Importing] Started...
ECHO [Importing] ArcEmu Logon Updates ...
for %%L in (..\logon_updates\*.sql) do (
	ECHO [Importing] %%~nxL
	mysql -h %server% --user=%user% --password=%pass% --port=%port% --force %logon% < "%%~fL"
)
ECHO [Importing] Logon Updates Finished
ECHO  Update Success.
ECHO.
PAUSE    
GOTO %next%

:installall
set next=all2
goto world
:all2
set next=all3
goto chr
:all3
set next=menu
goto logon

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
rem in case we're in a command prompt, unset the variables to free up environment (and clear out sql login info!)
set next=
set server=
set port=
set user=
set pass=
set logon=
set chr=
set wdb=
set l=
set imp=
