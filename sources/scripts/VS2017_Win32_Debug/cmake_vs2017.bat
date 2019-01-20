cd ../..
rmdir generated /s /q
cd scripts/VS2017_Win32_Debug
cmake ../../CMakeLists.txt -B../../generated
