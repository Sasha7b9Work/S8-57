@echo off
@echo %TIME%   Compile Panel ...

c:\Keil_v5\UV4\UV4 -b ..\..\Panel\PanelOSC.uvprojx -j0 -z -o PanelOSC.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Panel    failed!!!
type ..\..\Panel\PanelOSC.out

:Exit
