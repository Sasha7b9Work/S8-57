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
#include "Utils/Math.h"
#include "Utils/Values.h"


using namespace Display::Primitives;

extern const Page pROM;

const Page * const PageROM::self = (const Page *)&pROM;

/// Нарисовать карту памяти сохраннных сигналов
static void DrawMemoryMap(int num, bool exist);
/// Обработчик нажатия кнопки при открытой странице
static bool OnArrows_ROM(const KeyEvent &event);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Next()
{
    OnArrows_ROM(KeyEvent(Key::Right, TypePress::Release));
}

static void Draw_Next(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bNext,
    "Следующий",
    "Перейти к следующему сигналу",
    &PageROM::self, Item::Active, OnPress_Next, Draw_Next
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{
    OnArrows_ROM(KeyEvent(Key::Left, TypePress::Release));
}

static void Draw_Prev(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_ARROW_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bPrev,
    "Предыдущий",
    "Перейти к предыдущему сигналу",
    &PageROM::self, Item::Active, OnPress_Prev, Draw_Prev
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Delete()
{
    Display::FuncOnWaitStart("Удаляю сохранённые данные", false);
    Memory::DeleteData(NUM_ROM_SIGNAL);
    Display::FuncOnWaitStop();
}

static void Draw_Delete(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_DELETE).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bDelete,                                                                                                                              //--- ПАМЯТЬ - ВНУТР ЗУ - Удалить ---
    "Удалить",
    "Удаляет выбранный сигнал из внутреннего запоминающего устройства",
    &PageROM::self, Item::Active, OnPress_Delete, Draw_Delete
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

static void OnPress_SaveToMemory()
{
    Display::FuncOnWaitStart("Записываю в память", false);
    SaveSignalToIntMemory();
    Display::FuncOnWaitStop();
}

static void Draw_SaveToMemory(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSave,                                                                                                                     //--- ПАМЯТЬ - ВНУТР ЗУ - Сохранить в памяти ---
    "Сохранить в памяти",
    "Сохранить сигнал во внутреннем запоминующем устройстве",
    &PageROM::self, Item::Active, OnPress_SaveToMemory, Draw_SaveToMemory
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_ROM(bool)
{
    MODE_WORK = ModeWork::ROM;
}

static void AfterDraw_ROM()
{
    // Теперь нарисуем состояние памяти

    bool exist[MAX_NUM_SAVED_WAVES] = { false };

    Memory::GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryMap(i, exist[i]);
    }
}

static void DrawMemoryMap(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 13;
    int width = 12;
    Region(width, 10).Fill(x, y, num == NUM_ROM_SIGNAL ? Color::FLASH_10 : Color::BACK);
    Rectangle(width, 10).Draw(x, y, Color::FILL);

    Color((num == NUM_ROM_SIGNAL) ? Color::FLASH_01 : Color::FILL).SetAsCurrent();

    if (exist)
    {
        Integer(num + 1).ToString(false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        String("\x88").Draw(x + 3, y + 1);
    }
}

static bool OnArrows_ROM(const KeyEvent &event)
{
    if (event.type == TypePress::Release || event.type == TypePress::Long)
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
    }

    return true;
}

DEF_PAGE_4( pROM, // -V641                                                                                                                                        //--- ПАМЯТЬ - ВНУТР ЗУ ---
    "ВНУТР ЗУ",
    "Переход в режим работы с внутренней памятью",
    &bPrev,         ///< ПАМЯТЬ - ВНУТР ЗУ - Предыдущий
    &bNext,         ///< ПАМЯТЬ - ВНУТР ЗУ - Следующий
    &bSave,         ///< ПАМЯТЬ - ВНУТР ЗУ - Сохранить
    &bDelete,       ///< ПАМЯТЬ - ВНУТР ЗУ - Удалить
    PageName::Memory_Internal,
    &PageMemory::self, Item::Active, OnOpenClose_ROM, AfterDraw_ROM, OnArrows_ROM
)
