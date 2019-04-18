#include "defines.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Memory.h"
#include "Hardware/Beeper.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Settings/Settings.h"
#include "Settings/SettingsMemory.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


using namespace Display::Primitives;


extern const PageBase pageROM;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMemory::PageROM::pointer = &pageROM;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawMemoryWave(int num, bool exist);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Internal_ShowAlways()
{
    ALWAYS_SHOW_ROM_SIGNAL = ALWAYS_SHOW_ROM_SIGNAL ? 0U : 1U;
}

static void Draw_Internal_ShowAlways_Yes(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x66').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

static void Draw_Internal_ShowAlways_No(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x68').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

static void Draw_Internal_ShowAlways(int x, int y)
{
    if (ALWAYS_SHOW_ROM_SIGNAL == 0)
    {
        Draw_Internal_ShowAlways_No(x, y);
    }
    else
    {
        Draw_Internal_ShowAlways_Yes(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2( bInternal_ShowAlways,                                                                                               //--- ПАМЯТЬ - ВНУТР ЗУ - Показывать всегда ---
    "Показывать всегда",
    "Позволяет всегда показывать выбранный сохранённый сигнал поверх текущего",
    pageROM, FuncActive, OnPress_Internal_ShowAlways, Draw_Internal_ShowAlways,
    Draw_Internal_ShowAlways_Yes, "показывать выбранный сигнал из внутренней памяти поверх текущего",
    Draw_Internal_ShowAlways_No,  "сигнал из внутренней памяти виден только в режиме работы с внутренним запоминающим устройством"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_ModeShow()
{
    Math::CircleIncrease<int8>((int8 *)&SHOW_IN_INT, 0, 2);
}

static void Draw_Internal_ModeShow_Direct(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x6a').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}


static void Draw_Internal_ModeShow_Saved(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x6c').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

static void Draw_Internal_ModeShow_Both(int x, int y)
{
    String("ОБА").Draw(x + 1, y + 5);
    VLine(12).Draw(x + 1, y + 2, Color::BACK);
    VLine(5).Draw(x + 2, y + 6, Color::FILL);
}

static void Draw_Internal_ModeShow(int x, int y)
{
    if (SHOW_IN_INT_DIRECT)
    {
        Draw_Internal_ModeShow_Direct(x, y);
    }
    else if (SHOW_IN_INT_SAVED)
    {
        Draw_Internal_ModeShow_Saved(x, y);
    }
    else
    {
        Draw_Internal_ModeShow_Both(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_3( bInternal_ModeShow,                                                                                                       //--- ПАМЯТЬ - ВНУТР ЗУ - Вид сигнала ---
    "Вид сигнала",
    "Показывать записанный или текущий сигнал в режиме ВНУТР ЗУ",
    pageROM, FuncActive, OnPress_Internal_ModeShow, Draw_Internal_ModeShow,
    Draw_Internal_ModeShow_Direct, "на дисплее текущий сигнал",
    Draw_Internal_ModeShow_Saved,  "на дисплее сохранённый сигнал",
    Draw_Internal_ModeShow_Both,   "на дисплее оба сигнала"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_Delete()
{
    Display::FuncOnWaitStart("Удаляю сохранённые данные", false);
    Memory::DeleteData(NUM_ROM_SIGNAL);
    Display::FuncOnWaitStop();
}

static void Draw_Internal_Delete(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_DELETE).Draw(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bInternal_Delete,                                                                                                                     //--- ПАМЯТЬ - ВНУТР ЗУ - Удалить ---
    "Удалить",
    "Удаляет выбранный сигнал из внутреннего запоминающего устройства",
    pageROM, FuncActive, OnPress_Internal_Delete, Draw_Internal_Delete
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SaveSignalToIntMemory()
{
    // Заносим в указатели DS, DATA_A, DATA_B данные из ОЗУ или последний считанный сигнал, в зависимости от того, из какого режима зашли в 
    // "ПАМЯТЬ-ВНУТР ЗУ"
    //Reader::ReadFromRAM(EXIT_FROM_ROM_TO_RAM ? NUM_RAM_SIGNAL : 0, 0, false);

    if (DS)                                             // Если есть что сохранять
    {
//        Memory::SaveData(NUM_ROM_SIGNAL, DS, IN_A, IN_B);   // То сохраняем данные из DS, DATA_A, DATA_B на место NUM_ROM_SIGNAL в ППЗУ
        Display::ShowWarning(Warning::SignalIsSaved);
    }
}

static void OnPress_Internal_SaveToMemory()
{
    Display::FuncOnWaitStart("Записываю в память", false);
    SaveSignalToIntMemory();
    Display::FuncOnWaitStop();
}

static void Draw_Internal_SaveToMemory(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SAVE_TO_MEM).Draw(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON( bInternal_SaveToMemory,                                                                                                    //--- ПАМЯТЬ - ВНУТР ЗУ - Сохранить в памяти ---
    "Сохранить в памяти",
    "Сохранить сигнал во внутреннем запоминующем устройстве",
    pageROM, FuncActive, OnPress_Internal_SaveToMemory, Draw_Internal_SaveToMemory
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_SaveToDrive()
{
    EXIT_FROM_SETNAME_TO = RETURN_TO_INT_MEM;
    PageMemory::SaveSignalToFlashDrive();
}

static void Draw_Internal_SaveToDrive(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Font::SetCurrent(Font::Type::_UGO2);
        Char(SYMBOL_FLASH_DRIVE_BIG).Draw4SymbolsInRect(x + 2, y + 1);
        Font::SetCurrent(Font::Type::_8);
    }
}

DEF_SMALL_BUTTON( bInternal_SaveToDrive,                                                                                                    //--- ПАМЯТЬ - ВНУТР ЗУ - Сохранить на флешку ---
    "Сохранить на флешку",
    "Сохраняет сигнал на флешку",
    pageROM, FuncActive, OnPress_Internal_SaveToDrive, Draw_Internal_SaveToDrive
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Internal(bool)
{
    MODE_WORK = ModeWork::ROM;
}

static void OnDraw_Internal()
{
    // Теперь нарисуем состояние памяти

    bool exist[MAX_NUM_SAVED_WAVES] = { false };

    Memory::GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave(i, exist[i]);
    }
}

static void DrawMemoryWave(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 10;
    int width = 12;
    Region(width, 10).Fill(x, y, num == NUM_ROM_SIGNAL ? Color::FLASH_10 : Color::BACK);
    Rectangle(width, 10).Draw(x, y, Color::FILL);
    Color::SetCurrent(num == NUM_ROM_SIGNAL ? Color::FLASH_01 : Color::FILL);

    if (exist)
    {
        Integer(num + 1).ToString(false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        String("\x88").Draw(x + 3, y + 1);
    }
}

static bool HandlerKey_Internal(KeyEvent event)
{
    Key::E key = event.key;

    Beeper::RegulatorSwitchRotate();

    if (key == Key::Left || key == Key::Down)
    {
        Math::CircleDecrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else if (key == Key::Right || key == Key::Up)
    {
        Math::CircleIncrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else
    {
        // остальные кнопки не обрабатываем
    }

    Color::ResetFlash();

    return true;
}


DEF_PAGE_5( pageROM, // -V641                                                                                                                                     //--- ПАМЯТЬ - ВНУТР ЗУ ---
    "ВНУТР ЗУ",
    "Переход в режим работы с внутренней памятью",
    &bInternal_ShowAlways,      ///< ПАМЯТЬ - ВНУТР ЗУ - Показывать всегда
    &bInternal_ModeShow,        ///< ПАМЯТЬ - ВНУТР ЗУ - Вид сигнала
    &bInternal_Delete,          ///< ПАМЯТЬ - ВНУТР ЗУ - Удалить
    &bInternal_SaveToMemory,    ///< ПАМЯТЬ - ВНУТР ЗУ - Сохранить
    &bInternal_SaveToDrive,     ///< ПАМЯТЬ - ВНУТР ЗУ - Сохранить на флешку
    Page::Name::SB_Memory_Internal, PageMemory::pointer, FuncActive, OnPress_Internal, OnDraw_Internal, HandlerKey_Internal
)
