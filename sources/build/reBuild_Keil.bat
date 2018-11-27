@echo off

@echo Rebuild OSC

c:\Keil_v5\UV4\UV4 -c ..\Panel\PanelOSC.uvprojx -j0
c:\Keil_v5\UV4\UV4 -c ..\Device\DeviceOSC.uvprojx -j0

call __build_Keil.bat
