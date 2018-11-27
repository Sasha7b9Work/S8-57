"c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" FPGA.vcxproj /Rebuild

if exist Debug\Device.exe (
Debug\Device.exe
) else (
error_message.vbs
)
