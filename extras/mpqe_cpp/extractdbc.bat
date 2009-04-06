@echo off
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto checkmpqe_cpp
if not exist mpqe.exe goto checkmpqe_cpp
set extractor=mpqe.exe
set extswitches=/v
goto checklib

:checkmpqe_cpp
if not exist mpqe_cpp.exe goto noextractor
set extractor=mpqe_cpp.exe
set extswitches=/v /s

:checklib
if not exist sfmpq.dll goto nolibrary

:begin
echo.
set /p wowdir= World of Warcraft directory (C:\Program Files\World of Warcraft, . if current directory) : 
set /p arcdir= ArcEmu directory (C:\ArcEmu) : 
set /p wowloc= World of Warcraft Language (enUS, enGB, deDE, frFR, koKR, etc.) : 
echo.
echo.
if not exist "%wowdir%\data\%wowloc%\*.mpq" goto nompq
for %%I in ("%wowdir%\data\%wowloc%\*.mpq") do (
   if "%extractor%"=="mpqe.exe" (
	   %extractor% %extswitches% /d "%arcdir%" "%%~fI" *.dbc
	) else (
		echo %%~fI
	   %extractor% %extswitches% /d "%arcdir%\dbc" "%%~fI" *.dbc
	)
)
if "%extractor%"=="mpqe.exe" ren %arcdir%\DBFilesClient dbc
goto exit

:noextractor
echo.
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
   echo mpqe_cpp.exe not found in current directory. Please copy it here and try again.
	goto exit
)
echo mpqe.exe or mpqe_cpp.exe not found in current directory. Please copy one here and try again.
goto exit

:nolibrary
echo.
echo SFmpq.dll not found in current directory. Please copy it here and try again.
goto exit

:nompq
echo.
echo No MPQ files found in %wowdir%\data\%wowloc%
set /p tryagain= Enter directory locations again (Y) or exit (n) : 
if '%tryagain%'=='Y' goto begin
if '%tryagain%'=='y' goto begin
goto exit

:exit
set extswitches=
set tryagain=
set extractor=
set wowdir=
set arcdir=
set wowloc=
pause
