@echo off

del ..\Debug\Device.exe
del DeviceWIN.out

@echo Build Device-WIN ... %TIME%

"c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" ..\FPGA.vcxproj /Out DeviceWIN.out /Build "Debug|x86"

if exist ..\Debug\Device.exe goto Return

if exist "c:\Program Files\Notepad++\notepad++.exe" (
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop DeviceWIN.out -n1000 -x0 -y0
) else (
"c:\Program Files (x86)\Notepad++\notepad++.exe" -multiInst -alwaysOnTop DeviceWIN.out -n1000 -x0 -y0
)

:Return