@echo off

del ..\Debug\Device.lib
del DeviceSTM32.out

@echo Build Device-STM32 ... %TIME%

"c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" ..\FPGA.vcxproj /Out DeviceSTM32.out /ReBuild "Debug_stm32|x86"

if exist ..\Debug\Device.lib goto Return

if exist "c:\Program Files\Notepad++\notepad++.exe" (
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop DeviceSTM32.out -n1000 -x0 -y0
) else (
"c:\Program Files (x86)\Notepad++\notepad++.exe" -multiInst -alwaysOnTop DeviceSTM32.out -n1000 -x0 -y0
)

:Return