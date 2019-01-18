cd ../..
rmdir generated /s /q
cd scripts/VS2017_Keil_Debug
cmake ../../CMakeLists.txt -B ../../generated
