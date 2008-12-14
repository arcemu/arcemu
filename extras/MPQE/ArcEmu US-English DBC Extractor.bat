REM ## Universal Dbc ExtractorMrHooHa   Version 1.0 ##
COLOR 3

@CLS
@ECHO OFF
REM ## Main Menu
:menu
cls

ECHO.
ECHO.
ECHO		 ####################################
ECHO		 #######        ArcEmu        #######
ECHO		 ######  English DBC Extractor ######
ECHO		 #######      Version 1.0     #######
ECHO		 ####################################
ECHO	      ###########################################
ECHO	      # For Support Go To http://www.arcemu.org # 
ECHO	      ###########################################
ECHO		 Be sure to read DBC readme.txt first
ECHO.
ECHO		 Please type the letter for the option:
ECHO.
ECHO		  1 = Extract US English DBC Files
ECHO.
ECHO.
ECHO		  x - Exit
ECHO.

set /p l=             Enter Letter:

if %l%==1 goto us_english
if %l%==x goto quit
if %l%==X goto quit
goto error

REM ## US English
:us_english
CLS
ECHO.
ECHO.
ECHO [Extraction] Started...
ECHO [Extraction] Extracting DBC Files ...
mpqe /p locale-enUS.MPQ DBFilesClient\*.dbc
md dbc
copy MPQOUT\DBFilesClient dbc

RMDIR /s /q MPQOUT
CLS
ECHO [Extraction] Finished
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

GOTO quit  
:quit