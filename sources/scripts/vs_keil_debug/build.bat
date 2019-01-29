@echo off

@echo .
@echo %TIME%   Building Device ...
@echo .

MSBuild.exe ..\..\generated\Device\Device56.vcxproj -clp:ErrorsOnly
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

@echo %TIME%   Building Panel ...

MSBuild.exe ..\..\generated\Panel\Panel56.vcxproj -clp:ErrorsOnly
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
echo .
echo .
echo .
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo .
echo .
echo .
goto Exit

:Success
echo .
echo .
echo .
echo                          Build success
echo .
echo .
echo .

:Exit

