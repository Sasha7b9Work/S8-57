# OSC

[trello](https://trello.com/b/xgE5XoKD/%D0%BA%D0%BE%D1%80%D0%BD%D0%B5%D1%82-osc)


### Предустановленное программное обеспечение

uVision V5.36.0.0  ARM Compiler v5.06 update 7 (build 960)
Microsoft Visual Studio Professional 2017 Version 15.9.6
Python 3.7.2


### Установка

Проект Keil не нуждается в дополнительных действиях и готов к работе сразу после скачивания.

Для сборки win32-версий нужно произвести дополнительные действия.

1. Перейдите в каталог sources/scripts

2. Запустите командную строку разработчика Visual Studio 2017, кликнув по файлу 

    prompt.bat

3. Скачайте и установите библиотеки python, необходимые для скачки библиотек SDL2 и wxWidgets. Для этого запустите скрипт

    install_python_libs.bat
    
4. Скачайте и установите библиотеки SDL2 и wxWidgets, необходимые для сборки win32-версий приложения. Для этого запустите скрипт

    install_libs.bat
    
5. Сборка всех версий выполняется запуском

    remake.bat
    
6. Компиляция

    build.bat
    
7. Полная перекомпиляция

    rebuild.bat
