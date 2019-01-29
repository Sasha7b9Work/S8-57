cd keil
call rebuild.bat
cd ..

cd vs_keil_debug
call make_build.bat
cd..

cd vs_win_debug
call make_build.bat
cd..
