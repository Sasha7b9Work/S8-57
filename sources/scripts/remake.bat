@echo off

cd keil
call rebuild.bat
cd ..

cd vs_keil
call make_build.bat
cd..

cd vs_win
call make_build.bat
cd..
