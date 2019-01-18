MSBuild.exe ..\..\generated\Device\Device56.vcxproj -t:rebuild
@echo off
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 echo Build success
if not %BUILD_STATUS%==0 echo Build failed

MSBuild.exe ..\..\generated\Panel\Panel56.vcxproj -t:rebuild
