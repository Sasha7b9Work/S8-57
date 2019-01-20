rmdir temp /s /q

@echo Installation SDL2

cd SDL2
call install_and_build.bat
cd ..

@echo Installation wxWidgets

cd wxWidgets
call install_and_build.bat
cd ..
