@echo off

if "%1" EQU "" goto HINT
if %1==load    goto BUILD_LOAD
if %1==device  ( call :BUILD Device  Device ..\..\Device\S8-57.bin & goto EXIT )
if %1==panel   ( call :BUILD Panel   Panel   & goto EXIT )
if %1==dloader ( call :BUILD DLoader DLoader & goto EXIT )
if %1==all     ( call build.bat panel & call build.bat device & call build.bat dloader & goto EXIT)

goto HINT

:BUILD_LOAD
    call build.bat %2
    call load.bat %2
    goto EXIT

:HINT
    echo.
    echo Usage:
    echo       build.bat [device^|panel^|dloader^|all]      - compile
    echo       build.bat load [device^|panel^|dloader^|all] - compile and load
    goto EXIT

:BUILD
    echo Compile Keil %1
    c:\Keil_v5\UV4\UV4 -b ..\..\%2\%2.uvprojx -j0 -z -o %2.out
    set BUILD_STATUS=%ERRORLEVEL%
    if %BUILD_STATUS%==0 goto BUILD_SUCCESS
    echo ERROR!!! %2    failed!!!
    type ..\..\%2\%2.out
    exit /b
:BUILD_SUCCESS
    if "%3" EQU "" exit /b
    echo copy %3 h:\
    copy %3 h:\
    exit /b

:EXIT
