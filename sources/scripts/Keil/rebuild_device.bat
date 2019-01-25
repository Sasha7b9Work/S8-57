echo off
@echo Rebuild Device
@echo Cleanup...

c:\Keil_v5\UV4\UV4 -c..\..\Device\DeviceOSC.uvprojx -j0

call build_device.bat
