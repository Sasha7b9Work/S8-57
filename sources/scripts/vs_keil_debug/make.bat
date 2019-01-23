cd ../..
rmdir generated /s /q
cd scripts/vs_keil_debug
cmake ../../CMakeLists.txt -B../../generated
