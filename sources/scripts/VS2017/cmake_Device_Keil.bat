cd ../..
rmdir generated /s /q
cd scripts/VS2017
cmake ../../CMakeLists.txt -B ../../generated
