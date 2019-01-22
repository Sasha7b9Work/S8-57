#include "defines.h"
#include "Data/DataStorage.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


using namespace Display::Primitives;


extern const PageBase pageRAM;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMemory::PageRAM::pointer = &pageRAM;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Last_Next()
{
    Math::CircleIncrease<int16>((int16 *)&NUM_RAM_SIGNAL, 0, (int16)(DataStorage::NumElementsInStorage() - 1));
}

static void Draw_Last_Next(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);

    //Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x64');
    Char('\x64').Draw4SymbolsInRect(x + 2, y + 2);

    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON(bLast_Next,                                                                               //--- ПАМЯТЬ - ПОСЛЕДНИЕ - Следующий ---
    "Следующий", "Next",
    "Перейти к следующему сигналу",
    "Go to the next signal",
    pageRAM, FuncActive, OnPress_Last_Next, Draw_Last_Next
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_Prev()
{
    Math::CircleDecrease<int16>((int16 *)&NUM_RAM_SIGNAL, 0, (int16)(DataStorage::NumElementsInStorage() - 1));
}

static void Draw_Last_Prev(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);

    //Painter::Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_BACKSPACE);
    Char(SYMBOL_BACKSPACE).Draw(x + 2, y + 2);

    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON(bLast_Prev,                                                                              //--- ПАМЯТЬ - ПОСЛЕДНИЕ - Предыдущий ---
    "Предыдущий", "Previous",
    "Перейти к предыдущему сигналу",
    "Go to the previous signal",
    pageRAM, FuncActive, OnPress_Last_Prev, Draw_Last_Prev
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_SaveToROM()
{
    ((Page *)PageMemory::PageRAM::pointer)->SetAsCurrent();
    MODE_WORK = ModeWork::ROM;      // Находимся в режиме внутренного ЗУ
    EXIT_FROM_ROM_TO_RAM = 1;       // Этот признак мы устанавливаем для того, чтобы:
                                    // 1 - по нажатии кнопки Выход со страницы "ВНУТР ЗУ" выходить в "Последние", а не в основное меню;
                                    // 2 - для того, чтобы на странице "ВНУТР ЗУ" выводить не последний считанный сигнал, а выбранный на странице 
                                    // "Последние";
                                    // 3 - чтобы в Data_Load() устанавливать соответствующие указатели.
}

static void Draw_Last_SaveToROM(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);

    //Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_ROM);
    Char(SYMBOL_ROM).Draw4SymbolsInRect(x + 2, y + 1);

    Font::SetCurrent(Font::Type::_8);
}

DEF_SMALL_BUTTON(bLast_SaveToROM,                                                                           //--- ПАМЯТЬ - ПОСЛЕДНИЕ - Внутр ЗУ ---
    "Внутр ЗУ", "Internal storage",
    "Нажмите эту кнопку, чтобы сохранить сигнал во внутреннем запоминающем устройстве",
    "Press this button to keep a signal in an internal memory",
    pageRAM, FuncActive, OnPress_Last_SaveToROM, Draw_Last_SaveToROM
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_SaveToDrive()
{
    EXIT_FROM_SETNAME_TO = RETURN_TO_LAST_MEM;
    PageMemory::SaveSignalToFlashDrive();
}

static void Draw_Last_SaveToDrive(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Font::SetCurrent(Font::Type::_UGO2);

        //Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        Char(SYMBOL_FLASH_DRIVE_BIG).Draw4SymbolsInRect(x + 2, y + 1);

        Font::SetCurrent(Font::Type::_8);
    }
}

DEF_SMALL_BUTTON(bLast_SaveToDrive,                                                                        //--- ПАМЯТЬ - ПОСЛЕДНИЕ - Сохранить ---
    "Сохранить", "Save",
    "Кнопка становится доступна при присоединённом внешнем ЗУ. Позволяет сохранить сигнал на внешем ЗУ",
    "Click this button to save the signal on the external FLASH",
    pageRAM, FuncActive, OnPress_Last_SaveToDrive, Draw_Last_SaveToDrive
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_RAM(bool enter)
{
    if (enter)
    {
        NUM_RAM_SIGNAL = 0;
        RUN_FPGA_BEFORE_SB = FPGA::IsRunning() ? 1U : 0U;
        Osci::Stop(false);
        MODE_WORK = ModeWork::RAM;
    }
    else
    {
        MODE_WORK = ModeWork::Dir;
        FPGA::OnPressStart();
    }
}

static void OnDraw_RAM()
{
    int width = 40;
    int height = 10;
    Region(width, height).Fill(Grid::Right() - width, Grid::Top(), Color::BACK);
    Rectangle(width, height).Draw(Grid::Right() - width, Grid::Top(), Color::FILL);
    Integer(NUM_RAM_SIGNAL + 1).ToString(false, 3).Draw(Grid::Right() - width + 2, Grid::Top() + 1);
    String("/").Draw(Grid::Right() - width + 17, Grid::Top() + 1);
    Integer((int)DataStorage::NumElementsInStorage()).ToString(false, 3).Draw(Grid::Right() - width + 23, Grid::Top() + 1);
}

static bool HandlerKey_RAM(KeyEvent event)
{
    Key::E key = event.key;

    if (DataStorage::NumElementsInStorage() > 1)
    {
        Sound::RegulatorSwitchRotate();
    }
    if (key == Key::Up || key == Key::Right)
    {
        OnPress_Last_Next();
    }
    else
    {
        OnPress_Last_Prev();
    }

    return true;
}

DEF_PAGE_4( pageRAM, // -V641 // -V1027                                                                                                                          //--- ПАМЯТЬ - ПОСЛЕДНИЕ ---
    "ПОСЛЕДНИЕ", "LATEST",
    "Переход в режим работы с последними полученными сигналами",
    "Transition to an operating mode with the last received signals",
    &bLast_Next,                        ///< ПАМЯТЬ - ПОСЛЕДНИЕ - Следующий
    &bLast_Prev,                        ///< ПАМЯТЬ - ПОСЛЕДНИЕ - Предыдущий
    &bLast_SaveToROM,                   ///< ПАМЯТЬ - ПОСЛЕДНИЕ - Внутр ЗУ
    &bLast_SaveToDrive,                 ///< ПАМЯТЬ - ПОСЛЕДНИЕ - Сохранить
    Page::Name::SB_Memory_Last, PageMemory::pointer, FuncActive, OnPress_RAM, OnDraw_RAM, HandlerKey_RAM
)
