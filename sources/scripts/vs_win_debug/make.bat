cd ../..
rmdir generated /s /q
cd scripts/vs_win_debug
cmake ../../VS/CMakeLists.txt -B../../generated
