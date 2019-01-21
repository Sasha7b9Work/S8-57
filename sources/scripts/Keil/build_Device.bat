@echo off
echo Compile Device    ... %TIME%

c:\Keil_v5\UV4\UV4 -b ..\..\Device\DeviceOSC.uvprojx -j0 -z -o DeviceOSC.out
if ERRORLEVEL 0 goto Exit

echo ERROR!!! Panel    failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\..\Device\DeviceOSC.out -n1000 -x0 -y0

:Exit
