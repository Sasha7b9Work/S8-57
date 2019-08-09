@echo off

echo "keil version"

cd keil
call build.bat
cd ..

cd vs_keil
call build.bat
cd ..

rem cd vs_win
rem call build.bat
rem cd ..
