#include "defines.h"
#include "Multimeter/Multimeter.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Display/Display_Primitives.h"
#include "Hardware/Beeper.h"


using namespace Multimeter::Settings;

using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этом буфере готовая к выводу информация
#define SIZE_OUT 15
char out[SIZE_OUT];

/// Сюда копируем принятые измерения
/// Если нулевой элемент == 0, то выводить ничего не нужно
static char buffer[13] = { 0 };

static void PrepareBell();
static void PrepareConstantVoltage();
static void PrepareVariableVoltage();
static void PrepareConstantCurrent();
static void PrepareVariableCurrent();
static void PrepareResistance();
static void PrepareTestDiode();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool MeasureNotReceive()
{
    return buffer[0] == '-';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChar(uint i)
{
    static const int y = 50;
    
    static int x = 0;
    
    static const int dX = 30;

    /// true, если точка уже выведена
    static bool point = false;
    
    char symbols[2] = {out[i], 0};
    
    if(i == 0)
    {
        Text(symbols).Draw(10, y);
    }
    else
    {
        Text(symbols).Draw(point ? (x - 18) : x, y);
    }
    
    x += dX;

    if (i == 0)
    {
        x = 35;
        point = false;
    }

    if (out[i] == '.')
    {
        point = true;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMeasure()
{
    Color color = MeasureNotReceive() ? Color::GRAY_50 : Color::FILL;

    Font::SetCurrent(Font::Type::_Big51);

    Color::SetCurrent(color);
    
    for(uint i = 0; i < 7; i++)
    {
        DrawChar(i);
    }

    Font::SetSpacing(5);

    Text(&out[7]).Draw(205, 50);

    Font::SetSpacing(1);

    Font::Pop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::Update()
{
    static const struct Func
    {
        pFuncVV func;
        Func(pFuncVV f) : func(f) {};
    }
    funcs[Multimeter::Measure::Size] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareBell
    };

    Painter::BeginScene(Color::BACK);

    std::memset(out, 0, SIZE_OUT);

    Measure::E meas = Measure::GetCode(buffer);
    if (meas == Measure::Size)
    {
        meas = MULTI_MEASURE;
    }

    funcs[meas].func();

    DrawMeasure();

    Color::SetCurrent(Color::FILL);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetRange()
{
    if (MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        return MULTI_RANGE_VOLTAGE_DC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        return MULTI_RANGE_VOLTAGE_AC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC)
    {
        return MULTI_RANGE_CURRENT_DC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        return MULTI_RANGE_CURRENT_AC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        return MULTI_RANGE_RESISTANCE;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::ChangedMode()
{
    std::memset(buffer, '-', 10); //-V512

    static const int position[Measure::Size][4] =
    {
        {2, 3, 4},      // VoltageDC
        {2, 3, 4},      // VoltageAC
        {3, 2},         // CurrentDC
        {3, 2},         // CurrentAC
        {2, 3, 4, 3},   // Resistance
        {2},            // TestDiode
        (2),            // Bell
    };
    
    static const pString suffix[Measure::Size][4] =
    {
        {"V=", "V=", "V="},
        {"V~", "V~", "V~"},
        {"mA=", "A="},
        {"mA=", "A="},
        {"kQ", "kQ", "kQ", "MQ"},
        {"V="},
        {"kQ="}
    };

    buffer[position[MULTI_MEASURE][GetRange()]] = '.';
    
    std::strcpy(&buffer[10], suffix[MULTI_MEASURE][GetRange()]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::SetMeasure(const uint8 buf[13])
{
    std::memcpy(buffer, buf, 13);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareTestDiode()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, "V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantVoltage() //-V524
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, "V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableVoltage()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, "V~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantCurrent()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, (buffer[10] == '1') ? "A=" : "mA=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableCurrent()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, (buffer[10] == '1') ? "A~" : "mA~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PrepareResistance()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    out[7] = ' ';
    out[8] = buffer[8];
    out[9] = 0x1b;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ResistanceLess100()
{
    return ((out[1] == '0') && (out[3] == '0'));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareBell()
{
    std::memcpy(out, buffer + 1, 7); //-V512
    std::strcpy(out + 7, "kQ");
    out[9] = 0x1b;

    if (ResistanceLess100())
    {
        Beeper::Bell::On();
    }
    else
    {
        Beeper::Bell::Off();
    }
}
