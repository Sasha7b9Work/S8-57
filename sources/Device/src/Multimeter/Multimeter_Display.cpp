#include "defines.h"
#include "Multimeter/Multimeter.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Display/Display_Primitives.h"
#include "Hardware/Beeper.h"
#include "Utils/Debug.h"


using namespace Multimeter::Settings;

using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Данные для вывода.
static char outBuffer[15];

static void PrepareBell(const char *);
static void PrepareConstantVoltage(const char *);
static void PrepareVariableVoltage(const char *);
static void PrepareConstantCurrent(const char *);
static void PrepareVariableCurrent(const char *);
static void PrepareResistance(const char *);
static void PrepareTestDiode(const char *);

static bool received = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static char Symbol(uint i)
{
    return outBuffer[i];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChar(uint numSymbol, int x)
{
    static const int y = 35;
    
    char symbols[2] = {Symbol(numSymbol), 0};
    
    Text(symbols).Draw(x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSymbols()
{
    int x0 = 20;

    DrawChar(0, x0 + 10);

    int x = 48;

    for (uint i = 1; i < 7; i++)
    {
        char symbol = Symbol(i);

        if (symbol == '1')
        {
            x += 10;
        }

        DrawChar(i, x0 + x);

        if (symbol == '1')
        {
            x -= 10;
        }

        x += (Symbol(i) == '.') ? 16 : 38;
    }

    Font::SetSpacing(5);

    Text(&outBuffer[7]).Draw(120, 125);

    Font::SetSpacing(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMeasure()
{
    Color color = received ? Color::FILL : Color::GRAY_50;

    Font::SetCurrent(Font::Type::_Big64);

    Color::SetCurrent(color);
    
    DrawSymbols();

    Font::Pop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::Update()
{
    Painter::BeginScene(Color::BACK);

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
    else
    {
        // здесь ничего
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::ChangedMode()
{
    received = false;

    std::memset(outBuffer, '-', 7); //-V512

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
        {"k\x5e", "k\x5e", "k\x5e", "M\x5e"},
        {"V="},
        {"k\x5e="}
    };

    outBuffer[position[MULTI_MEASURE][GetRange()]] = '.';
    
    std::strcpy(&outBuffer[7], suffix[MULTI_MEASURE][GetRange()]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::SetMeasure(const uint8 buf[13])
{
    typedef void(*pFuncVCC)(const char *);

    static const struct Func
    {
        pFuncVCC func;
        Func(pFuncVCC f) : func(f) {};
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

    Measure::E meas = Measure::GetCode((const char *)buf);

    if (meas >= Measure::Size)
    {
        return;
    }

    std::memcpy(outBuffer, buf + 1, 7); //-V512

    funcs[meas].func((const char *)buf);

    received = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareTestDiode(const char *)
{
    std::strcpy(outBuffer + 7, "V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantVoltage(const char *) //-V524
{
    std::strcpy(outBuffer + 7, "V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableVoltage(const char *)
{
    std::strcpy(outBuffer + 7, "V~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantCurrent(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A=" : "mA=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableCurrent(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A~" : "mA~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PrepareResistance(const char *buf)
{
    outBuffer[7] = buf[8];
    outBuffer[8] = '\x5e';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ResistanceLess100()
{
    return ((outBuffer[1] == '0') && (outBuffer[3] == '0'));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareBell(const char *)
{
    std::strcpy(outBuffer + 7, "k\x5e");

    if (ResistanceLess100())
    {
        Beeper::Bell::On();
    }
    else
    {
        Beeper::Bell::Off();
    }
}
