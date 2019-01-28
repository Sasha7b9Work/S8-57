@echo off
@echo %TIME%   Compile Panel ...

c:\Keil_v5\UV4\UV4 -b ..\..\Panel\PanelOSC.uvprojx -j0 -z -o PanelOSC.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Panel    failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\..\Panel\PanelOSC.out -n1000 -x0 -y0

:Exit
