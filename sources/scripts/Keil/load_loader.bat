@echo off
@echo %TIME%   Load firmware to Loader ...

c:\Keil_v5\UV4\UV4 -f ..\..\DeviceLoader\DeviceLoader57.uvprojx -j0 -o ..\scripts\Keil\out.txt
