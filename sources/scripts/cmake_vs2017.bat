cd ..
rmdir generated /s /q
cd scripts
cmake ../CMakeLists.txt -B ../generated
