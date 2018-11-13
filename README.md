# OSC

[trello](https://trello.com/b/xgE5XoKD/%D0%BA%D0%BE%D1%80%D0%BD%D0%B5%D1%82-osc)


### Установка

Проект Keil не нуждается в дополнительных действиях и готов к работе сразу после скачивания.

Проект для Visual Studio для своей работы требует SDL2 и wxWidgets.
Рабочая конфиграция - x86 Debug

Нужно скачать по ссылке https://www.libsdl.org/download-2.0.php SDL версии 2.0.9 (библиотеки для разработчиков https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip) и скопировать в каталог ThirdParty\SDL2\lib\x86 файл SDL2.lib из одноименного каталога в архиве.
Файл SDL2.dll скопируйте в каталог OSC-VS\Device\Debug\

Затем скачать библиотеку wxWidgets со страницы https://www.wxwidgets.org/downloads/ по ссылке https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.1/wxWidgets-3.1.1.zip и распаковать во временный каталог.
Запустить командную строку Visual Studio 2017, зайти в каталог build/msw и запустить команду nmake /f makefile.vc
После завершения компиляции скопируйте *.lib-файлы из каталога lib/vc_lib/ в одноимённый каталог
