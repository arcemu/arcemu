@echo off
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" goto checkmpqe_cpp
if not exist mpqe.exe goto checkmpqe_cpp
set extractor=mpqe_cpp.exe
set extswitches=/v
goto checklib

:checkmpqe_cpp
if not exist mpqe_cpp.exe goto noextractor
set extractor=mpqe_cpp.exe
set extswitches=/v /s

:checklib
if not exist sfmpq.dll goto nolibrary

:begin
%extractor% %extswitches% /d "dbc" "*.mpq" *.dbc
goto exit

:noextractor
echo.
if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
   echo mpqe_cpp.exe not found in current directory. Please copy it here and try again.
	goto exit
)
echo mpqe_cpp.exe not found in current directory. Please copy one here and try again.
goto exit

:nolibrary
echo.
echo SFmpq.dll not found in current directory. Please copy it here and try again.
goto exit

:nompq
echo.
echo No MPQ files found
goto exit

:exit
set extswitches=
set tryagain=
set extractor=
set wowdir=
set arcdir=
set wowloc=
pause
