cd ../..
rmdir generated /s /q
cd scripts/VS2017_Win32_Debug
cmake ../../VS/CMakeLists.txt -B../../generated
