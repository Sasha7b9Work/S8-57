#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Settings/SettingsMath.h"
#include "Utils/Math.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Osci/Osci_Settings.h"
#include "FPGA/FPGA.h"
#include "Hardware/Beeper.h"
#include "FPGA/FPGA_Math.h"


using namespace Display::Primitives;
using namespace Osci::Settings;

extern const PageBase pageMath;

const Page * const PageMeasures::PageMath::self = (const Page *)&pageMath;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Function_Screen()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        Math::CircleIncrease<int8>((int8 *)&FUNC_MODE_DRAW, 0, 2);
    }
}

static void Draw_Function_Screen_Disable(int x, int y)
{
    String("Выx").Draw(x + 2, y + 5);
}

static void Draw_Function_Screen_Separate(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
    HLine line(13);
    line.Draw(x + 3, y + 9);
    line.Draw(x + 3, y + 10);
}

static void Draw_Function_Screen_Together(int x, int y)
{
    Rectangle(13, 9).Draw(x + 3, y + 5);
}

static void Draw_Function_Screen(int x, int y)
{
    const pFuncVII funcs[3] =
    {
        Draw_Function_Screen_Disable,
        Draw_Function_Screen_Separate,
        Draw_Function_Screen_Together
    };
    funcs[FUNC_MODE_DRAW](x, y);
}

DEF_SMALL_BUTTON_HINTS_3(bFunction_Screen,                                                                                                                //--- СЕРВИС - ФУНКЦИЯ - Экран ---
    "Экран",
    "Выбирает режим отображения математического сигнала",
    &PageMeasures::PageMath::self, 0, OnPress_Function_Screen, Draw_Function_Screen,
    Draw_Function_Screen_Disable, "Вывод математической функции отключён",
    Draw_Function_Screen_Separate, "Сигналы и математическая функция выводятся в разных окнах",
    Draw_Function_Screen_Together, "Сигналы и математическая функция выводятся в одном окне"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Type()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}

static void Draw_Function_Type_Sum(int x, int y)
{
    HLine(10).Draw(x + 4, y + 9);
    VLine(10).Draw(x + 9, y + 4);
}

static void Draw_Function_Type_Mul(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3);
    Font::SetCurrent(Font::Type::_8);
}

static void Draw_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Function_Type_Sum, Draw_Function_Type_Mul };
    funcs[MATH_FUNC](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(bFunction_Type,                                                                                                                    //--- СЕРВИС - ФУНКЦИЯ - Вид ---
    "Вид",
    "Выбор математической функции",
    &PageMeasures::PageMath::self, 0, OnPress_Function_Type, Draw_Function_Type,
    Draw_Function_Type_Sum, "Сложение",
    Draw_Function_Type_Mul, "Умножение"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_ModeRegSet()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}

static void Draw_Function_ModeRegSet_Range(int x, int y)
{
    Char('M').Draw(x + 7, y + 5);
}

static void Draw_Function_ModeRegSet_RShift(int x, int y)
{
    String("См").Draw(x + 5, y + 5);
}

static void Draw_Function_ModeRegSet(int x, int y)
{
    static const pFuncVII funcs[2] = { Draw_Function_ModeRegSet_Range, Draw_Function_ModeRegSet_RShift };
    funcs[MATH_MODE_REG_SET](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(bFunction_ModeRegSet,                                                                                            //--- СЕРВИС - ФУНКЦИЯ - Режим ручки УСТАНОВКА ---
    "Режим ручки УСТАНОВКА",
    "Выбор режима ручки УСТАНОВКА - управление масштабом или смещением",
    &PageMeasures::PageMath::self, 0, OnPress_Function_ModeRegSet, Draw_Function_ModeRegSet,
    Draw_Function_ModeRegSet_Range, "Управление масштабом",
    Draw_Function_ModeRegSet_RShift, "Управление смещением"
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeA()
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_DIVIDER = (int8)SET_DIVIDER(Chan::A);
}

static void Draw_Function_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}

DEF_SMALL_BUTTON(bFunction_RangeA,                                                                                                          //--- СЕРВИС - ФУНКЦИЯ - Масштаб 1-го канала ---
    "Масштаб 1-го канала",
    "Использует масштаб первого канала для отображения результата",
    &PageMeasures::PageMath::self, 0, OnPress_Function_RangeA, Draw_Function_RangeA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeB()
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_DIVIDER = (int8)SET_DIVIDER(Chan::B);
}

static void Draw_Function_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}

DEF_SMALL_BUTTON(bFunction_RangeB,                                                                                                          //--- СЕРВИС - ФУНКЦИЯ - Масштаб 2-го канала ---
    "Масштаб 2-го канала",
    "Использует масштаб второго канала для отображения результата",
    &PageMeasures::PageMath::self, 0, OnPress_Function_RangeB, Draw_Function_RangeB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Function()
{
    return !FFT_ENABLED;
}

static void OnPress_Function(bool)
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
}

static bool HandlerKey_Function(KeyEvent event) // -V2506
{
    if (!FUNC_MODE_DRAW_IS_ENABLED)
    {
        return false;
    }

    Key::E key = event.key;

    int delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        uint16 prevRShift = SET_RSHIFT_MATH;
        uint16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShift::MAX)
            {
                rShift += 4 * STEP_RSHIFT;  // -V112
                LIMIT_ABOVE(rShift, RShift::MAX);
                if (prevRShift < RShift::ZERO && rShift > RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Beeper::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
        else
        {
            if (rShift > RShift::MIN)
            {
                rShift -= 4 * STEP_RSHIFT; // -V112
                LIMIT_BELOW(rShift, RShift::MIN);
                if (prevRShift > RShift::ZERO && rShift < RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Beeper::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = FPGA::Math::RShift2Abs(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < Range::Size - 1)
            {
                SET_RANGE_MATH = (Range::E)((uint8)(SET_RANGE_MATH + 1));  // SET_RANGE_MATH++;
                SET_RSHIFT_MATH = (uint16)FPGA::Math::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Beeper::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                SET_RANGE_MATH = (Range::E)((uint8)(SET_RANGE_MATH - 1));  // SET_RANGE_MATH--;
                SET_RSHIFT_MATH = (uint16)FPGA::Math::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Beeper::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else
        {
            // ничего делать не нужно
        }
    }

    return true;
}

DEF_PAGE_5( pageMath, // -V641                                                                                                                                   //--- СЕРВИС - ФУНКЦИЯ ---
    "МАТЕМАТИКА",
    "Установка и выбор математической функции - сложения или умножения",
    &bFunction_Screen,
    &bFunction_Type,
    &bFunction_ModeRegSet,
    &bFunction_RangeA,
    &bFunction_RangeB,
    Page::Name::SB_Service_Function, &PageMeasures::self, IsActive_Function, OnPress_Function, 0, HandlerKey_Function
)
