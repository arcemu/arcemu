@echo off
cls
echo.
echo                    Welcome to the ArcEmu vmaps extractor and assembler
echo                          For support visit http://www.arcemu.org
echo.
echo You need 2GB of free space in disk, CTRL+C to stop process
echo Hit Enter to start . . .
pause>nul
cls
echo.
echo.
echo.
IF EXIST buildings\dir (ECHO The buildings folder already exist do you want to delete it?
echo If YES hit Enter to continue if no CLOSE the program now! . . .
pause>nul
DEL /S /Q buildings)
vmapextract_v2.exe
cls
echo.
echo.
echo.
IF NOT %ERRORLEVEL% LEQ 1 (echo The vmap extract tool finalized with errors.
echo Hit Enter to continue . . .
pause>nul)
cls
echo.
echo.
echo.
echo Vmaps extracted , now it's time to assemble the vmaps Please patent.
echo Please push enter to start. Notice Do not close this window until process has ended.
pause>nul
md vmaps
vmap_assembler.exe buildings vmaps splitConfig.txt
cls
echo.
echo.
echo.
IF NOT %ERRORLEVEL% LEQ 1 (echo The vmap assembler tool finalized with errors.
echo Hit Enter to continue . . .
pause>nul)
cls
echo.
echo.
echo.
echo Gongrats process done! copy vmaps folder to the ArcEmu server directory
echo Press any key to exit . . .
pause>nul