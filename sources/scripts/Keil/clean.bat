@echo off

if "%1" EQU "" goto HINT
if %1==device  ( call :CLEAN Device  & goto EXIT )
if %1==panel   ( call :CLEAN Panel   & goto EXIT )
if %1==dloader ( call :CLEAN DLoader & goto EXIT )
if %1==all     ( call clean.bat device & call clean.bat panel & call clean.bat dloader & goto EXIT )

goto HINT

:CLEAN
    echo Clean %1
    c:\Keil_v5\UV4\UV4 -c..\..\%1\%1.uvprojx -j0
    exit /b

:HINT

    echo.
    echo Usage:
    echo       clean [panel^|device^|dloader^|all]
    echo.
    goto EXIT

:EXIT
