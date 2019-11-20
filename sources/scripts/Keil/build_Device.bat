@echo off
@echo %TIME%   Compile Device ...

c:\Keil_v5\UV4\UV4 -b ..\..\Device\Device57.uvprojx -j0 -z -o Device57.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Device    failed!!!
rem "c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop ..\..\Device\Device57.out -n1000 -x0 -y0
type ..\..\Device\Device57.out

:Exit

copy ..\..\Device\S8-57.bin h:\
copy ..\..\Device\S8-57.bin i:\
