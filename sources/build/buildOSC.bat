@echo off

@echo Build OSC

echo Compile PanelOSC  ... %TIME%

c:\Keil_v5\UV4\UV4 -b ..\Panel\PanelOSC.uvprojx -j0 -z -o PanelOSC.out
if ERRORLEVEL 1 goto BadPanelOSC

echo Compile Device    ... %TIME%

:FinePanelOSC
c:\Keil_v5\UV4\UV4 -b ..\Device\DeviceOSC.uvprojx -j0 -z -o DeviceOSC.out
if ERRORLEVEL 1 goto BadDeviceOSC

:FineDeviceOSC
goto leave

:BadPanelOSC
echo ERROR!!! PanelOSC failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\Panel\PanelOSC.out -n1000 -x0 -y0
goto FinePanelOSC

:BadDeviceOSC
echo ERROR!!! DeviceOSC failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\Device\DeviceOSC.out -n1000 -x0 -y0
goto FineDeviceOSC

:leave
