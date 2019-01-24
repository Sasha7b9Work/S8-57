@echo off
echo Load firmware to Device    ... %TIME%

c:\Keil_v5\UV4\UV4 -f ..\..\Device\DeviceOSC.uvprojx -j0 -o ..\scripts\Keil\out.txt
