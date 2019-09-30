#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Tables.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Memory.h"
#include "Hardware/Beeper.h"
#include "FlashDrive/FileManager.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "Utils/CommonFunctions.h"
#include "Osci/Display/PainterData.h"
#include "Osci/Osci.h"


using namespace Display::Primitives;

static void DrawSetMask();  // Эта функция рисует, когда выбран режим задания маски.
static void DrawFileMask(int x, int y);
static void DrawSetName();  // Эта функция рисует, когда нужно задать имя файла для сохранения
       void OnMemExtSetMaskNameRegSet(int angle, int maxIndex);



static bool IsActive_Points()
{
    return (set.time.peakDet == PeakDetMode::Disabled);
}

void PageMemory::OnChanged_Points(bool active)
{
    // Если включен пиковый детектор, то не можем переключать память
    if ((set.time.peakDet == PeakDetMode::Enabled) && !active)
    {
        Display::ShowWarning(Warning::WrongModePeackDet);
        return;
    }

    Osci::OnChangedPoints();
}

static pString namesLengthMemory[] = {"512", "1k", "2k", "4k", "8k", "16k", "32k"};

DEF_CHOICE_5( cPoints,                                                                                                                                               //--- ПАМЯТЬ - Точки ---
    "Длина памяти",
    "Выбор количества отсчётов для сохраняемых сигналов. При увеличении количества отсчётов уменьшается количество сохранённых в памяти сигналов.",
    namesLengthMemory[0],
    namesLengthMemory[1],
    namesLengthMemory[2],
    namesLengthMemory[3],
    namesLengthMemory[4],
    set.mem.enumPoints, &PageMemory::self, IsActive_Points, PageMemory::OnChanged_Points, Choice::AfterDraw
)


static void Draw_Manager_Tab(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_TAB).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bManager_Tab,                                                                                                                   //--- ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Tab ---
    "Tab",
    "Переход между каталогами и файлами",
    &PageDrive::PageManager::self, Item::Active, FileManager::Press_Tab, Draw_Manager_Tab
)


static void Draw_Manager_LevelUp(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bManager_LevelUp,                                                                                                 //--- ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Выйти из каталога ---
    "Выйти из каталого",
    "Переход в родительский каталог",
    &PageDrive::PageManager::self, Item::Active, FileManager::Press_LevelUp, Draw_Manager_LevelUp
)


static void Draw_Manager_LevelDown(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bManager_LevelDown,                                                                                                 //--- ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Войти в каталог ---
    "Войти в каталог",
    "Переход в выбранный каталог",
    &PageDrive::PageManager::self, Item::Active, FileManager::Press_LevelDown, Draw_Manager_LevelDown
)


static bool IsActive_Drive_Manager()
{
    return FDrive::IsConnected();
}

void PageMemory::OnOpenClose_Drive_Manager(bool)
{
    if (FDrive::IsConnected())
    {
        FDrive::Mount();
        Display::SetDrawMode(Display::DrawMode::Auto, FileManager::Draw);
        FM_NEED_REDRAW = FM_REDRAW_FULL;
    }
    else
    {
        Display::ShowWarning(Warning::WarnNeedForFlashDrive);
    }
}

/*
DEF_PAGE_SB(        pManager,                                                                               // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ ///
    "КАТАЛОГ", "DIRECTORY",
    "Открывает доступ к файловой системе подключенного накопителя",
    "Provides access to the file system of the connected drive",
    &bDrive_Manager_Exit,       // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Выход
    &bManager_Tab,        // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Tab
    0,
    0,
    &bManager_LevelUp,    // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Выйти из каталога
    &bManager_LevelDown,  // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Войти в каталог
    Page::Name::SB_Memory_Drive_Manager, &pDrive, IsActive_Drive_Manager, PageMemory::OnOpenClose_Drive_Manager, FuncDrawPage, FileManager::RotateRegSet
)
*/

DEF_PAGE_3( pManager, // -V641                                                                                                                          //--- ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ ---
    "КАТАЛОГ",
    "Открывает доступ к файловой системе подключенного накопителя",
    &bManager_Tab,        // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Tab
    &bManager_LevelUp,    // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Выйти из каталога
    &bManager_LevelDown,  // ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ - Войти в каталог
    PageName::Memory_Drive_Manager,
    &PageDrive::self, IsActive_Drive_Manager, PageMemory::OnOpenClose_Drive_Manager, Page::BeforeDraw, FileManager::OnArrows
)

const Page * const PageDrive::PageManager::self = (const Page *)&pManager;


DEF_CHOICE_2( cDrive_Name,                                                                                                                            //--- ПАМЯТЬ - ВНЕШН ЗУ - Имя файла ---
    "Имя файла"
    ,
    "Задаёт режим наименования файлов при сохранении на внешний накопитель:\n"
    "\"По маске\" - файлы именуются автоматически по заранее введённой маске (след. пункт меню),\n"
    "\"Вручную\" - каждый раз имя файла нужно задавать вручную"
    ,
    "По маске",
    "Вручную",
    set.mem.fileNamingMode, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Mask_Delete()
{
    set.mem.fileNameMask[0] = '\0';
}

static void Draw_Delete(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_DELETE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bMask_Delete,                                                                                                                 //--- ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Удалить ---
    "Удалить",
    "Удаляет все введённые символы",
    &PageDrive::PageMask::self, Item::Active, OnPress_Mask_Delete, Draw_Delete
)


static void OnPress_Mask_Backspace()
{
    int size = (int)std::strlen(set.mem.fileNameMask);
    if (size > 0)
    {
        if (size > 1 && set.mem.fileNameMask[size - 2] == 0x07)
        {
            set.mem.fileNameMask[size - 2] = '\0';
        }
        else
        {
            set.mem.fileNameMask[size - 1] = '\0';
        }
    }
}

static void Draw_Backspace(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_LEFT).Draw(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bMask_Backspace,                                                                                                      //--- ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Backspace ---
    "Backspace",
    "Удаляет последний введённый символ",
    &PageDrive::PageMask::self, Item::Active, OnPress_Mask_Backspace, Draw_Backspace
)


static void OnPress_Mask_Insert()
{
    int index = set.mem.indexCurSymbolNameMask;
    int size = (int)std::strlen(set.mem.fileNameMask);
    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }
    if (index < 0x41)
    {
        set.mem.fileNameMask[size] = Tables::Get(index)[0];
        set.mem.fileNameMask[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // Для %nN - отдельный случай
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (set.mem.fileNameMask[size - 1] >= 0x30 && set.mem.fileNameMask[size - 1] <= 0x39) // Если ранее введено число
                {
                    set.mem.fileNameMask[size] = set.mem.fileNameMask[size - 1] - 0x30;
                    set.mem.fileNameMask[size - 1] = 0x07;
                    set.mem.fileNameMask[size + 1] = '\0';
                }
            }
        }
        else
        {
            set.mem.fileNameMask[size] = (char)index;
            set.mem.fileNameMask[size + 1] = '\0';
        }
    }
}

static void Draw_Insert(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_INSERT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bMask_Insert,                                                                                                                //--- ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Вставить ---
    "Вставить",
    "Вставляет выбранный символ",
    &PageDrive::PageMask::self, Item::Active, OnPress_Mask_Insert, Draw_Insert
)


DEF_CHOICE_2( cDrive_SaveAs,                                                                                                                      //--- ПАМЯТЬ - ВНЕШН ЗУ - Сохранять как ---
    "Сохранять как"
    ,
    "Если выбран вариант \"Изображение\", сигнал будет сохранён в текущем каталоге в графическом файле с расширением BMP\n"
    "Если выбран вариант \"Текст\", сигнал будет сохранён в текущем каталоге в текстовом виде в файле с раширением TXT"
    ,
    "Изображение",
    "Текст",
    set.mem.modeSaveSignal, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cDrive_ModeBtnMemory,                                                                                                               //--- ПАМЯТЬ - ВНЕШН ЗУ - Реж кн ПАМЯТЬ ---
    "Реж кн ПАМЯТЬ",
    "",
    "Меню",
    "Сохранение",
    set.mem.modeBtnMemory, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Mask()
{
    return (set.mem.fileNamingMode == FileNamingMode::Mask);
}

static void OnOpenClose_Mask(bool)
{
    Display::SetAddDrawFunction(DrawSetMask);
}

static void DrawSetMask()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 20;
    int width = Grid::Width() - 80;
    int height = 160;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // Рисуем большие буквы английского алфавита
    while (Tables::Get(index)[0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // Теперь рисуем цифры и пробел
    position = 0;
    while (Tables::Get(index)[0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // Теперь рисуем малые буквы алфавита
    position = 0;
    while (Tables::Get(index)[0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // Теперь рисуем спецсимволы
    position = 0;
    while (index < (Tables::Size() / 4))
    {
        Tables::DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static pString strings[] =
    {
        "%y - год, %m - месяц, %d - день",
        "%H - часы, %M - минуты, %S - секунды",
        "%nN - порядковый номер, где",
        "n - минимальное количество знаков для N"
    };

    deltaY--;
    Color::FILL.SetAsCurrent();
    for (int i = 0; i < (int)sizeof(strings) / 4; i++)
    {
        String(strings[i]).Draw(x0 + deltaX, y0 + 100 + deltaY * i);
    }
}

static void DrawFileMask(int x, int y)
{
    char *ch = set.mem.fileNameMask;

    Color::FILL.SetAsCurrent();
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            Char(*ch).Draw(x, y);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = Char('%').Draw(x, y);
                x = Char((char)(0x30 | *(ch + 1))).Draw(x, y);
                x = Char('N').Draw(x, y);
                ch++;
            }
            else
            {
                x = String(Tables::Get(*ch + 0x40)).Draw(x, y);
            }
        }
        ch++;
    }
    Region(5, 8).Fill(x, y, Color::FLASH_10);
}

static bool OnArrows_Mask(const KeyEvent &event)
{
    Key::E key = event.key;

    int angle = (key == Key::Up || key == Key::Right) ? 1 : -1;

    OnMemExtSetMaskNameRegSet(angle, Tables::Size() / 4);

    return true;
}


/*
DEF_PAGE_SB( pMask,                                                                                                                               //--- Память - ВНЕШН ЗУ - МАСКА ---
    "МАСКА", "MASK",
    "Режим ввода маски для автоматического именования файлов",
    "Input mode mask for automatic file naming",
    &bDrive_Mask_Exit,      // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Выход
    &bMask_Delete,    // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Удалить
    0,
    0,
    &bMask_Backspace, // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Backspace
    &bMask_Insert,    // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Вставить
    Page::Name::SB_Memory_Drive_Mask, &pDrive, IsActive_Mask, OnPress_Drive_Mask, FuncDrawPage, OnRegSet_Drive_Mask
)
*/


DEF_PAGE_3( pMask, // -V641                                                                                                                               //--- Память - ВНЕШН ЗУ - МАСКА ---
    "МАСКА",
    "Режим ввода маски для автоматического именования файлов",
    &bMask_Delete,    // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Удалить
    &bMask_Backspace, // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Backspace
    &bMask_Insert,    // ПАМЯТЬ - ВНЕШН ЗУ - МАСКА - Вставить
    PageName::Memory_Drive_Mask,
    &PageDrive::self, IsActive_Mask, OnOpenClose_Mask, Page::BeforeDraw, OnArrows_Mask
)

const Page * const PageDrive::PageMask::self = (const Page *)&pMask;


DEF_CHOICE_2( cDrive_Autoconnect,                                                                                                               //--- ПАМЯТЬ - ВНЕШН ЗУ - Автоподключение ---
    "Автоподкл.",
    "Eсли \"Вкл\", при подключении внешнего накопителя происходит автоматический переход на страницу ПАМЯТЬ - Внешн ЗУ",
    DISABLE_RU,
    ENABLE_RU,
    set.mem.flashAutoConnect, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_6( pDrive,  //-V641 //-V1027                                                                                                                             //--- ПАМЯТЬ - ВНЕШН ЗУ ---
    "ВНЕШН ЗУ",
    "Работа с внешним запоминающим устройством.",
    PageDrive::PageManager::self,   ///< ПАМЯТЬ - ВНЕШН ЗУ - КАТАЛОГ
    &cDrive_Name,                   ///< ПАМЯТЬ - ВНЕШН ЗУ - Имя файла
    PageDrive::PageMask::self,      ///< Память - ВНЕШН ЗУ - МАСКА
    &cDrive_SaveAs,                 ///< ПАМЯТЬ - ВНЕШН ЗУ - Сохранять как
    &cDrive_ModeBtnMemory,          ///< ПАМЯТЬ - ВНЕШН ЗУ - Реж кн ПАМЯТЬ
    &cDrive_Autoconnect,            ///< ПАМЯТЬ - ВНЕШН ЗУ - Автоподключение
    PageName::Memory_Drive,
    &PageMemory::self,               ///< ПАМЯТЬ
    Item::Active, Page::Changed, Page::BeforeDraw, Page::Handler
)

const Page * const PageDrive::self = (const Page *)&pDrive;


DEF_PAGE_4( pMemory, // -V641 // -V1027                                                                                                                                      //--- ПЯМЯТЬ ---
    "ПАМЯТЬ",
    "Работа с внешней и внутренней памятью.",
    &cPoints,               ///< ПАМЯТЬ - Точки
    PageRAM::self,          ///< ПАМЯТЬ - ПОСЛЕДНИЕ
    PageROM::self,          ///< ПАМЯТЬ - ВНУТР ЗУ
    PageDrive::self,        ///< ПАМЯТЬ - ВНЕШН ЗУ
    PageName::Memory, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, Page::Handler
)

const Page * const PageMemory::self = (const Page *)&pMemory;


void PageMemory::SaveSignalToFlashDrive()
{
    if (FDrive::IsConnected())
    {
        if (set.mem.fileNamingMode == FileNamingMode::Manually)
        {
            Display::SetAddDrawFunction(DrawSetName);
        }
        else
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }
    }
    else
    {
        EXIT_FROM_SETNAME_TO = 0;
    }
}

static void DrawSetName()
{
    int x0 = Grid::Left() + 40;
    int y0 = Grid::Top() + 60;
    int width = Grid::Width() - 80;
    int height = 80;

    Rectangle(width, height).Draw(x0, y0, Color::FILL);
    Region(width - 2, height - 2).Fill(x0 + 1, y0 + 1, Color::BACK);

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // Рисуем большие буквы английского алфавита
    while (Tables::Get(index)[0] != ' ')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // Теперь рисуем цифры и пробел
    position = 0;
    while (Tables::Get(index)[0] != 'a')
    {
        Tables::DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // Теперь рисуем малые буквы алфавита
    position = 0;
    while (Tables::Get(index)[0] != '%')
    {
        Tables::DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = String(set.mem.fileName).Draw(x0 + deltaX, y0 + 65, Color::FILL);

    Region(5, 8).Fill(x, y0 + 65, Color::FLASH_10);
}


static void OnPress_SetName_Exit()
{
    if (EXIT_FROM_SETNAME_TO == RETURN_TO_DISABLE_MENU)
    {
        //Menu::PagePointerFromName(Page::Name::SB_Memory_SetName)->ShortPressOnItem(0);
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_LAST_MEM)
    {
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_INT_MEM)
    {
    }
    else
    {
        //
    }

    EXIT_FROM_SETNAME_TO = RETURN_TO_DISABLE_MENU;
}


static void OnPress_SetName_Delete()
{
    set.mem.fileName[0] = '\0';
}

DEF_GRAPH_BUTTON( bSetName_Delete,                                                                                                                              //--- ИМЯ ФАЙЛА - Удалить ---
    "Удалить",
    "Удаляет все введённые символы",
    &PageSetName::self, Item::Active, OnPress_SetName_Delete, Draw_Delete
)


static void OnPress_SetName_Backspace()
{
    int size = (int)std::strlen(set.mem.fileName);
    if (size > 0)
    {
        set.mem.fileName[size - 1] = '\0';
    }
}

DEF_GRAPH_BUTTON( bSetName_Backspace,                                                                                                                         //--- ИМЯ ФАЙЛА - Backspace ---
    "Backspace",
    "Удаляет последний символ",
    &PageSetName::self, Item::Active, OnPress_SetName_Backspace, Draw_Backspace
)


static void OnPress_SetName_Insert()
{
    int size = (int)std::strlen(set.mem.fileName);
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        set.mem.fileName[size] = Tables::Get(set.mem.indexCurSymbolNameMask)[0];
        set.mem.fileName[size + 1] = '\0';
    }
}

DEF_GRAPH_BUTTON( bSetName_Insert,                                                                                                                             //--- ИМЯ ФАЙЛА - Вставить ---
    "Вставить",
    "Вводит очередной символ",
    &PageSetName::self, Item::Active, OnPress_SetName_Insert, Draw_Insert
)


static void OnPress_SetName_Save()
{
    if (FDrive::IsConnected())
    {
        OnPress_SetName_Exit();
        NEED_SAVE_TO_FLASHDRIVE = 1;
    }
}

static void Draw_SetName_Save(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Font::SetCurrent(Font::Type::_UGO2);
        Char(SYMBOL_FLASH_DRIVE_BIG).Draw4SymbolsInRect(x + 2, y + 1);
        Font::SetCurrent(Font::Type::_8);
    }
}

DEF_GRAPH_BUTTON( bSetName_Save,                                                                                                                              //--- ИМЯ ФАЙЛА - Сохранить ---
    "Сохранить",
    "Сохранение на флеш под заданным именем",
    &PageSetName::self, Item::Active, OnPress_SetName_Save, Draw_SetName_Save
)


static bool OnArrows_SetName(const KeyEvent &event)
{
    OnMemExtSetMaskNameRegSet(event.Delta(), Tables::Size() / 4 - 7);

    return true;
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    typedef void (*pFuncVpI8I8I8)(int8 *, int8, int8);

    static pFuncVpI8I8I8 func[3] =
    {
        Math::CircleDecrease<int8>,
        Math::CircleDecrease<int8>,
        Math::CircleIncrease<int8>
    };

    Color::ResetFlash();
    if (set.mem.indexCurSymbolNameMask > maxIndex)
    {
        set.mem.indexCurSymbolNameMask = (int8)(maxIndex - 1);
    }
    func[Math::Sign(angle) + 1](&set.mem.indexCurSymbolNameMask, 0, (int8)(maxIndex - 1));
    Beeper::RegulatorSwitchRotate();

}

DEF_PAGE_4( pSetName, // -V641                                                                                                            //--- Страница вызывается для ввода имени файла ---
    "",
    "",
    &bSetName_Delete,       /// ВВОД ИМЕНИ ФАЙЛА - Удалить
    &bSetName_Backspace,    /// ВВОД ИМЕНИ ФАЙЛА - Backspace
    &bSetName_Insert,       /// ВВОД ИМЕНИ ФАЙЛА - Вставить
    &bSetName_Save,         /// ВВОД ИМЕНИ ФАЙЛА - Сохранить
    PageName::Memory_SetName, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, OnArrows_SetName
)

const Page * const PageSetName::self = (const Page *)&pSetName;


ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints)
{
    if (numPoints == 8192) { return ENumPointsFPGA::_8k; }
    else if (numPoints == 4096) { return ENumPointsFPGA::_4k; }
    else if (numPoints == 2048) { return ENumPointsFPGA::_2k; }
    else if (numPoints == 1024) { return ENumPointsFPGA::_1k; }
    else
    {
        LOG_ERROR("");
    }
    return ENumPointsFPGA::_512;
}



int ENumPoints_2_NumPoints(ENumPointsFPGA::E numPoints)
{
    static const int n[ENumPointsFPGA::Size] =
    {
        512,
        1024,
        2048,
        4096,
        8192
    };
    return n[(uint)numPoints];
}
