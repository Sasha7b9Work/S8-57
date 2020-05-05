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
    set.mem.modeSaveSignal, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
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
    FileNamingMode::Ref(), &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cModeBtnMemory,                                                                                                                      //--- ПАМЯТЬ - ВНЕШН ЗУ - Реж кн ПАМЯТЬ ---
    "Реж кн ПАМЯТЬ",
    "",
    "Меню",
    "Сохранение",
    set.mem.modeBtnMemory, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cAutoconnect,                                                                                                                      //--- ПАМЯТЬ - ВНЕШН ЗУ - Автоподключение ---
    "Автоподкл.",
    "Eсли \"Вкл\", при подключении внешнего накопителя происходит автоматический переход на страницу ПАМЯТЬ - Внешн ЗУ",
    DISABLE_RU,
    ENABLE_RU,
    set.mem.flashAutoConnect, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_6(pDrive,   //-V1027                                                                                                                                     //--- ПАМЯТЬ - ВНЕШН ЗУ ---
    "ВНЕШН ЗУ",
    "Работа с внешним запоминающим устройством.",
    PageDrive::Manager::self,
    &cName,
    PageDrive::Mask::self,
    &cSaveAs,
    &cModeBtnMemory,
    &cAutoconnect,
    PageName::Memory_Drive,
    &PageMemory::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDrive::self = static_cast<const Page *>(&pDrive);
