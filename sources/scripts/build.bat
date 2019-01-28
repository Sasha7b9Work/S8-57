@echo off

cd keil
call build.bat
cd ..

cd vs_keil_debug
call build.bat
cd ..
