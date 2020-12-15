#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cSaveAs,                                                                                                                             //--- ПАМЯТЬ - ВНЕШН ЗУ - Сохранять как ---
    "Сохранять как"
    ,
    "Если выбран вариант \"Изображение\", сигнал будет сохранён в текущем каталоге в графическом файле с расширением BMP\n"
    "Если выбран вариант \"Текст\", сигнал будет сохранён в текущем каталоге в текстовом виде в файле с раширением TXT"
    ,
    "Изображение",
    "Текст",
    S_MEM_MODE_SAVE_SIGNAL, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cName,                                                                                                                                   //--- ПАМЯТЬ - ВНЕШН ЗУ - Имя файла ---
    "Имя файла"
    ,
    "Задаёт режим наименования файлов при сохранении на внешний накопитель:\n"
    "\"По маске\" - файлы именуются автоматически по заранее введённой маске (след. пункт меню),\n"
    "\"Вручную\" - каждый раз имя файла нужно задавать вручную"
    ,
    "По маске",
    "Вручную",
    S_MEM_FILE_NAMING_MODE, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cModeBtnMemory,                                                                                                                      //--- ПАМЯТЬ - ВНЕШН ЗУ - Реж кн ПАМЯТЬ ---
    "Реж кн ПАМЯТЬ",
    "",
    "Меню",
    "Сохранение",
    S_MEM_MODE_BTN_MEMORY, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cAutoconnect,                                                                                                                      //--- ПАМЯТЬ - ВНЕШН ЗУ - Автоподключение ---
    "Автоподкл.",
    "Eсли \"Вкл\", при подключении внешнего накопителя происходит автоматический переход на страницу ПАМЯТЬ - Внешн ЗУ",
    DISABLE_RU,
    ENABLE_RU,
    S_MEM_FLASH_AUTO_CONNECT, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_1(pDrive,   //-V1027                                                                                                                                     //--- ПАМЯТЬ - ВНЕШН ЗУ --- //-V2567
    "ВНЕШН ЗУ",
    "Работа с внешним запоминающим устройством.",
    &cModeBtnMemory,
    //PageDrive::Manager::self,
    //&cName,
    //PageDrive::Mask::self,
    //&cSaveAs,
    //&cAutoconnect,
    PageName::Memory_Drive,
    &PageMemory::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDrive::self = static_cast<const Page *>(&pDrive);
