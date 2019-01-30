@echo off

@echo %TIME%   Making Visual Studio Win Debug ...

cd ../..

rmdir generated /s /q

cd scripts/vs_win_debug

cmake ../../VS/CMakeLists.txt -B../../generated > cmake_out.txt
