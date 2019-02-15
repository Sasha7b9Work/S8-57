@echo off

@echo .

@echo %TIME%   Building Win32 version

@echo %TIME%   Building Device ...

MSBuild.exe ..\..\generated\Keil\Device\Device56.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

@echo %TIME%   Building Panel ...

MSBuild.exe ..\..\generated\Keil\Panel\Panel56.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

@echo %TIME%   Building DeviceLoader ...

MSBuild.exe ..\..\generated\Keil\DeviceLoader\DeviceLoader.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
@echo %TIME%   !!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!! Build Failed !!!!!!
goto Exit

:Success
@echo %TIME%   Complete

:Exit

