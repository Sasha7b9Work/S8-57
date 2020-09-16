@echo off

if "%1" EQU "" goto HINT
if %1==device  ( call :LOAD Device  & call load.bat loaders & goto EXIT )
if %1==panel   ( call :LOAD Panel   & call load.bat loaders & goto EXIT )
if %1==dloader ( call :LOAD DLoader & goto EXIT )
if %1==all     ( call :LOAD Device & call :LOAD Panel & call :LOAD DLoader & goto EXIT )

goto HINT

:LOAD
    echo Load firmware to %1
    c:\Keil_v5\UV4\UV4 -f ..\..\%1\%1.uvprojx -j0 -o ..\scripts\Keil\out.txt
    exit /b

:HINT

    echo.
    echo Usage:
    echo       load [device^|panel^|dloader^|all]
    echo.
    goto EXIT

:EXIT

