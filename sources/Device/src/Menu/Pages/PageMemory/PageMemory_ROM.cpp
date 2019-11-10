#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "Data/Reader.h"
#include "Display/Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/Beeper.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


/// Нарисовать одну ячейчку памяти
static void DrawMemoryCell(int num, bool exist);

static bool HandlerKey_ROM(const KeyEvent &event);



static void OnPress_Next()
{
    HandlerKey_ROM(KeyEvent(Key::Right, TypePress::Release));
}

static void Draw_Next(int x, int y)
{
    Char(SymbolUGO2::ARROW_RIGHT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bNext,
    "Следующий",
    "Перейти к следующему сигналу",
    &PageROM::self, Item::Active, OnPress_Next, Draw_Next
)


static void OnPress_Prev()
{
    HandlerKey_ROM(KeyEvent(Key::Left, TypePress::Release));
}

static void Draw_Prev(int x, int y)
{
    Char(SymbolUGO2::ARROW_LEFT).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bPrev,
    "Предыдущий",
    "Перейти к предыдущему сигналу",
    &PageROM::self, Item::Active, OnPress_Prev, Draw_Prev
)


static void OnPress_Delete()
{
    Display::FuncOnWaitStart("Удаляю сохранённые данные", false);
    FlashMemory::Data::Erase(NUM_ROM_SIGNAL);
    Display::FuncOnWaitStop();

    Display::ShowWarning("Сигнал удален");
}

static void Draw_Delete(int x, int y)
{
    Char(SymbolUGO2::DELETE).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bDelete,                                                                                                                              //--- ПАМЯТЬ - ВНУТР ЗУ - Удалить ---
    "Удалить",
    "Удаляет выбранный сигнал из внутреннего запоминающего устройства",
    &PageROM::self, Item::Active, OnPress_Delete, Draw_Delete
)


static void OnPress_SaveToMemory()
{
    Display::FuncOnWaitStart("Записываю в память", false);

    if(DS)
    {

    }

    Display::FuncOnWaitStop();
}

static void Draw_SaveToMemory(int x, int y)
{
    Char(SymbolUGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bSave,                                                                                                                     //--- ПАМЯТЬ - ВНУТР ЗУ - Сохранить в памяти ---
    "Сохранить в памяти",
    "Сохранить сигнал во внутреннем запоминующем устройстве",
    &PageROM::self, Item::Active, OnPress_SaveToMemory, Draw_SaveToMemory
)


static void OnPress_TypeSignal()
{
    Math::CircleIncrease<uint8>(reinterpret_cast<uint8*>(&set.mem.typeSignalROM), 0, 1);
}

static void Draw_Recorded(int x, int y)
{
    Char(SymbolUGO2::RECORDERD).Draw4SymbolsInRect(x + 1, y + 1);
}

static void Draw_Current(int x, int y)
{
    Char(SymbolUGO2::CURRENT).Draw4SymbolsInRect(x + 1, y + 1);
}

static void Draw_TypeSignal(int x, int y)
{
    typedef void (*pFuncDraw)(int, int);

    static const pFuncDraw func[2] = { Draw_Recorded, Draw_Current };
    func[set.mem.typeSignalROM](x, y);
}

DEF_GRAPH_BUTTON_HINTS_2( bTypeSignal,                                                                                                              //--- ПАМЯТЬ - ВНУТР ЗУ - Тип сигнала ---
    "Тип сигнала",
    "Какой сигнал выводить",
    &PageROM::self, Item::Active, OnPress_TypeSignal, Draw_TypeSignal,
    Draw_Recorded, "Выводится записанный сигнал из памяти",
    Draw_Current, "Выводится текущий сигнал"
)


static void OnOpenClose_ROM(bool open)
{
    if(open)
    {
        Color::ResetFlash();
    }
    Osci::SetModeWork(open ? ModeWork::ROM : ModeWork::Dir);
}

static void AfterDraw_ROM()
{
    // Теперь нарисуем состояние памяти

    bool exist[FlashMemory::Data::MAX_NUM_SAVED_WAVES] = { false };

    FlashMemory::Data::GetInfo(exist);

    for (int i = 0; i < FlashMemory::Data::MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryCell(i, exist[i]);
    }
}

static void DrawMemoryCell(int num, bool exist)
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
        String(Symbol8::CROSS).Draw(x + 3, y + 1);
    }
}

static bool HandlerKey_ROM(const KeyEvent &event)
{
    bool result = false;

    if (event.type == TypePress::Release || event.type == TypePress::Long)
    {
        Key::E key = event.key;

        Beeper::RegulatorSwitchRotate();

        if (key == Key::Left || key == Key::Down)
        {
            Math::CircleDecrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, FlashMemory::Data::MAX_NUM_SAVED_WAVES - 1);

            Color::ResetFlash();

            result = true;
        }
        else if (key == Key::Right || key == Key::Up)
        {
            Math::CircleIncrease<int8>((int8 *)&NUM_ROM_SIGNAL, 0, FlashMemory::Data::MAX_NUM_SAVED_WAVES - 1);

            Color::ResetFlash();

            result = true;
        }
        else
        {
            // остальные кнопки не обрабатываем
        }

        
    }

    return result;
}

static bool NormalTitle_ROM()
{
    return false;
}

DEF_PAGE_5( pROM,                                                                                                                                                 //--- ПАМЯТЬ - ВНУТР ЗУ ---
    "ВНУТР ЗУ",
    "Переход в режим работы с внутренней памятью",
    &bPrev,
    &bNext,
    &bSave,
    &bDelete,
    &bTypeSignal,
    PageName::Memory_Internal,
    &PageMemory::self, Item::Active, NormalTitle_ROM, OnOpenClose_ROM, AfterDraw_ROM, HandlerKey_ROM
)

const Page *const PageROM::self = static_cast<const Page *>(&pROM);
