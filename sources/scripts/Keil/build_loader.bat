@echo off
@echo %TIME%   Compile Loader ...

c:\Keil_v5\UV4\UV4 -b ..\..\DeviceLoader\DeviceLoader57.uvprojx -j0 -z -o DeviceLoader.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Loader    failed!!!

type ..\..\DeviceLoader\DeviceLoader.out

:Exit
